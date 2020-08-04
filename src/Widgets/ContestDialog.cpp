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

#include "Widgets/ContestDialog.hpp"
#include "Core/EventLogger.hpp"
#include "Settings/DefaultPathManager.hpp"
#include "generated/SettingsHelper.hpp"
#include <QApplication>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QDir>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QToolButton>
#include <QVBoxLayout>

namespace Widgets
{

ContestDialog::ContestDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("Create a new contest"));

    auto mainLayout = new QVBoxLayout(this);

    auto groupBox = new QGroupBox(tr("Contest Details"));
    mainLayout->addWidget(groupBox);

    auto formLayout = new QFormLayout(groupBox);

    auto mainDirectoryLayout = new QHBoxLayout();
    mainDirectoryLayout->setContentsMargins(0, 0, 0, 0);
    formLayout->addRow(tr("Main Directory"), mainDirectoryLayout);

    mainDirectoryEdit = new QLineEdit();
    mainDirectoryLayout->addWidget(mainDirectoryEdit);

    auto pathToolButton = new QToolButton();
    connect(pathToolButton, &QToolButton::clicked, this, &ContestDialog::chooseDirectory);
    pathToolButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogOpenButton));
    mainDirectoryLayout->addWidget(pathToolButton);

    subdirectoryEdit = new QLineEdit();
    formLayout->addRow(tr("Subdirectory"), subdirectoryEdit);

    contestPathLabel = new QLabel();
    contestPathLabel->setMinimumWidth(50);
    contestPathLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    connect(mainDirectoryEdit, &QLineEdit::textChanged, this, &ContestDialog::updateContestPath);
    connect(subdirectoryEdit, &QLineEdit::textChanged, this, &ContestDialog::updateContestPath);
    formLayout->addRow(tr("Save Path"), contestPathLabel);

    languageComboBox = new QComboBox();
    languageComboBox->addItems({"C++", "Java", "Python"});
    formLayout->addRow(tr("Language"), languageComboBox);

    problemCountSpinBox = new QSpinBox();
    problemCountSpinBox->setMinimum(1);
    problemCountSpinBox->setMaximum(26);
    formLayout->addRow(tr("Number of Problems"), problemCountSpinBox);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Reset);
    mainLayout->addWidget(buttonBox);
    connect(buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &ContestDialog::submit);
    connect(buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &QDialog::reject);
    connect(buttonBox->button(QDialogButtonBox::Reset), &QPushButton::clicked, this, &ContestDialog::reset);

    reset();
}

ContestDialog::ContestData ContestDialog::contestData() const
{
    return {fullPath(), problemCountSpinBox->value(), languageComboBox->currentText()};
}

void ContestDialog::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    updateContestPath();
}

void ContestDialog::reset()
{
    mainDirectoryEdit->setText(DefaultPathManager::defaultPathForAction("Open Contest"));
    subdirectoryEdit->clear();
    problemCountSpinBox->setValue(SettingsHelper::getNumberOfProblemsInContest());
    languageComboBox->setCurrentText(SettingsHelper::getDefaultLanguage());
}

void ContestDialog::submit()
{
    LOG_INFO(INFO_OF(mainDirectoryEdit->text())
             << INFO_OF(subdirectoryEdit->text()) << INFO_OF(fullPath()) << INFO_OF(languageComboBox->currentText())
             << INFO_OF(problemCountSpinBox->value()));

    if (!QDir(mainDirectoryEdit->text()).exists())
    {
        LOG_INFO("Main Diretory doesn't exist");
        auto res =
            QMessageBox::question(this, tr("Main Directory doesn't exist"),
                                  tr("The Main Directory [%1] doesn't exist. Do you want to create it and continue?")
                                      .arg(mainDirectoryEdit->text()),
                                  QMessageBox::Yes | QMessageBox::No);
        if (res == QMessageBox::No)
            return;
    }

    if (!QDir().mkpath(fullPath()))
    {
        LOG_WARN("Failed to create direcotry");
        QMessageBox::warning(this, tr("Failed to create directory"),
                             tr("Failed to create the directory [%1].").arg(fullPath()));
        return;
    }

    DefaultPathManager::setDefaultPathForAction("Open Contest", mainDirectoryEdit->text());
    SettingsHelper::setNumberOfProblemsInContest(problemCountSpinBox->value());

    accept();
}

void ContestDialog::chooseDirectory()
{
    const auto path = DefaultPathManager::getExistingDirectory("Open Contest", this, tr("Choose Main Directory"));
    if (!path.isEmpty())
        mainDirectoryEdit->setText(path);
}

void ContestDialog::updateContestPath()
{
    contestPathLabel->setText(fontMetrics().elidedText(fullPath(), Qt::ElideMiddle, contestPathLabel->width()));
}

QString ContestDialog::fullPath() const
{
    return QDir(mainDirectoryEdit->text()).filePath(subdirectoryEdit->text());
}

} // namespace Widgets
