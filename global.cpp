#include "global.h"
#include <QSettings>

namespace Global {

QVariant loadSettings(const QString &key, const QVariant &val, const QString &group)
{
    QSettings settings;
    settings.beginGroup(group.isNull() ? "default_config" : group);

    return settings.value(key, val);
}

void saveSettings(const QString &key, const QVariant &val, const QString &group)
{
    QSettings settings;
    settings.beginGroup(group.isNull() ? "default_config" : group);
    settings.setValue(key, val);
    settings.endGroup();
    settings.sync();
}

}
