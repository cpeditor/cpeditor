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

#include <Core/EventLogger.hpp>
#include <QApplication>
#include <QCloseEvent>
#include <QComboBox>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QToolButton>
#include <QVBoxLayout>
#include <Widgets/ContestDialog.hpp>
#include <generated/SettingsHelper.hpp>
#include <QMessageBox>

namespace Widgets
{
ContestDialog::ContestDialog(QWidget *parent) : QDialog(parent)
{
    allocate();
    restrictWidgets();
    setupUI();
    setWindowTitle(tr("Create a new contest"));

    QObject::connect(reset, &QPushButton::clicked, [this]() {
        pathLineEdit->clear();
        contestNameLineEdit->clear();
        problemCountSpinBox->setValue(5);
        languageComboBox->setCurrentText(SettingsHelper::getDefaultLanguage());
    });

    QObject::connect(cancel, &QPushButton::clicked, [this]() { close(); });

    QObject::connect(create, &QPushButton::clicked, [this]() {
    
        if (!validate())
        {
            QMessageBox::warning(this, tr("Cannot create contest"), validationErrorMessage);
        }
        else
        {
            ContestDialog::ContestData data;
            data.language = languageComboBox->currentText();
            data.number = problemCountSpinBox->value();
            data.path = QDir(pathLineEdit->text().append(QDir::separator()).append(contestNameLineEdit->text())).absolutePath();
            emit onContestCreated(data);
            close();
        }
    });

    QObject::connect(pathToolButton, &QToolButton::clicked, [this]() {
        auto path = QFileDialog::getExistingDirectory(this, tr("Choose Contest Root"));
        if (!path.isEmpty())
            pathLineEdit->setText(path);
    });
}

ContestDialog::ContestData &ContestDialog::contestData()
{
    return _data;
}

void ContestDialog::closeEvent(QCloseEvent *e)
{
    SettingsHelper::setLastRootPath(pathLineEdit->text());
    e->accept();
}

void ContestDialog::updateContestDialog()
{
    languageComboBox->setCurrentText(SettingsHelper::getDefaultLanguage());
    pathLineEdit->setText(SettingsHelper::getLastRootPath());
}

ContestDialog::~ContestDialog()
{
    delete pathToolButton;
    delete pathLineEdit;
    delete pathItem;
    delete contestNameLineEdit;
    delete languageComboBox;
    delete problemCountSpinBox;
    delete formLayout;
    delete groupBox;
    delete cancel;
    delete reset;
    delete create;
    delete actionButtons;
    delete parentLayout;
}

// Private methods
void ContestDialog::allocate()
{
    groupBox = new QGroupBox(tr("Contest Details"), this);
    formLayout = new QFormLayout(this);
    problemCountSpinBox = new QSpinBox(this);
    languageComboBox = new QComboBox(this);
    contestNameLineEdit = new QLineEdit(this);
    pathLineEdit = new QLineEdit(this);
    pathToolButton = new QToolButton;
    pathItem = new QHBoxLayout(this);

    actionButtons = new QHBoxLayout(this);
    reset = new QPushButton(tr("Reset"), this);
    cancel = new QPushButton(tr("Cancel"), this);
    create = new QPushButton(tr("Create"), this);

    parentLayout = new QVBoxLayout(this);
}

void ContestDialog::restrictWidgets()
{
    problemCountSpinBox->setMinimum(1);
    problemCountSpinBox->setMaximum(20);
    problemCountSpinBox->setValue(5); // default

    languageComboBox->addItems({"C++", "Java", "Python"});
    languageComboBox->setMaxVisibleItems(3);
    languageComboBox->setCurrentText(SettingsHelper::getDefaultLanguage());
}

bool ContestDialog::validate()
{
    validationErrorMessage.clear();
    QDir dir(pathLineEdit->text());
    if (!dir.exists())
    {
        validationErrorMessage = QString(tr("Contest path [ %1 ] does not exists").arg(pathLineEdit->text()));

        return false;
    }

    if (!dir.mkpath(contestNameLineEdit->text()))
    {
        validationErrorMessage = QString(tr("Cannot create directory with name [ %1 ]").arg(contestNameLineEdit->text()));
        return false;
    }
    return true;
}

void ContestDialog::setupUI()
{
    pathItem->setContentsMargins(0, 0, 0, 0);
    pathItem->addWidget(pathLineEdit);
    pathItem->addWidget(pathToolButton);

    formLayout->addRow(tr("Contest Root"), pathItem);
    formLayout->addRow(tr("Contest Name"), contestNameLineEdit);
    formLayout->addRow(tr("Contest language"), languageComboBox);
    formLayout->addRow(tr("Number of problems"), problemCountSpinBox);

    groupBox->setLayout(formLayout);

    actionButtons->addWidget(cancel, 2);
    actionButtons->addWidget(reset, 2);
    actionButtons->addWidget(create, 6);

    parentLayout->addWidget(groupBox);
    parentLayout->addLayout(actionButtons);

    setLayout(parentLayout);

    pathToolButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogOpenButton));
}

} // namespace Widgets
