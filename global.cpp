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

QString MakeGeoPrintable(double val, GeoType type)
{
    static QString buffer;
    const char	*pszHemisphere = NULL;

    double dfRound = 0.5 / 60 * 0.01;

    int nDegrees = (int) fabs(val);
    int nMinutes = (int) ((fabs(val) - nDegrees) * 60 + dfRound);
    double dfSeconds = fabs((fabs(val) * 3600 - nDegrees*3600 - nMinutes*60));

    if( type == GeoType::Longitude && val < 0.0 )
        pszHemisphere = "W";
    else if( type == GeoType::Longitude )
        pszHemisphere = "E";
    else if( val < 0.0 )
        pszHemisphere = "S";
    else
        pszHemisphere = "N";

    buffer.sprintf("%2dd %2d\'%.3f\" %s", nDegrees, nMinutes, dfSeconds, pszHemisphere);

    return buffer;
}

QString MakeMeterPrintable(double val)
{
    if(val > 1000)
        return QStringLiteral(" %1km ").arg(val / 1000.0, 0, 'f', 3);
    else
        return QStringLiteral(" %1m ").arg(val, 0, 'f', 2);
}

}
