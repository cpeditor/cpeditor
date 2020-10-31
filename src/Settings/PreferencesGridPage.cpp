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

#include "Settings/PreferencesGridPage.hpp"
#include "Settings/ValueWrapper.hpp"
#include <QGridLayout>
#include <QLabel>

PreferencesGridPage::PreferencesGridPage(bool alignTop, QWidget *parent) : PreferencesPage(parent)
{
    auto VLayout = new QVBoxLayout();
    auto HLayout = new QHBoxLayout();
    gridLayout = new QGridLayout();

    addLayout(VLayout);

    VLayout->addSpacing(30);
    VLayout->addLayout(HLayout);
    if (alignTop)
        VLayout->addStretch();
    VLayout->addSpacing(30);

    HLayout->addStretch();
    HLayout->addLayout(gridLayout);
    HLayout->addStretch();
}

void PreferencesGridPage::addRow(ValueWidget *widget, const QString &tip, const QString &help, const QString &labelText)
{
    registerWidget(widget);
    int row = gridLayout->rowCount();
    if (labelText.isEmpty())
    {
        if (!tip.isEmpty())
            widget->coreWidget()->setToolTip(tr(tip.toUtf8()));
        if (!help.isEmpty())
            widget->coreWidget()->setWhatsThis(tr(help.toUtf8()));
    }
    else
    {
        QLabel *label = new QLabel(tr(labelText.toUtf8()), this);
        if (!tip.isEmpty())
            label->setToolTip(tr(tip.toUtf8()));
        if (!help.isEmpty())
            label->setWhatsThis(tr(help.toUtf8()));
        gridLayout->addWidget(label, row, 0);
    }
    gridLayout->addWidget(widget->coreWidget(), row, 1);
}
