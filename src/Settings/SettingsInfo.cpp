#include "Settings/SettingsInfo.hpp"

QVariant SettingsInfo::SettingInfo::buildChildDefault() const
{
    if (type != "Object")
        return QVariant();
    QMap<QString, QVariant> ret;
    for (const auto &si : child)
    {
        if (si.type == "Object")
        {
            QMap<QString, QVariant> mp;
            auto d = si.buildChildDefault();
            for (const auto &k : si.def.toStringList())
            {
                mp[k] = d;
            }
            ret[si.name] = mp;
        }
        else
            ret[si.name] = si.def;
    }
    return ret;
}

SettingsInfo::SettingIter &SettingsInfo::SettingIter::child(QString key, QString name)
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
            }
            info = &c;
            return *this;
        }
    }
    throw "Getting a child which is not exist";
}

QString SettingsInfo::SettingIter::format(const QString &fmt) const
{
    QStringList res;
    int pos = 0;
    while (pos < fmt.length())
    {
        int idx;
        idx = fmt.indexOf('@', pos);
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
        else
        {
            if (pos < idx)
                res.push_back(fmt.mid(pos, idx - pos).replace("@@", "@"));
            int n = fmt[idx + 1].digitValue(); // only @%d
            if (n == 0)
                res.push_back(info->desc);
            else
                res.push_back(pre[pre.size() - n]);
            pos = idx + 2;
        }
    }
    return res.join("");
}
