#include "include/preferencewindow.hpp"
#include "../ui/ui_preferencewindow.h"

PreferenceWindow::PreferenceWindow(Settings::SettingManager* manager ,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PreferenceWindow){
    ui->setupUi(this);
    this->manager = manager;
    setWindowTitle("Preferences");
}

void PreferenceWindow::resetSettings(){
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
    manager->setTabs(true);
    manager->setSaveTests(false);
    manager->setCompetitiveCompanionActive(false);
    manager->setMaximizedWindow(false);
}

PreferenceWindow::~PreferenceWindow(){
    delete ui;
}


void PreferenceWindow::on_exit_clicked()
{
    close();
}

void PreferenceWindow::on_apply_clicked()
{
    emit settingsApplied();
    close();
}
