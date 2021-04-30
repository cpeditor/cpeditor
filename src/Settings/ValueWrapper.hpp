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

#ifndef VALUEWRAPPER_HPP
#define VALUEWRAPPER_HPP

#include <QVariant>
#include <QWidget>

class ValueWidget : public QObject
{
    Q_OBJECT

  public:
    virtual QWidget *coreWidget() = 0;
    virtual QVariant getVariant() = 0;
    virtual void setVariant(const QVariant &value) = 0;

  signals:
    void valueChanged();
};

template <typename Type> class Wrapper : public ValueWidget
{
  public:
    ~Wrapper() override
    {
        delete widget;
    }
    QWidget *coreWidget() override
    {
        return widget;
    }
    virtual void init(QWidget *parent, const QVariant &param) = 0;
    virtual Type get() = 0;
    virtual void set(const Type &b) = 0;
    QVariant getVariant() override
    {
        return QVariant(get());
    }
    void setVariant(const QVariant &value) override
    {
        set(value.value<Type>());
    }

  protected:
    void setWidget(QWidget *w)
    {
        widget = w;
    }

  private:
    QWidget *widget = nullptr;
};

class CheckBoxWrapper : public Wrapper<bool>
{
  public:
    void init(QWidget *parent, const QVariant &param = QVariant()) override;
    bool get() override;
    void set(const bool &b) override;
};

class LineEditWrapper : public Wrapper<QString>
{
  public:
    void init(QWidget *parent, const QVariant &param = QVariant()) override;
    QString get() override;
    void set(const QString &s) override;
};

class PlainTextEditWrapper : public Wrapper<QString>
{
  public:
    void init(QWidget *parent, const QVariant &param = QVariant()) override;
    QString get() override;
    void set(const QString &s) override;
};

class ComboBoxWrapper : public Wrapper<QString>
{
  public:
    void init(QWidget *parent, const QVariant &param = QVariant()) override;
    QString get() override;
    void set(const QString &s) override;
};

class PathItemWrapper : public Wrapper<QString>
{
  public:
    void init(QWidget *parent, const QVariant &param = QVariant()) override;
    QString get() override;
    void set(const QString &s) override;
};

class ShortcutItemWrapper : public Wrapper<QString>
{
  public:
    void init(QWidget *parent, const QVariant &param = QVariant()) override;
    QString get() override;
    void set(const QString &s) override;
};

class CodecBoxWrapper : public ComboBoxWrapper
{
  public:
    void init(QWidget *parent, const QVariant &param = QVariant()) override;
};

class SpinBoxWrapper : public Wrapper<int>
{
  public:
    void init(QWidget *parent, const QVariant &param = QVariant()) override;
    int get() override;
    void set(const int &i) override;
};

class ScrollBarWrapper : public Wrapper<int>
{
  public:
    void init(QWidget *parent, const QVariant &param = QVariant()) override;
    int get() override;
    void set(const int &i) override;
};

class SliderWrapper : public Wrapper<int>
{
  public:
    void init(QWidget *parent, const QVariant &param = QVariant()) override;
    int get() override;
    void set(const int &i) override;
};

class FontItemWrapper : public Wrapper<QFont>
{
  public:
    void init(QWidget *parent, const QVariant &param = QVariant()) override;
    QFont get() override;
    void set(const QFont &f) override;
};

class StringListsItemWrapper : public Wrapper<QVariantList>
{
  public:
    void init(QWidget *parent, const QVariant &param = QVariant()) override;
    QVariantList get() override;
    void set(const QVariantList &sl) override;
};

Wrapper<bool> *createBoolWrapper(const QString &type);
Wrapper<QString> *createStringWrapper(const QString &type);
Wrapper<int> *createIntWrapper(const QString &type);
Wrapper<QFont> *createFontWrapper(const QString &type);
Wrapper<QVariantList> *createStringListsWrapper(const QString &type);

#endif
