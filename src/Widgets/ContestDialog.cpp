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
#include <QComboBox>
#include <QDir>
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

namespace Widgets
{
ContestDialog::ContestDialog(QWidget *parent) : QDialog(parent)
{
    allocate();
    restrictWidgets();
    setupUI();

    QObject::connect(reset, &QPushButton::clicked, []() {});
    QObject::connect(cancel, &QPushButton::clicked, []() {});
    QObject::connect(create, &QPushButton::clicked, []() {});

    QObject::connect(pathToolButton, &QToolButton::clicked, []() {});
}

ContestDialog::ContestData &ContestDialog::contestData()
{
    return _data;
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
    delete reset;
    delete create;
    delete cancel;
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

    actionButtons = new QVBoxLayout(this);
    reset = new QPushButton(tr("Reset"), this);
    cancel = new QPushButton(tr("Cancel"), this);
    create = new QPushButton(tr("Create"), this);

    parentLayout = new QHBoxLayout(this);
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
        validationErrorMessage = QString(tr("Contest path %1 does not exists").arg(pathLineEdit->text()));

        return false;
    }

    if (!dir.mkpath(contestNameLineEdit->text()))
    {
        validationErrorMessage = QString(tr("Cannot create directory with name %1").arg(contestNameLineEdit->text()));
        return false;
    }
    return true;
}

void ContestDialog::setupUI()
{
    pathItem->setContentsMargins(0, 0, 0, 0);
    pathItem->addWidget(pathLineEdit);
    pathItem->addWidget(pathToolButton);

    formLayout->addRow(tr("Contest Root Path"), pathItem);
    formLayout->addRow(tr("Contest Name"), contestNameLineEdit);
    formLayout->addRow(tr("Contest language"), languageComboBox);
    formLayout->addRow(tr("Number of problems"), problemCountSpinBox);

    groupBox->setLayout(formLayout);

    actionButtons->addWidget(cancel, 0, Qt::AlignRight);
    actionButtons->addWidget(reset, 0, Qt::AlignRight);
    actionButtons->addWidget(create, 0, Qt::AlignRight);

    parentLayout->addWidget(groupBox);
    parentLayout->addLayout(actionButtons);

    setLayout(parentLayout);
}

} // namespace Widgets
