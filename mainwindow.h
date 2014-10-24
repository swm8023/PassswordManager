#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QLabel>
#include <QSqlRelationalTableModel>

#include "passwdfileutil.h"
#include "passwdmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionNewFile_triggered();

    void on_actionLoadFile_triggered();

    void on_btDelete_clicked();

    void on_btAdd_clicked();

    void on_btView_clicked();

    void on_btEdit_clicked();

    void on_action_triggered();

private:
    QLabel *stlabel;
    PasswdFileUtil *pfu;
    PasswdModel *pm;
    Ui::MainWindow *ui;
    QMap<QString, QVariant> config;
};

#endif // MAINWINDOW_H
