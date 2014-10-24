#ifndef PASSWDMODEL_H
#define PASSWDMODEL_H

#include <QAbstractTableModel>
#include <QVector>

#include "passwdfileutil.h"

class PasswdModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit PasswdModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void setReccord(QVector<PassRecord>);
    void refresh();
    QVector<PassRecord> rec;
private:



signals:

public slots:

};

#endif // PASSWDMODEL_H
