#include "Settings/SettingWidget.hpp"
#include "Settings/SettingsManager.hpp"
#include "Util/Util.hpp"
#include <QAction>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <QTextCodec>
#include <QVBoxLayout>

void SettingBase::checkout(int pos, QString key)
{
    iter.pre[pos] = std::move(key);
}

void SettingBase::setdef()
{
    setV(iter->def);
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
        QVariant vp;
        vp.setValue(rootWidget());
        iter.info->call("onApply", "parent", vp);
    }
}

bool SettingBase::changed() const
{
    return getV() != SettingsManager::get(iter.key());
}

QString SettingBase::docLink() const
{
    auto prefix = Util::websiteLink("docs/preferences/" + Util::sanitizeAnchorName(path.split('/').first()));
    return QString("<a href='%1#%2'>(?)</a>")
        .arg(prefix)
        .arg(iter->docAnchor.isEmpty() ? Util::sanitizeAnchorName(iter->desc) : iter->docAnchor);
}

void CheckBoxWrapper::init(QWidget *parent, QVariant param)
{
    widget = new RichTextCheckBox(param.toString() + docLink(), parent);
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
    if (iter->immediatelyApply)
        QObject::connect(wrap, &SettingBase::valueChanged, wrap, &SettingBase::apply);
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
            if (iter->findChild(s) != -1)
            {
                entries.push_back(s);
            }
        }
    }
    else
    {
        for (const auto &i : iter->child)
        {
            entries.push_back(i.name);
        }
        std::sort(entries.begin(), entries.end());
    }
    for (const auto &name : entries)
    {
        auto siter = iter;
        siter.child(key, name);
        QString desc = siter->desc;
        SettingBase *wrap = createWrapper(siter, widget, desc);
        wrap->parent = this;
        wrap->path = path + '/' + siter->name;
        wrap->trPath = trPath + '/' + siter->desc;
        SettingsManager::setPath(siter->name, path + "/" + siter->untrDesc, path + "/" + siter->desc);
        connect(wrap, &SettingBase::valueChanged, this, &SettingsWrapper::update);
        wrap->enable(false);
        wraps[name] = wrap;
        auto *label = new QLabel;
        label->setOpenExternalLinks(true);
        if (siter->type == "bool" || siter->type == "Object")
        {
            layout->addRow(label, wrap->rootWidget());
        }
        else
        {
            label->setText(desc + " " + wrap->docLink());
            layout->addRow(label, wrap->rootWidget());
        }
    }
    for (const auto &name : entries)
    {
        auto *wrap = wraps[name];
        if (wrap->iter->depends.isEmpty())
            continue;
        for (const auto &dep : wrap->iter->depends)
        {
            auto *base = locate(dep.first);
            connect(base, &SettingBase::valueChanged, this, [this, name]() { this->check(name); });
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

void SettingsWrapper::enable(bool e)
{
    if (enabled == e)
        return;
    enabled = e;
    for (const auto &name : entries)
    {
        wraps[name]->enable(enabled);
    }
    if (enabled)
    {
        for (const auto &name : entries)
        {
            check(name);
        }
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

SettingBase *SettingsWrapper::locate(const QString &name)
{
    if (wraps.contains(name))
    {
        return wraps[name];
    }
    if (parent)
    {
        auto *mp = dynamic_cast<MapWrapper *>(parent);
        auto *st = dynamic_cast<SettingsWrapper *>(mp ? mp->parent : nullptr);
        return st ? st->locate(name) : nullptr;
    }
    return nullptr;
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

void SettingsWrapper::check(const QString &name)
{
    auto *wrap = wraps[name];
    if (wrap->iter->depends.isEmpty())
        return;
    for (const auto &dep : wrap->iter->depends)
    {
        auto *base = locate(dep.first);
        if (dep.second(base->getV()))
        {
            if (!wrap->iter->requireAllDepends)
            {
                wrap->enable(enabled);
                return;
            }
        }
        else
        {
            if (wrap->iter->requireAllDepends)
            {
                wrap->enable(false);
                return;
            }
        }
    }
    if (wrap->iter->requireAllDepends)
    {
        wrap->enable(true);
    }
    else
    {
        wrap->enable(false);
    }
}

void MapWrapper::KeyCheck::init(const QVariant &cfg)
{
    for (const auto &r : cfg.toList())
    {
        auto m = r.toMap();
        rules.push_back(Rule(m["regex"].toString(), m["message"].toString()));
        if (m.contains("invert") && m["invert"].toBool())
            rules.back().invert = true;
    }
}

bool MapWrapper::KeyCheck::check(const QString &key, QString &msg) const
{
    for (const auto &rule : rules)
    {
        QRegularExpression r(rule.regex);
        if (r.match(key).hasMatch())
        {
            if (rule.invert)
            {
                msg = rule.message;
                return false;
            }
        }
        else
        {
            if (!rule.invert)
            {
                msg = rule.message;
                return false;
            }
        }
    }
    return true;
}

void MapWrapper::Actions::init(const QVariant &cfg)
{
    for (const auto &a : cfg.toList())
    {
        auto am = a.toMap();
        const auto &nm = am["name"].toString();
        const auto &mth = am["method"].toString();
        if (!parent->iter->methods.contains(mth))
        {
            continue;
        }
        data.push_back(std::make_pair(nm, mth));
    }
}

void MapWrapper::Actions::genMenu(QMenu *menu) const
{
    QVariant p;
    p.setValue(parent);
    for (const auto &d : data)
    {
        auto *act = new QAction(d.first);
        auto f = [this, &d, p, act]() {
            act->setEnabled(parent->iter->call(d.second, "widget", p, "valid", true).toBool());
        };
        QObject::connect(parent, &MapWrapper::valueChanged, f);
        QObject::connect(parent, &MapWrapper::curChanged, f);
        QObject::connect(act, &QAction::triggered,
                         [this, &d, p]() { parent->iter->call(d.second, "widget", p, "valid", false); });
        f();
        menu->addAction(act);
    }
}

void MapWrapper::init(QWidget *parent, QVariant param)
{
    action.parent = this;

    widget = new QSplitter(parent);

    auto *leftWidget = new QWidget(widget);
    auto *leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setMargin(0);
    leftWidget->setLayout(leftLayout);

    auto *name = new QLabel;
    name->setText(iter->desc + " " + docLink());
    name->setOpenExternalLinks(true);
    leftLayout->addWidget(name);

    bool allowRename = false;

    list = new QListWidget(leftWidget);
    if (!param.isNull())
    {
        auto cfg = param.toMap();
        if (cfg.contains("filter"))
            filt = cfg["filter"].toStringList();
        if (cfg.contains("restrict"))
            rstrc = cfg["restrict"].toStringList();
        if (cfg.contains("check"))
            check.init(cfg["check"]);
        if (cfg.contains("action"))
            action.init(cfg["action"]);
        if (cfg.contains("rename"))
            allowRename = cfg["rename"].toBool();
    }
    leftLayout->addWidget(list);

    auto *btnLayout = new QHBoxLayout;

    btnadd = new QPushButton(tr("Add"), leftWidget);
    btnLayout->addWidget(btnadd);
    connect(btnadd, &QPushButton::clicked, this, &MapWrapper::reqAdd);

    btndel = new QPushButton(tr("Del"), leftWidget);
    btnLayout->addWidget(btndel);
    connect(btndel, &QPushButton::clicked, this, &MapWrapper::reqDel);
    btndel->setEnabled(false);

    if (action.hasMore() || allowRename)
    {
        btnmre = new QPushButton(tr("More"), leftWidget);
        auto *menu = new QMenu;
        if (allowRename)
        {
            auto *ren = new QAction(tr("rename"));
            connect(this, &MapWrapper::curChanged, [ren](QString cur) { ren->setEnabled(cur != ""); });
            connect(ren, &QAction::triggered, [this]() {
                QString key = askKey(cur);
                if (key.isEmpty())
                    return;
                rename(key);
            });
            menu->addAction(ren);
        }
        action.genMenu(menu);
        btnmre->setMenu(menu);
        btnLayout->addWidget(btnmre);
    }

    leftLayout->addLayout(btnLayout);

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
    resetLayout();
    for (const auto &name : SettingsManager::get(iter.key() + '@').toStringList())
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
    auto odl = SettingsManager::get(iter.key() + '@').toStringList();
    QSet<QString> ods(odl.begin(), odl.end());
    for (const auto &name : data.keys())
    {
        rights[name]->apply();
        ods.remove(name);
    }
    for (const auto &name : ods)
    {
        SettingsManager::remove(SettingsManager::keyStartsWith(iter.key() + '/' + name + '/'));
        if (rights.contains(name))
        {
            delete rights[name];
            rights.remove(name);
        }
    }
    SettingsManager::set(iter.key() + '@', QVariant(data.keys()));
}

bool MapWrapper::changed() const
{
    if (data.keys() != SettingsManager::get(iter.key() + '@').toStringList())
    {
        return true;
    }
    const auto &keys = data.keys();
    return std::any_of(keys.begin(), keys.end(),
                       [this](const QString &name) -> bool { return rights[name]->changed(); });
}

QString MapWrapper::askKey(const QString &suggest) const
{
    QString key;
    bool ok = false;
    if (rstrc.isEmpty())
    {
        key = QInputDialog::getText(rootWidget(), tr("Add %1").arg(iter->desc), tr("Add %1").arg(iter->desc),
                                    QLineEdit::Normal, suggest, &ok);
    }
    else
    {
        QStringList items = rstrc;
        items.erase(std::remove_if(items.begin(), items.end(), [this](const QString &r) { return rights.contains(r); }),
                    items.end());
        if (items.isEmpty())
        {
            QMessageBox::warning(rootWidget(), tr("Add failed"), tr("All possible names have been used."));
            return QString();
        }
        auto idx = items.indexOf(suggest);
        key = QInputDialog::getItem(rootWidget(), tr("Add %1").arg(iter->desc), tr("Select %1").arg(iter->desc), items,
                                    std::max(idx, 0), false, &ok);
    }
    if (!ok)
        return QString();
    if (rights.contains(key))
    {
        QMessageBox::warning(rootWidget(), tr("Add failed"),
                             QString(tr("%1 %2 already exists")).arg(iter->desc).arg(key));
        return QString();
    }
    QString message;
    if (!check.check(key, message))
    {
        QMessageBox::warning(rootWidget(), tr("Add failed"), message);
        return QString();
    }
    return key;
}

void MapWrapper::add(const QString &key)
{
    list->addItem(key);
    auto *panel = new SettingsWrapper;
    panel->parent = this;
    panel->iter = iter;
    panel->setKey(key);
    panel->path = path + "/" + iter->name;
    panel->trPath = trPath + "/" + iter->desc;
    panel->init(widget, filt);
    rights[key] = panel;
    right->layout()->addWidget(panel->rootWidget());
    connect(panel, &SettingsWrapper::valueChanged, this, &MapWrapper::update);
    panel->rootWidget()->setVisible(false);
    panel->enable(true);
    update();
}

void MapWrapper::del(const QString &key)
{
    for (int i = 0; i < list->count(); i++)
    {
        if (list->item(i)->text() == key)
        {
            if (list->count() > 1)
            {
                if (i == 0)
                {
                    show(list->item(1)->text());
                }
                else
                {
                    show(list->item(i - 1)->text());
                }
            }
            else
            {
                show("");
            }
            rights[key]->deleteLater();
            rights.remove(key);
            delete list->takeItem(i);
            data.remove(key);
            emit valueChanged();
            break;
        }
    }
}

void MapWrapper::show(const QString &key)
{
    if (cur != "")
    {
        rights[cur]->rootWidget()->setVisible(false);
    }
    if (key != "")
    {
        rights[key]->rootWidget()->setVisible(true);
    }
    btndel->setEnabled(key != "");
    cur = key;
    emit curChanged(cur);
}

void MapWrapper::rename(const QString &target)
{
    if (cur == "")
        return;
    data[target] = data.take(cur);
    reload();
    show(target);
    emit valueChanged();
}

QStringList MapWrapper::keys() const
{
    return rights.keys();
}

SettingsWrapper *MapWrapper::getSub(const QString &key) const
{
    if (rights.contains(key))
        return rights[key];
    return nullptr;
}

void MapWrapper::resetLayout() const
{
    list->clear();
    QWidget().setLayout(right->layout());
    auto *rightLayout = new QVBoxLayout(right);
    rightLayout->setMargin(0);
    right->setLayout(rightLayout);
}

void MapWrapper::reload()
{
    resetLayout();
    updateDisabled = true;
    for (const auto &name : data.keys())
    {
        add(name);
        rights[name]->setV(data[name]);
    }
    updateDisabled = false;
    cur = "";
    show("");
}

void MapWrapper::update()
{
    if (updateDisabled)
        return;
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

void MapWrapper::reqAdd()
{
    auto key = askKey();
    if (key != "")
        add(key);
}

void MapWrapper::reqDel()
{
    del(list->currentItem()->text());
}
