#include <QWidget>
#include <QCheckBox>
#include "Settings/SettingsInfo.hpp"

class RichTextCheckBox;
class QLineEdit;
class QSplitter;
class QListWidget;

namespace WIP {

struct SettingBase : public QObject {
    Q_OBJECT

public:
    SettingsInfo::SettingIter iter;

    virtual QVariant getV() const = 0;
    virtual void setV(QVariant ) = 0;
    virtual QWidget *rootWidget() const = 0;

    void setdef();
    void reset();
    void apply();
    bool changed() const;

signals:
    void valueChanged();
};

template <typename Type>
struct SettingTemplate : public SettingBase {
    virtual void init(QWidget *parent, QVariant param = QVariant()) = 0;
    virtual Type get() const = 0;
    virtual void set(const Type &) = 0;

    virtual QVariant getV() const override final { return get(); }
    virtual void setV(QVariant v) override final { set(v.value<Type>()); }
};

template <typename Type, typename RootWidget>
struct WrapperTemplate : public SettingTemplate<Type> {
    virtual QWidget *rootWidget() const override final { return widget; }
protected:
    RootWidget *widget;
};

struct CheckBoxWrapper : public WrapperTemplate<bool, RichTextCheckBox> {
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
    virtual bool get() const override;
    virtual void set(const bool &v) override;
};

struct LineEditWrapper : public WrapperTemplate<QString, QLineEdit> {
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
    virtual QString get() const override;
    virtual void set(const QString &v) override;
};

struct SettingsWrapper : public WrapperTemplate<QMap<QString, QVariant>, QWidget> { // a form of settings
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
    virtual QMap<QString, QVariant> get() const override;
    virtual void set(const QMap<QString, QVariant> &v) override;

    void reload();

    void transSig();

    QStringList entries;
    QString key;
    QMap<QString, QVariant> data;
    QMap<QString, SettingBase*> wraps;
};

struct MapWrapper : public WrapperTemplate<QMap<QString, QVariant>, QSplitter> {
    virtual void init(QWidget *parent, QVariant param = QVariant()) override;
    virtual QMap<QString, QVariant> get() const override;
    virtual void set(const QMap<QString, QVariant> &v) override;

    void reload();

    void transSig();

    QListWidget *list;
    QMap<QString, QVariant> data;
};

}