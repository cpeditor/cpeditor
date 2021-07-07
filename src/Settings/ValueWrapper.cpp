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

#include "Settings/ValueWrapper.hpp"

// bool
#include "Widgets/RichTextCheckBox.hpp"

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

void CheckBoxWrapper::init(QWidget *parent, const QVariant &param)
{
    auto *item = new RichTextCheckBox(param.toString(), parent);
    connect(item, &RichTextCheckBox::toggled, this, &ValueWidget::valueChanged);
    setWidget(item);
}

bool CheckBoxWrapper::get()
{
    return qobject_cast<RichTextCheckBox *>(coreWidget())->isChecked();
}

void CheckBoxWrapper::set(const bool &b)
{
    qobject_cast<RichTextCheckBox *>(coreWidget())->setChecked(b);
}

void LineEditWrapper::init(QWidget *parent, const QVariant & /*param*/)
{
    auto *item = new QLineEdit(parent);
    item->setMinimumWidth(400);
    connect(item, &QLineEdit::textChanged, this, &ValueWidget::valueChanged);
    setWidget(item);
}

QString LineEditWrapper::get()
{
    return qobject_cast<QLineEdit *>(coreWidget())->text();
}

void LineEditWrapper::set(const QString &s)
{
    qobject_cast<QLineEdit *>(coreWidget())->setText(s);
}

void PlainTextEditWrapper::init(QWidget *parent, const QVariant & /*param*/)
{
    auto *item = new QPlainTextEdit(parent);
    item->setMinimumWidth(400);
    item->setWordWrapMode(QTextOption::NoWrap);
    connect(item, &QPlainTextEdit::textChanged, this, &ValueWidget::valueChanged);
    setWidget(item);
}

QString PlainTextEditWrapper::get()
{
    return qobject_cast<QPlainTextEdit *>(coreWidget())->toPlainText();
}

void PlainTextEditWrapper::set(const QString &s)
{
    qobject_cast<QPlainTextEdit *>(coreWidget())->setPlainText(s);
}

void ComboBoxWrapper::init(QWidget *parent, const QVariant &param)
{
    auto *item = new QComboBox(parent);
    item->addItems(param.toStringList());
    connect(item, &QComboBox::currentTextChanged, this, &ValueWidget::valueChanged);
    setWidget(item);
}

QString ComboBoxWrapper::get()
{
    return qobject_cast<QComboBox *>(coreWidget())->currentText();
}

void ComboBoxWrapper::set(const QString &s)
{
    qobject_cast<QComboBox *>(coreWidget())->setCurrentText(s);
}

void PathItemWrapper::init(QWidget *parent, const QVariant &param)
{
    auto *item = new PathItem(PathItem::Type(param.toInt()), parent);
    connect(item->getLineEdit(), &QLineEdit::textChanged, this, &ValueWidget::valueChanged);
    setWidget(item);
}

QString PathItemWrapper::get()
{
    return qobject_cast<PathItem *>(coreWidget())->getLineEdit()->text();
}

void PathItemWrapper::set(const QString &s)
{
    qobject_cast<PathItem *>(coreWidget())->getLineEdit()->setText(s);
}

void ShortcutItemWrapper::init(QWidget *parent, const QVariant &param)
{
    auto *item = new ShortcutItem(parent);
    connect(item, &ShortcutItem::shortcutChanged, this, &ValueWidget::valueChanged);
    setWidget(item);
}

QString ShortcutItemWrapper::get()
{
    return qobject_cast<ShortcutItem *>(coreWidget())->getShortcut();
}

void ShortcutItemWrapper::set(const QString &s)
{
    qobject_cast<ShortcutItem *>(coreWidget())->setShortcut(s);
}

void CodecBoxWrapper::init(QWidget *parent, const QVariant & /*param*/)
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

void SpinBoxWrapper::init(QWidget *parent, const QVariant &param)
{
    auto *item = new QSpinBox(parent);
    if (!param.isNull())
    {
        QVariantList il = param.toList();
        item->setRange(il[0].toInt(), il[1].toInt());
        if (il.length() >= 3)
            item->setSingleStep(il[2].toInt());
    }
    connect(item, QOverload<int>::of(&QSpinBox::valueChanged), this, &SpinBoxWrapper::valueChanged);
    setWidget(item);
}

int SpinBoxWrapper::get()
{
    return qobject_cast<QSpinBox *>(coreWidget())->value();
}

void SpinBoxWrapper::set(const int &i)
{
    qobject_cast<QSpinBox *>(coreWidget())->setValue(i);
}

void ScrollBarWrapper::init(QWidget *parent, const QVariant &param)
{
    auto *item = new QScrollBar(Qt::Horizontal, parent);
    if (!param.isNull())
    {
        QVariantList il = param.toList();
        item->setRange(il[0].toInt(), il[1].toInt());
    }
    connect(item, &QScrollBar::valueChanged, this, &ValueWidget::valueChanged);
    setWidget(item);
}

int ScrollBarWrapper::get()
{
    return qobject_cast<QScrollBar *>(coreWidget())->value();
}

void ScrollBarWrapper::set(const int &i)
{
    qobject_cast<QScrollBar *>(coreWidget())->setValue(i);
}

void SliderWrapper::init(QWidget *parent, const QVariant &param)
{
    auto *item = new QSlider(Qt::Horizontal, parent);
    if (!param.isNull())
    {
        QVariantList il = param.toList();
        item->setRange(il[0].toInt(), il[1].toInt());
    }
    connect(item, &QSlider::valueChanged, this, &ValueWidget::valueChanged);
    setWidget(item);
}

int SliderWrapper::get()
{
    return qobject_cast<QSlider *>(coreWidget())->value();
}

void SliderWrapper::set(const int &i)
{
    qobject_cast<QSlider *>(coreWidget())->setValue(i);
}

void FontItemWrapper::init(QWidget *parent, const QVariant &param)
{
    auto *item = new FontItem(parent, param);
    connect(item, &FontItem::fontChanged, this, &ValueWidget::valueChanged);
    setWidget(item);
}

QFont FontItemWrapper::get()
{
    return qobject_cast<FontItem *>(coreWidget())->getFont();
}

void FontItemWrapper::set(const QFont &f)
{
    qobject_cast<FontItem *>(coreWidget())->setFont(f);
}

void StringListsItemWrapper::init(QWidget *parent, const QVariant &param)
{
    auto *item = new StringListsItem(param.toList(), parent);
    connect(item, &StringListsItem::valueChanged, this, &ValueWidget::valueChanged);
    setWidget(item);
}

QVariantList StringListsItemWrapper::get()
{
    return qobject_cast<StringListsItem *>(coreWidget())->getStringLists();
}

void StringListsItemWrapper::set(const QVariantList &sl)
{
    qobject_cast<StringListsItem *>(coreWidget())->setStringLists(sl);
}

Wrapper<bool> *createBoolWrapper(const QString &type)
{
    if (type.isEmpty() || type == "QCheckBox")
        return new CheckBoxWrapper();
    return nullptr;
}

Wrapper<QString> *createStringWrapper(const QString &type)
{
    if (type.isEmpty() || type == "QLineEdit")
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

Wrapper<int> *createIntWrapper(const QString &type)
{
    if (type.isEmpty() || type == "QSpinBox")
        return new SpinBoxWrapper();
    if (type == "QScrollBar")
        return new ScrollBarWrapper();
    if (type == "QSlider")
        return new SliderWrapper();
    return nullptr;
}

Wrapper<QFont> *createFontWrapper(const QString &type)
{
    if (type.isEmpty() || type == "FontItem")
        return new FontItemWrapper();
    return nullptr;
}

Wrapper<QVariantList> *createStringListsWrapper(const QString &type)
{
    if (type.isEmpty() || type == "StringListsItem")
        return new StringListsItemWrapper();
    return nullptr;
}
