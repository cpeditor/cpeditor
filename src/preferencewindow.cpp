#include "include/preferencewindow.hpp"
#include "../ui/ui_preferencewindow.h"

#include <QAction>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QFontDialog>

PreferenceWindow::PreferenceWindow(Settings::SettingManager* manager ,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PreferenceWindow){
    ui->setupUi(this);
    this->manager = manager;
    setWindowTitle("Preferences");
    applySettingsToui();
    resize(QDesktopWidget().availableGeometry(this).size() * 0.35);
    setConstraints();
}

void PreferenceWindow::setConstraints(){
    ui->tab_length->setMinimum(1);
    ui->tab_length->setMaximum(20);

    ui->companion_port->setMinimum(10000);
    ui->companion_port->setMaximum(65535);
}

void PreferenceWindow::applySettingsToui()
{
    ui->tabWidget->setCurrentIndex(0);

    ui->systemwide_theme->setCurrentIndex(!manager->isSystemThemeDark());
    ui->editor_theme->setCurrentText(manager->getEditorTheme());
    ui->tab_length->setValue(manager->getTabStop());

    if(!manager->getFont().isEmpty()) currentFont.fromString(manager->getFont());
    ui->font_button->setText(currentFont.family() + " " + QString::number(currentFont.pointSize()));

    ui->savetest->setChecked(manager->isSaveTests());
    ui->wrap->setChecked(manager->isWrapText());
    ui->indent->setChecked(manager->isAutoIndent());
    ui->parentheses->setChecked(manager->isAutoParenthesis());
    ui->tabs->setChecked(manager->isTabs());

    ui->defaultLang->setCurrentText(manager->getDefaultLang());
    ui->format_cmd->setText(manager->getFormatCommand());

    ui->cpp_compiler_cmd->setText(manager->getCompileCommandCpp());
    ui->cpp_args_cmd->setText(manager->getRuntimeArgumentsCpp());

    ui->java_compiler_cmd->setText(manager->getCompileCommandJava());
    ui->java_args_cmd->setText(manager->getRuntimeArgumentsJava());
    ui->java_start_cmd->setText(manager->getRunCommandJava());

    ui->python_args_cmd->setText(manager->getRuntimeArgumentsPython());
    ui->python_start_cmd->setText(manager->getRunCommandPython());

    ui->companion_use->setChecked(manager->isCompetitiveCompanionActive());
    ui->companion_port->setValue(manager->getConnectionPort());
    ui->companion_contests->setChecked(manager->isCompanionParsingContests());

    cppTemplatePath = manager->getTemplatePathCpp();
    pythonTemplatePath = manager->getTemplatePathPython();
    javaTemplatePath = manager->getTemplatePathJava();

    ui->update_startup->setChecked(manager->isCheckUpdateOnStartup());
    ui->beta_update->setChecked(manager->isBeta());

    ui->cpp_template->setText(cppTemplatePath.isEmpty()? "<Not selected>" : "..." + cppTemplatePath.right(30));
    ui->py_template->setText(pythonTemplatePath.isEmpty()? "<Not selected>" : "..." + pythonTemplatePath.right(30));
    ui->java_template->setText(javaTemplatePath.isEmpty()? "<Not selected>" : "..." + javaTemplatePath.right(30));

    ui->hotkeys->setChecked(manager->isHotkeyInUse());
    on_hotkeys_clicked(manager->isHotkeyInUse());

    ui->compile_hotkey->setKeySequence(manager->getHotkeyCompile());
    ui->run_hotkey->setKeySequence(manager->getHotkeyRun());
    ui->format_hotkey->setKeySequence(manager->getHotkeyFormat());
    ui->compileRun_hotkey->setKeySequence(manager->getHotkeyCompileRun());
    ui->kill_hotkey->setKeySequence(manager->getHotkeyKill());

}


void PreferenceWindow::extractSettingsFromUi()
{
    manager->setSystemThemeDark(ui->systemwide_theme->currentText() == "Dark");
    manager->setEditorTheme(ui->editor_theme->currentText());
    manager->setTabStop(ui->tab_length->value());
    manager->setFont(currentFont.toString());

    manager->setSaveTests(ui->savetest->isChecked());
    manager->setWrapText(ui->wrap->isChecked());
    manager->setAutoIndent(ui->indent->isChecked());
    manager->setAutoParenthesis(ui->parentheses->isChecked());
    manager->setTabs(ui->tabs->isChecked());

    manager->setDefaultLanguage(ui->defaultLang->currentText());
    manager->setFormatCommand(ui->format_cmd->text());

    manager->setCompileCommandsCpp(ui->cpp_compiler_cmd->text());
    manager->setRuntimeArgumentsCpp(ui->cpp_args_cmd->text());

    manager->setCompileCommandsJava(ui->java_compiler_cmd->text());
    manager->setRuntimeArgumentsJava(ui->java_args_cmd->text());
    manager->setRunCommandJava(ui->java_start_cmd->text());

    manager->setRunCommandPython(ui->python_start_cmd->text());
    manager->setRuntimeArgumentsPython(ui->python_args_cmd->text());

    manager->setCompetitiveCompanionActive(ui->companion_use->isChecked());
    manager->setConnectionPort(ui->companion_port->value());
    manager->setCompetitiveCompanionParseContests(ui->companion_contests->isChecked());

    manager->setBeta(ui->beta_update->isChecked());
    manager->checkUpdateOnStartup(ui->update_startup->isChecked());

    manager->setTemplatePathCpp(cppTemplatePath);
    manager->setTemplatePathJava(javaTemplatePath);
    manager->setTemplatePathPython(pythonTemplatePath);

    manager->setHotKeyInUse(ui->hotkeys->isChecked());

    manager->setHotkeyRun(ui->run_hotkey->keySequence());
    manager->setHotkeyKill(ui->kill_hotkey->keySequence());
    manager->setHotkeyFormat(ui->format_hotkey->keySequence());
    manager->setHotkeyCompile(ui->compile_hotkey->keySequence());
    manager->setHotkeyCompileRun(ui->compileRun_hotkey->keySequence());
}

void PreferenceWindow::resetSettings(){
    QFont defaultFont;
    currentFont = defaultFont;

    manager->setConnectionPort(10045);
    manager->setTabStop(4);
    manager->setGeometry(QRect{});
    manager->setFont("");
    manager->setDefaultLanguage("Cpp");

    manager->setTemplatePathCpp("");
    manager->setTemplatePathJava("");
    manager->setTemplatePathPython("");

    manager->setRuntimeArgumentsCpp("");
    manager->setRuntimeArgumentsJava("");
    manager->setRuntimeArgumentsPython("");

    manager->setFormatCommand("clang-format -i");

    manager->setCompileCommandsJava("javac");
    manager->setCompileCommandsCpp("g++ -Wall");

    manager->setRunCommandJava("java");
    manager->setRunCommandPython("python");

    manager->setEditorTheme("Light");

    manager->setSystemThemeDark(false);
    manager->setHotKeyInUse(false);
    manager->setAutoParenthesis(true);
    manager->setAutoIndent(true);
    manager->setAutoSave(false);
    manager->setWrapText(false);
    manager->setBeta(false);
    manager->setTabs(false);
    manager->setSaveTests(false);
    manager->setCompetitiveCompanionActive(false);
    manager->setMaximizedWindow(false);
    manager->checkUpdateOnStartup(true);
    manager->setCompetitiveCompanionParseContests(false);

    manager->setHotkeyRun(QKeySequence());
    manager->setHotkeyKill(QKeySequence());
    manager->setHotkeyCompile(QKeySequence());
    manager->setHotkeyCompileRun(QKeySequence());
    manager->setHotkeyFormat(QKeySequence());
}

void PreferenceWindow::updateShow(){
    applySettingsToui();
    show();
}

PreferenceWindow::~PreferenceWindow()
{
    delete ui;
}


void PreferenceWindow::on_exit_clicked()
{
    close();
}

void PreferenceWindow::on_apply_clicked()
{
    extractSettingsFromUi();
    close();
    emit settingsApplied();
}

void PreferenceWindow::on_hotkeys_clicked(bool checked)
{
    ui->compile_hotkey->setEnabled(checked);
    ui->compileRun_hotkey->setEnabled(checked);
    ui->run_hotkey->setEnabled(checked);
    ui->kill_hotkey->setEnabled(checked);
    ui->format_hotkey->setEnabled(checked);
}

void PreferenceWindow::on_font_button_clicked()
{
    bool ok = false;
    QFont fp = QFontDialog::getFont(&ok, currentFont);

    if(ok)
    {
        currentFont = fp;
        ui->font_button->setText(currentFont.family() + " " + QString::number(currentFont.pointSize()));
    }
}

void PreferenceWindow::on_cpp_template_clicked()
{
   auto filename = QFileDialog::getOpenFileName(this, tr("Choose C++ template File"), "",
                                                    "C++ Files (*.cpp *.hpp *.h *.cc *.cxx *.c)");
   if (filename.isEmpty()) return;
   cppTemplatePath = filename;
   ui->cpp_template->setText("..."+ cppTemplatePath.right(30));
}

void PreferenceWindow::on_py_template_clicked()
{
    auto filename = QFileDialog::getOpenFileName(this, tr("Choose Python template File"), "",
                                                     "Python Files (*.py)");
    if(filename.isEmpty()) return ;
    pythonTemplatePath = filename;
    ui->py_template->setText("..."+ pythonTemplatePath.right(30));
}

void PreferenceWindow::on_java_template_clicked()
{
    auto filename = QFileDialog::getOpenFileName(this, tr("Choose Java template File"), "",
                                                     "Java Files (*.java)");
    if(filename.isEmpty()) return ;
    javaTemplatePath = filename;
    ui->java_template->setText("..."+ javaTemplatePath.right(30));
}
