#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_detail.h"
#include "ui_config.h"
#include "dbutil.h"
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <iostream>

#include <QDebug>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->passTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    DButil::initDB();


    this->pfu = new PasswdFileUtil();

    this->pm = new PasswdModel();
    ui->passTable->setModel(this->pm);

    stlabel = new QLabel(this);
    ui->statusBar->addWidget(stlabel);
    stlabel->setText("密钥文件未打开");

    this->config.insert("entype", 1);

}

MainWindow::~MainWindow()
{
    delete this->pfu;
    delete ui;
}

void MainWindow::on_actionNewFile_triggered()
{
    QString filename, filepass;
    filename = QFileDialog::getSaveFileName(this, tr("新建密码文件"), "", tr("数据文件(*.db)"));
    if (filename.isEmpty()) {
        QMessageBox::information(this, tr("错误"), tr("文件未打开"));
        return;
    }
    filepass = QInputDialog::getText(this, tr("输入密钥"), tr("密钥"));

    if (filepass.isEmpty()) {
        QMessageBox::information(this, tr("错误"), tr("密钥为空"));
        return;
    }
    this->pfu->createFile(filename, filepass);

    this->pfu->filename = filename;
    this->pfu->filepass = filepass;
    this->config = this->pfu->readConfig();

    QVector<PassRecord> qrec;
    this->pfu->getContent(qrec);
    this->pm->setReccord(qrec);


    stlabel->setText(tr("密钥文件  ") + filename);
}

void MainWindow::on_actionLoadFile_triggered()
{
    QString filename, filepass;
    filename = QFileDialog::getOpenFileName(this, tr("打开密码文件"), "", tr("数据文件(*.db);;所有文件(*.*)"));
    if (filename.isEmpty()) {
        QMessageBox::information(this, tr("错误"), tr("文件未打开"));
        return;
    }

    filepass = QInputDialog::getText(this, tr("输入密钥"), tr("密钥"), QLineEdit::Password);

    if (filepass.isEmpty()) {
        QMessageBox::information(this, tr("错误"), tr("密钥为空"));
        return;
    }


    if (this->pfu->checkPasswd(filename, filepass) == false) {
        QMessageBox::information(this, tr("错误"), tr("密钥错误"));
        return;

    }

    this->pfu->filename = filename;
    this->pfu->filepass = filepass;
    this->config = this->pfu->readConfig();

    QVector<PassRecord> qrec;
    this->pfu->getContent(qrec);
    this->pm->setReccord(qrec);

    stlabel->setText(tr("密钥文件  ") + filename);
}

void MainWindow::on_btDelete_clicked()
{
    QModelIndex index = ui->passTable->currentIndex();
    if (!index.isValid()) {
        QMessageBox::information(this, tr("错误"), tr("未选中任何一行"));
        return;
    }

    PassRecord p = this->pm->rec.at(index.row());

    if (QMessageBox::question(this, tr("确定"), tr("确定删除记录 ") + p.des + tr("?")) == QMessageBox::Yes) {
        this->pfu->removeRecord(p.id);
        this->pm->rec.removeAt(index.row());
        this->pm->refresh();
    }
}

void MainWindow::on_btAdd_clicked()
{
    if (this->pfu->filename.isEmpty()) {
        QMessageBox::information(this, tr("错误"), tr("未打开文件"));
        return;
    }

    Ui_Detail ud;
    QDialog dlg;
    ud.setupUi(&dlg);
    dlg.setWindowTitle(tr("添加记录"));
    if (dlg.exec() == QDialog::Rejected) {
        return;
    }

    PassRecord p;
    p.account = ud.account->text();
    p.des = ud.des->text();
    p.passwd = ud.passwd->text();
    p.entype = this->config[QString("entype")].toInt();



    if (this->pfu->addRecord(p)) {
        QVector<PassRecord> qrec;
        this->pfu->getContent(qrec);
        this->pm->setReccord(qrec);
    } else {
        QMessageBox::information(this, tr("错误"), tr("记录添加失败"));
    }
}

void MainWindow::on_btView_clicked()
{
    QModelIndex index = ui->passTable->currentIndex();
    if (!index.isValid()) {
        QMessageBox::information(this, tr("错误"), tr("未选中任何一行"));
        return;
    }

    PassRecord p = this->pm->rec.at(index.row());

    Ui_Detail ud;
    QDialog dlg;

    ud.setupUi(&dlg);
    ud.account->setText(p.account);
    ud.des->setText(p.des);
    ud.passwd->setText(p.passwd);
    ud.account->setEnabled(false);
    ud.des->setEnabled(false);
    ud.passwd->setEnabled(false);
    dlg.setWindowTitle(tr("查看记录"));
    dlg.exec();
}

void MainWindow::on_btEdit_clicked()
{
    QModelIndex index = ui->passTable->currentIndex();
    if (!index.isValid()) {
        QMessageBox::information(this, tr("错误"), tr("未选中任何一行"));
        return;
    }

    PassRecord p = this->pm->rec.at(index.row());

    Ui_Detail ud;
    QDialog dlg;

    ud.setupUi(&dlg);
    ud.account->setText(p.account);
    ud.des->setText(p.des);
    ud.passwd->setText(p.passwd);
    dlg.setWindowTitle(tr("修改记录"));
    if (dlg.exec() == QDialog::Rejected) {
        return;
    }

    p.account = ud.account->text();
    p.des = ud.des->text();
    p.passwd = ud.passwd->text();

    if (this->pfu->updateRecord(p)) {
        this->pm->rec.replace(index.row(), p);
        this->pm->refresh();
    } else {
        QMessageBox::information(this, tr("错误"), tr("记录修改失败"));
    }
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("关于"), tr("Version: 1.0\nAuthor: Vimer"));
}

void MainWindow::on_actionConfig_triggered()
{
    if (this->pfu->filename.isEmpty()) {
        QMessageBox::information(this, tr("错误"), tr("未打开文件"));
        return;
    }

    Ui_Config ud;
    QDialog dlg;
    ud.setupUi(&dlg);


    int oldeType = this->config[QString("entype")].toInt();
    QButtonGroup qb;
    qb.addButton(ud.radioe0, 0);
    qb.addButton(ud.radioe1, 1);
    qb.addButton(ud.radioe2, 2);
    qb.addButton(ud.radioe3, 3);
    qb.button(oldeType)->setChecked(true);
    if (dlg.exec() == QDialog::Rejected) {
        return;
    }
    int neweType = qb.checkedId();
    if (neweType != oldeType) {
        this->config[QString("entype")] = neweType;
        this->pfu->updateConfig(this->config);
        foreach (PassRecord p, this->pm->rec) {
            p.entype = neweType;
            this->pfu->updateRecord(p);
        }

    }
}
