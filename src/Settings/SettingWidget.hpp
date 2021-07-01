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
#include <QStackedWidget>
#include <QTabWidget>

struct SettingBase : public QObject
{
    Q_OBJECT

  public:
    SettingBase *parent = nullptr;
    QString path, trPath;
    SettingsInfo::SettingIter iter;

    virtual QVariant getV() const = 0;
    virtual void setV(QVariant) = 0;
    virtual QWidget *rootWidget() const = 0;
    virtual void init(QWidget *parent, QVariant param = QVariant()) = 0;
    virtual void enable(bool enabled = true)
    {
        rootWidget()->setEnabled(enabled);
    }
    virtual bool needVExpand() const = 0;

    virtual void setdef();
    virtual void reset();
    virtual void apply();
    virtual bool changed() const;
    virtual QStringList content() const;

    QString docLink() const;

  signals:
    void valueChanged();
};

template <typename Type> struct SettingTemplate : public SettingBase
{
    virtual Type get() const = 0;
    virtual void set(const Type &) = 0;

    QVariant getV() const final
    {
        return get();
    }
    void setV(QVariant v) final
    {
        set(v.value<Type>());
    }
};

template <typename Type, typename RootWidget> struct WrapperTemplate : public SettingTemplate<Type>
{
    QWidget *rootWidget() const final
    {
        return widget;
    }
    virtual bool needVExpand() const override
    {
        return false;
    }

  protected:
    RootWidget *widget;
};

struct CheckBoxWrapper : public WrapperTemplate<bool, RichTextCheckBox>
{
    void init(QWidget *parent, QVariant param = QVariant()) override;
    bool get() const override;
    void set(const bool &v) override;
};

struct TristateCheckBoxWrapper : public WrapperTemplate<int, RichTextCheckBox>
{
    void init(QWidget *parent, QVariant param = QVariant()) override;
    int get() const override;
    void set(const int &v) override;
};

struct LineEditWrapper : public WrapperTemplate<QString, QLineEdit>
{
    void init(QWidget *parent, QVariant param = QVariant()) override;
    QString get() const override;
    void set(const QString &v) override;
};

struct PlainTextEditWrapper : public WrapperTemplate<QString, QPlainTextEdit>
{
    void init(QWidget *parent, QVariant param = QVariant()) override;
    QString get() const override;
    void set(const QString &v) override;
    virtual bool needVExpand() const override
    {
        return true;
    }
};

struct ComboBoxWrapper : public WrapperTemplate<QString, QComboBox>
{
    void init(QWidget *parent, QVariant param = QVariant()) override;
    QString get() const override;
    void set(const QString &v) override;
};

struct PathItemWrapper : public WrapperTemplate<QString, PathItem>
{
    void init(QWidget *parent, QVariant param = QVariant()) override;
    QString get() const override;
    void set(const QString &v) override;
};

struct ShortcutItemWrapper : public WrapperTemplate<QString, ShortcutItem>
{
    void init(QWidget *parent, QVariant param = QVariant()) override;
    QString get() const override;
    void set(const QString &v) override;
};

struct CodecBoxWrapper : public ComboBoxWrapper
{
    void init(QWidget *parent, QVariant param = QVariant()) override;
};

struct SpinBoxWrapper : public WrapperTemplate<int, QSpinBox>
{
    void init(QWidget *parent, QVariant param = QVariant()) override;
    int get() const override;
    void set(const int &v) override;
};

struct ScrollBarWrapper : public WrapperTemplate<int, QScrollBar>
{
    void init(QWidget *parent, QVariant param = QVariant()) override;
    int get() const override;
    void set(const int &v) override;
};

struct SliderWrapper : public WrapperTemplate<int, QSlider>
{
    void init(QWidget *parent, QVariant param = QVariant()) override;
    int get() const override;
    void set(const int &v) override;
};

struct FontItemWrapper : public WrapperTemplate<QFont, FontItem>
{
    void init(QWidget *parent, QVariant param = QVariant()) override;
    QFont get() const override;
    void set(const QFont &v) override;
};

struct StringListsItemWrapper : public WrapperTemplate<QVariantList, StringListsItem>
{
    void init(QWidget *parent, QVariant param = QVariant()) override;
    QVariantList get() const override;
    void set(const QVariantList &v) override;
    bool changed() const override;
    bool needVExpand() const override
    {
        return true;
    }
};

struct SettingsWrapper : public WrapperTemplate<QMap<QString, QVariant>, QWidget>
{ // a form of settings
    Q_OBJECT

  public:
    void init(QWidget *parent, QVariant param = QVariant()) override;
    QMap<QString, QVariant> get() const override;
    void set(const QMap<QString, QVariant> &v) override;
    void enable(bool enabled = true) override;
    bool needVExpand() const override;

    void setdef() override;
    void reset() override;
    void apply() override;
    bool changed() const override;
    QStringList content() const override;

    SettingBase *locate(const QString &name);

    void setKey(QString k = "", QString tk = "")
    {
        key = std::move(k);
        trKey = std::move(tk);
    }
  public slots:
    void reload();
    void update();
    void check(const QString &name);

  public:
    bool updateDisabled = false;
    bool enabled = false;
    QStringList entries;
    QString key, trKey;
    QMap<QString, QVariant> data;
    QTabWidget *tab;
    QMap<QString, SettingBase *> wraps;
};

struct MapWrapper : public WrapperTemplate<QMap<QString, QVariant>, QSplitter>
{
    Q_OBJECT

  public:
    struct KeyCheck
    {
        struct Rule
        {
            QString regex;
            QString message;
            bool invert = false;
            Rule(QString r, QString m) : regex(std::move(r)), message(std::move(m))
            {
            }
        };
        QList<Rule> rules;
        void init(const QVariant &cfg);
        bool check(const QString &key, QString &msg) const;
    };

    struct Actions
    {
        QList<std::pair<QString, QString>> data;
        MapWrapper *parent;

        void init(const QVariant &cfg);
        bool hasMore() const
        {
            return data.length() > 0;
        }
        void genMenu(QMenu *menu) const;
    };

    void init(QWidget *parent, QVariant param = QVariant()) override;
    QMap<QString, QVariant> get() const override;
    void set(const QMap<QString, QVariant> &v) override;
    void enable(bool enabled = true) override;
    bool needVExpand() const override
    {
        return true;
    }

    void setdef() override;
    void reset() override;
    void apply() override;
    bool changed() const override;
    QStringList content() const override;

    QString askKey(const QString &suggest = "") const;
    void add(const QString &key)
    {
        add(key, key);
    }
    void add(const QString &key, const QString &trkey);
    void del(const QString &key);
    void show(const QString &key);
    void showFirst(bool updateList = false);
    void rename(const QString &target);
    QStringList keys() const;
    SettingsWrapper *getSub(const QString &key) const;

    void resetLayout();

  public slots:
    void reload();
    void update();
    void reqAdd();
    void reqDel();

  signals:
    void curChanged(QString cur);

  public:
    bool updateDisabled = false;

    QStringList rstrc;
    QMap<QString, QString> *rstrcTr = nullptr;
    KeyCheck check;
    Actions action;
    QVariant pass;

    QString cur;
    QPushButton *btnadd = nullptr;
    QPushButton *btndel = nullptr;
    QPushButton *btnmre;
    QListWidget *list;
    QStackedWidget *right;
    QMap<QString, SettingsWrapper *> rights;
    QMap<QString, QVariant> data;
};

#endif
