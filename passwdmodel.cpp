#include "passwdmodel.h"
#include <QDebug>

PasswdModel::PasswdModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

void PasswdModel::setReccord(QVector<PassRecord> rec) {
    beginResetModel();
    this->rec = rec;
    endResetModel();
}

void PasswdModel::refresh() {
    beginResetModel();
    endResetModel();
}

int PasswdModel::rowCount(const QModelIndex & /*parent*/) const
{
    return this->rec.count();
}

int PasswdModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 3;
}

QVariant PasswdModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (role == Qt::DisplayRole) {
        PassRecord recd = this->rec[index.row()];
        if (index.column() == 0) {
            return recd.des;
        } else if (index.column() == 1) {
            return recd.account;
        } else if (index.column() == 2) {
            return recd.passwd;
        }
    }
    return QVariant();
}

QVariant PasswdModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal) {
        QString title[] = {tr("描述"), tr("帐号"), tr("密码")};
        return title[section];
    } else if (Qt::Vertical) {
        return section + 1;
    }
    return QVariant();
}
