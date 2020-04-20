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

#include "Settings/PreferencesWindow.hpp"
#include "Settings/AppearancePage.hpp"
#include "Settings/CodeSnippetsPage.hpp"
#include "Settings/PreferencesPageTemplate.hpp"
#include <QApplication>
#include <QCloseEvent>
#include <QScreen>

PreferencesWindow::PreferencesWindow(QWidget *parent) : QMainWindow(parent)
{
    // set attributes
    hide();
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
    homeButton->setToolTip("Go to the home page");
    connect(homeButton, &QPushButton::clicked, this, [this]() { switchToPage(homePage); });
    searchLayout->addWidget(homeButton);

    menuTree = new QTreeWidget();
    menuTree->setHeaderHidden(true);
    connect(menuTree, &QTreeView::clicked, [this](const QModelIndex &index) {
        menuTree->isExpanded(index) ? menuTree->collapse(index) : menuTree->expand(index);
    });
    connect(menuTree, &QTreeWidget::itemActivated, [this](QTreeWidgetItem *item) { switchToPage(pageWidget[item]); });
    connect(menuTree, &QTreeWidget::itemClicked, [this](QTreeWidgetItem *item) { switchToPage(pageWidget[item]); });
    leftLayout->addWidget(menuTree);

    stackedWidget = new QStackedWidget();
    splitter->addWidget(stackedWidget);

    homePage = new PreferencesHomePage();
    connect(homePage, &PreferencesHomePage::requestPage,
            [this](const QString &path) { switchToPage(getPageWidget(path)); });
    stackedWidget->addWidget(homePage);

    splitter->setSizes({10000, 30000});

    exitShortcut = new QShortcut(QKeySequence::Cancel, this);
    connect(exitShortcut, SIGNAL(activated()), this, SLOT(close()));

    travelShortcut = new QShortcut({"Ctrl+Tab"}, this);
    connect(travelShortcut, &QShortcut::activated, [this]() {
        switchToPage(stackedWidget->widget((stackedWidget->currentIndex() + 1) % stackedWidget->count()));
    });

    travelBackShortcut = new QShortcut({"Ctrl+Shift+Tab"}, this);
    connect(travelBackShortcut, &QShortcut::activated, [this]() {
        switchToPage(stackedWidget->widget((stackedWidget->currentIndex() + stackedWidget->count() - 1) %
                                           stackedWidget->count()));
    });

    // add pages

    addPage("Code Edit", {"Tab Width", "Auto Indent", "Wrap Text", "Auto Complete Parentheses",
                          "Auto Remove Parentheses", "Replace Tabs"});

    addPage("Language/General", {"Default Language"});

    addPage("Language/Commands",
            {"C++/Compile Command", "C++/Run Arguments", "Java/Compile Command", "Java/Run Arguments",
             "Java/Run Command", "Python/Run Arguments", "Python/Run Command"});

    addPage("Language/Code Template", {"C++/Template Path", "Java/Template Path", "Python/Template Path"});

    auto cppSnippetsPage = new CodeSnippetsPage("C++");
    auto javaSnippetsPage = new CodeSnippetsPage("Java");
    auto pythonSnippetsPage = new CodeSnippetsPage("Python");
    addPage("Language/Code Snippets/C++ Snippets", cppSnippetsPage,
            {"C++ Snippets", "C++ Code Snippets", "Cpp Snippets", "Cpp Code Snippets"});
    addPage("Language/Code Snippets/Java Snippets", javaSnippetsPage, {"Java Snippets", "Java Code Snippets"});
    addPage("Language/Code Snippets/Python Snippets", pythonSnippetsPage, {"Python Snippets", "Python Code Snippets"});

    auto appearancePage = new AppearancePage();
    addPage("Appearance", appearancePage, appearancePage->content());

    addPage("Actions/General", {"Hot Exit/Enable"});

    addPage("Actions/Save", {"Auto Save", "Save Faster", "Auto Format", "Save Tests"});

    addPage("Extensions/Clang Format", {"Clang Format/Path", "Clang Format/Style"}, false);

    addPage("Extensions/Competitive Companion", {"Competitive Companion/Enable", "Competitive Companion/Open New Tab",
                                                 "Competitive Companion/Connection Port"});

    addPage("Extensions/CF Tool", {"CF/Path"});

    addPage("Key Bindings", {"Hotkey/Compile", "Hotkey/Run", "Hotkey/Compile Run", "Hotkey/Format", "Hotkey/Kill",
                             "Hotkey/Change View Mode", "Hotkey/Snippets"});

    addPage("Advanced/Update", {"Check Update", "Beta"});

    addPage("Advanced/Limits",
            {"Time Limit", "Output Length Limit", "Message Length Limit", "HTML Diff Viewer Length Limit",
             "Open File Length Limit", "Load Test Case File Length Limit"});

    addPage("Test", {"Testcase Scan Rule"});
}

void PreferencesWindow::display()
{
    bool hidden = isHidden();
    show();
    setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    activateWindow();
    raise();
    if (hidden)
    {
        switchToPage(homePage);
        menuTree->clearSelection();
        menuTree->collapseAll();
        searchEdit->clear();
        searchEdit->setFocus();
        move(parentWidget()->geometry().center().x() - width() / 2,
             parentWidget()->geometry().center().y() - height() / 2);
    }
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

    auto preferencesPage = dynamic_cast<PreferencesPage *>(page);
    if (preferencesPage != nullptr)
    {
        menuTree->setCurrentItem(pageTreeItem[preferencesPage]);
        preferencesPage->loadSettings();
    }
    else
    {
        menuTree->clearSelection();
    }
}

void PreferencesWindow::addPage(const QString &path, PreferencesPage *page, const QStringList &pageContent)
{
    auto parts = path.split('/');

    QTreeWidgetItem *current = getTopLevelItem(parts.front());

    // add if not exists
    if (current == nullptr)
    {
        current = new QTreeWidgetItem({parts.front()});
        menuTree->addTopLevelItem(current);
    }

    // get non-top-level items step by step
    for (int i = 1; i < parts.count(); ++i)
    {
        QTreeWidgetItem *nxt = getChild(current, parts[i]);

        // add if not exists
        if (nxt == nullptr)
        {
            nxt = new QTreeWidgetItem({parts[i]});
            current->addChild(nxt);
        }

        current = nxt;
    }

    page->setPath(path);
    content[current] = pageContent;
    pageWidget[current] = page;
    pageTreeItem[page] = current;
    stackedWidget->addWidget(page);
    connect(page, SIGNAL(settingsApplied(const QString &)), this, SIGNAL(settingsApplied(const QString &)));
}

void PreferencesWindow::addPage(const QString &path, const QStringList &opts, bool alignTop)
{
    auto page = new PreferencesPageTemplate(opts, alignTop);
    addPage(path, page, page->content());
}

PreferencesPage *PreferencesWindow::getPageWidget(const QString &pagePath)
{
    auto parts = pagePath.split('/');

    QTreeWidgetItem *current = getTopLevelItem(parts.front());

    if (current == nullptr)
        return nullptr;

    // get the non-top-level items step by step
    for (int i = 1; i < parts.count(); ++i)
    {
        QTreeWidgetItem *nxt = getChild(current, parts[i]);
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

    item->setExpanded(!text.isEmpty());

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
        // check whether the leaf should be hidden or not
        if (pageWidget[item]->path().contains(text, Qt::CaseInsensitive)) // check whether the path contains text
        {
            shouldHide = false;
        }
        else
        {
            // check whether the content contains text
            for (auto s : content[item])
            {
                if (s.contains(text, Qt::CaseInsensitive))
                {
                    shouldHide = false;
                    break;
                }
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

QTreeWidgetItem *PreferencesWindow::getTopLevelItem(const QString &text)
{
    for (int i = 0; i < menuTree->topLevelItemCount(); ++i)
    {
        auto item = menuTree->topLevelItem(i);
        if (item->text(0) == text)
        {
            return item;
        }
    }
    return nullptr;
}

QTreeWidgetItem *PreferencesWindow::getChild(QTreeWidgetItem *item, const QString &text)
{
    for (int i = 0; i < item->childCount(); ++i)
    {
        auto child = item->child(i);
        if (child->text(0) == text)
        {
            return child;
        }
    }
    return nullptr;
}
