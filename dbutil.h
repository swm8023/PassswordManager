#ifndef DBUTIL_H
#define DBUTIL_H

#include <QString>
#include <QtSql>

class DButil
{
public:
    DButil(const QString);
    ~DButil();

    bool exec(const QString&);
    bool execp(const QString& sql, QVector<QVariant> val);
    QVector<QSqlRecord> query(const QString&);

    static void initDB();



private:
    QString filename;

};

#endif // DBUTIL_H
