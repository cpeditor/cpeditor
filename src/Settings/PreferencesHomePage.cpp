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

#include "Settings/PreferencesHomePage.hpp"
#include <QPixmap>
#include <generated/version.hpp>

PreferencesHomePage::PreferencesHomePage(QWidget *parent) : QWidget(parent)
{
    // construct the layout
    layout = new QVBoxLayout(this);

    // add stretch so that the contents are vertically centered
    layout->addStretch();

    iconLabel = new QLabel();
    iconLabel->setPixmap(QPixmap(":/icon.png").scaledToHeight(128, Qt::SmoothTransformation));
    layout->addWidget(iconLabel);
    layout->setAlignment(iconLabel, Qt::AlignCenter);

    // add spacing between the icon and the webcome label
    layout->addSpacing(30);

    // add welcome label
    welcomeLabel = new QLabel("Welcome to CP Editor! Let's get started.");
    layout->addWidget(welcomeLabel);
    layout->setAlignment(welcomeLabel, Qt::AlignCenter);

    // add spacing between the webcome label and the buttons
    layout->addSpacing(20);

    // add buttons
    addButton("Edit", "Code Editor Settings");
    addButton("Language/Commands", "Compile and Run Commands");
    addButton("Appearance", "Appearance");

    // add spacing between the buttons and the manual label
    layout->addSpacing(20);

    // add manual label
    manualLabel = new QLabel("You can read the <a href=\"https://github.com/cpeditor/cpeditor/blob/" APP_VERSION
                             "/doc/MANUAL.md\">Manual</a> or go through the settings for more information.");
    manualLabel->setOpenExternalLinks(true);
    layout->addWidget(manualLabel);
    layout->setAlignment(manualLabel, Qt::AlignCenter);

    // add stretch so that the contents are vertically centered
    layout->addStretch();
}

void PreferencesHomePage::addButton(const QString &page, const QString &text)
{
    auto button = new QPushButton(text, this);
    connect(button, &QPushButton::clicked, [this, page]() { emit requestPage(page); });
    layout->addWidget(button);
}
