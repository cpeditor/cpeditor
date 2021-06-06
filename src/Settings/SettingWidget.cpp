#include "Settings/SettingWidget.hpp"
#include "Settings/SettingsManager.hpp"
#include <QFormLayout>
#include <QHBoxLayout>
#include <QTextCodec>
#include <QVBoxLayout>

namespace WIP
{

void SettingBase::checkout(int pos, QString key)
{
    iter.pre[pos] = std::move(key);
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

void LineEditWrapper::init(QWidget *parent, QVariant param)
{
    widget = new QLineEdit(parent);
    widget->setMinimumWidth(400);
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

void PlainTextEditWrapper::init(QWidget *parent, QVariant param)
{
    widget = new QPlainTextEdit(parent);
    widget->setMinimumWidth(400);
    widget->setWordWrapMode(QTextOption::NoWrap);
    connect(widget, &QPlainTextEdit::textChanged, this, &SettingBase::valueChanged);
}

QString PlainTextEditWrapper::get() const
{
    return widget->toPlainText();
}

void PlainTextEditWrapper::set(const QString &v)
{
    widget->setPlainText(v);
}

void ComboBoxWrapper::init(QWidget *parent, QVariant param)
{
    widget = new QComboBox(parent);
    widget->addItems(param.toStringList());
    connect(widget, &QComboBox::currentTextChanged, this, &SettingBase::valueChanged);
}

QString ComboBoxWrapper::get() const
{
    return widget->currentText();
}

void ComboBoxWrapper::set(const QString &v)
{
    widget->setCurrentText(v);
}

void PathItemWrapper::init(QWidget *parent, QVariant param)
{
    widget = new PathItem(PathItem::Type(param.toInt()), parent);
    connect(widget->getLineEdit(), &QLineEdit::textChanged, this, &SettingBase::valueChanged);
}

QString PathItemWrapper::get() const
{
    return widget->getLineEdit()->text();
}

void PathItemWrapper::set(const QString &v)
{
    widget->getLineEdit()->setText(v);
}

void ShortcutItemWrapper::init(QWidget *parent, QVariant param)
{
    widget = new ShortcutItem(parent);
    connect(widget, &ShortcutItem::shortcutChanged, this, &SettingBase::valueChanged);
}

QString ShortcutItemWrapper::get() const
{
    return widget->getShortcut();
}

void ShortcutItemWrapper::set(const QString &v)
{
    widget->setShortcut(v);
}

void CodecBoxWrapper::init(QWidget *parent, QVariant param)
{
    QStringList names;
    for (const auto &mib : QTextCodec::availableMibs())
    {
        names.push_back(QString::fromLocal8Bit(QTextCodec::codecForMib(mib)->name()));
    }
    names.sort(Qt::CaseInsensitive);
    names.removeDuplicates();
    ComboBoxWrapper::init(parent, names);
}

void SpinBoxWrapper::init(QWidget *parent, QVariant param)
{
    widget = new QSpinBox(parent);
    if (!param.isNull())
    {
        QVariantList il = param.toList();
        widget->setRange(il[0].toInt(), il[1].toInt());
        if (il.length() >= 3)
            widget->setSingleStep(il[2].toInt());
    }
    connect(widget, QOverload<int>::of(&QSpinBox::valueChanged), this, &SettingBase::valueChanged);
}

int SpinBoxWrapper::get() const
{
    return widget->value();
}

void SpinBoxWrapper::set(const int &v)
{
    widget->setValue(v);
}

void ScrollBarWrapper::init(QWidget *parent, QVariant param)
{
    widget = new QScrollBar(Qt::Horizontal, parent);
    if (!param.isNull())
    {
        QVariantList il = param.toList();
        widget->setRange(il[0].toInt(), il[1].toInt());
        if (il.length() >= 3)
            widget->setSingleStep(il[2].toInt());
    }
    connect(widget, &QScrollBar::valueChanged, this, &SettingBase::valueChanged);
}

int ScrollBarWrapper::get() const
{
    return widget->value();
}

void ScrollBarWrapper::set(const int &v)
{
    widget->setValue(v);
}

void SliderWrapper::init(QWidget *parent, QVariant param)
{
    widget = new QSlider(Qt::Horizontal, parent);
    if (!param.isNull())
    {
        QVariantList il = param.toList();
        widget->setRange(il[0].toInt(), il[1].toInt());
        if (il.length() >= 3)
            widget->setSingleStep(il[2].toInt());
    }
    connect(widget, &QSlider::valueChanged, this, &SettingBase::valueChanged);
}

int SliderWrapper::get() const
{
    return widget->value();
}

void SliderWrapper::set(const int &v)
{
    widget->setValue(v);
}

void FontItemWrapper::init(QWidget *parent, QVariant param)
{
    widget = new FontItem(parent, param);
    connect(widget, &FontItem::fontChanged, this, &SettingBase::valueChanged);
}

QFont FontItemWrapper::get() const
{
    return widget->getFont();
}

void FontItemWrapper::set(const QFont &v)
{
    widget->setFont(v);
}

void StringListsItemWrapper::init(QWidget *parent, QVariant param)
{
    widget = new StringListsItem(param.toList(), parent);
    connect(widget, &StringListsItem::valueChanged, this, &SettingBase::valueChanged);
}

QVariantList StringListsItemWrapper::get() const
{
    return widget->getStringLists();
}

void StringListsItemWrapper::set(const QVariantList &v)
{
    widget->setStringLists(v);
}

static SettingBase *createWrapper(const SettingsInfo::SettingIter &iter, QWidget *widget, const QString &desc)
{
    const auto &info = *iter.info;
    SettingBase *wrap = nullptr;
    if (info.type == "bool")
    {
        if (info.ui == "QCheckBox" || info.ui.isEmpty())
            wrap = new CheckBoxWrapper;
        else
            Q_UNREACHABLE();
    }
    else if (info.type == "QString")
    {
        if (info.ui.isEmpty() || info.ui == "QLineEdit")
            wrap = new LineEditWrapper();
        else if (info.ui == "QPlainTextEdit")
            wrap = new PlainTextEditWrapper();
        else if (info.ui == "QComboBox")
            wrap = new ComboBoxWrapper();
        else if (info.ui == "PathItem")
            wrap = new PathItemWrapper();
        else if (info.ui == "ShortcutItem")
            wrap = new ShortcutItemWrapper();
        else if (info.ui == "CodecBox")
            wrap = new CodecBoxWrapper();
        else
            Q_UNREACHABLE();
    }
    else if (info.type == "int")
    {
        if (info.ui.isEmpty() || info.ui == "QSpinBox")
            wrap = new SpinBoxWrapper();
        else if (info.ui == "QScrollBar")
            wrap = new ScrollBarWrapper();
        else if (info.ui == "QSlider")
            wrap = new SliderWrapper();
        else
            Q_UNREACHABLE();
    }
    else if (info.type == "QFont")
    {
        if (info.ui.isEmpty() || info.ui == "FontItem")
            wrap = new FontItemWrapper();
        else
            Q_UNREACHABLE();
    }
    else if (info.type == "QVariantList")
    {
        if (info.ui.isEmpty() || info.ui == "StringListsItem")
            wrap = new StringListsItemWrapper();
        else
            Q_UNREACHABLE();
    }
    else if (info.type == "Object")
    {
        wrap = new MapWrapper;
    }
    else
        Q_UNREACHABLE();
    wrap->iter = iter;
    wrap->init(widget, info.type == "bool" ? desc : info.param);
    return wrap;
}

void SettingsWrapper::init(QWidget *parent, QVariant param)
{
    widget = new QWidget(parent);
    auto *layout = new QFormLayout(widget);
    layout->setMargin(0);
    widget->setLayout(layout);
    auto filt = param.toStringList();
    if (filt.length() > 0)
    {
        for (const auto &s : filt)
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
        siter.child(key, name);
        QString desc = siter.info->desc; // TODO: add doc anchor
        SettingBase *wrap = createWrapper(siter, widget, desc);
        connect(wrap, &SettingBase::valueChanged, this, &SettingsWrapper::update);
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
    emit valueChanged();
}

void SettingsWrapper::enable(bool enabled)
{
    for (const auto &name : entries)
    {
        wraps[name]->enable(enabled);
    }
}

void SettingsWrapper::setdef()
{
    for (const auto &name : entries)
    {
        wraps[name]->setdef();
    }
    update();
}

void SettingsWrapper::reset()
{
    for (const auto &name : entries)
    {
        wraps[name]->reset();
    }
    update();
}

void SettingsWrapper::apply()
{
    for (const auto &name : entries)
    {
        wraps[name]->apply();
    }
    update();
}

bool SettingsWrapper::changed() const
{
    return std::any_of(entries.begin(), entries.end(), [this](const QString &name) { return wraps[name]->changed(); });
}

void SettingsWrapper::reload()
{
    for (const auto &name : entries)
    {
        wraps[name]->setV(data[name]);
    }
}

void SettingsWrapper::update()
{
    bool updated = false;
    for (const auto &name : entries)
    {
        auto &od = data[name];
        auto nw = wraps[name]->getV();
        if (od != nw)
        {
            od = nw;
            updated = true;
        }
    }
    if (updated)
        emit valueChanged();
}

void MapWrapper::init(QWidget *parent, QVariant param)
{
    cur = "";
    widget = new QSplitter(parent);

    auto *leftWidget = new QWidget(widget);
    auto *leftLayout = new QVBoxLayout(leftWidget);
    leftWidget->setLayout(leftLayout);

    list = new QListWidget(leftWidget);
    if (!param.isNull())
    {
        auto cfg = param.toMap();
        if (cfg.contains("filter"))
            filt = cfg["filter"].toStringList();
    }
    leftLayout->addWidget(list);

    widget->addWidget(leftWidget);

    right = new QWidget(widget);
    auto *rightLayout = new QVBoxLayout(right);
    rightLayout->setMargin(0);
    right->setLayout(rightLayout);

    connect(list, &QListWidget::currentTextChanged, this, &MapWrapper::show);
}

QMap<QString, QVariant> MapWrapper::get() const
{
    return data;
}

void MapWrapper::set(const QMap<QString, QVariant> &v)
{
    data = v;
    reload();
    emit valueChanged();
}

void MapWrapper::enable(bool enabled)
{
    if (enabled)
    {
        list->setEnabled(true);
        if (cur != "")
        {
            rights[cur]->enable(true);
        }
    }
    else
    {
        list->setEnabled(false);
        show("");
    }
}

void MapWrapper::setdef()
{
    set(QMap<QString, QVariant>());
}

void MapWrapper::reset()
{
    list->clear();
    delete right->layout();
    auto *rightLayout = new QVBoxLayout(right);
    rightLayout->setMargin(0);
    right->setLayout(rightLayout);
    SettingsManager::itemUnder(iter.key() + "/");
    for (const auto &name : SettingsManager::itemUnder(iter.key() + "/"))
    {
        add(name);
        rights[name]->reset();
    }
    data.clear();
    update();
    show("");
}

void MapWrapper::apply()
{
    for (const auto &name : data.keys())
    {
        rights[name]->apply();
    }
}

bool MapWrapper::changed() const
{
    if (data.keys() != SettingsManager::itemUnder(iter.key() + "/"))
    {
        return true;
    }
    for (const auto &name : data.keys())
    {
        if (rights[name]->changed())
        {
            return true;
        }
    }
    return false;
}

void MapWrapper::add(QString key)
{
    list->addItem(key);
    auto *panel = new SettingsWrapper;
    panel->iter = iter;
    panel->setKey(key);
    panel->init(widget, filt);
    rights[key] = panel;
    right->layout()->addWidget(panel->rootWidget());
    connect(panel, &SettingsWrapper::valueChanged, this, &MapWrapper::update);
    panel->rootWidget()->setVisible(false);
    panel->enable(true);
}

void MapWrapper::show(QString key)
{
    if (cur != "")
    {
        rights[cur]->rootWidget()->setVisible(false);
    }
    if (key != "")
    {
        rights[key]->rootWidget()->setVisible(true);
    }
    cur = key;
}

void MapWrapper::reload()
{
    list->clear();
    delete right->layout();
    auto *rightLayout = new QVBoxLayout(right);
    rightLayout->setMargin(0);
    right->setLayout(rightLayout);
    for (const auto &name : data.keys())
    {
        add(name);
        rights[name]->reload();
    }
    show("");
}

void MapWrapper::update()
{
    bool updated = false;
    for (const auto &name : rights.keys())
    {
        auto &od = data[name];
        auto nw = rights[name]->getV();
        if (od != nw)
        {
            od = nw;
            updated = true;
        }
    }
    if (updated)
        emit valueChanged();
}

} // namespace WIP
