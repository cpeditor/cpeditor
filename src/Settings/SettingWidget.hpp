#ifndef SETTINGWIDGET_HPP
#define SETTINGWIDGET_HPP

#include "Settings/FontItem.hpp"
#include "Settings/PathItem.hpp"
#include "Settings/SettingsInfo.hpp"
#include "Settings/ShortcutItem.hpp"
#include "Settings/StringListsItem.hpp"
#include "Widgets/RichTextCheckBox.hpp"
#include <QComboBox>
#include <QLineEdit>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QSlider>
#include <QSpinBox>
#include <QSplitter>

namespace WIP
{

struct SettingBase : public QObject
{
    Q_OBJECT

  public:
    SettingsInfo::SettingIter iter;

    virtual QVariant getV() const = 0;
    virtual void setV(QVariant) = 0;
    virtual QWidget *rootWidget() const = 0;
    virtual void init(QWidget *parent, QVariant param = QVariant()) = 0;
    virtual void enable(bool enabled = true)
    {
        rootWidget()->setEnabled(enabled);
    }
    virtual void checkout(int pos, QString key);

    void setdef();
    void reset();
    void apply();
    bool changed() const;

  signals:
    void valueChanged();
};

template <typename Type> struct SettingTemplate : public SettingBase
{
    virtual Type get() const = 0;
    virtual void set(const Type &) = 0;

    virtual QVariant getV() const override final
    {
        return get();
    }
    virtual void setV(QVariant v) override final
    {
        set(v.value<Type>());
    }
};

template <typename Type, typename RootWidget> struct WrapperTemplate : public SettingTemplate<Type>
{
    virtual QWidget *rootWidget() const override final
    {
        return widget;
    }

  protected:
    RootWidget *widget;
};

struct CheckBoxWrapper : public WrapperTemplate<bool, RichTextCheckBox>
{
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
    virtual bool get() const override;
    virtual void set(const bool &v) override;
};

struct LineEditWrapper : public WrapperTemplate<QString, QLineEdit>
{
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
    virtual QString get() const override;
    virtual void set(const QString &v) override;
};

struct PlainTextEditWrapper : public WrapperTemplate<QString, QPlainTextEdit>
{
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
    virtual QString get() const override;
    virtual void set(const QString &v) override;
};

struct ComboBoxWrapper : public WrapperTemplate<QString, QComboBox>
{
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
    virtual QString get() const override;
    virtual void set(const QString &v) override;
};

struct PathItemWrapper : public WrapperTemplate<QString, PathItem>
{
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
    virtual QString get() const override;
    virtual void set(const QString &v) override;
};

struct ShortcutItemWrapper : public WrapperTemplate<QString, ShortcutItem>
{
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
    virtual QString get() const override;
    virtual void set(const QString &v) override;
};

struct CodecBoxWrapper : public ComboBoxWrapper
{
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
};

struct SpinBoxWrapper : public WrapperTemplate<int, QSpinBox>
{
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
    virtual int get() const override;
    virtual void set(const int &v) override;
};

struct ScrollBarWrapper : public WrapperTemplate<int, QScrollBar>
{
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
    virtual int get() const override;
    virtual void set(const int &v) override;
};

struct SliderWrapper : public WrapperTemplate<int, QSlider>
{
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
    virtual int get() const override;
    virtual void set(const int &v) override;
};

struct FontItemWrapper : public WrapperTemplate<QFont, FontItem>
{
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
    virtual QFont get() const override;
    virtual void set(const QFont &v) override;
};

struct StringListsItemWrapper : public WrapperTemplate<QVariantList, StringListsItem>
{
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
    virtual QVariantList get() const override;
    virtual void set(const QVariantList &v) override;
};

struct SettingsWrapper : public WrapperTemplate<QMap<QString, QVariant>, QWidget>
{ // a form of settings
    Q_OBJECT

  public:
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
    virtual QMap<QString, QVariant> get() const override;
    virtual void set(const QMap<QString, QVariant> &v) override;
    virtual void enable(bool enabled = true) override;
    virtual void checkout(int pos, QString key) override;

    void reload();

  public slots:
    void transSig();

  public:
    QStringList entries;
    QMap<QString, QVariant> data;
    QMap<QString, SettingBase *> wraps;
};

struct MapWrapper : public WrapperTemplate<QMap<QString, QVariant>, QSplitter>
{
    Q_OBJECT

  public:
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
    virtual QMap<QString, QVariant> get() const override;
    virtual void set(const QMap<QString, QVariant> &v) override;
    virtual void enable(bool enabled = true) override;

    void reload();

  public slots:
    void transSig();
    void select(QString key);
    void update();

  public:
    QString cur;
    QListWidget *list;
    SettingsWrapper *right;
    QMap<QString, QVariant> data;
};

} // namespace WIP

#endif