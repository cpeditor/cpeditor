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

#include "Settings/ValueWrapper.hpp"

// bool
#include <QCheckBox>

// QString
#include "Settings/PathItem.hpp"
#include "Settings/ShortcutItem.hpp"
#include <QComboBox>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QTextCodec>

// int
#include <QScrollBar>
#include <QSlider>
#include <QSpinBox>

// QFont
#include "Settings/FontItem.hpp"

// QVariantList
#include "Settings/StringListsItem.hpp"

void ValueWidget::emitSignal()
{
    emit valueChanged();
}

void CheckBoxWrapper::init(QString name, QWidget *parent, QVariant /*param*/)
{
    auto *item = new QCheckBox(name, parent);
    connect(item, &QCheckBox::toggled, this, &ValueWidget::emitSignal);
    widget = item;
}

bool CheckBoxWrapper::get()
{
    return qobject_cast<QCheckBox *>(widget)->isChecked();
}

void CheckBoxWrapper::set(bool b)
{
    qobject_cast<QCheckBox *>(widget)->setChecked(b);
}

void LineEditWrapper::init(QWidget *parent, QVariant /*param*/)
{
    auto *item = new QLineEdit(parent);
    item->setMinimumWidth(400);
    connect(item, &QLineEdit::textChanged, this, &ValueWidget::emitSignal);
    widget = item;
}

QString LineEditWrapper::get()
{
    return qobject_cast<QLineEdit *>(widget)->text();
}

void LineEditWrapper::set(QString s)
{
    qobject_cast<QLineEdit *>(widget)->setText(s);
}

void PlainTextEditWrapper::init(QWidget *parent, QVariant /*param*/)
{
    auto *item = new QPlainTextEdit(parent);
    item->setMinimumWidth(400);
    item->setWordWrapMode(QTextOption::NoWrap);
    connect(item, &QPlainTextEdit::textChanged, this, &ValueWidget::emitSignal);
    widget = item;
}

QString PlainTextEditWrapper::get()
{
    return qobject_cast<QPlainTextEdit *>(widget)->toPlainText();
}

void PlainTextEditWrapper::set(QString s)
{
    qobject_cast<QPlainTextEdit *>(widget)->setPlainText(s);
}

void ComboBoxWrapper::init(QWidget *parent, QVariant param)
{
    auto *item = new QComboBox(parent);
    item->addItems(param.toStringList());
    connect(item, &QComboBox::currentTextChanged, this, &ValueWidget::emitSignal);
    widget = item;
}

QString ComboBoxWrapper::get()
{
    return qobject_cast<QComboBox *>(widget)->currentText();
}

void ComboBoxWrapper::set(QString s)
{
    qobject_cast<QComboBox *>(widget)->setCurrentText(s);
}

void PathItemWrapper::init(QWidget *parent, QVariant param)
{
    auto *item = new PathItem(PathItem::Type(param.toInt()), parent);
    connect(item->getLineEdit(), &QLineEdit::textChanged, this, &ValueWidget::emitSignal);
    widget = item;
}

QString PathItemWrapper::get()
{
    return qobject_cast<PathItem *>(widget)->getLineEdit()->text();
}

void PathItemWrapper::set(QString s)
{
    qobject_cast<PathItem *>(widget)->getLineEdit()->setText(s);
}

void ShortcutItemWrapper::init(QWidget *parent, QVariant param)
{
    auto *item = new ShortcutItem(parent);
    connect(item, &ShortcutItem::shortcutChanged, this, &ValueWidget::emitSignal);
    widget = item;
}

QString ShortcutItemWrapper::get()
{
    return qobject_cast<ShortcutItem *>(widget)->getShortcut();
}

void ShortcutItemWrapper::set(QString s)
{
    qobject_cast<ShortcutItem *>(widget)->setShortcut(s);
}

void CodecBoxWrapper::init(QWidget *parent, QVariant /*param*/)
{
    QStringList names;
    for (auto const &mib : QTextCodec::availableMibs())
    {
        names.push_back(QString::fromLocal8Bit(QTextCodec::codecForMib(mib)->name()));
    }
    names.sort(Qt::CaseInsensitive);
    names.removeDuplicates();
    ComboBoxWrapper::init(parent, names);
}

void SpinBoxWrapper::init(QWidget *parent, QVariant param)
{
    auto *item = new QSpinBox(parent);
    if (!param.isNull())
    {
        QVariantList il = param.toList();
        item->setRange(il[0].toInt(), il[1].toInt());
        if (il.length() >= 3)
            item->setSingleStep(il[2].toInt());
    }
    connect(item, QOverload<int>::of(&QSpinBox::valueChanged), this, &SpinBoxWrapper::emitSignal);
    widget = item;
}

int SpinBoxWrapper::get()
{
    return qobject_cast<QSpinBox *>(widget)->value();
}

void SpinBoxWrapper::set(int i)
{
    qobject_cast<QSpinBox *>(widget)->setValue(i);
}

void ScrollBarWrapper::init(QWidget *parent, QVariant param)
{
    auto *item = new QScrollBar(Qt::Horizontal, parent);
    if (!param.isNull())
    {
        QVariantList il = param.toList();
        item->setRange(il[0].toInt(), il[1].toInt());
    }
    connect(item, &QScrollBar::valueChanged, this, &ValueWidget::emitSignal);
    widget = item;
}

int ScrollBarWrapper::get()
{
    return qobject_cast<QScrollBar *>(widget)->value();
}

void ScrollBarWrapper::set(int i)
{
    qobject_cast<QScrollBar *>(widget)->setValue(i);
}

void SliderWrapper::init(QWidget *parent, QVariant param)
{
    auto *item = new QSlider(Qt::Horizontal, parent);
    if (!param.isNull())
    {
        QVariantList il = param.toList();
        item->setRange(il[0].toInt(), il[1].toInt());
    }
    connect(item, &QSlider::valueChanged, this, &ValueWidget::emitSignal);
    widget = item;
}

int SliderWrapper::get()
{
    return qobject_cast<QSlider *>(widget)->value();
}

void SliderWrapper::set(int i)
{
    qobject_cast<QSlider *>(widget)->setValue(i);
}

void FontItemWrapper::init(QWidget *parent, QVariant param)
{
    auto *item = new FontItem(parent, param);
    connect(item, &FontItem::fontChanged, this, &ValueWidget::emitSignal);
    widget = item;
}

QFont FontItemWrapper::get()
{
    return qobject_cast<FontItem *>(widget)->getFont();
}

void FontItemWrapper::set(QFont f)
{
    qobject_cast<FontItem *>(widget)->setFont(f);
}

void StringListsItemWrapper::init(QWidget *parent, QVariant param)
{
    auto *item = new StringListsItem(param.toList(), parent);
    connect(item, &StringListsItem::valueChanged, this, &ValueWidget::emitSignal);
    widget = item;
}

QVariantList StringListsItemWrapper::get()
{
    return qobject_cast<StringListsItem *>(widget)->getStringLists();
}

void StringListsItemWrapper::set(QVariantList sl)
{
    qobject_cast<StringListsItem *>(widget)->setStringLists(sl);
}

Wrapper<bool> *createBoolWrapper(QString type)
{
    if (type == "")
        type = "QCheckBox";
    if (type == "QCheckBox")
        return new CheckBoxWrapper();
    return nullptr;
}

Wrapper<QString> *createStringWrapper(QString type)
{
    if (type == "")
        type = "QLineEdit";
    if (type == "QLineEdit")
        return new LineEditWrapper();
    if (type == "QPlainTextEdit")
        return new PlainTextEditWrapper();
    if (type == "QComboBox")
        return new ComboBoxWrapper();
    if (type == "PathItem")
        return new PathItemWrapper();
    if (type == "ShortcutItem")
        return new ShortcutItemWrapper();
    if (type == "CodecBox")
        return new CodecBoxWrapper();
    return nullptr;
}

Wrapper<int> *createIntWrapper(QString type)
{
    if (type == "")
        type = "QSpinBox";
    if (type == "QSpinBox")
        return new SpinBoxWrapper();
    if (type == "QScrollBar")
        return new ScrollBarWrapper();
    if (type == "QSlider")
        return new SliderWrapper();
    return nullptr;
}

Wrapper<QFont> *createFontWrapper(QString type)
{
    if (type == "")
        type = "FontItem";
    if (type == "FontItem")
        return new FontItemWrapper();
    return nullptr;
}

Wrapper<QVariantList> *createStringListsWrapper(QString type)
{
    if (type == "")
        type = "StringListsItem";
    if (type == "StringListsItem")
        return new StringListsItemWrapper();
    return nullptr;
}
