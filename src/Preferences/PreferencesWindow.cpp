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

#include "Preferences/PreferencesWindow.hpp"
#include <QApplication>
#include <QCloseEvent>
#include <QScreen>

PreferencesWindow::PreferencesWindow(QWidget *parent) : QMainWindow(parent)
{
    // set attributes
    hide();
    resize(QApplication::screenAt(parent->pos())->geometry().size() / 1.5);
    setWindowTitle("Preferences");

    // setup UI
    splitter = new QSplitter();
    splitter->setChildrenCollapsible(false);
    setCentralWidget(splitter);

    leftWidget = new QWidget();
    splitter->addWidget(leftWidget);

    leftLayout = new QVBoxLayout(leftWidget);

    searchLayout = new QHBoxLayout();
    leftLayout->addLayout(searchLayout);

    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Search...");
    connect(searchEdit, SIGNAL(textChanged(const QString &)), this, SLOT(updateSearch(const QString &)));
    searchLayout->addWidget(searchEdit);

    homeButton = new QPushButton("Home");
    homeButton->setDisabled(true);
    connect(homeButton, &QPushButton::clicked, this, [this]() { switchToPage(homePage); });
    searchLayout->addWidget(homeButton);

    menuTree = new QTreeWidget();
    menuTree->setHeaderHidden(true);
    connect(menuTree, &QTreeWidget::itemActivated, [this](QTreeWidgetItem *item) { switchToPage(pageWidget[item]); });
    leftLayout->addWidget(menuTree);

    stackedWidget = new QStackedWidget();
    splitter->addWidget(stackedWidget);

    homePage = new PreferencesHomePage();
    connect(homePage, &PreferencesHomePage::requestPage,
            [this](const QString &path) { switchToPage(getPageWidget(path)); });
    stackedWidget->addWidget(homePage);

    splitter->setSizes({1, 3});

    // add pages
}

void PreferencesWindow::display()
{
    switchToPage(homePage, true);
    show();
    setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    activateWindow();
    raise();
}

void PreferencesWindow::updateSearch(const QString &text)
{
    for (int i = 0; i < menuTree->topLevelItemCount(); ++i)
    {
        updateSearch(menuTree->topLevelItem(i), text);
    }
}

void PreferencesWindow::switchToPage(QWidget *page, bool force)
{
    // return if page is nullptr
    if (page == nullptr)
        return;

    // return if it's no need to switch
    if (stackedWidget->currentWidget() == page)
        return;

    // ask for saving changes or not if not force
    if (!force)
    {
        auto current = dynamic_cast<PreferencesPage *>(stackedWidget->currentWidget());
        if (current != nullptr && !current->aboutToExit())
            return;
    }

    // disable home button when it's already at home
    homeButton->setDisabled(page == homePage);

    // switch if everything is OK
    stackedWidget->setCurrentWidget(page);
}

void PreferencesWindow::addPage(const QString &path, PreferencesPage *page, const QStringList &pageContent)
{
    auto parts = path.split('/');

    QTreeWidgetItem *current = nullptr;

    // get the top level item
    for (int i = 0; i < menuTree->topLevelItemCount(); ++i)
    {
        auto item = menuTree->topLevelItem(i);
        if (item->text(0) == parts.front())
        {
            current = item;
            break;
        }
    }

    // add if not exists
    if (current == nullptr)
    {
        current = new QTreeWidgetItem({parts.front()});
        menuTree->addTopLevelItem(current);
    }

    // get non-top-level items step by step
    for (int i = 1; i < parts.count(); ++i)
    {
        QTreeWidgetItem *nxt = nullptr;
        for (int j = 0; j < current->childCount(); ++j)
        {
            auto child = current->child(j);
            if (child->text(0) == parts[i])
            {
                nxt = child;
                break;
            }
        }

        // add if not exists
        if (nxt == nullptr)
        {
            nxt = new QTreeWidgetItem({parts[i]});
            current->addChild(nxt);
        }

        current = nxt;
    }

    content[current] = pageContent;
    pageWidget[current] = page;
    stackedWidget->addWidget(page);
    connect(page, SIGNAL(settingsApplied(const QString &)), this, SIGNAL(settingsApplied(const QString &)));
}

PreferencesPage *PreferencesWindow::getPageWidget(const QString &pagePath)
{
    auto parts = pagePath.split('/');

    QTreeWidgetItem *current = nullptr;

    // get the top level item
    for (int i = 0; i < menuTree->topLevelItemCount(); ++i)
    {
        auto item = menuTree->topLevelItem(i);
        if (item->text(0) == parts.front())
        {
            current = item;
            break;
        }
    }

    if (current == nullptr)
        return nullptr;

    // get the non-top-level items step by step
    for (int i = 1; i < parts.count(); ++i)
    {
        QTreeWidgetItem *nxt = nullptr;
        for (int j = 0; j < current->childCount(); ++j)
        {
            auto child = current->child(j);
            if (child->text(0) == parts[i])
            {
                nxt = child;
                break;
            }
        }
        if (nxt == nullptr)
            return nullptr;
        current = nxt;
    }

    return pageWidget[current];
}

void PreferencesWindow::closeEvent(QCloseEvent *event)
{
    auto current = dynamic_cast<PreferencesPage *>(stackedWidget->currentWidget());
    if (current != nullptr && !current->aboutToExit())
        event->ignore();
    else
        event->accept();
}

void PreferencesWindow::updateSearch(QTreeWidgetItem *item, const QString &text)
{
    if (item == nullptr)
        return;

    if (text.isEmpty())
    {
        // If text is empty, set all items visible
        item->setHidden(false);
        for (int i = 0; i < item->childCount(); ++i)
            updateSearch(item->child(i), text);
        return;
    }

    bool shouldHide = true;

    if (item->childCount() == 0)
    {
        // check whether the content matches text in the leaves
        for (auto s : content[item])
        {
            if (s.contains(text))
            {
                shouldHide = false;
                break;
            }
        }
    }

    for (int i = 0; i < item->childCount(); ++i)
    {
        auto child = item->child(i);
        updateSearch(child, text);
        // If at least one child is not hidden, a non-leaf item is not hidden
        if (!child->isHidden())
            shouldHide = false;
    }

    item->setHidden(shouldHide);
}
