#ifndef EXTENDEDTABLEMODEL_H
#define EXTENDEDTABLEMODEL_H

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlError>

#include "ipstack.h"

class ExtendedTableModel : public QSqlTableModel
{
    Q_OBJECT
    Q_PROPERTY(IpStack * ipStack READ getIpStack WRITE setIpStack NOTIFY ipStackChanged);
    Q_PROPERTY(QSqlTableModel::EditStrategy editStrategy READ editStrategy WRITE setEditStrategy NOTIFY editStrategyChanged);
public:
    explicit ExtendedTableModel(QObject *parent = nullptr, const QSqlDatabase & db = QSqlDatabase());

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Q_INVOKABLE bool setHeaderData(int, Qt::Orientation, const QVariant &, int) override;
    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE void revertRow(int) override;
    
    Q_INVOKABLE QString lastError();
    Q_INVOKABLE void insertEmptyRecord(int);
    Q_INVOKABLE void clearSelected();
    Q_INVOKABLE void removeSelected();
    Q_INVOKABLE void updateCoordinatesSelected();

    void setEditStrategy(QSqlTableModel::EditStrategy) override;
    QSqlTableModel::EditStrategy editStrategy();

    IpStack * getIpStack();
    void setIpStack(IpStack *);
signals:
    void editStrategyChanged();
    void ipStackChanged();


protected slots:
    void processResponse(const IpStackResponse &);

private:
    QSet<int> m_selected;
    IpStack * m_ipstack;
    QSqlTableModel::EditStrategy m_edit_strategy;
    //IpStack query related data
    QVector<QString> m_hostnames;
    QVector<int> m_rows;
    bool m_querying = false;
};

#endif // EXTENDEDTABLEMODEL_H
