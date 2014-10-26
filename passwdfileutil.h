#ifndef PASSWDFILEUTIL_H
#define PASSWDFILEUTIL_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QVariant>

struct PassRecord{
    int id;
    int entype;
    QString des;
    QString account;
    QString passwd;
};

class PasswdFileUtil
{
public:
    QString filename;
    QString filepass;

    PasswdFileUtil();
    bool getContent(QVector<PassRecord> &qvec);
    bool removeRecord(int id);
    bool updateRecord(PassRecord record);
    bool addRecord(PassRecord record);

    bool createFile(QString filename, QString filepass);
    bool checkPasswd(QString filename, QString filepass);
    QString decodepass(QByteArray bar, int entype);
    QByteArray encodepass(QString str, int entype);

    QMap<QString, QVariant> readConfig();
    bool updateConfig(QMap<QString, QVariant> config);

};



#endif // PASSWDFILEUTIL_H
