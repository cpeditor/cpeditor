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

/*
 * This is the main window for preferences.
 * It's constructed of PreferencesPages.
 * It use QTreeWidget to organize the settings.
 * You can search the settings by keywords.
 */

#ifndef PREFERENCESWINDOW_HPP
#define PREFERENCESWINDOW_HPP

#include "Preferences/PreferencesHomePage.hpp"
#include "Preferences/PreferencesPage.hpp"
#include "Preferences/PreferencesPageTemplate.hpp"
#include <QLineEdit>
#include <QMainWindow>
#include <QMap>
#include <QShortcut>
#include <QSplitter>
#include <QStackedWidget>
#include <QTreeWidget>

class PreferencesWindow : public QMainWindow
{
    Q_OBJECT

  public:
    /**
     * @brief construct a preferences window
     */
    explicit PreferencesWindow(QWidget *parent);

  public slots:
    /**
     * @brief switch to home and show up
     */
    void display();

  signals:
    /**
     * @brief the settings are applied
     * @param pagePath the path to the applied page
     */
    void settingsApplied(const QString &pagePath);

  private slots:
    /**
     * @brief show/hide items in the menu according to the new text
     */
    void updateSearch(const QString &text);

  private:
    /**
     * @brief add a preferences page
     * @param path the path to the page, for example: "Actions/Save" / "Appearance"
     * @param page the PreferencesPage widget
     * @param pageContent the content of the page, used for search
     */
    void addPage(const QString &path, PreferencesPage *page, const QStringList &pageContent);

    /**
     * @brief add a preferences page
     * @param path the path to the page, for example: "Actions/Save" / "Appearance"
     * @param page the PreferencesPage widget
     * pageContent will use page->content()
     */
    void addPage(const QString &path, PreferencesPageTemplate *page);

    /**
     * @brief switch to a page
     * @param page the target page, can be either a preferences page or home page
     * @param force if force, user won't be asked whether to save changes or not
     * @note If there are unsaved changes in the current page, the user will be asked whether to save/discard
     *       the changes or stay. If the user chooses to stay, the switch operation will fail.
     */
    void switchToPage(QWidget *page, bool force = false);

    /**
     * @brief get the page widget to the page of the given path
     * @param pagePath the path to the page
     * @returns returns the widget if it's found, otherwise returns nullptr
     */
    PreferencesPage *getPageWidget(const QString &pagePath);

    /**
     * @brief if there are unsaved changes, ask the user to save/discard the changes or cancel the close
     */
    void closeEvent(QCloseEvent *event) override;

    /**
     * @brief show/hide items in the menu according to the new text recursively
     * @param item the current item
     * @param text the search text
     */
    void updateSearch(QTreeWidgetItem *item, const QString &text);

    /**
     * @brief get the top level item with the text *text*
     * @returns the top level item or nullptr if not found
     */
    QTreeWidgetItem *getTopLevelItem(const QString &text);

    /**
     * @brief get the child of *item* with text *text*
     * @returns the child or nullptr if not found
     */
    QTreeWidgetItem *getChild(QTreeWidgetItem *item, const QString &text);

    /**
     * The GUI:
     *
     * - splitter
     *   - leftWidget
     *     - leftLayout
     *       - searchLayout
     *         - searchEdit
     *         - homeButton
     *     - menuTree
     *       - ...
     *         - ...
     *       - ...
     *         - ...
     *   - stackedWidget
     *     - xxx
     */

    QSplitter *splitter = nullptr;
    QWidget *leftWidget = nullptr;
    QVBoxLayout *leftLayout = nullptr;
    QHBoxLayout *searchLayout = nullptr;
    QLineEdit *searchEdit = nullptr;
    QPushButton *homeButton = nullptr;
    QTreeWidget *menuTree = nullptr;
    QStackedWidget *stackedWidget = nullptr;

    PreferencesHomePage *homePage = nullptr;

    QMap<QTreeWidgetItem *, QStringList> content;
    QMap<QTreeWidgetItem *, PreferencesPage *> pageWidget;
    QMap<PreferencesPage *, QTreeWidgetItem *> pageTreeItem;

    QShortcut *exitShortcut = nullptr;
    QShortcut *travelShortcut = nullptr;
    QShortcut *travelBackShortcut = nullptr;
};

#endif // PREFERENCESWINDOW_HPP
