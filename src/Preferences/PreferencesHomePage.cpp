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

#include "Preferences/PreferencesHomePage.hpp"
#include <generated/version.hpp>

PreferencesHomePage::PreferencesHomePage(QWidget *parent) : QWidget(parent)
{
    // construct the layout
    layout = new QVBoxLayout(this);

    // add welcome label
    welcomeLabel = new QLabel("Welcome to CP Editor! Let's get started 🚀");
    layout->addWidget(welcomeLabel);

    // add spacing between the webcome label and the buttons
    layout->addSpacing(20);

    // add buttons
    addButton("Edit", "Editor Settings");
    addButton("Language", "Default Language");
    addButton("Language/Commands", "Compile and Run Commands");
    addButton("Appearance", "Appearance");

    // add spacing between the buttons and the manual label
    layout->addSpacing(30);

    // add manual label
    manualLabel = new QLabel("You can read the <a href=\"https://github.com/cpeditor/cpeditor/blob/" APP_VERSION
                             "/doc/MANUAL.md\">Manual</a> for more information.");
    manualLabel->setOpenExternalLinks(true);
    layout->addWidget(manualLabel);
}

void PreferencesHomePage::addButton(const QString &page, const QString &text)
{
    auto button = new QPushButton(text, this);
    connect(button, &QPushButton::clicked, [this, page]() { emit requestPage(page); });
    layout->addWidget(button);
}
