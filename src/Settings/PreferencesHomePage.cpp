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

#include "Settings/PreferencesHomePage.hpp"
#include "Core/EventLogger.hpp"
#include "Settings/PreferencesWindow.hpp"
#include "Util/Util.hpp"
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QUrl>
#include <QVBoxLayout>

PreferencesHomePage::PreferencesHomePage(PreferencesWindow *parent) : QWidget(parent), preferencesWindow(parent)
{
}

void PreferencesHomePage::init()
{
    layout = new QVBoxLayout(this);

    layout->addSpacing(20);

    // add stretch so that the contents are vertically centered
    layout->addStretch();

    auto *iconLabel = new QLabel();
    iconLabel->setPixmap(QPixmap(":/icon.png").scaledToHeight(128, Qt::SmoothTransformation));
    layout->addWidget(iconLabel);
    layout->setAlignment(iconLabel, Qt::AlignCenter);

    // add spacing between the icon and the webcome label
    layout->addSpacing(30);

    // add welcome label
    auto *welcomeLabel = new QLabel(tr("Welcome to CP Editor! Let's get started."));
    layout->addWidget(welcomeLabel);
    layout->setAlignment(welcomeLabel, Qt::AlignCenter);

    // add spacing between the webcome label and the buttons
    layout->addSpacing(20);

    // add buttons
    addButton("Code Edit", tr("Code Editor Settings"));
    addButton("Language/C++/C++ Commands", tr("C++ Compile and Run Commands"));
    addButton("Language/Java/Java Commands", tr("Java Compile and Run Commands"));
    addButton("Language/Python/Python Commands", tr("Python Run Commands"));
    addButton("Appearance/General", tr("Appearance Settings"));
    addButton("Appearance/Font", tr("Font Settings"));

    // add spacing between the buttons and the manual label
    layout->addSpacing(20);

    // add manual label
    auto *manualLabel = new QLabel(tr("You can read the <a href=\"%1\">documentation</a> or go "
                                      "through the settings for more information.")
                                       .arg(Util::websiteLink("docs")));
    manualLabel->setOpenExternalLinks(true);
    layout->addWidget(manualLabel);
    layout->setAlignment(manualLabel, Qt::AlignCenter);

    // add stretch so that the contents are vertically centered
    layout->addStretch();

    layout->addSpacing(20);
}

void PreferencesHomePage::addButton(const QString &page, const QString &text)
{
    if (!preferencesWindow->pathExists(page))
        LOG_DEV("Unknown path: " << page);
    auto *button = new QPushButton(text, this);
    connect(button, &QPushButton::clicked, [this, page]() { emit requestPage(page); });
    layout->addWidget(button);
}
