#include "ipstack.h"

#include <QDebug>
#include <QQmlEngine>
#include <QJSEngine>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

void IpStackResponse::parseResponse(const QJsonDocument & root){
    if (root.isObject()){
        parseIndividual(0,root.object());
    } else if (root.isArray()){
        QJsonArray arr = root.array();
        int i = 0;
        for (auto val : arr) {
            if (val.isObject()){
                parseIndividual(i, val.toObject());
            } else {
                errors.insert(i, "JSON not an object.");
            }
        }
    } else {
        main_error = "Could not parse the root object.";
    }
}

void IpStackResponse::parseIndividual(int ix, const QJsonObject & json){
    QJsonValue json_lat = json["latitude"];
    QJsonValue json_lon = json["longitude"];
    QJsonValue json_err = json["error"];
    if (json_err.isObject()){
        QJsonObject err = json_err.toObject();
        int error_code = err["code"].toInt();
        QString info = err["info"].toString();
        QString msg = QString("Request error %1: %2").arg(error_code).arg(info);
        errors.insert(ix, msg);
    } else if (json_lat.isDouble() && json_lon.isDouble()) {
        double lat = json_lat.toDouble();
        double lon = json_lon.toDouble();
        coordinates.insert(ix, QGeoCoordinate(lat,lon));
    } else {
        errors.insert(ix, QString("Error parsing JSON response."));
    }
}

QObject* IpStack::singletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    IpStack * ipstack = new IpStack();
    return ipstack;
}

IpStack::IpStack(QObject * parent) : QObject(parent)
{
    m_manager = new QNetworkAccessManager(this);
}

void IpStack::requestFor(QVector<QString> addresses){
    if(addresses.empty()) {
        return;
    }
    QString addresses_str = addresses[0];
    for(int i = 1; i < addresses.size(); i++){
        addresses_str += ",";
        addresses_str += addresses[i];
    }

    QNetworkRequest request;
    QString request_str("http://api.ipstack.com/%1?access_key=%2");

    request_str = request_str.arg(addresses_str).arg(m_key);
    qDebug() << "Request str: " << request_str;
    request.setUrl(QUrl(request_str));
    request.setRawHeader("User-Agent", "QtApp");

    QNetworkReply *reply = m_manager->get(request);
    connect(reply, &QIODevice::readyRead, this, [this,reply](){
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        IpStackResponse response;
        response.parseResponse(document);
        emit responseReady(response);
    });
    connect(reply, &QNetworkReply::errorOccurred, this, [this,reply](QNetworkReply::NetworkError code){
        IpStackResponse response;
        response.main_error = "IpStack network error.";
        emit responseReady(response);
    });
}


