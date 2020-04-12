/*
 * Copyright (C) 2019-2020 Ashar Khan <ashar786khan@gmail.com>
 *
 * This file is part of CP Editor.
 *
 * CP Editor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * I will not be responsible if CP Editor behaves in unexpected way and
 * causes your ratings to go down and or lose any important contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

#include "Settings/CodeSnippetsPage.hpp"
#include "Settings/SettingsManager.hpp"
#include "Util.hpp"
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

CodeSnippetsPage::CodeSnippetsPage(const QString &language, QWidget *parent) : PreferencesPage(parent), lang(language)
{
    setTitle(language + " Snippets");

    splitter = new QSplitter();
    addWidget(splitter);

    leftWidget = new QWidget();
    splitter->addWidget(leftWidget);

    leftLayout = new QVBoxLayout(leftWidget);

    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Search...");
    connect(searchEdit, SIGNAL(textChanged(const QString &)), this, SLOT(updateSearch(const QString &)));
    leftLayout->addWidget(searchEdit);

    snippetsList = new QListWidget();
    snippetsList->setSortingEnabled(true);
    connect(snippetsList, SIGNAL(itemActivated(QListWidgetItem *)), this, SLOT(switchToSnippet(QListWidgetItem *)));
    connect(snippetsList, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(switchToSnippet(QListWidgetItem *)));
    leftLayout->addWidget(snippetsList);

    buttonsLayout = new QHBoxLayout();
    leftLayout->addLayout(buttonsLayout);

    addButton = new QPushButton("Add");
    addButton->setShortcut({"Ctrl+N"});
    connect(addButton, SIGNAL(clicked()), this, SLOT(addSnippet()));
    buttonsLayout->addWidget(addButton);

    deleteButton = new QPushButton("Del");
    deleteButton->setShortcut({"Ctrl+W"});
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteSnippet()));
    buttonsLayout->addWidget(deleteButton);

    renameAction = new QAction("Rename Snippet");
    renameAction->setShortcut({"F2"});
    connect(renameAction, SIGNAL(triggered()), this, SLOT(renameSnippet()));

    loadSnippetsFromFilesAction = new QAction("Load Snippets From Files");
    connect(loadSnippetsFromFilesAction, SIGNAL(triggered()), this, SLOT(loadSnippetsFromFiles()));

    extractSnippetsToFilesAction = new QAction("Extract Snippets To Files");
    connect(extractSnippetsToFilesAction, SIGNAL(triggered()), this, SLOT(extractSnippetsToFiles()));

    updateActions();

    moreMenu = new QMenu();
    moreMenu->addAction(renameAction);
    moreMenu->addAction(loadSnippetsFromFilesAction);
    moreMenu->addAction(extractSnippetsToFilesAction);

    moreButton = new QPushButton("More");
    moreButton->setMenu(moreMenu);
    buttonsLayout->addWidget(moreButton);

    rightWidget = new QStackedWidget();
    splitter->addWidget(rightWidget);

    editor = new QCodeEditor();
    Util::setEditorLanguage(editor, lang);
    connect(editor, SIGNAL(textChanged()), this, SLOT(updateButtons()));
    rightWidget->addWidget(editor);

    noSnippetWidget = new QWidget();
    rightWidget->addWidget(noSnippetWidget);
    rightWidget->setCurrentWidget(noSnippetWidget);

    VStretchLayout = new QVBoxLayout(noSnippetWidget);
    VStretchLayout->addStretch();

    HStretchLayout = new QHBoxLayout();
    VStretchLayout->addLayout(HStretchLayout);
    HStretchLayout->addStretch();

    noSnippetLabel = new QLabel("No Snippet Selected");
    HStretchLayout->addWidget(noSnippetLabel);

    HStretchLayout->addStretch();

    VStretchLayout->addStretch();

    splitter->setSizes({10000, 30000});
}

bool CodeSnippetsPage::areSettingsChanged()
{
    auto settingsKeys = SettingsManager::keyStartsWith(snippetKey(""));
    auto UIkeys = snippetItem.keys();
    for (auto &key : UIkeys)
        key = snippetKey(key);
    if (settingsKeys != UIkeys)
        return true;
    if (currentItem == nullptr)
        return false;
    return editor->toPlainText() != SettingsManager::get(snippetKey(currentItem->text()));
}

void CodeSnippetsPage::makeUITheSameAsDefault()
{
    if (switchToSnippet(nullptr))
    {
        snippetsList->clear();
        snippetItem.clear();

        currentItem = nullptr;
        updateActions();
        updateButtons();
    }
}

void CodeSnippetsPage::makeUITheSameAsSettings()
{
    Util::applySettingsToEditor(editor);
    auto settingsKeys = SettingsManager::keyStartsWith(snippetKey(""));
    for (auto key : settingsKeys)
    {
        if (!snippetItem.contains(key.mid(snippetKey("").length())))
        {
            addSnippet(key.mid(snippetKey("").length()));
        }
    }
    auto UIKeys = snippetItem.keys();
    for (auto key : UIKeys)
    {
        if (!settingsKeys.contains(snippetKey(key)))
        {
            deleteSnippet(snippetItem[key]);
        }
    }
    if (currentItem != nullptr)
    {
        auto cursor = editor->textCursor();
        cursor.select(QTextCursor::Document);
        cursor.insertText(SettingsManager::get(snippetKey(currentItem->text())).toString());
    }
}

void CodeSnippetsPage::makeSettingsTheSameAsUI()
{
    auto settingsKeys = SettingsManager::keyStartsWith(snippetKey(""));
    for (auto key : settingsKeys)
    {
        if (!snippetItem.contains(key.mid(snippetKey("").length())))
        {
            SettingsManager::remove({key});
        }
    }
    auto UIKeys = snippetItem.keys();
    for (auto key : UIKeys)
    {
        if (!settingsKeys.contains(snippetKey(key)))
        {
            SettingsManager::set(snippetKey(key), QString());
        }
    }
    if (currentItem != nullptr)
        SettingsManager::set(snippetKey(currentItem->text()), editor->toPlainText());
    updateButtons();
}

void CodeSnippetsPage::updateSearch(const QString &text)
{
    if (text.isEmpty())
        return;
    for (int i = 0; i < snippetsList->count(); ++i)
    {
        auto item = snippetsList->item(i);
        if (item->text().startsWith(text, Qt::CaseInsensitive))
        {
            snippetsList->setCurrentItem(item);
            break;
        }
    }
}

bool CodeSnippetsPage::switchToSnippet(QListWidgetItem *item, bool force)
{
    if (item == currentItem)
        return true;

    if (force || aboutToSwitchToSnippet())
    {
        snippetsList->setCurrentItem(item);
        currentItem = item;
        if (item == nullptr)
        {
            rightWidget->setCurrentWidget(noSnippetWidget);
        }
        else
        {
            rightWidget->setCurrentWidget(editor);
            editor->setPlainText(SettingsManager::get(snippetKey(item->text())).toString());
        }
        updateActions();
        updateButtons();
        return true;
    }

    return false;
}

bool CodeSnippetsPage::aboutToSwitchToSnippet()
{
    if (currentItem == nullptr)
        return true;

    if (SettingsManager::contains(snippetKey(currentItem->text())) &&
        editor->toPlainText() == SettingsManager::get(snippetKey(currentItem->text())))
        return true;

    auto res =
        QMessageBox::question(this, "Unsaved Snippets",
                              "The current snippet has been changed. Do you want to save it or discard the changes?",
                              QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (res)
    {
    case QMessageBox::Save:
        SettingsManager::set(snippetKey(currentItem->text()), editor->toPlainText());
        updateButtons();
        emit settingsApplied(path());
        return true;
    case QMessageBox::Discard:
        return true;
    case QMessageBox::Cancel:
        return false;
    default:
        Q_UNREACHABLE();
        break;
    }
}

void CodeSnippetsPage::addSnippet()
{
    auto name = getNewSnippetName();
    if (!name.isEmpty())
        addSnippet(name);
}

void CodeSnippetsPage::deleteSnippet()
{
    if (currentItem == nullptr)
        return;
    auto res = QMessageBox::question(this, "Delete Snippet",
                                     QString("Do you really want to delete the snippet [%1]?").arg(currentItem->text()),
                                     QMessageBox::Yes | QMessageBox::No);
    if (res == QMessageBox::Yes)
        deleteSnippet(currentItem);
}

void CodeSnippetsPage::renameSnippet()
{
    if (currentItem == nullptr)
        return;

    auto oldName = currentItem->text();
    snippetItem.remove(oldName); // Remove from the keys so that you can rename to the old name
    auto newName = getNewSnippetName(oldName);
    if (newName.isEmpty() || newName == oldName)
    {
        snippetItem[oldName] = currentItem;
        return;
    }
    delete currentItem;
    currentItem = new QListWidgetItem(newName);
    snippetsList->addItem(currentItem);
    snippetsList->setCurrentItem(currentItem);
    snippetItem[newName] = currentItem;
    updateButtons();
}

void CodeSnippetsPage::loadSnippetsFromFiles()
{
    auto files = QFileDialog::getOpenFileNames(this, "Load Snippets", QString(),
                                               Util::fileNameFilter(lang == "C++", lang == "Java", lang == "Python"));

    for (auto file : files)
    {
        QFile openFile(file);
        if (!openFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::warning(this, "Load Snippets",
                                 QString("Failed to open [%1]. Do I have read permission?").arg(file));
            break;
        }
        auto name = QFileInfo(file).completeBaseName();
        if (snippetItem.contains(name) || SettingsManager::contains(snippetKey(name)))
            name = getNewSnippetName(name, true);
        if (name.isEmpty())
            break;
        addSnippet(name);
        SettingsManager::set(snippetKey(name), openFile.readAll());
    }
}

void CodeSnippetsPage::extractSnippetsToFiles()
{
    auto dirPath = QFileDialog::getExistingDirectory(this, "Extract Snippets");
    if (dirPath.isEmpty())
        return;
    auto dir = QDir(dirPath);

    auto names = SettingsManager::keyStartsWith(snippetKey(""));
    for (auto &name : names)
        name = name.mid(snippetKey("").length());

    QString suffix;
    if (lang == "C++")
        suffix = "." + Util::cppSuffix.front();
    else if (lang == "Java")
        suffix = "." + Util::javaSuffix.front();
    else if (lang == "Python")
        suffix = "." + Util::pythonSuffix.front();

    for (auto name : names)
    {
        auto filePath = dir.filePath(name + suffix);
        if (QFile::exists(filePath))
        {
            filePath =
                QFileDialog::getSaveFileName(this, "Extract Snippets: " + name, filePath,
                                             Util::fileNameFilter(lang == "C++", lang == "Java", lang == "Python"));
        }
        if (filePath.isEmpty())
            break;
        if (!Util::saveFile(filePath, SettingsManager::get(snippetKey(name)).toString(), "Extract Snippets"))
        {
            QMessageBox::warning(this, "Extract Snippets",
                                 QString("Failed to write to [%1]. Do I have write permission?").arg(filePath));
            break;
        }
    }
}

QString CodeSnippetsPage::getNewSnippetName(const QString &oldName, bool avoidConflictWithSettings)
{
    QString name;
    bool firstTime = true;
    while (name.isEmpty() || snippetItem.contains(name) ||
           (avoidConflictWithSettings && SettingsManager::contains(snippetKey(name))))
    {
        bool ok;
        QString head;
        if (firstTime)
        {
            firstTime = false;
            name = oldName;
        }
        else if (name.isEmpty())
            head = "Snippet name can't be empty.\n";
        else
            head = QString("The name \"%1\" is already in use.\n").arg(name);
        name = QInputDialog::getText(this, "Add Snippet", head + "New Snippet Name:", QLineEdit::Normal, name, &ok);
        if (!ok)
            return QString();
    }
    return name;
}

void CodeSnippetsPage::addSnippet(const QString &name)
{
    auto item = new QListWidgetItem(name);
    snippetsList->addItem(item);
    snippetItem[name] = item;

    updateActions();
    updateButtons();
}

void CodeSnippetsPage::deleteSnippet(QListWidgetItem *item)
{
    if (item)
    {
        if (item == currentItem)
        {
            int row = snippetsList->row(item);
            if (row + 1 < snippetsList->count())
                switchToSnippet(snippetsList->item(row + 1), true);
            else if (row >= 1)
                switchToSnippet(snippetsList->item(0), true);
            else
                switchToSnippet(nullptr, true);
        }
        snippetItem.remove(item->text());
        delete item;

        updateActions();
        updateButtons();
    }
}

QString CodeSnippetsPage::snippetKey(const QString &name)
{
    return QString("Snippets/%1/%2").arg(lang, name);
}

void CodeSnippetsPage::updateActions()
{
    deleteButton->setEnabled(currentItem != nullptr);
    renameAction->setEnabled(currentItem != nullptr);
    extractSnippetsToFilesAction->setEnabled(!snippetItem.isEmpty());
}
