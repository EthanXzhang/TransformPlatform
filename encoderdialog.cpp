#include "encoderdialog.h"
#include "ui_encoderdialog.h"

EncoderDialog::EncoderDialog(QWidget *parent,MovieInfo *p) :
    QDialog(parent),
    ui(new Ui::EncoderDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("编码器设置"));
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
    videoencoder->insertItem(0,QString::fromLocal8Bit("H.264 MPEG-4-AVC——x264vfw Encoder"));
    videoencoder->insertItem(1,QString::fromLocal8Bit("H.265 MPEG-H——x265vfw Encoder"));
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
                        QString::fromLocal8Bit("由于输入媒体文件的音频轨道格式不确定性，软件默认使用Microsoft Audio Decoder进行解码，缺省使用Microsoft ADPCM进行音频编码。若解码器无法识别音频轨道格式，将尝试使用FFdshow Audio Decoder。若选择的编码器无法识别解码后格式，转换将正常进行，并将原始音频数据直接写入文件，请见谅！"),
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
            hr = pBag->Read(L"FriendlyName", &var, NULL); //还有其他属性,像描述信息等等...
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
