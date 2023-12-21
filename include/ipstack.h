#ifndef IPSTACK_H
#define IPSTACK_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QGeoCoordinate>

class QQmlEngine;
class QJSEngine;

struct IpStackResponse{
    Q_GADGET
public:
    QMap<int,QGeoCoordinate> coordinates;
    QMap<int,QString> errors;
    QString main_error;

    void parseResponse(const QJsonDocument & root);
    void parseIndividual(int ix, const QJsonObject & json);
};
Q_DECLARE_METATYPE(IpStackResponse)


class IpStack : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString accessKey MEMBER m_key);
public:
    static QObject* singletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine);

    IpStack(QObject * parent = nullptr);
    Q_INVOKABLE void requestFor(QVector<QString> addresses);

signals:
    void responseReady(IpStackResponse response);

private:
    QString m_key = "";
    QNetworkAccessManager * m_manager = nullptr;
};

#endif // IPSTACK_H
