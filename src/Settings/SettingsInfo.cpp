#include "Settings/SettingsInfo.hpp"

QVariant SettingsInfo::SettingIter::buildChildDefault(const QString &key) const
{
    if (info->type != "Object")
        return QVariant();
    QMap<QString, QVariant> ret;
    for (const auto &si : info->child)
    {
        auto it = *this;
        it.child(key, &si);
        if (si.type == "Object")
        {
            QMap<QString, QVariant> mp;
            for (const auto &k : si.def.toStringList())
                mp[k] = it.buildChildDefault(k);
            ret[si.name] = mp;
        }
        else
            ret[si.name] = it.getDefault();
    }
    return ret;
}

int SettingsInfo::SettingInfo::findChild(const QString &name) const
{
    for (int i = 0; i < child.size(); i++)
    {
        if (child[i].name == name)
        {
            return i;
        }
    }
    return -1;
}

SettingsInfo::SettingIter &SettingsInfo::SettingIter::child(const QString &key, const QString &name)
{
    if (!info || info->type != "Object")
    {
        throw "Getting child on a value";
    }
    for (const auto &c : info->child)
    {
        if (c.name == name)
        {
            if (key != "")
            {
                pre.push_back(info->name);
                pre.push_back(key);
                trPre.push_back(info->desc);
                trPre.push_back(key);
            }
            info = &c;
            return *this;
        }
    }
    throw "Getting a child which is not exist";
}

SettingsInfo::SettingIter &SettingsInfo::SettingIter::child(const QString &key, const SettingsInfo::SettingInfo *ch)
{
    if (key != "")
    {
        pre.push_back(info->name);
        pre.push_back(key);
        trPre.push_back(info->desc);
        trPre.push_back(key);
    }
    info = ch;
    return *this;
}

QString SettingsInfo::SettingIter::_format(const QString &fmt, const QStringList &p) const
{
    QStringList res;
    int pos = 0;
    while (pos < fmt.length())
    {
        int idx = fmt.indexOf('@', pos);
        do
        {
            if (idx == -1)
                break;
            if ((idx + 1 < fmt.length() && fmt[idx + 1] == '@') || !fmt[idx + 1].isDigit())
            {
                idx = fmt.indexOf('@', idx + 2);
                continue;
            }
            break;
        } while (true);
        if (idx == -1 || idx + 1 == fmt.length())
        {
            res.push_back(fmt.mid(pos).replace("@@", "@"));
            break;
        }
        if (pos < idx)
            res.push_back(fmt.mid(pos, idx - pos).replace("@@", "@"));
        int n = fmt[idx + 1].digitValue(); // only @%d
        if (n == 0)
            res.push_back(info->desc);
        else
            res.push_back(p[p.size() - n]);
        pos = idx + 2;
    }
    return res.join("");
}
