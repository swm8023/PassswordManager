#include "dbutil.h"
#include "passwdfileutil.h"

DButil::DButil(const QString filename)
{
    this->filename = filename;

}



DButil::~DButil()
{

}

void DButil::initDB()
{
    QSqlDatabase::addDatabase("QSQLITE");
}

bool DButil::exec(const QString& sql)
{
    QSqlDatabase db = QSqlDatabase::database();
    db.setDatabaseName(this->filename);
    bool ok = db.open();
    if (!ok) {
        qDebug() << "connect to sqlite failed";
        return false;
    }

    QSqlQuery query(db);
    query.exec(sql);
    if (!query.isActive()) {
        db.close();
        qDebug() << "exec sql error: " << sql;
        return false;
    }
    db.close();
    return true;
}

bool DButil::execp(const QString& sql, QVector<QVariant> val)
{
    QSqlDatabase db = QSqlDatabase::database();
    db.setDatabaseName(this->filename);
    bool ok = db.open();
    if (!ok) {
        qDebug() << "connect to sqlite failed";
        return false;
    }

    QSqlQuery query(db);
    query.prepare(sql);
    for (int i = 0; i < val.size(); i++) {
        query.bindValue(i, val[i]);
    }

    query.exec();
    if (!query.isActive()) {
        db.close();
        qDebug() << "exec sql error: " << sql;
        return false;
    }
    db.close();
    return true;
}


QVector<QSqlRecord> DButil::query(const QString& sql)
{
    QSqlDatabase db = QSqlDatabase::database();
    db.setDatabaseName(this->filename);
    QVector<QSqlRecord> rvec;

    bool ok = db.open();
    if (!ok) {

        qDebug() << "connect to sqlite failed";
        return rvec;
    }

    QSqlQuery query(db);

    query.exec(sql);
    if (!query.isActive()) {
        db.close();
        qDebug() << "exec sql error: " << sql;
        return rvec;
    }

    while (query.next()) {
        rvec.append(query.record());
    }

    return rvec;
}
