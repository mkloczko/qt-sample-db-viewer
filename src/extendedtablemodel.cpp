#include "extendedtablemodel.h"

#include <QSqlField>


ExtendedTableModel::ExtendedTableModel(QObject *parent, const QSqlDatabase & db)
    : QSqlTableModel{parent,db}
{
    m_edit_strategy = QSqlTableModel::editStrategy();
}

QVariant ExtendedTableModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if (orientation == Qt::Horizontal) {
        return QSqlTableModel::headerData(section,orientation,role);
    }

    return QVariant(m_selected.contains(section));
}

bool ExtendedTableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role){
    if (orientation == Qt::Horizontal){
        return QSqlTableModel::setHeaderData(section,orientation,value,role);
    }

    if (value.typeId() == QMetaType::Bool) {
        if (value.toBool()){
            m_selected.insert(section);
        } else {
            m_selected.remove(section);
        }
        emit headerDataChanged(orientation,section,section);
        return true;
    }
    return false;
}

QHash<int, QByteArray> ExtendedTableModel::roleNames() const
{
    return { {Qt::DisplayRole, "display"}, {Qt::BackgroundRole, "background"} };
}


QString ExtendedTableModel::lastError(){
    return QSqlTableModel::lastError().text();
}

void ExtendedTableModel::revertRow(int row){
    return QSqlTableModel::revertRow(row);
}

void ExtendedTableModel::insertEmptyRecord(int row){
    QSqlRecord rec = QSqlTableModel::record();
    QSqlTableModel::insertRecord(row,rec);
}

void ExtendedTableModel::clearSelected(){
    m_selected.clear();
    emit headerDataChanged(Qt::Vertical,0,columnCount()+1);
}
void ExtendedTableModel::removeSelected(){
    QVector<int> sorted = m_selected.values();
    std::sort(sorted.begin(), sorted.end(), [](int a, int b){return a > b;});
    for(int s : sorted){
        bool success = removeRows(s, 1);
        if (!success){
            qDebug() << "Could not delete " << s;
        } else {
            m_selected.remove(s);
        }
    }
}

void ExtendedTableModel::updateCoordinatesSelected(){
    if(m_querying || m_ipstack == nullptr){
        return;
    }

    m_rows.clear();
    m_hostnames.clear();
    m_rows.reserve(m_selected.size());
    m_hostnames.reserve(m_selected.size());

    for(int row : m_selected){
        QString candidate = record(row).field(0).value().toString();
        if(candidate != ""){
            m_hostnames.push_back(std::move(candidate));
            m_rows.push_back(row);
        }
    }

    if(m_hostnames.size() > 0){
        m_querying = true;
        m_ipstack->requestFor(m_hostnames);
    }
}

void ExtendedTableModel::processResponse(const IpStackResponse & response){
    if (m_querying == false){
        qDebug() << "Received unexpected IpStack response.";
        return;
    }

    if (response.main_error != ""){
        qDebug() << "Response error:" << response.main_error;
    }

    for(auto kv = response.coordinates.begin(); kv != response.coordinates.end(); kv++){
        int ix = kv.key();
        QGeoCoordinate val = kv.value();

        QSqlRecord rec = record(m_rows[ix]);
        rec.setValue(1,val.latitude());
        rec.setValue(2,val.longitude());
        setRecord(m_rows[ix],rec);
    }

    for(auto kv = response.errors.begin(); kv != response.errors.end(); kv++){
        int ix = kv.key();
        qDebug() << "Error processing row" << m_rows[ix] << ": " << kv.value();
    }

    m_querying = false;
}

void ExtendedTableModel::setEditStrategy(QSqlTableModel::EditStrategy edit_strategy){
    if(m_edit_strategy == edit_strategy){
        return;
    }
    m_edit_strategy = edit_strategy;
    QSqlTableModel::setEditStrategy(m_edit_strategy);
    emit editStrategyChanged();
}

QSqlTableModel::EditStrategy ExtendedTableModel::editStrategy(){
    return m_edit_strategy;
}

IpStack * ExtendedTableModel::getIpStack(){
    return m_ipstack;
}

void ExtendedTableModel::setIpStack(IpStack * ipstack){
    if (m_ipstack == ipstack){
        return;
    }
    if (m_ipstack != nullptr){
        disconnect(ipstack,nullptr, this, nullptr);
    }
    if (ipstack != nullptr){
        connect(ipstack, &IpStack::responseReady, this, &ExtendedTableModel::processResponse);
    }
    m_ipstack = ipstack;
    emit ipStackChanged();
}

