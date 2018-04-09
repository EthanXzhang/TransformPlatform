#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("TransformPlatform V1.0");
    this->setWindowIcon(QIcon(":/icon/icon"));
    initSys();
    initUI();
    initSlots();
    hr=getCLSID();
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::initSys()
{
    pathlist[0]=QDir::currentPath();
    pathlist[1]=QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    pathlist[2]=QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    pathlist[3]=QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    transformtimer=new QTimer(this);
}
void MainWindow::initUI()
{
    movietable=ui->tableWidget;
    formatbox=ui->formatBox;
    pathbox=ui->pathBox;
    transbar=ui->progressBar;

    initTable();
    initCombobox();
}
void MainWindow::initSlots()
{
    connect(ui->openpathButton,SIGNAL(clicked(bool)),this,SLOT(openDir()));
    connect(ui->selectpathButton,SIGNAL(clicked(bool)),this,SLOT(setPath()));
    connect(ui->addMovieButton,SIGNAL(clicked(bool)),this,SLOT(addMovie()));
    connect(ui->removeMovieButton,SIGNAL(clicked(bool)),this,SLOT(removeMovie()));
    connect(ui->deleteAllButton,SIGNAL(clicked(bool)),this,SLOT(deleteAll()));
    connect(movietable,SIGNAL(clicked(QModelIndex)),this,SLOT(getCurrentItemSet(QModelIndex)));
    connect(movietable,SIGNAL(cellClicked(int,int)),this,SLOT(settingTableChanged(int, int)));
    connect(pathbox,SIGNAL(activated(int)),this,SLOT(setMoviePath(int)));
    connect(formatbox,SIGNAL(activated(int)),this,SLOT(setMux()));
    connect(ui->stratTransformButton,SIGNAL(clicked(bool)),this,SLOT(startTransform()));
    connect(transformtimer,SIGNAL(timeout()),this,SLOT(updateTransformProgressBar()));
    connect(ui->editProjectionButton,SIGNAL(clicked(bool)),this,SLOT(editProjection()));
    connect(ui->settingButton,SIGNAL(clicked(bool)),this,SLOT(formatSetting()));
    connect(ui->playerButton,SIGNAL(clicked(bool)),this,SLOT(sendoutPlayerWindow()));
}
void MainWindow::initTable()
{
    movietable->setColumnCount(6);
    movietable->horizontalHeader()->setDefaultSectionSize(150);
    QStringList header;
    QString name=QString::fromLocal8Bit("文件名");
    QString time=QString::fromLocal8Bit("点击预览");
    QString format=QString::fromLocal8Bit("格式");
    QString edit=QString::fromLocal8Bit("预览转换");
    QString projection=QString::fromLocal8Bit("格式");
    QString transform=QString::fromLocal8Bit("转换进度");
    header<<name<<time<<format<<edit<<projection<<transform;
    movietable->setHorizontalHeaderLabels(header);
    QFont font=movietable->horizontalHeader()->font();
    font.setBold(true);
    movietable->horizontalHeader()->setFont(font);
    movietable->horizontalHeader()->setStretchLastSection(true);
    movietable->verticalHeader()->setVisible(false);
    movietable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    movietable->setSelectionBehavior(QAbstractItemView::SelectRows);
    movietable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    movietable->horizontalHeader()->resizeSection(0,150);
    movietable->horizontalHeader()->resizeSection(1,60);
    movietable->horizontalHeader()->resizeSection(2,60);
    movietable->horizontalHeader()->resizeSection(3,60);
    movietable->horizontalHeader()->resizeSection(4,40);
    movietable->horizontalHeader()->resizeSection(5,60);
    movietable->horizontalHeader()->setFixedHeight(25);
    movietable->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
    movietable->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
    movietable->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
    "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
    "QScrollBar::handle:hover{background:gray;}"
    "QScrollBar::sub-line{background:transparent;}"
    "QScrollBar::add-line{background:transparent;}");
    movietable->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
    "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
    "QScrollBar::handle:hover{background:gray;}"
    "QScrollBar::sub-line{background:transparent;}"
    "QScrollBar::add-line{background:transparent;}");
}
void MainWindow::initCombobox()
{
    QString pavi=QString::fromLocal8Bit("AVI 音视频交错格式(*.avi)");
    QString pmp4=QString::fromLocal8Bit("MEPG-4 视频格式(*.mp4)");
    QString yuv=QString::fromLocal8Bit("YUV 原始帧输出（*.yuv）");
    formatbox->insertItem(0,pavi);
    formatbox->insertItem(1,pmp4);
    formatbox->insertItem(2,yuv);
    pathbox->setInsertPolicy(QComboBox::InsertBeforeCurrent);
    for(int i=0;i<8&&pathlist[i]!=NULL;i++)
    {
        pathbox->addItem(pathlist[i]);
    }
}
void MainWindow::addMovie()
{
    QStringList filelist=QFileDialog::getOpenFileNames(this,tr("Open File"),"/",tr("Media(*.*)"));
    for(int i=0;i<filelist.count();i++)
    {
        int row=movietable->rowCount();
        movietable->insertRow(row);
        QTableWidgetItem *item=new QTableWidgetItem();
        QFileInfo file_info(filelist[i]);
        QString name=file_info.fileName();
        QFileIconProvider icon_provider;
        QIcon icon = icon_provider.icon(file_info);
        item->setIcon(icon);
        item->setText(name);
        movietable->setItem(row,0,item);
        //插入AVI格式图标
        QIcon icon_preview;
        icon_preview.addFile(":/icon/preview");
        item=new QTableWidgetItem();
        item->setIcon(icon_preview);
        item->setText(QString::fromLocal8Bit("播放"));
        movietable->setItem(row,1,item);
        //插入AVI格式图标
        QIcon icon_avi;
        icon_avi.addFile(":/icon/itemavi");
        item=new QTableWidgetItem();
        item->setIcon(icon_avi);
        item->setText("AVI");
        movietable->setItem(row,2,item);
        item=new QTableWidgetItem();
        item->setText(QString::fromLocal8Bit("等待开始"));
        item->setTextAlignment(Qt::AlignCenter);
        movietable->setItem(row,5,item);
        //插入Eidt图标
        QTableWidgetItem *check=new QTableWidgetItem;
        check->setCheckState (Qt::Unchecked);
        check->setText(QString::fromLocal8Bit("投影"));
        movietable->setItem(row,3,check);
        //插入Project图标
        QPixmap *pixmap= new QPixmap();
        QPixmap scaledPixmap;
        QLabel *label=new QLabel();
        QSize picSize(25,25);
        label=new QLabel();
        pixmap->load(":/icon/itemproject");
        scaledPixmap=pixmap->scaled(picSize,Qt::KeepAspectRatio);
        label->setPixmap(scaledPixmap);
        label->setAlignment(Qt::AlignHCenter);
        movietable->setCellWidget(row,4,label);
        //加入容器
        addMovieVector(file_info,row);
    }

}
void MainWindow::removeMovie()
{
    removeMovieVector();
    movietable->removeRow(movietable->currentRow());
}
void MainWindow::deleteAll()
{
    movietable->setRowCount(0);
    movietable->clearContents();
}
void MainWindow::editMovie()
{

}
void MainWindow::editProjection(int row,int col)
{
    if(col!=4)
    {
        row=movietable->currentRow();
//        if(movietable->is(movietable->currentItem())==false)
//        {

//        }
        if(row<0)
        {
            QMessageBox::about(NULL, QString::fromLocal8Bit("空选"),QString::fromLocal8Bit("请先选择列表中一项"));
            return ;
        }
    }
    ProjectionDialog *pd=new ProjectionDialog(this,movielist.at(row));
    int res=pd->exec();
    if(res==QDialog::Accepted)
    {
        movielist.at(row)->projectflag=true;
        //插入图标
        QPixmap *pixmap= new QPixmap();
        QPixmap scaledPixmap;
        QLabel *label=new QLabel();
        QSize picSize(25,25);
        pixmap->load(":/icon/itemcorrect");
        scaledPixmap=pixmap->scaled(picSize,Qt::KeepAspectRatio);
        label->setPixmap(scaledPixmap);
        label->setAlignment(Qt::AlignCenter);
        movietable->setCellWidget(row,4,label);
    }
}
void MainWindow::formatSetting()
{
    int row=movietable->currentRow();
    if(movietable->isItemSelected(movietable->currentItem())==false)
    {
        QMessageBox::about(NULL, QString::fromLocal8Bit("空选"),QString::fromLocal8Bit("请先选择列表中一项"));
        return ;
    }
    EncoderDialog *pe=new EncoderDialog(this,movielist.at(row));
    int res=pe->exec();
}
void MainWindow::setPath()
{
    QString path=QFileDialog::getExistingDirectory(NULL,"caption",".");
    if(path.isEmpty())
    {
        return ;
    }
    else
    {
        pathbox->insertItem(0,path);
        pathbox->setCurrentIndex(0);
        for(int i=0;i<movielist.size();i++)
        {
            movielist.at(i)->path_index++;
        }
    }
}
void MainWindow::openDir()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(pathbox->currentText()));
}
void MainWindow::updateTransformProgressBar()
{
    LONGLONG postion;
    if(!duration)
    {
        pSeeking->GetDuration(&duration);
    }
    pSeeking->GetCurrentPosition(&postion);
    transbar->setValue((int)(postion/10000000));
    OAFilterState pfs;
    pControl->GetState(0,&pfs);
    //获得事件通知
    long evCode,param1,param2;
    pEvent->GetEvent(&evCode,&param1,&param2,0);
    pEvent->FreeEventParams(evCode,param1,param2);
    //当通知播放完成 或 Graph停止的时候
    if(evCode==EC_COMPLETE||pfs==State_Stopped)
    {
        pControl->Stop();
        destroyDShow();
        destroyTransformFilter();
        currentmission->finishflag=true;
        movietable->item(currentmission->row_num,5)->setText(QString::fromLocal8Bit("已完成"));
        transformtimer->stop();
        transflag=false;
        startTransform();

    }
}
void MainWindow::startTransform()
{
    int i=0;
    while(i<movielist.size() && movielist.at(i)->finishflag)
    {
        i++;
    }
    if(i<movielist.size())
    {
        doTransformMission(movielist.at(i));
    }
    else
    {
        transbar->setFormat(QString::fromLocal8Bit("已完成所有任务"));
    }
}
void MainWindow::playMovie(int row)
{
//    if(item->column()!=1)
//    {
//        return ;
//    }
//    if(!dshowflag)
//    {
//        initDirectShow();
//        initPlayerFilter();
//    }
//    else
//    {
//        destroyDShow();
//        destroyPlayerFilter();
//        initDirectShow();
//        initPlayerFilter();
//    }
//    dshowflag=true;
//    int row=item->row();
//    MovieInfo *p=movielist.at(row);
//    LPCOLESTR str=reinterpret_cast<const wchar_t *>(p->path.utf16());
//    hr=pSource->QueryInterface(IID_IFileSourceFilter,(void **)&pFileSource);
//    pFileSource->Load(str,NULL);
//        TransformFilterInterface *pTransformInterface;
//        hr=pTransform->QueryInterface(IID_TransformFilterInterface,(void **)&pTransformInterface);
//        if(p->projectflag)
//        {
//            pTransformInterface->DoSetting(p->setting.w,p->setting.h,(int)p->setting.input_layout,(int)p->setting.output_layout,
//                                           0,p->setting.cube_edge_length,p->setting.max_cube_edge_length,p->setting.interpolation_alg,
//                                           p->setting.enable_low_pass_filter,p->setting.enable_multi_threading,p->setting.num_vertical_segments,
//                                           p->setting.num_horizontal_segments);
//        }
//        pTransformInterface->Release();
//    if(FAILED(hr))
//    {
//        return ;
//    }
//    addPlayerFilter();
//    //getPages(pTransform,pProp);
//    connectPlayerFilter();
//    bindwindows();
//    hr = pControl->Run();
//    if(FAILED(hr))
//    {
//        return ;
//    }
    MovieInfo *p=movielist.at(row);
    if(player!=NULL)
    {
        delete(player);
    }
    player=new Player(p);
    player->dshowLayout=ui->dshowLayout;
    player->setPlay();
    player->runPlay();
}
int MainWindow::initDirectShow()
{
    hr = CoInitialize(NULL);
    hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&pGraph);
    if (FAILED(hr))
    {
        return -1;
    }
    hr=initCaptureGraphBuilder(&pGraph, &pGraph2);
    hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl);
    hr = pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);
    hr = pGraph->QueryInterface(IID_IVideoWindow, (void**)&m_pVW);
    long evCode = 0;
    pEvent->WaitForCompletion(INFINITE, &evCode);
    return hr;
}
HRESULT MainWindow::initCaptureGraphBuilder(IGraphBuilder **ppGraph, ICaptureGraphBuilder2 **ppBuild)
{
    if (!ppGraph || !ppBuild)
    {
        return NULL;
    }
    IGraphBuilder *pGraph = NULL;
    ICaptureGraphBuilder2 *pBuild = NULL;
    hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void**)&pBuild);
    if (SUCCEEDED(hr))
    {
        hr = CoCreateInstance(CLSID_FilterGraph, 0, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&pGraph);
        if (SUCCEEDED(hr))
        {
            pBuild->SetFiltergraph(pGraph);
            *ppBuild = pBuild;
            *ppGraph = pGraph;
            return S_OK;
        }
        else
        {
            pBuild->Release();
        }
    }
    return hr;
}
HRESULT MainWindow::getUnconnectedPin(IBaseFilter *pFilter,PIN_DIRECTION PinDir,IPin **ppPin)//, PIN_INFO *pinInfo
{
    *ppPin = 0;
    IEnumPins *pEnum = 0;
    IPin *pPin = 0;
    hr = pFilter->EnumPins(&pEnum);
    if (FAILED(hr))
    {
        return hr;
    }
    while (pEnum->Next(1, &pPin, NULL) == S_OK)
    {
        PIN_DIRECTION ThisPinDir;
        pPin->QueryDirection(&ThisPinDir);
        if (ThisPinDir == PinDir)
        {
            IPin *pTmp = 0;
            hr = pPin->ConnectedTo(&pTmp);
            if (SUCCEEDED(hr))
            {
                pTmp->Release();
            }
            else
            {
                pEnum->Release();
                //pPin->QueryPinInfo(pinInfo);
                *ppPin = pPin;
                return S_OK;
            }
        }
        pPin->Release();
    }
    pEnum->Release();
    return E_FAIL;
}
HRESULT MainWindow::getCLSID()
{
    hr = CLSIDFromString(OLESTR("{8F43B7D9-9D6B-4F48-BE18-4D787C795EEA}"), &source);
    hr = CLSIDFromString(OLESTR("{04FE9017-F873-410E-871E-AB91661A4EF7}"), &vdec);
    hr = CLSIDFromString(OLESTR("{0F40E1E5-4F79-4988-B1A9-CC98794E6B55}"), &adec);
    hr = CLSIDFromString(OLESTR("{9E5A9E31-1C34-4873-863D-D5441C645398}"), &transform);
    hr = CLSIDFromString(OLESTR("{152F4328-67D0-4B28-A98B-DEEE7D27B63E}"), &vrenderer);
    hr = CLSIDFromString(OLESTR("{79376820-07D0-11CF-A24D-0020AFD79767}"), &arenderer);
    hr = CLSIDFromString(OLESTR("{E2510970-F137-11CE-8B67-00AA00A3F1A6}"), &avimuxer);
    hr = CLSIDFromString(OLESTR("{5FD85181-E542-4E52-8D9D-5D613C30131B}"), &mp4muxer);
    hr = CLSIDFromString(OLESTR("{8596E5F0-0DA5-11D0-BD21-00A0C911CE86}"), &writer);
    return hr;
}
void MainWindow::destroyDShow()
{
    pGraph->Release();
    pControl->Release();
    pEvent->Release();
}
void MainWindow::destroyTransformFilter()
{
    pSource->Release();
    pVDec->Release();
    pADec->Release();
    pTransform->Release();
    pMuxer->Release();
    pWriter->Release();
}
void MainWindow::addTransformFilter()
{
    pGraph->AddFilter(pSource,L"Source");
    pGraph->AddFilter(pVDec,L"Video Decodec");
    pGraph->AddFilter(pADec,L"Audio Decodec");
    pGraph->AddFilter(pTransform,L"Transform");
    pGraph->AddFilter(pMuxer,L"AVI Muxer");
    CreateCompressorFilter(&pVCod);
    pGraph->AddFilter(pVCod,L"x264 encoder");
    pGraph->AddFilter(pWriter,L"File Writer");
}
void MainWindow::CreateCompressorFilter(IBaseFilter **pBaseFilter)
{
    ICreateDevEnum *pCreateDevEnum = NULL;
    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
        IID_ICreateDevEnum, (void**)&pCreateDevEnum);

    IEnumMoniker *pEm = NULL;
    hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoCompressorCategory, &pEm, 0);
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
            switch (currentmission->encoder) {
            case x264:
                if (str.Find(L"x264vfw - H.264/MPEG-4 AVC codec",0)!=-1)//"x264vfw - H.264/MPEG-4 AVC codec"
                {
                    //获取设备名称
                    char camera_name[1024];
                    WideCharToMultiByte(CP_ACP, 0, var.bstrVal, -1, camera_name, sizeof(camera_name), "", NULL);
                    SysFreeString(var.bstrVal);
                    hr = pM->BindToObject(0, 0, IID_IBaseFilter, (void**)pBaseFilter);//就是这句获得Filter
                    if (FAILED(hr))
                    {
                        printf("error");
                    }
                }
                break;
            case x265:
                if (str.Find(L"x265vfw - H.265/MPEG-H codec",0)!=-1)//"x265vfw - H.265/MPEG-H codec"
                {
                    //获取设备名称
                    char camera_name[1024];
                    WideCharToMultiByte(CP_ACP, 0, var.bstrVal, -1, camera_name, sizeof(camera_name), "", NULL);
                    SysFreeString(var.bstrVal);
                    hr = pM->BindToObject(0, 0, IID_IBaseFilter, (void**)pBaseFilter);//就是这句获得Filter
                    if (FAILED(hr))
                    {
                        printf("error");
                    }
                }
                break;
            default:
                break;
            }

            pBag->Release();
        }
        pM->Release();
    }
    pEm->Release();
    pCreateDevEnum->Release();
}
void MainWindow::initTransformFilter()
{
    hr=CoCreateInstance(source, NULL, CLSCTX_ALL, IID_IBaseFilter, (void**)&pSource);
    hr=CoCreateInstance(vdec, NULL, CLSCTX_ALL, IID_IBaseFilter, (void**)&pVDec);
    hr=CoCreateInstance(adec, NULL, CLSCTX_ALL, IID_IBaseFilter, (void**)&pADec);
    hr=CoCreateInstance(transform, NULL, CLSCTX_ALL, IID_IBaseFilter, (void**)&pTransform);
    switch (currentmission->muxer)
    {
    case avi:
        hr=CoCreateInstance(avimuxer, NULL, CLSCTX_ALL, IID_IBaseFilter, (void**)&pMuxer);
        break;
    case mp4:
        hr=CoCreateInstance(mp4muxer, NULL, CLSCTX_ALL, IID_IBaseFilter, (void**)&pMuxer);
        break;
    default:
        break;
    }
    hr=CoCreateInstance(writer, NULL, CLSCTX_ALL, IID_IBaseFilter, (void**)&pWriter);
    hr = pMuxer->QueryInterface(IID_IMediaSeeking, (void**)&pSeeking);
    if(pSeeking!=NULL)
    {
        pSeeking->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);//将时间设置为以100ns为单位的格式
    }
}
void MainWindow::connectTransformFilter()
{
    IPin *pOut,*pIn;
    getUnconnectedPin(pSource,PINDIR_OUTPUT,&pOut);
    getUnconnectedPin(pVDec,PINDIR_INPUT,&pIn);
    hr=pGraph->Connect(pOut,pIn);
    getUnconnectedPin(pSource,PINDIR_OUTPUT,&pOut);
    getUnconnectedPin(pADec,PINDIR_INPUT,&pIn);
    hr=pGraph->Connect(pOut,pIn);
    getUnconnectedPin(pVDec,PINDIR_OUTPUT,&pOut);
    getUnconnectedPin(pTransform,PINDIR_INPUT,&pIn);
    hr=pGraph->Connect(pOut,pIn);
    getUnconnectedPin(pTransform,PINDIR_OUTPUT,&pOut);
    getUnconnectedPin(pVCod,PINDIR_INPUT,&pIn);
    hr=pGraph->Connect(pOut,pIn);
    getUnconnectedPin(pVCod,PINDIR_OUTPUT,&pOut);
    getUnconnectedPin(pMuxer,PINDIR_INPUT,&pIn);
    hr=pGraph->Connect(pOut,pIn);
    //hr=pGraph2->RenderStream(NULL,&MEDIATYPE_Video,pVDec,pTransform,pMuxer);
//    getUnconnectedPin(pADec,PINDIR_OUTPUT,&pOut);
//    getUnconnectedPin(pMuxer,PINDIR_INPUT,&pIn);
//    hr=pGraph->Connect(pOut,pIn);
    hr=pGraph2->RenderStream(NULL,&MEDIATYPE_Video,pADec,NULL,pMuxer);
    getUnconnectedPin(pMuxer,PINDIR_OUTPUT,&pOut);
    getUnconnectedPin(pWriter,PINDIR_INPUT,&pIn);
    hr=pGraph->Connect(pOut,pIn);
}
void MainWindow::initTransformProgressBar()
{
    if(pSeeking==NULL)
    {
        transbar->setMinimum(0);
        transbar->setMaximum(0);
    }
    else
    {
        pSeeking->GetDuration(&duration);
        transbar->setMaximum((int)(duration/10000000));
        transbar->setMinimum(0);
        transbar->setFormat(QString::fromLocal8Bit("当前进度为：%p%"));
    }
}
void MainWindow::destoryTransformProgressBar()
{
    transbar->setMinimum(0);
    transbar->setMaximum(100);
    transbar->setValue(0);
    transbar->setFormat(QString::fromLocal8Bit("等待开始"));
}
void MainWindow::addMovieVector(QFileInfo fileinfo,int row)
{
    MovieInfo *pmi=new MovieInfo();
    pmi->path=fileinfo.absoluteFilePath();
    pmi->name=fileinfo.baseName();
    pmi->output=fileinfo.baseName();
    pmi->outputpath=pathbox->currentText()+"\\";
    pmi->row_num=row;
    pmi->path_index=pathbox->currentIndex();
    pmi->muxer=(Muxer)formatbox->currentIndex();
    pmi->projectionpreview=false;
    movielist.insert(movielist.end(),pmi);
}
void MainWindow::removeMovieVector()
{
    MovieInfo *p=movielist[movietable->currentRow()];
    movielist.erase(movielist.begin()+movietable->currentRow());
    delete(p);
    for(int i=movietable->currentRow();i<movielist.size();i++)
    {
        movielist.at(i)->row_num--;
    }

}
void MainWindow::doTransformMission(MovieInfo *p)
{
    if(p->finishflag)
    {
        return ;
    }
    currentmission=p;
    if(!transflag)
    {
        initDirectShow();
        initTransformFilter();
    }
    else
    {
        hr = pControl->Stop();
        destroyDShow();
        destroyTransformFilter();
        initDirectShow();
        initTransformFilter();
    }
    transflag=true;
    LPCOLESTR input=reinterpret_cast<const wchar_t *>(p->path.utf16());
    hr=pSource->QueryInterface(IID_IFileSourceFilter,(void **)&pFileSource);
    pFileSource->Load(input,NULL);
    hr=pWriter->QueryInterface(IID_IFileSinkFilter,(void **)&pFileWriter);
    TransformFilterInterface *pTransformInterface;
    hr=pTransform->QueryInterface(IID_TransformFilterInterface,(void **)&pTransformInterface);
    if(p->projectflag)
    {
        pTransformInterface->DoSetting(p->setting.w,p->setting.h,(int)p->setting.input_layout,(int)p->setting.output_layout,
                                       0,p->setting.cube_edge_length,p->setting.max_cube_edge_length,p->setting.interpolation_alg,
                                       p->setting.enable_low_pass_filter,p->setting.enable_multi_threading,p->setting.num_vertical_segments,
                                       p->setting.num_horizontal_segments);
    }
    switch (p->muxer)
    {
    case avi:
        p->outputfile=p->outputpath+p->output+'.'+"avi";
        break;
    case mp4:
        p->outputfile=p->outputpath+p->output+'.'+"mp4";
        break;
    default:
        break;
    }

    LPCOLESTR output=reinterpret_cast<const wchar_t *>(p->outputfile.utf16());
    pFileWriter->SetFileName(output,NULL);
    if(FAILED(hr))
    {
        return ;
    }
    addTransformFilter();
    connectTransformFilter();
    initTransformProgressBar();
    hr = pControl->Run();
    transformtimer->start(300);
    if(FAILED(hr))
    {
        transformtimer->stop();
        return ;
    }
}
void MainWindow::getCurrentItemSet(const QModelIndex & index)
{
    pathbox->setCurrentIndex(movielist.at(index.row())->path_index);
    formatbox->setCurrentIndex((int)movielist.at(index.row())->muxer);
}
void MainWindow::setMoviePath(int index)
{
    if(movietable->isItemSelected(movietable->currentItem())==false)
    {
        return ;
    }
    movielist.at(movietable->currentRow())->path_index=index;
    movielist.at(movietable->currentRow())->outputpath=pathbox->currentText()+'\\';
}
void MainWindow::setMux()
{
    if(movietable->isItemSelected(movietable->currentItem())==false)
    {
        return ;
    }
    movielist.at(movietable->currentRow())->muxer=(Muxer)formatbox->currentIndex();
}
void MainWindow::settingTableChanged(int row, int col)
{
    switch(col)
    {
    case 1:
        playMovie(row);
        break;
    case 3:
        if(movietable->item(row,3)->checkState()==Qt::Checked)
        {
            movielist.at(row)->projectionpreview=true;
        }else if(movietable->item(row,3)->checkState()==Qt::Unchecked)
        {
            movielist.at(row)->projectionpreview=false;
        }
        break;
    case 4:
        editProjection(row,col);
        break;
    default:
        break;
    }

}
void MainWindow::sendoutPlayerWindow()
{
    if(player==NULL)
    {
        return ;
    }

    if(playerwindow==NULL)
    {
        playerwindow=new PlayerWindow(NULL,this);
        playerwindow->setAttribute(Qt::WA_DeleteOnClose);
        player->removeWindow();
        player->dshowLayout=playerwindow->main;
        player->updateWindow();
        playerwindow->show();
    }else
    {
        if(playerwindow->isVisible())
        {
            playerwindow->close();
            return ;
        }
        player->removeWindow();
        player->dshowLayout=ui->dshowLayout;
        player->updateWindow();
        playerwindow=NULL;
    }

}
