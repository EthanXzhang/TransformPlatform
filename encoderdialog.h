#ifndef ENCODERDIALOG_H
#define ENCODERDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QToolButton>
#include <QProcess>
#include <QCheckBox>
#include <QMessageBox>
#include <atlbase.h>
#include <dshow.h>
#include <atlstr.h>
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
    QComboBox *videoencoder,*audioencoder;
    QToolButton *videosetting,*help,*audiosetting;
    QCheckBox *audio;
    void initUI();
    void initSetting();
    void CreateAudioCompressorFilter();
private slots:
    void getSetting();
    void setMovieInfo();
    void setAudio(int state);
    void getHelpInfo();
    void acceptSetting();
};

#endif // ENCODERDIALOG_H
