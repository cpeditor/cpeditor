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

#ifndef VALUEWRAPPER_HPP
#define VALUEWRAPPER_HPP

#include <QVariant>
#include <QWidget>

class ValueWidget : public QObject
{
    Q_OBJECT

  public:
    ValueWidget() = default;
    virtual ~ValueWidget() = default;
    virtual QWidget *coreWidget() = 0;
    virtual QVariant getVariant() = 0;
    virtual void setVariant(QVariant value) = 0;

  public slots:
    void emitSignal();

  signals:
    void valueChanged();
};

template <typename Type> struct Wrapper : public ValueWidget
{
    virtual ~Wrapper() override
    {
        if (widget)
            delete widget;
    }
    virtual QWidget *coreWidget() override
    {
        return widget;
    }
    virtual void init(QWidget *parent, QVariant param) = 0;
    virtual Type get() = 0;
    virtual void set(Type b) = 0;
    virtual QVariant getVariant() override
    {
        return QVariant(get());
    }
    virtual void setVariant(QVariant value) override
    {
        set(value.value<Type>());
    }

    QWidget *widget = nullptr;
};

template <> struct Wrapper<bool> : public ValueWidget
{
    virtual ~Wrapper() override
    {
        if (widget)
            delete widget;
    }
    virtual QWidget *coreWidget() override
    {
        return widget;
    }
    virtual void init(QString name, QWidget *parent, QVariant param) = 0;
    virtual bool get() = 0;
    virtual void set(bool b) = 0;
    virtual QVariant getVariant() override
    {
        return QVariant(get());
    }
    virtual void setVariant(QVariant value) override
    {
        set(value.toBool());
    }

    QWidget *widget = nullptr;
};

struct CheckBoxWrapper : public Wrapper<bool>
{
    virtual void init(QString name, QWidget *parent, QVariant param = QVariant()) override;
    virtual bool get() override;
    virtual void set(bool b) override;
};

struct LineEditWrapper : public Wrapper<QString>
{
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
    virtual QString get() override;
    virtual void set(QString s) override;
};

struct PlainTextEditWrapper : public Wrapper<QString>
{
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
    virtual QString get() override;
    virtual void set(QString s) override;
};

struct ComboBoxWrapper : public Wrapper<QString>
{
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
    virtual QString get() override;
    virtual void set(QString s) override;
};

struct PathItemWrapper : public Wrapper<QString>
{
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
    virtual QString get() override;
    virtual void set(QString s) override;
};

struct ShortcutItemWrapper : public Wrapper<QString>
{
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
    virtual QString get() override;
    virtual void set(QString s) override;
};

struct CodecBoxWrapper : public ComboBoxWrapper
{
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
};

struct SpinBoxWrapper : public Wrapper<int>
{
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
    virtual int get() override;
    virtual void set(int i) override;
};

struct ScrollBarWrapper : public Wrapper<int>
{
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
    virtual int get() override;
    virtual void set(int i) override;
};

struct SliderWrapper : public Wrapper<int>
{
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
    virtual int get() override;
    virtual void set(int i) override;
};

struct FontItemWrapper : public Wrapper<QFont>
{
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
    virtual QFont get() override;
    virtual void set(QFont f) override;
};

struct StringListsItemWrapper : public Wrapper<QVariantList>
{
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
    virtual QVariantList get() override;
    virtual void set(QVariantList sl) override;
};

Wrapper<bool> *createBoolWrapper(QString type);
Wrapper<QString> *createStringWrapper(QString type);
Wrapper<int> *createIntWrapper(QString type);
Wrapper<QFont> *createFontWrapper(QString type);
Wrapper<QVariantList> *createStringListsWrapper(QString type);

#endif
