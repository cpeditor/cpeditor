#include "SettingWidget.hpp"
#include "SettingsManager.hpp"
#include "Widgets/RichTextCheckBox.hpp"
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QSplitter>
#include <QVBoxLayout>

namespace WIP
{

void SettingBase::setdef()
{
    setV(iter.info->def);
}

void SettingBase::reset()
{
    setV(SettingsManager::get(iter.key()));
}

void SettingBase::apply()
{
    if (changed())
    {
        SettingsManager::set(iter.key(), getV());
        iter.info->onApply(iter.info, nullptr, nullptr); // TODO: pass in other things maybe
    }
}

bool SettingBase::changed() const
{
    return getV() != SettingsManager::get(iter.key());
}

void CheckBoxWrapper::init(QWidget *parent, QVariant param)
{
    widget = new RichTextCheckBox(param.toString(), parent);
    connect(widget, &RichTextCheckBox::toggled, this, &SettingBase::valueChanged);
}

bool CheckBoxWrapper::get() const
{
    return widget->isChecked();
}

void CheckBoxWrapper::set(const bool &v)
{
    widget->setChecked(v);
}

void LineEditWrapper::init(QWidget *parent, QVariant)
{
    widget = new QLineEdit(parent);
    connect(widget, &QLineEdit::textChanged, this, &SettingBase::valueChanged);
}

QString LineEditWrapper::get() const
{
    return widget->text();
}

void LineEditWrapper::set(const QString &v)
{
    widget->setText(v);
}

void SettingsWrapper::init(QWidget *parent, QVariant param)
{
    key = param.toString();
    widget = new QWidget(parent);
    auto layout = new QFormLayout(widget);
    widget->setLayout(layout);
    if (!param.isNull())
    {
        for (const auto &s : param.toStringList())
        {
            if (iter.info->findChild(s) != -1)
            {
                entries.push_back(s);
            }
        }
    }
    else
    {
        for (const auto &i : iter.info->child)
        {
            entries.push_back(i.name);
        }
        std::sort(entries.begin(), entries.end());
    }
    for (const auto &name : entries)
    {
        const auto &info = iter.info->child[iter.info->findChild(name)];
        SettingBase *wrap;
        QString desc = info.desc; // TODO: add doc anchor
        auto siter = iter;
        siter.child(key, name);
        if (info.type == "bool")
        {
            SettingTemplate<bool> *w;
            if (info.ui == "QCheckBox" || info.ui.isEmpty())
            {
                w = new CheckBoxWrapper;
                w->iter = siter;
                w->init(widget, desc);
            }
            wrap = w;
        }
        else if (info.type == "Object")
        {
            MapWrapper *w = new MapWrapper;
            w->iter = siter;
            w->init(widget);
        }
        wraps[name] = wrap;
        if (info.type == "bool")
        {
            layout->addRow("", wrap->rootWidget());
        }
        else
        {
            layout->addRow(desc, wrap->rootWidget());
        }
    }
}

QMap<QString, QVariant> SettingsWrapper::get() const
{
    return data;
}

void SettingsWrapper::set(const QMap<QString, QVariant> &v)
{
    data = v;
    reload();
    transSig();
}

void SettingsWrapper::reload()
{
    for (const auto &name : entries)
    {
        auto w = wraps[name];
        disconnect(w);
        wraps[name]->setV(data[name]);
        connect(w, &SettingBase::valueChanged, this, &SettingsWrapper::transSig);
    }
}

void SettingsWrapper::transSig()
{
    emit valueChanged();
}

void MapWrapper::init(QWidget *parent, QVariant)
{
    widget = new QSplitter(parent);

    auto leftWidget = new QWidget(widget);
    auto leftLayout = new QVBoxLayout(leftWidget);
    leftWidget->setLayout(leftLayout);

    list = new QListWidget(widget);
    leftLayout->addWidget(list);

    widget->addWidget(leftWidget);
}

QMap<QString, QVariant> MapWrapper::get() const
{
    return data;
}

void MapWrapper::set(const QMap<QString, QVariant> &v)
{
    data = v;
    reload();
    transSig();
}

void MapWrapper::reload()
{
    if (iter.info->type == "Object")
    {
    }
    else
    { // QMap::xxx
    }
}

void SettingsWrapper::transSig()
{
    emit valueChanged();
}

} // namespace WIP