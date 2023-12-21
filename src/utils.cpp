#include "utils.h"

#include <QDebug>
#include <QHostAddress>
#include <QRegularExpression>


QObject* Utils::singletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    Utils * utils = new Utils();
    return utils;
}

Utils::Utils(QObject *parent)
    : QObject{parent}
{
}

bool Utils::isValidHostname(const QString & address){
    QRegularExpression re("^([a-zA-Z0-9]+(-[a-zA-Z0-9]+)*)+(\\.([a-zA-Z0-9]+(-[a-zA-Z0-9]+)*))*$");
    return re.match(address).hasMatch();
}

bool Utils::isValidAddress(const QString & address){
    QHostAddress ip_check;
    return ip_check.setAddress(address);
}

bool Utils::isValidLatitude(const QString & latitude){
    bool ok = false;
    double lat = latitude.toDouble(&ok);
    return ok && lat <= 90 && lat >= -90;
}

bool Utils::isValidLongitude(const QString & longitude){
    bool ok = false;
    double lon = longitude.toDouble(&ok);
    return ok && lon <= 180 && lon >= -180;
}
