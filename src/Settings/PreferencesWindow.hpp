/*
 * Copyright (C) 2019-2021 Ashar Khan <ashar786khan@gmail.com>
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

#include <QMainWindow>
#include <QMap>

class PreferencesHomePage;
class PreferencesPage;
class QHBoxLayout;
class QLineEdit;
class QPushButton;
class QShortcut;
class QSplitter;
class QStackedWidget;
class QTreeWidget;
class QTreeWidgetItem;
class QVBoxLayout;

class PreferencesWindow;

class AddPageHelper
{
  public:
    explicit AddPageHelper(PreferencesWindow *w);

    AddPageHelper &page(const QString &key, const QString &trkey, const QStringList &content, bool alignTop = true);
    AddPageHelper &page(const QString &key, const QString &trkey, PreferencesPage *newpage);
    AddPageHelper &page(const QString &key, const QString &trkey, PreferencesPage *newpage, const QStringList &content);

    AddPageHelper &dir(const QString &key, const QString &trkey);

    AddPageHelper &end();

    void ensureAtTop() const;

  private:
    bool atTop() const;

    QString pathFor(const QString &key);
    QString trPathFor(const QString &trkey);

    PreferencesWindow *window;
    QTreeWidget *tree;
    QTreeWidgetItem *currentItem = nullptr;
    QStringList currentPath, currentTrPath;
};

class PreferencesWindow : public QMainWindow
{
    Q_OBJECT

    friend class AddPageHelper;

  public:
    /**
     * @brief construct a preferences window
     */
    explicit PreferencesWindow(QWidget *parent);

    /**
     * @brief check if *pagePath* is a valid settings page path
     */
    bool pathExists(const QString &pagePath) const;

  public slots:
    /**
     * @brief switch to home and show up
     */
    void display();

    /**
     * @brief switch to a certain path and show up
     * @param path the path of the setting to be displayed
     */
    void open(const QString &path);

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
    void registerName(const QString &key, const QString &trkey);

    void addPage(QTreeWidgetItem *item, PreferencesPage *page, const QStringList &pageContent);

    /**
     * @brief switch to a page
     * @param page the target page, can be either a preferences page or home page
     * @param force if force, user won't be asked whether to save changes or not
     * @note If there are unsaved changes in the current page, the user will be asked whether to save/discard
     *       the changes or stay. If the user chooses to stay, the switch operation will fail.
     * @returns whether the switch operation suceeded or not, i.e. if the current page is *page*
     */
    bool switchToPage(QWidget *page, bool force = false);

    /**
     * @brief get the page widget to the page of the given path
     * @param pagePath the path to the page
     * @param allowPrefix if a prefix of *pagePath* is a page, return it
     * @returns returns the widget if it's found, otherwise returns nullptr
     */
    PreferencesPage *getPageWidget(const QString &pagePath, bool allowPrefix) const;

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
    QTreeWidgetItem *getTopLevelItem(const QString &text) const;

    /**
     * @brief get the child of *item* with text *text*
     * @returns the child or nullptr if not found
     */
    static QTreeWidgetItem *getChild(QTreeWidgetItem *item, const QString &text);

    /**
     * @brief get the index of the next/previous non-hidden page (including the home page)
     * @param index the index of the current page in the stackedWidget
     * @param direction either 1 or -1, 1 for the next non-hidden page and -1 for the previous non-hidden-page
     * @param includingSelf when it's true, the result can be the current page
     * @returns the index of the (strictly) next/previous non-hidden page in the stackedWidget
     */
    int nextNonHiddenPage(int index, int direction = 1, bool includingSelf = false) const;

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

    QWidget *leftWidget = nullptr;
    QLineEdit *searchEdit = nullptr;
    QPushButton *homeButton = nullptr;
    QTreeWidget *menuTree = nullptr;
    QStackedWidget *stackedWidget = nullptr;

    PreferencesHomePage *homePage = nullptr;

    QMap<QTreeWidgetItem *, QStringList> content;
    QMap<QTreeWidgetItem *, PreferencesPage *> pageWidget;
    QMap<PreferencesPage *, QTreeWidgetItem *> pageTreeItem;
    QMap<QString, QString> treeEntryTranslation;

    QShortcut *exitShortcut = nullptr;
    QShortcut *travelShortcut = nullptr;
    QShortcut *travelBackShortcut = nullptr;
};

#endif // PREFERENCESWINDOW_HPP
