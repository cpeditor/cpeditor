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

// https://www.qtcentre.org/threads/60455-SOLVED-Issues-with-displaying-rich-text-for-a-QCheckBox?p=268211#post268211

#include "Widgets/RichTextCheckBox.hpp"
#include <QDesktopServices>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QStyleOptionButton>
#include <QStylePainter>
#include <QTextDocument>

ClickableLabel::ClickableLabel(QWidget *parent) : QLabel(parent)
{
    connect(
        this, &QLabel::linkActivated, this,
        [this](const QString &link) {
            linkClicked = true;
            QDesktopServices::openUrl(link);
        },
        Qt::DirectConnection);
}

void ClickableLabel::mouseReleaseEvent(QMouseEvent *event)
{
    linkClicked = false;
    QLabel::mouseReleaseEvent(event);
    if (!linkClicked && contentsRect().contains(event->pos()) &&
        (event->buttons() == Qt::LeftButton || event->button() == Qt::LeftButton))
    {
        emit released();
    }
}

void ClickableLabel::mousePressEvent(QMouseEvent *event)
{
    QLabel::mousePressEvent(event);
    if ((event->buttons() == Qt::LeftButton || event->button() == Qt::LeftButton))
    {
        emit pressed();
    }
}

void ClickableLabel::mouseMoveEvent(QMouseEvent *event)
{
    QLabel::mouseMoveEvent(event);
    if (contentsRect().contains(event->pos()) &&
        (event->buttons() == Qt::LeftButton || event->button() == Qt::LeftButton))
    {
        emit left();
    }
}

HoverCheckBox::HoverCheckBox(QWidget *parent) : QCheckBox(parent)
{
}

void HoverCheckBox::paintEvent(QPaintEvent * /*unused*/)
{
    QStylePainter p(this);
    QStyleOptionButton opt;
    initStyleOption(&opt);
    if (isHover)
    {
        opt.state |= QStyle::State_MouseOver;
    }
    if (isPressed)
    {
        opt.state |= QStyle::State_Sunken;
    }
    p.drawControl(QStyle::CE_CheckBox, opt);
}

void HoverCheckBox::setHover()
{
    isHover = true;
    update();
}

void HoverCheckBox::setPressed()
{
    isPressed = true;
    update();
}

void HoverCheckBox::clearPressed()
{
    isPressed = false;
    update();
}

void HoverCheckBox::clearStates()
{
    isHover = false;
    isPressed = false;
    update();
}

RichTextCheckBox::RichTextCheckBox(const QString &text, QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);
    checkBox = new HoverCheckBox();
    label = new ClickableLabel();
    label->setTextFormat(Qt::RichText);
    label->setText(text);
    auto *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(checkBox);
    layout->addWidget(label);
    layout->setAlignment(Qt::AlignLeft);
    connect(label, &ClickableLabel::released, checkBox, &HoverCheckBox::toggle);
    connect(label, &ClickableLabel::released, checkBox, &HoverCheckBox::clearPressed);
    connect(label, &ClickableLabel::pressed, checkBox, &HoverCheckBox::setPressed);
    connect(label, &ClickableLabel::left, checkBox, &HoverCheckBox::clearStates);
    connect(checkBox, &HoverCheckBox::toggled, this, &RichTextCheckBox::toggled);
    connect(checkBox, &HoverCheckBox::clicked, this, &RichTextCheckBox::clicked);
    connect(label, &ClickableLabel::released, this, &RichTextCheckBox::clicked);
}

QCheckBox *RichTextCheckBox::getCheckBox()
{
    return checkBox;
}

bool RichTextCheckBox::isChecked() const
{
    return checkBox->isChecked();
}

Qt::CheckState RichTextCheckBox::checkState() const
{
    return checkBox->checkState();
}

QString RichTextCheckBox::text() const
{
    return label->text();
}

QString RichTextCheckBox::plainText() const
{
    QTextDocument doc;
    doc.setHtml(label->text());
    return doc.toPlainText();
}

void RichTextCheckBox::clearStates()
{
    checkBox->clearStates();
}

void RichTextCheckBox::setCheckState(Qt::CheckState state)
{
    checkBox->setCheckState(state);
}

void RichTextCheckBox::setChecked(bool value)
{
    checkBox->setChecked(value);
}

void RichTextCheckBox::enterEvent(QEvent * /*unused*/)
{
    checkBox->setHover();
}

void RichTextCheckBox::leaveEvent(QEvent * /*unused*/)
{
    checkBox->clearStates();
}
