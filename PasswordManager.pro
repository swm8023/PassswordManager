#-------------------------------------------------
#
# Project created by QtCreator 2014-10-22T19:32:44
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PasswordManager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    passwdfileutil.cpp \
    dbutil.cpp \
    passwdmodel.cpp

HEADERS  += mainwindow.h \
    passwdfileutil.h \
    dbutil.h \
    passwdmodel.h

FORMS    += mainwindow.ui \
    detail.ui

OTHER_FILES +=

RESOURCES += \
    res.qrc

RC_FILE += res.rc
