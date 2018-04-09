#include "playerwindow.h"
#include "ui_playerwindow.h"
#include "mainwindow.h"
PlayerWindow::PlayerWindow(QWidget *parent,QMainWindow *mainwindow) :
    QMainWindow(parent),
    ui(new Ui::PlayerWindow)
{
    ui->setupUi(this);
    main=ui->mainLayout;
    mw=mainwindow;
    this->setWindowTitle("TransformPlatform V1.0");
    this->setWindowIcon(QIcon(":/icon/icon"));
}

PlayerWindow::~PlayerWindow()
{
    ((MainWindow*)mw)->sendoutPlayerWindow();
    delete ui;
}
void PlayerWindow::closeEvent(QCloseEvent *event)
{
    switch( QMessageBox::information(this,QString::fromLocal8Bit("��ʾ"),QString::fromLocal8Bit("ȷ���رղ��Ŵ���?"),QString::fromLocal8Bit("ȷ��"), QString::fromLocal8Bit("ȡ��"),0,1))
    {
    case 0:
        event->accept();
        break;
    case 1:
    default:
        event->ignore();
        break;
    }
}
