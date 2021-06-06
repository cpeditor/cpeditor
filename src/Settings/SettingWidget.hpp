#include "Settings/SettingsInfo.hpp"
#include <QSplitter>
#include <QWidget>

class RichTextCheckBox;
class QLineEdit;
class QListWidget;

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
    virtual void enable(bool enabled = true) = 0;
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
    virtual void init(QWidget *parent, QVariant param = QVariant()) = 0;
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
    virtual void enable(bool enabled = true) override;
};

struct LineEditWrapper : public WrapperTemplate<QString, QLineEdit>
{
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
    virtual QString get() const override;
    virtual void set(const QString &v) override;
    virtual void enable(bool enabled = true) override;
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