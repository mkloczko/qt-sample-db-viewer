#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlTableModel>
#include <QThread>
#include <QFuture>

#include "extendedtablemodel.h"

class QQmlEngine;
class QJSEngine;

class DatabaseChecker : public QThread
{
    Q_OBJECT
public:
    DatabaseChecker(QObject * parent = nullptr, const QSqlDatabase & db = QSqlDatabase());
public slots:
    void stop();
    void startPing(const QSqlDatabase &);
protected:
    void run() override;
signals:
    void pingResult(bool);
private:
    QSqlDatabase m_db;
    bool m_go = true;
};

class Database : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ExtendedTableModel * tableModel READ getTableModel NOTIFY tableModelChanged)
    Q_PROPERTY(bool connected READ getConnected NOTIFY connectedChanged);
    Q_PROPERTY(bool pingStatus READ getPingStatus NOTIFY pingStatusChanged);
public:
    static QObject* singletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine);

    explicit Database(QObject *parent = nullptr);
    ExtendedTableModel * getTableModel();
    bool getConnected();
    bool getPingStatus();
    

public slots:
    Q_INVOKABLE bool connect(const QString & host, const QString & db_name, const QString & username, const QString & password);
    Q_INVOKABLE bool disconnect();
    Q_INVOKABLE void prepareTableModel(QString table_name = "locations");

signals:
    void tableModelChanged();
    void connectedChanged();
    void stopPing();
    void pingStatusChanged();
    void errorMsg(QString);

private:
    QSqlDatabase m_database;
    ExtendedTableModel * m_table_model = nullptr;
    DatabaseChecker * m_checker = nullptr;
    bool m_connected = false;
    bool m_ping_status = false;
};

#endif // DATABASE_H
