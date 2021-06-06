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

void SettingBase::checkout(int pos, QString key)
{
    iter.pre[pos] = key;
}

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
        iter.info->onApply(iter.info, nullptr, rootWidget()); // TODO: pass in other things maybe
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

void CheckBoxWrapper::enable(bool enabled)
{
    widget->setEnabled(enabled);
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

void LineEditWrapper::enable(bool enabled)
{
    widget->setEnabled(enabled);
}

static SettingBase *createWrapper(SettingsInfo::SettingIter iter, QWidget *widget, QString desc)
{
    const auto &info = *iter.info;
    SettingBase *wrap;
    if (info.type == "bool")
    {
        SettingTemplate<bool> *w;
        if (info.ui == "QCheckBox" || info.ui.isEmpty())
        {
            w = new CheckBoxWrapper;
            w->iter = iter;
            w->init(widget, desc);
        }
        wrap = w;
    }
    else if (info.type == "Object")
    {
        MapWrapper *w = new MapWrapper;
        w->iter = iter;
        w->init(widget);
        wrap = w;
    }
    return wrap;
}

void SettingsWrapper::init(QWidget *parent, QVariant param)
{
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
        auto siter = iter;
        siter.child("/", name);
        QString desc = siter.info->desc; // TODO: add doc anchor
        SettingBase *wrap = createWrapper(siter, widget, desc);
        wrap->enable(false);
        wraps[name] = wrap;
        if (siter.info->type == "bool")
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

void SettingsWrapper::enable(bool enabled)
{
    for (const auto &name : entries)
    {
        wraps[name]->enable(enabled);
    }
}

void SettingsWrapper::checkout(int pos, QString key)
{
    SettingBase::checkout(pos, key);
    for (const auto &name : entries)
    {
        wraps[name]->checkout(pos, key);
    }
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

void MapWrapper::init(QWidget *parent, QVariant param)
{
    cur = "";
    widget = new QSplitter(parent);

    auto leftWidget = new QWidget(widget);
    auto leftLayout = new QVBoxLayout(leftWidget);
    leftWidget->setLayout(leftLayout);

    list = new QListWidget(widget);
    leftLayout->addWidget(list);

    widget->addWidget(leftWidget);

    right = new SettingsWrapper;
    right->iter = iter;
    right->init(widget, param);

    widget->addWidget(right->rootWidget());

    connect(list, &QListWidget::currentTextChanged, this, &MapWrapper::select);
    connect(right, &SettingsWrapper::valueChanged, this, &MapWrapper::update);
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

void MapWrapper::enable(bool enabled)
{
    if (enabled)
    {
        right->enable(cur != "");
    }
    else
    {
        right->enable(false);
    }
}

void MapWrapper::reload()
{
    list->clear();
    list->addItems(data.keys());
    right->enable(false);
}

void MapWrapper::transSig()
{
    emit valueChanged();
}

void MapWrapper::select(QString key)
{
    cur = key;
    right->checkout(iter.pre.length() + 1, key);
    right->set(data[key].toMap());
    right->enable(true);
}

void MapWrapper::update()
{
    auto od = data[cur];
    auto nw = right->getV();
    if (od != nw)
    {
        data[cur] = nw;
        transSig();
    }
}

} // namespace WIP