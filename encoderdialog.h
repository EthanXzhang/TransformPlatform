#ifndef ENCODERDIALOG_H
#define ENCODERDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QToolButton>
#include <QProcess>
#include "movieinfo.h"
namespace Ui {
class EncoderDialog;
}

class EncoderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EncoderDialog(QWidget *parent = 0,MovieInfo *p=NULL);
    ~EncoderDialog();

private:
    Ui::EncoderDialog *ui;
    MovieInfo *movieinfo;
private:
    QComboBox *encoder;
    QToolButton *setting;
    void initUI();
    void initSetting();
private slots:
    void getSetting();
    void setMovieInfo();
};

#endif // ENCODERDIALOG_H
