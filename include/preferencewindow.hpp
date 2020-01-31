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
 * I will not be responsible if CPEditor behaves in an unexpected way and
 * causes your bad performance and / or lose any contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

#ifndef PREFERENCEWINDOW_HPP
#define PREFERENCEWINDOW_HPP

#include "SettingsManager.hpp"
#include <QCodeEditor>
#include <QListWidget>
#include <QMainWindow>

namespace Ui
{
class PreferenceWindow;
}

class PreferenceWindow : public QMainWindow
{
    Q_OBJECT

  signals:
    void settingsApplied();

  public:
    explicit PreferenceWindow(Settings::SettingManager *manager, QWidget *parent = nullptr);
    void updateShow();
    ~PreferenceWindow();

  private slots:

    void on_ok_clicked();

    void on_cancel_clicked();

    void on_apply_clicked();

    void on_hotkeys_clicked(bool checked);

    void on_font_button_clicked();

    void on_cpp_template_clicked();

    void on_py_template_clicked();

    void on_java_template_clicked();

    void on_load_snippets_from_files_clicked();

    void on_extract_snippets_to_files_clicked();

    void onSnippetsLangChanged(const QString &lang);

    void onCurrentSnippetChanged(const QString &text);

    void applySettingsToEditor();

    void on_snippet_save_clicked();

    void on_snippet_new_clicked();

    void on_snippet_delete_clicked();

    void on_snippet_rename_clicked();

    void on_transparency_slider_valueChanged(int value);

  private:
    Ui::PreferenceWindow *ui;
    QFont currentFont;
    QString cppTemplatePath, pythonTemplatePath, javaTemplatePath;
    Settings::SettingManager *manager;
    QCodeEditor *editor = nullptr; // for snippets

    void extractSettingsFromUi();
    void applySettingsToui();
    void setConstraints();
    void updateSnippets();
    void switchToSnippet(const QString &text);

    QString getNewSnippetName(const QString &lang, const QString &old = QString());
};

#endif // PREFERENCEWINDOW_HPP
