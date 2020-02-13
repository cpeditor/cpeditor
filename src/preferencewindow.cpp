/*
 * Copyright (C) 2019-2020 Ashar Khan <ashar786khan@gmail.com>
 *
 * This file is part of CPEditor.
 *
 * CPEditor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * I will not be responsible if CPEditor behaves in unexpected way and
 * causes your ratings to go down and or loose any important contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

#include "include/preferencewindow.hpp"
#include "../ui/ui_preferencewindow.h"

#include "Extensions/EditorTheme.hpp"
#include <QAction>
#include <QCXXHighlighter>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QPythonCompleter>
#include <QPythonHighlighter>
#include <QSaveFile>

#define PREFERENCE_WINDOW_SIZE_FACTOR 0.5

PreferenceWindow::PreferenceWindow(Settings::SettingManager *manager, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::PreferenceWindow)
{
    ui->setupUi(this);
    settingManager = manager;
    setWindowTitle("Preferences");

    editor = new QCodeEditor();
    ui->verticalLayout_3->insertWidget(0, editor);

    connect(ui->snippets, SIGNAL(currentTextChanged(const QString &)), this,
            SLOT(onCurrentSnippetChanged(const QString &)));
    connect(ui->snippets_lang, SIGNAL(currentTextChanged(const QString &)), this,
            SLOT(onSnippetsLangChanged(const QString &)));

    applySettingsToui();
    resize(QDesktopWidget().availableGeometry(this).size() * PREFERENCE_WINDOW_SIZE_FACTOR);
    setConstraints();
    applySettingsToEditor();
}

void PreferenceWindow::setConstraints()
{
    ui->tab_length->setMinimum(1);
    ui->tab_length->setMaximum(20);

    ui->time_limit->setMinimum(1);
    ui->time_limit->setMaximum(3600000);

    ui->companion_port->setMinimum(10000);
    ui->companion_port->setMaximum(65535);

    ui->transparency_slider->setMinimum(60);
    ui->transparency_slider->setMaximum(100);
}

void PreferenceWindow::updateSnippets()
{
    ui->snippets->clear();
    auto lang = ui->snippets_lang->currentText();
    auto names = settingManager->getSnippetsNames(lang);
    ui->snippets->addItems(names);
}

void PreferenceWindow::switchToSnippet(const QString &text)
{
    updateSnippets();
    int index = ui->snippets->findText(text);
    if (index != -1)
        ui->snippets->setCurrentIndex(index);
}

void PreferenceWindow::applySettingsToui()
{
    ui->tabWidget->setCurrentIndex(0);

    ui->editor_theme->setCurrentText(settingManager->getEditorTheme());
    ui->tab_length->setValue(settingManager->getTabStop());

    if (!settingManager->getFont().isEmpty())
        currentFont.fromString(settingManager->getFont());
    ui->font_button->setText(currentFont.family() + " " + QString::number(currentFont.pointSize()));

    ui->savetest->setChecked(settingManager->isSaveTests());
    ui->wrap->setChecked(settingManager->isWrapText());
    ui->indent->setChecked(settingManager->isAutoIndent());
    ui->parentheses->setChecked(settingManager->isAutoParenthesis());
    ui->replace_tabs->setChecked(settingManager->isTabsReplaced());
    ui->format_on_save->setChecked(settingManager->isFormatOnSave());
    ui->use_hot_exit->setChecked(settingManager->isUseHotExit());

    ui->defaultLang->setCurrentText(settingManager->getDefaultLanguage());

    ui->cpp_compiler_cmd->setText(settingManager->getCompileCommand("C++"));
    ui->cpp_args_cmd->setText(settingManager->getRuntimeArguments("C++"));

    ui->java_compiler_cmd->setText(settingManager->getCompileCommand("Java"));
    ui->java_args_cmd->setText(settingManager->getRuntimeArguments("Java"));
    ui->java_start_cmd->setText(settingManager->getRunCommand("Java"));

    ui->python_args_cmd->setText(settingManager->getRuntimeArguments("Python"));
    ui->python_start_cmd->setText(settingManager->getRunCommand("Python"));

    ui->companion_use->setChecked(settingManager->isCompetitiveCompanionActive());
    ui->companion_new_tab->setChecked(settingManager->isCompetitiveCompanionOpenNewTab());
    ui->companion_port->setValue(settingManager->getConnectionPort());

    ui->clang_format_binary->setText(settingManager->getClangFormatBinary());
    ui->clang_format_style->setPlainText(settingManager->getClangFormatStyle());

    cppTemplatePath = settingManager->getTemplatePath("C++");
    pythonTemplatePath = settingManager->getTemplatePath("Python");
    javaTemplatePath = settingManager->getTemplatePath("Java");

    ui->update_startup->setChecked(settingManager->isCheckUpdateOnStartup());
    ui->beta_update->setChecked(settingManager->isBeta());

    ui->time_limit->setValue(settingManager->getTimeLimit());

    ui->cpp_template->setText(cppTemplatePath.isEmpty() ? "<Not selected>" : "..." + cppTemplatePath.right(30));
    ui->py_template->setText(pythonTemplatePath.isEmpty() ? "<Not selected>" : "..." + pythonTemplatePath.right(30));
    ui->java_template->setText(javaTemplatePath.isEmpty() ? "<Not selected>" : "..." + javaTemplatePath.right(30));

    ui->hotkeys->setChecked(settingManager->isHotkeyInUse());
    on_hotkeys_clicked(settingManager->isHotkeyInUse());

    ui->compile_hotkey->setKeySequence(settingManager->getHotkeyCompile());
    ui->run_hotkey->setKeySequence(settingManager->getHotkeyRun());
    ui->format_hotkey->setKeySequence(settingManager->getHotkeyFormat());
    ui->compileRun_hotkey->setKeySequence(settingManager->getHotkeyCompileRun());
    ui->kill_hotkey->setKeySequence(settingManager->getHotkeyKill());
    ui->toggle_hotkey->setKeySequence(settingManager->getHotkeyViewModeToggler());
    ui->snippets_hotkey->setKeySequence(settingManager->getHotkeySnippets());

    ui->transparency_slider->setValue(settingManager->getTransparency());

    auto lang = settingManager->getDefaultLanguage();
    int lang_index = ui->snippets_lang->findText(lang);
    if (lang_index != -1)
        ui->snippets_lang->setCurrentIndex(lang_index);
    onSnippetsLangChanged(lang);

    ui->cf_path->setText(settingManager->getCFPath());
}

void PreferenceWindow::extractSettingsFromUi()
{
    settingManager->setEditorTheme(ui->editor_theme->currentText());
    settingManager->setTabStop(ui->tab_length->value());
    settingManager->setFont(currentFont.toString());

    settingManager->setSaveTests(ui->savetest->isChecked());
    settingManager->setWrapText(ui->wrap->isChecked());
    settingManager->setAutoIndent(ui->indent->isChecked());
    settingManager->setAutoParenthesis(ui->parentheses->isChecked());
    settingManager->setTabsReplaced(ui->replace_tabs->isChecked());
    settingManager->formatOnSave(ui->format_on_save->isChecked());
    settingManager->setUseHotExit(ui->use_hot_exit->isChecked());

    settingManager->setDefaultLanguage(ui->defaultLang->currentText());

    settingManager->setCompileCommand("C++", ui->cpp_compiler_cmd->text());
    settingManager->setRuntimeArguments("C++", ui->cpp_args_cmd->text());

    settingManager->setCompileCommand("Java", ui->java_compiler_cmd->text());
    settingManager->setRuntimeArguments("Java", ui->java_args_cmd->text());
    settingManager->setRunCommand("Java", ui->java_start_cmd->text());

    settingManager->setRunCommand("Python", ui->python_start_cmd->text());
    settingManager->setRuntimeArguments("Python", ui->python_args_cmd->text());

    settingManager->setClangFormatBinary(ui->clang_format_binary->text());
    settingManager->setClangFormatStyle(ui->clang_format_style->toPlainText());

    settingManager->setCompetitiveCompanionActive(ui->companion_use->isChecked());
    settingManager->setCompetitiveCompanionOpenNewTab(ui->companion_new_tab->isChecked());
    settingManager->setConnectionPort(ui->companion_port->value());

    settingManager->setBeta(ui->beta_update->isChecked());
    settingManager->checkUpdateOnStartup(ui->update_startup->isChecked());

    settingManager->setTimeLimit(ui->time_limit->value());

    settingManager->setTemplatePath("C++", cppTemplatePath);
    settingManager->setTemplatePath("Java", javaTemplatePath);
    settingManager->setTemplatePath("Python", pythonTemplatePath);

    settingManager->setHotKeyInUse(ui->hotkeys->isChecked());

    settingManager->setHotkeyRun(ui->run_hotkey->keySequence());
    settingManager->setHotkeyKill(ui->kill_hotkey->keySequence());
    settingManager->setHotkeyFormat(ui->format_hotkey->keySequence());
    settingManager->setHotkeyCompile(ui->compile_hotkey->keySequence());
    settingManager->setHotkeyCompileRun(ui->compileRun_hotkey->keySequence());
    settingManager->setHotkeyViewModeToggler(ui->toggle_hotkey->keySequence());
    settingManager->setHotkeySnippets(ui->snippets_hotkey->keySequence());

    settingManager->setCFPath(ui->cf_path->text());
}

void PreferenceWindow::updateShow()
{
    applySettingsToui();
    show();
    raise();
}

PreferenceWindow::~PreferenceWindow()
{
    delete ui;
    delete editor;
}

void PreferenceWindow::on_ok_clicked()
{
    extractSettingsFromUi();
    close();
    emit settingsApplied();
    applySettingsToEditor();
}

void PreferenceWindow::on_cancel_clicked()
{
    close();
}

void PreferenceWindow::on_apply_clicked()
{
    extractSettingsFromUi();
    emit settingsApplied();
    applySettingsToEditor();
}

void PreferenceWindow::on_hotkeys_clicked(bool checked)
{
    ui->compile_hotkey->setEnabled(checked);
    ui->compileRun_hotkey->setEnabled(checked);
    ui->run_hotkey->setEnabled(checked);
    ui->kill_hotkey->setEnabled(checked);
    ui->format_hotkey->setEnabled(checked);
    ui->toggle_hotkey->setEnabled(checked);
    ui->snippets_hotkey->setEnabled(checked);
}

void PreferenceWindow::on_font_button_clicked()
{
    bool ok = false;
    QFont fp = QFontDialog::getFont(&ok, currentFont, this, "Choose a font", QFontDialog::MonospacedFonts);

    if (ok)
    {
        currentFont = fp;
        ui->font_button->setText(currentFont.family() + " " + QString::number(currentFont.pointSize()));
    }
}

void PreferenceWindow::on_cpp_template_clicked()
{
    auto filename = QFileDialog::getOpenFileName(this, tr("Choose C++ template File"), "",
                                                 "C++ Files (*.cpp *.hpp *.h *.cc *.cxx *.c)");
    if (filename.isEmpty())
        return;
    cppTemplatePath = filename;
    ui->cpp_template->setText("..." + cppTemplatePath.right(30));
}

void PreferenceWindow::on_py_template_clicked()
{
    auto filename = QFileDialog::getOpenFileName(this, tr("Choose Python template File"), "", "Python Files (*.py)");
    if (filename.isEmpty())
        return;
    pythonTemplatePath = filename;
    ui->py_template->setText("..." + pythonTemplatePath.right(30));
}

void PreferenceWindow::on_java_template_clicked()
{
    auto filename = QFileDialog::getOpenFileName(this, tr("Choose Java template File"), "", "Java Files (*.java)");
    if (filename.isEmpty())
        return;
    javaTemplatePath = filename;
    ui->java_template->setText("..." + javaTemplatePath.right(30));
}

void PreferenceWindow::on_load_snippets_from_files_clicked()
{
    auto lang = ui->snippets_lang->currentText();
    QString fileType = "C++ Files (*.cpp *.hpp *.h *.cc *.cxx *.c)";
    if (lang == "Java")
        fileType = "Java Files (*.java)";
    else if (lang == "Python")
        fileType = "Python Files (*.py)";
    QStringList filenames = QFileDialog::getOpenFileNames(
        this, "Select one or more files to be loaded as snippets for " + lang, "", fileType);
    for (auto &filename : filenames)
    {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly | QFile::Text))
        {
            QMessageBox::warning(this, "File load error", "Failed to read " + filename);
        }
        else
        {
            auto snippetName = QFileInfo(file).baseName();
            if (snippetName.isEmpty())
                snippetName = QFileInfo(file).fileName();
            if (ui->snippets->findText(snippetName) != -1)
                snippetName = getNewSnippetName(lang, snippetName);
            if (!snippetName.isEmpty())
            {
                settingManager->setSnippet(lang, snippetName, file.readAll());
                switchToSnippet(snippetName);
            }
            file.close();
        }
    }
}

void PreferenceWindow::on_extract_snippets_to_files_clicked()
{
    auto lang = ui->snippets_lang->currentText();
    QString suffix = ".cpp";
    QString fileType = "C++ Files (*.cpp *.hpp *.h *.cc *.cxx *.c)";
    if (lang == "Java")
    {
        suffix = ".java";
        fileType = "Java Files (*.java)";
    }
    else if (lang == "Python")
    {
        suffix = ".py";
        fileType = "Python Files (*.py)";
    }
    auto dirPath = QFileDialog::getExistingDirectory(this, "Choose a directory to extract snippets to");
    if (QFile::exists(dirPath))
    {
        QDir dir(dirPath);
        auto names = settingManager->getSnippetsNames(lang);
        for (auto name : names)
        {
            auto content = settingManager->getSnippet(lang, name);
            auto filePath = dir.filePath(name + suffix);
            if (QFile::exists(filePath))
                filePath = QFileDialog::getSaveFileName(this, "Save snippet " + name + " to:", dirPath, fileType);
            while (!filePath.isEmpty())
            {
                QSaveFile saveFile(filePath);
                saveFile.open(QIODevice::WriteOnly | QIODevice::Text);
                saveFile.write(content.toStdString().c_str());
                if (saveFile.commit())
                    break;
                QMessageBox::warning(this, "File save error", "Failed to save to " + filePath);
                filePath = QFileDialog::getSaveFileName(this, "Save snippet " + name + " to:", dirPath, fileType);
            }
        }
    }
}

void PreferenceWindow::onSnippetsLangChanged(const QString &lang)
{
    updateSnippets();
    if (lang == "Python")
    {
        editor->setHighlighter(new QPythonHighlighter);
        editor->setCompleter(new QPythonCompleter);
    }
    else if (lang == "Java")
    {
        editor->setHighlighter(new QCXXHighlighter);
        editor->setCompleter(nullptr);
    }
    else
    {
        editor->setHighlighter(new QCXXHighlighter);
        editor->setCompleter(nullptr);
    }
}

void PreferenceWindow::onCurrentSnippetChanged(const QString &text)
{
    auto lang = ui->snippets_lang->currentText();
    auto content = settingManager->getSnippet(lang, text);
    editor->setPlainText(content);
    editor->setFocus(Qt::OtherFocusReason);
}

void PreferenceWindow::applySettingsToEditor()
{
    editor->setTabReplace(settingManager->isTabsReplaced());
    editor->setTabReplaceSize(settingManager->getTabStop());
    editor->setAutoIndentation(settingManager->isAutoIndent());
    editor->setAutoParentheses(settingManager->isAutoParenthesis());

    if (!settingManager->getFont().isEmpty())
    {
        QFont font;
        font.fromString(settingManager->getFont());
        editor->setFont(font);
    }

    const int tabStop = settingManager->getTabStop();
    QFontMetrics metric(editor->font());
    editor->setTabReplaceSize(tabStop);
    editor->setTabStopDistance(tabStop * metric.horizontalAdvance("9"));

    if (settingManager->isWrapText())
        editor->setWordWrapMode(QTextOption::WordWrap);
    else
        editor->setWordWrapMode(QTextOption::NoWrap);

    if (settingManager->getEditorTheme() == "Light")
        editor->setSyntaxStyle(Themes::EditorTheme::getLightTheme());
    else if (settingManager->getEditorTheme() == "Drakula")
        editor->setSyntaxStyle(Themes::EditorTheme::getDrakulaTheme());
    else if (settingManager->getEditorTheme() == "Monkai")
        editor->setSyntaxStyle(Themes::EditorTheme::getMonkaiTheme());
    else if (settingManager->getEditorTheme() == "Solarised")
        editor->setSyntaxStyle(Themes::EditorTheme::getSolarisedTheme());
    else if (settingManager->getEditorTheme() == "Solarised Dark")
        editor->setSyntaxStyle(Themes::EditorTheme::getSolarisedDarkTheme());
    else
        editor->setSyntaxStyle(Themes::EditorTheme::getLightTheme());
}

void PreferenceWindow::on_snippet_save_clicked()
{
    auto lang = ui->snippets_lang->currentText();
    if (ui->snippets->currentIndex() != -1)
    {
        auto name = ui->snippets->currentText();
        settingManager->setSnippet(lang, name, editor->toPlainText());
    }
    else
    {
        auto name = getNewSnippetName(lang);
        if (!name.isEmpty())
        {
            auto content = editor->toPlainText();
            settingManager->setSnippet(lang, name, content);
            switchToSnippet(name);
        }
    }
}

void PreferenceWindow::on_snippet_new_clicked()
{
    auto lang = ui->snippets_lang->currentText();
    auto name = getNewSnippetName(lang);
    if (!name.isEmpty())
    {
        settingManager->setSnippet(lang, name, "");
        switchToSnippet(name);
    }
}

void PreferenceWindow::on_snippet_delete_clicked()
{
    int index = ui->snippets->currentIndex();
    if (index != -1)
    {
        auto name = ui->snippets->currentText();
        auto res = QMessageBox::question(this, "Delete?", "Do you want to delete the snippet " + name + "?");
        if (res == QMessageBox::Yes)
        {
            auto lang = ui->snippets_lang->currentText();
            ui->snippets->removeItem(index);
            settingManager->removeSnippet(lang, name);
        }
    }
}

void PreferenceWindow::on_snippet_rename_clicked()
{
    if (ui->snippets->currentIndex() != -1)
    {
        auto lang = ui->snippets_lang->currentText();
        auto name = getNewSnippetName(lang);
        if (!name.isEmpty())
        {
            auto content = editor->toPlainText();
            auto currentName = ui->snippets->currentText();
            settingManager->removeSnippet(lang, currentName);
            settingManager->setSnippet(lang, name, content);
            switchToSnippet(name);
        }
    }
}

void PreferenceWindow::on_transparency_slider_valueChanged(int value)
{
    settingManager->setTransparency(value);
    parentWidget()->setWindowOpacity(value / 100.0);
}

QString PreferenceWindow::getNewSnippetName(const QString &lang, const QString &old)
{
    QString label = "New name:";
    if (!old.isEmpty())
        label = "The name " + old + " is used for " + lang + "\n" + label;
    auto name = QInputDialog::getText(this, tr("Snippet Name"), label);
    if (name.isEmpty())
        return QString();
    else if (ui->snippets->findText(name) == -1)
        return name;
    else
        return getNewSnippetName(lang, name);
}
