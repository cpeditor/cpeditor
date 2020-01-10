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
    void resetSettings();
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

    void on_save_snippet_clicked();

    void on_new_snippet_clicked();

    void on_delete_snippet_clicked();

    void on_rename_snippet_clicked();

    void on_snippets_lang_changed(const QString &lang);

    void on_current_snippet_changed(const QString &text);

    void applySettingsToEditor();

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
};

#endif // PREFERENCEWINDOW_HPP
