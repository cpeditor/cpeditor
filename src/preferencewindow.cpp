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

#include "preferencewindow.hpp"
#include "../ui/ui_preferencewindow.h"
#include "Core/EventLogger.hpp"
#include "Extensions/EditorTheme.hpp"
#include "Util.hpp"
#include <QAction>
#include <QCXXHighlighter>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <QJavaHighlighter>
#include <QMessageBox>
#include <QPythonCompleter>
#include <QPythonHighlighter>
#include <QSaveFile>

#define PREFERENCE_WINDOW_SIZE_FACTOR 0.5

PreferenceWindow::PreferenceWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::PreferenceWindow)
{
    Core::Log::i("preferencewindow/constructed", "Invoked");
    ui->setupUi(this);
    setWindowTitle("Preferences");

    editor = new QCodeEditor();
    editor->setSizePolicy({QSizePolicy::Expanding, QSizePolicy::Expanding});
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
    Core::Log::i("preferencewindow/setConstraints", "Invoked");

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
    Core::Log::i("preferencewindow/updateSnippets", "Invoked");
    ui->snippets->clear();
    auto lang = ui->snippets_lang->currentText();
    auto names = Settings::SettingsManager::getSnippetsNames(lang);
    ui->snippets->addItems(names);
}

void PreferenceWindow::switchToSnippet(const QString &text)
{
    Core::Log::i("preferencewindow/switchToSnippet", "Invoked");
    updateSnippets();
    int index = ui->snippets->findText(text);
    if (index != -1)
        ui->snippets->setCurrentIndex(index);
}

void PreferenceWindow::applySettingsToui()
{
    Core::Log::i("preferencewindow/applySettingsToui", "Invoked");
    ui->tabWidget->setCurrentIndex(0);

    ui->editor_theme->setCurrentText(Settings::SettingsManager::getEditorTheme());
    ui->tab_length->setValue(Settings::SettingsManager::getTabStop());

    if (!Settings::SettingsManager::getFont().isEmpty())
        currentFont.fromString(Settings::SettingsManager::getFont());
    ui->font_button->setText(currentFont.family() + " " + QString::number(currentFont.pointSize()));

    ui->savetest->setChecked(Settings::SettingsManager::isSaveTests());
    ui->wrap->setChecked(Settings::SettingsManager::isWrapText());
    ui->indent->setChecked(Settings::SettingsManager::isAutoIndent());
    ui->parentheses->setChecked(Settings::SettingsManager::isAutoParentheses());
    ui->remove_parentheses->setChecked(Settings::SettingsManager::isAutoRemoveParentheses());
    ui->replace_tabs->setChecked(Settings::SettingsManager::isTabsReplaced());
    ui->format_on_save->setChecked(Settings::SettingsManager::isFormatOnSave());
    ui->use_hot_exit->setChecked(Settings::SettingsManager::isUseHotExit());

    ui->defaultLang->setCurrentText(Settings::SettingsManager::getDefaultLanguage());

    ui->cpp_compiler_cmd->setText(Settings::SettingsManager::getCompileCommand("C++"));
    ui->cpp_args_cmd->setText(Settings::SettingsManager::getRuntimeArguments("C++"));

    ui->java_compiler_cmd->setText(Settings::SettingsManager::getCompileCommand("Java"));
    ui->java_args_cmd->setText(Settings::SettingsManager::getRuntimeArguments("Java"));
    ui->java_start_cmd->setText(Settings::SettingsManager::getRunCommand("Java"));

    ui->python_args_cmd->setText(Settings::SettingsManager::getRuntimeArguments("Python"));
    ui->python_start_cmd->setText(Settings::SettingsManager::getRunCommand("Python"));

    ui->companion_use->setChecked(Settings::SettingsManager::isCompetitiveCompanionActive());
    ui->companion_new_tab->setChecked(Settings::SettingsManager::isCompetitiveCompanionOpenNewTab());
    ui->companion_port->setValue(Settings::SettingsManager::getConnectionPort());

    ui->clang_format_binary->setText(Settings::SettingsManager::getClangFormatBinary());
    ui->clang_format_style->setPlainText(Settings::SettingsManager::getClangFormatStyle());

    cppTemplatePath = Settings::SettingsManager::getTemplatePath("C++");
    pythonTemplatePath = Settings::SettingsManager::getTemplatePath("Python");
    javaTemplatePath = Settings::SettingsManager::getTemplatePath("Java");

    ui->update_startup->setChecked(Settings::SettingsManager::isCheckUpdateOnStartup());
    ui->beta_update->setChecked(Settings::SettingsManager::isBeta());

    ui->time_limit->setValue(Settings::SettingsManager::getTimeLimit());

    ui->cpp_template->setText(cppTemplatePath.isEmpty() ? "<Not selected>" : "..." + cppTemplatePath.right(30));
    ui->py_template->setText(pythonTemplatePath.isEmpty() ? "<Not selected>" : "..." + pythonTemplatePath.right(30));
    ui->java_template->setText(javaTemplatePath.isEmpty() ? "<Not selected>" : "..." + javaTemplatePath.right(30));

    ui->hotkeys->setChecked(Settings::SettingsManager::isHotkeyInUse());
    on_hotkeys_clicked(Settings::SettingsManager::isHotkeyInUse());

    ui->compile_hotkey->setKeySequence(Settings::SettingsManager::getHotkeyCompile());
    ui->run_hotkey->setKeySequence(Settings::SettingsManager::getHotkeyRun());
    ui->format_hotkey->setKeySequence(Settings::SettingsManager::getHotkeyFormat());
    ui->compileRun_hotkey->setKeySequence(Settings::SettingsManager::getHotkeyCompileRun());
    ui->kill_hotkey->setKeySequence(Settings::SettingsManager::getHotkeyKill());
    ui->toggle_hotkey->setKeySequence(Settings::SettingsManager::getHotkeyViewModeToggler());
    ui->snippets_hotkey->setKeySequence(Settings::SettingsManager::getHotkeySnippets());

    ui->transparency_slider->setValue(Settings::SettingsManager::getTransparency());

    auto lang = Settings::SettingsManager::getDefaultLanguage();
    int lang_index = ui->snippets_lang->findText(lang);
    if (lang_index != -1)
        ui->snippets_lang->setCurrentIndex(lang_index);
    onSnippetsLangChanged(lang);

    ui->cf_path->setText(Settings::SettingsManager::getCFPath());
    ui->compile_and_run_only->setChecked(Settings::SettingsManager::isCompileAndRunOnly());
    ui->display_eoln_in_diff->setChecked(Settings::SettingsManager::isDisplayEolnInDiff());
}

void PreferenceWindow::extractSettingsFromUi()
{
    Core::Log::i("preferencewindow/extractSettingsFromUi", "Invoked");
    Settings::SettingsManager::setEditorTheme(ui->editor_theme->currentText());
    Settings::SettingsManager::setTabStop(ui->tab_length->value());
    Settings::SettingsManager::setFont(currentFont.toString());

    Settings::SettingsManager::setSaveTests(ui->savetest->isChecked());
    Settings::SettingsManager::setWrapText(ui->wrap->isChecked());
    Settings::SettingsManager::setAutoIndent(ui->indent->isChecked());
    Settings::SettingsManager::setAutoParentheses(ui->parentheses->isChecked());
    Settings::SettingsManager::setAutoRemoveParentheses(ui->remove_parentheses->isChecked());
    Settings::SettingsManager::setTabsReplaced(ui->replace_tabs->isChecked());
    Settings::SettingsManager::formatOnSave(ui->format_on_save->isChecked());
    Settings::SettingsManager::setUseHotExit(ui->use_hot_exit->isChecked());

    Settings::SettingsManager::setDefaultLanguage(ui->defaultLang->currentText());

    Settings::SettingsManager::setCompileCommand("C++", ui->cpp_compiler_cmd->text());
    Settings::SettingsManager::setRuntimeArguments("C++", ui->cpp_args_cmd->text());

    Settings::SettingsManager::setCompileCommand("Java", ui->java_compiler_cmd->text());
    Settings::SettingsManager::setRuntimeArguments("Java", ui->java_args_cmd->text());
    Settings::SettingsManager::setRunCommand("Java", ui->java_start_cmd->text());

    Settings::SettingsManager::setRunCommand("Python", ui->python_start_cmd->text());
    Settings::SettingsManager::setRuntimeArguments("Python", ui->python_args_cmd->text());

    Settings::SettingsManager::setClangFormatBinary(ui->clang_format_binary->text());
    Settings::SettingsManager::setClangFormatStyle(ui->clang_format_style->toPlainText());

    Settings::SettingsManager::setCompetitiveCompanionActive(ui->companion_use->isChecked());
    Settings::SettingsManager::setCompetitiveCompanionOpenNewTab(ui->companion_new_tab->isChecked());
    Settings::SettingsManager::setConnectionPort(ui->companion_port->value());

    Settings::SettingsManager::setBeta(ui->beta_update->isChecked());
    Settings::SettingsManager::checkUpdateOnStartup(ui->update_startup->isChecked());

    Settings::SettingsManager::setTimeLimit(ui->time_limit->value());

    Settings::SettingsManager::setTemplatePath("C++", cppTemplatePath);
    Settings::SettingsManager::setTemplatePath("Java", javaTemplatePath);
    Settings::SettingsManager::setTemplatePath("Python", pythonTemplatePath);

    Settings::SettingsManager::setHotKeyInUse(ui->hotkeys->isChecked());

    Settings::SettingsManager::setHotkeyRun(ui->run_hotkey->keySequence());
    Settings::SettingsManager::setHotkeyKill(ui->kill_hotkey->keySequence());
    Settings::SettingsManager::setHotkeyFormat(ui->format_hotkey->keySequence());
    Settings::SettingsManager::setHotkeyCompile(ui->compile_hotkey->keySequence());
    Settings::SettingsManager::setHotkeyCompileRun(ui->compileRun_hotkey->keySequence());
    Settings::SettingsManager::setHotkeyViewModeToggler(ui->toggle_hotkey->keySequence());
    Settings::SettingsManager::setHotkeySnippets(ui->snippets_hotkey->keySequence());

    Settings::SettingsManager::setCFPath(ui->cf_path->text());
    Settings::SettingsManager::setCompileAndRunOnly(ui->compile_and_run_only->isChecked());
    Settings::SettingsManager::setDisplayEolnInDiff(ui->display_eoln_in_diff->isChecked());
}

void PreferenceWindow::updateShow()
{
    Core::Log::i("preferencewindow/updateShow", "Invoked");
    applySettingsToui();
    show();
    raise();
}

PreferenceWindow::~PreferenceWindow()
{
    Core::Log::i("preferencewindow/destroyed", "Invoked");
    delete ui;
    delete editor;
}

void PreferenceWindow::on_ok_clicked()
{
    Core::Log::i("preferencewindow/on_ok_clicked", "Invoked");
    extractSettingsFromUi();
    close();
    emit settingsApplied();
    applySettingsToEditor();
}

void PreferenceWindow::on_cancel_clicked()
{
    Core::Log::i("preferencewindow/on_cancel_clicked", "Invoked");
    close();
}

void PreferenceWindow::on_apply_clicked()
{
    Core::Log::i("preferencewindow/on_apply_clicked", "Invoked");
    extractSettingsFromUi();
    emit settingsApplied();
    applySettingsToEditor();
}

void PreferenceWindow::on_hotkeys_clicked(bool checked)
{
    Core::Log::i("preferencewindow/on_hotkeys_clicked", "Invoked");
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
    Core::Log::i("preferencewindow/on_font_button_clicked", "Invoked");
    bool ok = false;
    QFont fp = QFontDialog::getFont(&ok, currentFont, this, "Choose a font", QFontDialog::MonospacedFonts);

    if (ok)
    {
        Core::Log::i("preferencewindow/on_font_button_clicked", "Changed font");
        currentFont = fp;
        ui->font_button->setText(currentFont.family() + " " + QString::number(currentFont.pointSize()));
        raise();
    }
}

void PreferenceWindow::on_cpp_template_clicked()
{
    Core::Log::i("preferencewindow/on_cpp_template_clicked", "Invoked");
    auto filename = QFileDialog::getOpenFileName(this, tr("Choose C++ template File"), "",
                                                 "C++ Files (*.cpp *.hpp *.h *.cc *.cxx *.c)");
    if (filename.isEmpty())
        return;
    cppTemplatePath = filename;
    ui->cpp_template->setText("..." + cppTemplatePath.right(30));
}

void PreferenceWindow::on_py_template_clicked()
{
    Core::Log::i("preferencewindow/on_py_template_clicked", "Invoked");
    auto filename = QFileDialog::getOpenFileName(this, tr("Choose Python template File"), "", "Python Files (*.py)");
    if (filename.isEmpty())
        return;
    pythonTemplatePath = filename;
    ui->py_template->setText("..." + pythonTemplatePath.right(30));
}

void PreferenceWindow::on_java_template_clicked()
{
    Core::Log::i("preferencewindow/on_java_template_clicked", "Invoked");
    auto filename = QFileDialog::getOpenFileName(this, tr("Choose Java template File"), "", "Java Files (*.java)");
    if (filename.isEmpty())
        return;
    javaTemplatePath = filename;
    ui->java_template->setText("..." + javaTemplatePath.right(30));
}

void PreferenceWindow::on_load_snippets_from_files_clicked()
{
    Core::Log::i("preferencewindow/on_load_snippets_from_files_clicked", "Invoked");
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
                Settings::SettingsManager::setSnippet(lang, snippetName, file.readAll());
                switchToSnippet(snippetName);
            }
            file.close();
        }
    }
}

void PreferenceWindow::on_extract_snippets_to_files_clicked()
{
    Core::Log::i("preferencewindow/on_extract_snippets_to_files_clicked", "Invoked");
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
        Core::Log::i("preferencewindow/on_extract_snippets_to_files_clicked", "branched to if");
        QDir dir(dirPath);
        auto names = Settings::SettingsManager::getSnippetsNames(lang);
        for (auto name : names)
        {
            auto content = Settings::SettingsManager::getSnippet(lang, name);
            auto filePath = dir.filePath(name + suffix);
            if (QFile::exists(filePath))
                filePath = QFileDialog::getSaveFileName(this, "Save snippet " + name + " to:", dirPath, fileType);
            while (!filePath.isEmpty())
            {
                if (Util::saveFile(filePath, content, "PreferenceWindow/on_extract_snippets_to_files_clicked"))
                    break;
                QMessageBox::warning(this, "File save error", "Failed to save to " + filePath);
                filePath = QFileDialog::getSaveFileName(this, "Save snippet " + name + " to:", dirPath, fileType);
            }
        }
    }
}

void PreferenceWindow::onSnippetsLangChanged(const QString &lang)
{
    Core::Log::i("preferencewindow/onSnippetsLangChanged") << " lang " << lang << endl;
    updateSnippets();
    if (lang == "Python")
    {
        editor->setHighlighter(new QPythonHighlighter);
        editor->setCompleter(new QPythonCompleter);
    }
    else if (lang == "Java")
    {
        editor->setHighlighter(new QJavaHighlighter);
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
    Core::Log::i("preferencewindow/onCurrentSnippetChanged", "Invoked");
    auto lang = ui->snippets_lang->currentText();
    auto content = Settings::SettingsManager::getSnippet(lang, text);
    editor->setPlainText(content);
    editor->setFocus(Qt::OtherFocusReason);
}

void PreferenceWindow::applySettingsToEditor()
{
    Core::Log::i("preferencewindow/applySettingsToEditor", "Invoked");
    editor->setTabReplace(Settings::SettingsManager::isTabsReplaced());
    editor->setTabReplaceSize(Settings::SettingsManager::getTabStop());
    editor->setAutoIndentation(Settings::SettingsManager::isAutoIndent());
    editor->setAutoParentheses(Settings::SettingsManager::isAutoParentheses());
    editor->setAutoRemoveParentheses(Settings::SettingsManager::isAutoRemoveParentheses());

    if (!Settings::SettingsManager::getFont().isEmpty())
    {
        QFont font;
        font.fromString(Settings::SettingsManager::getFont());
        editor->setFont(font);
    }

    const int tabStop = Settings::SettingsManager::getTabStop();
    QFontMetrics metric(editor->font());
    editor->setTabReplaceSize(tabStop);

    if (Settings::SettingsManager::isWrapText())
        editor->setWordWrapMode(QTextOption::WordWrap);
    else
        editor->setWordWrapMode(QTextOption::NoWrap);

    if (Settings::SettingsManager::getEditorTheme() == "Light")
        editor->setSyntaxStyle(Themes::EditorTheme::getLightTheme());
    else if (Settings::SettingsManager::getEditorTheme() == "Drakula")
        editor->setSyntaxStyle(Themes::EditorTheme::getDrakulaTheme());
    else if (Settings::SettingsManager::getEditorTheme() == "Monkai")
        editor->setSyntaxStyle(Themes::EditorTheme::getMonkaiTheme());
    else if (Settings::SettingsManager::getEditorTheme() == "Solarised")
        editor->setSyntaxStyle(Themes::EditorTheme::getSolarisedTheme());
    else if (Settings::SettingsManager::getEditorTheme() == "Solarised Dark")
        editor->setSyntaxStyle(Themes::EditorTheme::getSolarisedDarkTheme());
    else
        editor->setSyntaxStyle(Themes::EditorTheme::getLightTheme());
}

void PreferenceWindow::on_snippet_save_clicked()
{
    Core::Log::i("preferencewindow/on_snippet_save_clicked", "Invoked");
    auto lang = ui->snippets_lang->currentText();
    if (ui->snippets->currentIndex() != -1)
    {
        Core::Log::i("preferencewindow/on_snippet_save_clicked", "branched to if");
        auto name = ui->snippets->currentText();
        Settings::SettingsManager::setSnippet(lang, name, editor->toPlainText());
    }
    else
    {
        Core::Log::i("preferencewindow/on_snippet_save_clicked", "branched to else");
        auto name = getNewSnippetName(lang);
        if (!name.isEmpty())
        {
            auto content = editor->toPlainText();
            Settings::SettingsManager::setSnippet(lang, name, content);
            switchToSnippet(name);
        }
    }
}

void PreferenceWindow::on_snippet_new_clicked()
{
    Core::Log::i("preferencewindow/on_snippet_new_clicked", "Invoked");
    auto lang = ui->snippets_lang->currentText();
    auto name = getNewSnippetName(lang);
    if (!name.isEmpty())
    {
        Settings::SettingsManager::setSnippet(lang, name, "");
        switchToSnippet(name);
    }
}

void PreferenceWindow::on_snippet_delete_clicked()
{
    Core::Log::i("preferencewindow/on_snippet_delete_clicked", "Invoked");
    int index = ui->snippets->currentIndex();
    if (index != -1)
    {
        Core::Log::i("preferencewindow/on_snippet_delete_clicked", "branched to if");
        auto name = ui->snippets->currentText();
        auto res = QMessageBox::question(this, "Delete?", "Do you want to delete the snippet " + name + "?");
        if (res == QMessageBox::Yes)
        {
            auto lang = ui->snippets_lang->currentText();
            ui->snippets->removeItem(index);
            Settings::SettingsManager::removeSnippet(lang, name);
        }
    }
}

void PreferenceWindow::on_snippet_rename_clicked()
{
    Core::Log::i("preferencewindow/on_snippet_rename_clicked", "Invoked");

    if (ui->snippets->currentIndex() != -1)
    {
        Core::Log::i("preferencewindow/on_snippet_rename_clicked", "branched to if");
        auto lang = ui->snippets_lang->currentText();
        auto name = getNewSnippetName(lang);
        if (!name.isEmpty())
        {
            auto content = editor->toPlainText();
            auto currentName = ui->snippets->currentText();
            Settings::SettingsManager::removeSnippet(lang, currentName);
            Settings::SettingsManager::setSnippet(lang, name, content);
            switchToSnippet(name);
        }
    }
}

void PreferenceWindow::on_transparency_slider_sliderMoved(int value)
{
    Settings::SettingsManager::setTransparency(value);
    parentWidget()->setWindowOpacity(value / 100.0);
}

QString PreferenceWindow::getNewSnippetName(const QString &lang, const QString &old)
{
    Core::Log::i("preferencewindow/getNewSnippetName") << " lang " << lang << " old " << old << endl;
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
