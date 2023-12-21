#include "database.h"

#include <QtConcurrent>
#include <QQmlEngine>
#include <QJSEngine>

DatabaseChecker::DatabaseChecker(QObject * parent, const QSqlDatabase & db) : QThread(parent), m_db(db) {
}

void DatabaseChecker::run(){
    bool result = m_db.isOpen();
    emit pingResult(result);
    QThread::sleep(5);
    if (m_go){
        run();
    }
}

void DatabaseChecker::stop(){
    m_go = false;
}

void DatabaseChecker::startPing(const QSqlDatabase & db){
    m_go = true;
    m_db = db;
    start();
}


QObject* Database::singletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    Database * database = new Database();
    return database;
}

Database::Database(QObject *parent)
    : QObject{parent}
{
    QObject::connect(this, &Database::connectedChanged, this, [this](){
        if (m_connected){
            prepareTableModel();
        }
    });
    
    QObject::connect(this, &Database::errorMsg, this, [this](QString msg){
        qDebug() << "Error:" << msg;
    });
    //Set up the thread.
    m_checker = new DatabaseChecker(this, m_database);
    QObject::connect(m_checker, &DatabaseChecker::pingResult, this, [this](bool ping){
        if (m_ping_status == ping){
            return;
        }
        m_ping_status = ping;
        emit pingStatusChanged();
    });
    QObject::connect(this, &Database::stopPing, m_checker, &DatabaseChecker::stop);
    QObject::connect(this, &Database::connectedChanged, m_checker, [this](){
        if (m_connected == true){
            m_checker->startPing(m_database);
        }
    });
}

bool Database::connect(const QString & host, const QString & db_name, const QString & username, const QString & password) {
    if (m_connected) {
        return false;
    }

    if (m_table_model){
        delete m_table_model;
        m_table_model = nullptr;
        emit tableModelChanged();
    }
    
    if (m_database.isOpen()){
        disconnect();
    }

    m_database = QSqlDatabase::addDatabase("QMARIADB");
    m_database.setHostName(host);
    m_database.setDatabaseName(db_name);
    m_database.setUserName(username);
    m_database.setPassword(password);
    bool opened = m_database.open();
    if (opened) {
        m_connected = true;
        emit connectedChanged();
    } else if (m_database.isOpenError()) {
        emit errorMsg(m_database.lastError().databaseText());
    }
    return opened;
}

bool Database::disconnect(){
    if (m_connected == false){
        return false;
    }

    if (m_table_model){
        delete m_table_model;
        m_table_model = nullptr;
        emit tableModelChanged();
    }
    
    emit stopPing();

    QString db_name = m_database.databaseName();
    m_database.close();
    QSqlDatabase::removeDatabase(db_name);

    m_connected = false;
    emit connectedChanged();
    return true;
}

void Database::prepareTableModel(QString table_name){
    if (m_table_model == nullptr){
        m_table_model = new ExtendedTableModel(this,  m_database);
    }
    m_table_model->setTable(table_name);
    m_table_model->setHeaderData(0,Qt::Horizontal, "Hostname",Qt::DisplayRole);
    m_table_model->setHeaderData(1,Qt::Horizontal, "Lat.",Qt::DisplayRole);
    m_table_model->setHeaderData(2,Qt::Horizontal, "Lon.",Qt::DisplayRole);
    m_table_model->setEditStrategy(ExtendedTableModel::OnManualSubmit);
    bool ok = m_table_model->select();
    if(!ok) {
        qDebug() << "Failed to select the" << table_name << "table:" << m_table_model->lastError();
        emit errorMsg(m_table_model->lastError());
    }
    emit tableModelChanged();
}

ExtendedTableModel * Database::getTableModel(){
    return m_table_model;
}

bool Database::getConnected(){
    return m_connected;
}

bool Database::getPingStatus(){
    return m_ping_status;
}
