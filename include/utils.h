#ifndef UTILS_H
#define UTILS_H

#include <QObject>

class QQmlEngine;
class QJSEngine;

class Utils : public QObject
{
    Q_OBJECT
public:
    static QObject* singletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine);

    explicit Utils(QObject *parent = nullptr);
    Q_INVOKABLE static bool isValidHostname(const QString & hostname);
    Q_INVOKABLE static bool isValidAddress(const QString & address);
    Q_INVOKABLE static bool isValidLatitude(const QString & latitude);
    Q_INVOKABLE static bool isValidLongitude(const QString & longitude);

signals:

};

#endif // UTILS_H
