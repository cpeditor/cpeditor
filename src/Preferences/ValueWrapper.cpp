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

#include "Preferences/ValueWrapper.hpp"

ValueWidget::ValueWidget() : QObject()
{
}

ValueWidget::~ValueWidget()
{
}

void ValueWidget::emitSignal()
{
    emit valueChanged();
}

void CheckBoxWrapper::init(QString name, QWidget *parent, QVariant)
{
    QCheckBox *item = new QCheckBox(name, parent);
    connect(item, &QCheckBox::toggled, this, &ValueWidget::emitSignal);
    widget = item;
}

bool CheckBoxWrapper::get()
{
    return static_cast<QCheckBox *>(widget)->isChecked();
}

void CheckBoxWrapper::set(bool b)
{
    static_cast<QCheckBox *>(widget)->setChecked(b);
}

void LineEditWrapper::init(QWidget *parent, QVariant)
{
    QLineEdit *item = new QLineEdit(parent);
    item->setMinimumWidth(400);
    connect(item, &QLineEdit::textChanged, this, &ValueWidget::emitSignal);
    widget = item;
}

QString LineEditWrapper::get()
{
    return static_cast<QLineEdit *>(widget)->text();
}

void LineEditWrapper::set(QString s)
{
    static_cast<QLineEdit *>(widget)->setText(s);
}

void PlainTextEditWrapper::init(QWidget *parent, QVariant)
{
    auto *item = new QPlainTextEdit(parent);
    connect(item, &QPlainTextEdit::textChanged, this, &ValueWidget::emitSignal);
    widget = item;
}

QString PlainTextEditWrapper::get()
{
    return static_cast<QPlainTextEdit *>(widget)->toPlainText();
}

void PlainTextEditWrapper::set(QString s)
{
    static_cast<QPlainTextEdit *>(widget)->setPlainText(s);
}

void ComboBoxWrapper::init(QWidget *parent, QVariant param)
{
    QComboBox *item = new QComboBox(parent);
    item->addItems(param.toStringList());
    connect(item, &QComboBox::currentTextChanged, this, &ValueWidget::emitSignal);
    widget = item;
}

QString ComboBoxWrapper::get()
{
    return static_cast<QComboBox *>(widget)->currentText();
}

void ComboBoxWrapper::set(QString s)
{
    static_cast<QComboBox *>(widget)->setCurrentText(s);
}

void PathItemWrapper::init(QWidget *parent, QVariant param)
{
    int id = param.toInt();
    PathItem *item = new PathItem(id, id, parent); // TODO: split to two param
    connect(item->getLineEdit(), &QLineEdit::textChanged, this, &ValueWidget::emitSignal);
    widget = item;
}

QString PathItemWrapper::get()
{
    return static_cast<PathItem *>(widget)->getLineEdit()->text();
}

void PathItemWrapper::set(QString s)
{
    static_cast<PathItem *>(widget)->getLineEdit()->setText(s);
}

void ShortcutItemWrapper::init(QWidget *parent, QVariant param)
{
    ShortcutItem *item = new ShortcutItem(parent);
    connect(item, &ShortcutItem::shortcutChanged, this, &ValueWidget::emitSignal);
    widget = item;
}

QString ShortcutItemWrapper::get()
{
    return static_cast<ShortcutItem *>(widget)->getShortcut();
}

void ShortcutItemWrapper::set(QString s)
{
    static_cast<ShortcutItem *>(widget)->setShortcut(s);
}

void SpinBoxWrapper::init(QWidget *parent, QVariant param)
{
    QSpinBox *item = new QSpinBox(parent);
    if (!param.isNull())
    {
        QList<QVariant> il = param.toList();
        item->setRange(il[0].toInt(), il[1].toInt());
    }
    connect(item, QOverload<int>::of(&QSpinBox::valueChanged), this, &SpinBoxWrapper::emitSignal);
    widget = item;
}

int SpinBoxWrapper::get()
{
    return static_cast<QSpinBox *>(widget)->value();
}

void SpinBoxWrapper::set(int i)
{
    static_cast<QSpinBox *>(widget)->setValue(i);
}

void ScrollBarWrapper::init(QWidget *parent, QVariant param)
{
    QScrollBar *item = new QScrollBar(Qt::Horizontal, parent);
    if (!param.isNull())
    {
        QList<QVariant> il = param.toList();
        item->setRange(il[0].toInt(), il[1].toInt());
    }
    connect(item, &QScrollBar::valueChanged, this, &ValueWidget::emitSignal);
    widget = item;
}

int ScrollBarWrapper::get()
{
    return static_cast<QScrollBar *>(widget)->value();
}

void ScrollBarWrapper::set(int i)
{
    static_cast<QScrollBar *>(widget)->setValue(i);
}

void SliderWrapper::init(QWidget *parent, QVariant param)
{
    QSlider *item = new QSlider(Qt::Horizontal, parent);
    if (!param.isNull())
    {
        QList<QVariant> il = param.toList();
        item->setRange(il[0].toInt(), il[1].toInt());
    }
    connect(item, &QSlider::valueChanged, this, &ValueWidget::emitSignal);
    widget = item;
}

int SliderWrapper::get()
{
    return static_cast<QSlider *>(widget)->value();
}

void SliderWrapper::set(int i)
{
    static_cast<QSlider *>(widget)->setValue(i);
}

void FontItemWrapper::init(QWidget *parent, QVariant)
{
    FontItem *item = new FontItem(parent);
    connect(item, &FontItem::fontChanged, this, &ValueWidget::emitSignal);
    widget = item;
}

QFont FontItemWrapper::get()
{
    return static_cast<FontItem *>(widget)->getFont();
}

void FontItemWrapper::set(QFont f)
{
    static_cast<FontItem *>(widget)->setFont(f);
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
    else if (type == "QPlainTextEdit")
        return new PlainTextEditWrapper();
    else if (type == "QComboBox")
        return new ComboBoxWrapper();
    else if (type == "PathItem")
        return new PathItemWrapper();
    else if (type == "ShortcutItem")
        return new ShortcutItemWrapper();
    return nullptr;
}

Wrapper<int> *createIntWrapper(QString type)
{
    if (type == "")
        type = "QSpinBox";
    if (type == "QSpinBox")
        return new SpinBoxWrapper();
    else if (type == "QScrollBar")
        return new ScrollBarWrapper();
    else if (type == "QSlider")
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
