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

#ifndef PARENTHESESPAGE_HPP
#define PARENTHESESPAGE_HPP

#include "Settings/PreferencesPage.hpp"

class QCheckBox;
class QListWidget;
class QListWidgetItem;
class QSplitter;
class QStackedWidget;

class ParenthesisWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit ParenthesisWidget(QString language, QChar leftParenthesis, QChar rightParenthesis,
                               Qt::CheckState autoComplete = Qt::PartiallyChecked,
                               Qt::CheckState autoRemove = Qt::PartiallyChecked,
                               Qt::CheckState tabJumpOut = Qt::PartiallyChecked, QWidget *parent = nullptr);

    /**
     * @brief dump the settings of the current parenthesis to a QVariantList
     */
    QVariantList toList() const;

    /**
     * @brief get the parenthesis in a QString form
     */
    QString parenthesis() const;

  signals:
    /**
     * @brief a signal, the settings are changed
     */
    void changed();

  private:
    /**
     * - mainLayout
     *   - nameLabel
     *   - (spacing)
     *   - stretchLayout
     *     - (stretch)
     *     - checkBoxesLayout
     *       - autoCompleteCheckBox
     *       - autoRemoveCheckBox
     *       - tabJumpOutCheckBox
     *     - (stretch)
     *   - (stretch)
     */

    QLabel *nameLabel = nullptr;
    QCheckBox *autoCompleteCheckBox = nullptr;
    QCheckBox *autoRemoveCheckBox = nullptr;
    QCheckBox *tabJumpOutCheckBox = nullptr;

    QString lang;
    QChar left;
    QChar right;
};

class ParenthesesPage : public PreferencesPage
{
    Q_OBJECT

  public:
    explicit ParenthesesPage(const QString &language, QWidget *parent = nullptr);

  private:
    bool areSettingsChanged() override;

    void makeUITheSameAsDefault() override;

    void makeUITheSameAsSettings() override;

    void makeSettingsTheSameAsUI() override;

  private slots:
    /**
     * @brief ask for a parenthesis and add this new parenthesis
     */
    void addParenthesis();

    /**
     * @brief ask for confirmation and delete the current parenthesis
     */
    void deleteCurrentParenthesis();

    /**
     * @brief switch to a parenthesis
     * @param item the item to switch to, nullptr for switching to "empty"
     */
    void switchToParenthesis(QListWidgetItem *item);

  private:
    /**
     * @brief add a given parenthesis
     * @param left the left part of the parenthesis
     * @param right the right part of the parenthesis
     */
    void addParenthesis(QChar left, QChar right, Qt::CheckState autoComplete = Qt::PartiallyChecked,
                        Qt::CheckState autoRemove = Qt::PartiallyChecked,
                        Qt::CheckState tabJumpOut = Qt::PartiallyChecked);

    /**
     * @brief delete a parenthesis without confirmation
     * @param index the index to delete, the index of a parenthesis starts from 1
     */
    void deleteParenthesis(int index);

    /**
     * @brief switch to a parenthesis
     * @param index the index to switch to, the index of a parenthesis starts from 1, 0 is for switching to "empty"
     */
    void switchToParenthesis(int index);

    /**
     * @brief get a parenthesis widget
     * @param index the index of the parenthesis, it starts from 1
     */
    ParenthesisWidget *parenthesisWidget(int index) const;

    /**
     * @brief get the key of the parentheses of the current language in the settings
     */
    QString settingsKey() const;

    /**
     * @brief dump the settings on the current page to a QVariantList
     */
    QVariantList toList() const;

    /**
     * @brief load the settings from a QVariantList
     */
    void loadList(const QVariantList &list);

  private:
    /**
     * splitter
     *   - leftWidget
     *     - leftLayout
     *       - listWidget
     *       - buttonsLayout
     *         - addButton
     *         - delButton
     *   - stackedWidget
     *     - (parenthesisWidget)
     *     - ...
     *     - (parenthesisWidget)
     *     - noParenthesisWidget
     *       - noParenthesisLayout
     *         - (stretch)
     *         - noParenthesisStretchLayout
     *           - (stretch)
     *           - noParenthesisLabel
     *           - (stretch)
     *         - (stretch)
     */

    QSplitter *splitter = nullptr;
    QWidget *leftWidget = nullptr;
    QListWidget *listWidget = nullptr;
    QPushButton *addButton = nullptr;
    QPushButton *delButton = nullptr;
    QStackedWidget *stackedWidget = nullptr;

    QString lang;
};

#endif // PARENTHESESPAGE_HPP
