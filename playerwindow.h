#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

#include <QMainWindow>

namespace Ui {
class PlayerWindow;
}

class PlayerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayerWindow(QWidget *parent = 0,QMainWindow *mainwindow=0);
    ~PlayerWindow();
public:
    QLayout *main=NULL;
    QMainWindow *mw=NULL;
private:
    Ui::PlayerWindow *ui;
private:
    void closeEvent(QCloseEvent *event);
};

#endif // PLAYERWINDOW_H
