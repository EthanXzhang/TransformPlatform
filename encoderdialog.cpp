#include "encoderdialog.h"
#include "ui_encoderdialog.h"

EncoderDialog::EncoderDialog(QWidget *parent,MovieInfo *p) :
    QDialog(parent),
    ui(new Ui::EncoderDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("����������"));
    movieinfo=p;
    videoencoder=ui->videoencoder;
    videosetting=ui->videosetting;
    audioencoder=ui->audioencoder;
    help=ui->help;
    audio=ui->audiocheck;
    initUI();
    initSetting();
    connect(videosetting,SIGNAL(clicked(bool)),this,SLOT(getSetting()));
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(setMovieInfo()));
    connect(ui->audiocheck,SIGNAL(stateChanged(int)),this,SLOT(setAudio(int)));
    connect(help,SIGNAL(clicked(bool)),this,SLOT(getHelpInfo()));
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(acceptSetting()));
}

EncoderDialog::~EncoderDialog()
{
    delete ui;
}
void EncoderDialog::initUI()
{
    videoencoder->insertItem(0,QString::fromLocal8Bit("H.264 MPEG-4-AVC����x264vfw Encoder"));
    videoencoder->insertItem(1,QString::fromLocal8Bit("H.265 MPEG-H����x265vfw Encoder"));
    CreateAudioCompressorFilter();
}
void EncoderDialog::initSetting()
{
    videoencoder->setCurrentIndex((int)movieinfo->encoder);
    if(movieinfo->disableaudio)
    {
        audio->setCheckState(Qt::Checked);
    }else
    {
        audio->setCheckState(Qt::Unchecked);
    }
    for(int i=0;i<audioencoder->count();i++)
    {
        if(movieinfo->audioencoder.compare(audioencoder->itemText(i))==0)
        {
            audioencoder->setCurrentIndex(i);
        }
    }
}
void EncoderDialog::getSetting()
{
    QProcess *po = new QProcess(this);
    po->setWorkingDirectory("D:\\Program Files (x86)\\SIAT\\SIAT3DPlayer\\Plug-in");
    QString program = "C:\\Windows\\SysWOW64\\rundll32.exe";
    QStringList argu;
    switch (videoencoder->currentIndex()) {
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
    movieinfo->encoder=(Encoder)videoencoder->currentIndex();
}
void EncoderDialog::setAudio(int state)
{
    if(state==Qt::Checked)
    {
        movieinfo->disableaudio=true;
    }else if(state==Qt::Unchecked)
    {
        movieinfo->disableaudio=false;
    }
}
void EncoderDialog::getHelpInfo()
{
    QMessageBox message(QMessageBox::NoIcon, "Help",
                        QString::fromLocal8Bit("��������ý���ļ�����Ƶ�����ʽ��ȷ���ԣ����Ĭ��ʹ��Microsoft Audio Decoder���н��룬ȱʡʹ��Microsoft ADPCM������Ƶ���롣���������޷�ʶ����Ƶ�����ʽ��������ʹ��FFdshow Audio Decoder����ѡ��ı������޷�ʶ�������ʽ��ת�����������У�����ԭʼ��Ƶ����ֱ��д���ļ�������£�"),
                        QMessageBox::Ok, NULL);
    message.exec();
}
void EncoderDialog::acceptSetting()
{
    movieinfo->audioencoder=audioencoder->currentText();
    this->accept();
}
void EncoderDialog::CreateAudioCompressorFilter()
{
    ICreateDevEnum *pCreateDevEnum = NULL;
    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
        IID_ICreateDevEnum, (void**)&pCreateDevEnum);

    IEnumMoniker *pEm = NULL;
    hr = pCreateDevEnum->CreateClassEnumerator(CLSID_AudioCompressorCategory, &pEm, 0);
    if (hr != NOERROR)
        return;
    pEm->Reset();
    ULONG cFetched;
    IMoniker *pM = NULL;
    while (hr = pEm->Next(1, &pM, &cFetched), hr == S_OK)
    {
        IPropertyBag *pBag = 0;
        hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
        if (SUCCEEDED(hr))
        {
            VARIANT var;
            var.vt = VT_BSTR;
            hr = pBag->Read(L"FriendlyName", &var, NULL); //������������,��������Ϣ�ȵ�...
            CString str = var.bstrVal;
            std::wstring ss = (LPCTSTR)str;
            QString item=QString::fromStdWString(ss);
            audioencoder->addItem(item);
            pBag->Release();
        }
        pM->Release();
    }
    pEm->Release();
    pCreateDevEnum->Release();
}
