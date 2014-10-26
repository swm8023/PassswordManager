#include "passwdfileutil.h"
#include "dbutil.h"

#include <QCryptographicHash>
PasswdFileUtil::PasswdFileUtil()
{
}

bool PasswdFileUtil::getContent(QVector<PassRecord> &qvec)
{
    DButil db(this->filename);
    QVector<QSqlRecord>  rvec = db.query("SELECT id, des, account, passwd, entype FROM passwdt ");

    foreach (QSqlRecord qr, rvec) {
        PassRecord p;
        p.id = qr.value(0).toInt();
        p.des = qr.value(1).toString();
        p.account = qr.value(2).toString();
        p.entype = qr.value(4).toInt();
        QByteArray bar = qr.value(3).toByteArray();
        p.passwd = decodepass(bar, p.entype);
        qvec.append(p);
    }
    return true;
}

bool PasswdFileUtil::removeRecord(int id)
{
    DButil db(this->filename);
    return db.exec(QString("DELETE FROM passwdt WHERE id = %1").arg(id));
}

bool PasswdFileUtil::updateRecord(PassRecord rd)
{
    DButil db(this->filename);
    QVector<QVariant> vec;
    vec.append(encodepass(rd.passwd, rd.entype));
    return db.execp(QString("UPDATE passwdt SET des = '%1', account = '%2', passwd = ?, entype = '%3' WHERE id = %4").arg(rd.des, rd.account).arg(rd.entype).arg(rd.id), vec);

}

bool PasswdFileUtil::addRecord(PassRecord rd)
{
    DButil db(this->filename);
    QVector<QVariant> vec;
    vec.append(encodepass(rd.passwd, rd.entype));
    return db.execp(QString("INSERT INTO passwdt (des, account, passwd, entype) VALUES ('%1', '%2', ?, %3)").arg(rd.des, rd.account).arg(rd.entype), vec);
}

QString PasswdFileUtil::decodepass(QByteArray bar, int entype)
{
    // 不加密
    if (entype == 0) {
        return QString(bar);
    // 异或加密
    } else if (entype == 1) {
        QByteArray fpass = this->filepass.toUtf8();
        int pos = 0;
        for (int i = 0; i < bar.length(); i++) {
            bar[i] = bar[i] ^ fpass[pos];
            if (pos++ == fpass.length()) pos = 0;
        }
        return bar;
    // 公钥加密
    } else if(entype == 2) {
        QByteArray fpass = QString("vimersu").toUtf8();
        int pos = 0;
        for (int i = 0; i < bar.length(); i++) {
            bar[i] = bar[i] ^ fpass[pos];
            if (pos++ == fpass.length()) pos = 0;
        }
        return bar;
    }
    return QString();
}

QByteArray PasswdFileUtil::encodepass(QString str, int entype) {
    // 不加密
    if (entype == 0) {
        return QByteArray(str.toUtf8());
    // 异或加密
    } else if (entype == 1) {
        QByteArray fpass = this->filepass.toUtf8();
        QByteArray enpass = str.toUtf8();
        int pos = 0;
        for (int i = 0; i < enpass.length(); i++) {
            enpass[i] = enpass[i] ^ fpass[pos];
            if (pos++ == fpass.length()) pos = 0;
        }
        return enpass;
    // 公钥加密
    } else if(entype == 2) {
        QByteArray fpass = QString("vimersu").toUtf8();
        QByteArray enpass = str.toUtf8();
        int pos = 0;
        for (int i = 0; i < enpass.length(); i++) {
            enpass[i] = enpass[i] ^ fpass[pos];
            if (pos++ == fpass.length()) pos = 0;
        }
        return enpass;
    }
    return QByteArray();
}

bool PasswdFileUtil::createFile(QString filename, QString filepass)
{
    if (QFile::exists(filename)) {
        if (QFile::remove(filename)) {
            return false;
        }
    }
    DButil db(filename);
    QString creatStr = "CREATE TABLE \"passwdt\" ("
                        "\"id\"  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                        "\"des\"  TEXT, "
                        "\"account\"  TEXT,"
                        "\"passwd\"  TEXT,"
                        "\"entype\"  INTEGER);";

    if (db.exec(creatStr) == false) {
        return false;
    }

    creatStr = "CREATE TABLE \"config\" ("
                "\"pass\"  TEXT,"
                "\"entype\"  INTEGER,"
                "\"serverip\"  TEXT,"
                "\"serverpt\"  INTEGER);";


    if (db.exec(creatStr) == false) {
        return false;
    }

    QByteArray md5fname;
    md5fname = QCryptographicHash::hash(filepass.toLocal8Bit(), QCryptographicHash::Md5);
    md5fname = QCryptographicHash::hash(md5fname, QCryptographicHash::Md5);

    return db.exec(QString("INSERT INTO config (pass, entype) VALUES ('" +md5fname.toHex()+ "', 1)"));
}

bool PasswdFileUtil::checkPasswd(QString filename, QString filepass) {
    QByteArray md5fname;
    md5fname = QCryptographicHash::hash(filepass.toLocal8Bit(), QCryptographicHash::Md5);
    md5fname = QCryptographicHash::hash(md5fname, QCryptographicHash::Md5);

    DButil db(filename);
    QVector<QSqlRecord>  rvec = db.query("SELECT pass FROM config");
    if (rvec.count() == 0) {
        return false;
    }
    return rvec.at(0).value(0).toString() == md5fname.toHex();
}

QMap<QString, QVariant> PasswdFileUtil::readConfig() {
    DButil db(this->filename);
    QVector<QSqlRecord>  rvec = db.query("SELECT entype, serverip, serverpt FROM config ");

    QMap<QString, QVariant> qmap;
    if (rvec.size() != 1) {
        return qmap;
    }
    foreach (QSqlRecord qr, rvec) {
        qmap["entype"] = qr.value(0);
        qmap["serverip"] = qr.value(1);
        qmap["serverpt"] = qr.value(2);
    }
    return qmap;
}

bool PasswdFileUtil::updateConfig(QMap<QString, QVariant> config) {
    DButil db(this->filename);
    return  db.exec(QString("UPDATE config set entype = %1, serverip = '%2', serverpt = %3").
                    arg(config["entype"].toInt()).arg(config["serverip"].toString()).arg(config["serverpt"].toInt()));

}
