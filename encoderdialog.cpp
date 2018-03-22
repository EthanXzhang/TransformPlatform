#include "encoderdialog.h"
#include "ui_encoderdialog.h"

EncoderDialog::EncoderDialog(QWidget *parent,MovieInfo *p) :
    QDialog(parent),
    ui(new Ui::EncoderDialog)
{
    ui->setupUi(this);
    movieinfo=p;
    encoder=ui->encoder;
    setting=ui->setting;
    initUI();
    initSetting();
    connect(setting,SIGNAL(clicked(bool)),this,SLOT(getSetting()));
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(setMovieInfo()));
}

EncoderDialog::~EncoderDialog()
{
    delete ui;
}
void EncoderDialog::initUI()
{
    encoder->insertItem(0,QString::fromLocal8Bit("H.264 MPEG-4-AVC！！x264vfw Encoder"));
    encoder->insertItem(1,QString::fromLocal8Bit("H.265 MPEG-H！！x265vfw Encoder"));
}
void EncoderDialog::initSetting()
{
    encoder->setCurrentIndex((int)movieinfo->encoder);
}
void EncoderDialog::getSetting()
{
    QProcess *po = new QProcess(this);
    po->setWorkingDirectory("D:\\Program Files (x86)\\SIAT\\SIAT3DPlayer\\Plug-in");
    QString program = "C:\\Windows\\SysWOW64\\rundll32.exe";
    QStringList argu;
    switch (encoder->currentIndex()) {
    case 0:
        argu.append("x264vfw.dll,Configure");
        po->start(program,argu);
        break;
    case 1:
        argu.append("x265vfw.dll,Configure");
        po->start(program,argu);
        break;
    default:
        break;
    }
}
void EncoderDialog::setMovieInfo()
{
    movieinfo->encoder=(Encoder)encoder->currentIndex();
}
