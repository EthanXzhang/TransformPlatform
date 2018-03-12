#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDir>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QFileIconProvider>
#include <QIcon>
#include <QUrl>
#include <QFileDialog>
#include <QToolButton>
#include <QPushButton>
#include <QProgressBar>
#include <QSlider>
#include <QComboBox>
#include <QTableWidget>
#include <QScrollBar>
#include <QTimer>

#include <atlbase.h>
#include <dshow.h>
#include <vector>
#include <atlstr.h>

#include "movieinfo.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    
    QTableWidget *movietable;
    std::vector<MovieInfo*> movielist;
    MovieInfo *currentmission;
    QComboBox *formatbox,*pathbox;
    QWidget *dshowwidget;
    QTimer *timer;
    QProgressBar *transbar;

    QSqlDatabase database;
    QString pathlist[8];

    HWND g_hWnd;
    IVideoWindow *m_pVW;
    CLSID source,transform,vdec,adec,vrenderer,arenderer,vcodec,muxer,writer;
    IGraphBuilder *pGraph=NULL;
    IMediaControl *pControl=NULL;
    IMediaEvent   *pEvent = NULL;
    IMediaSeeking *pSeeking=NULL;
    ICaptureGraphBuilder2* pGraph2=NULL;

    IFileSourceFilter *pFileSource;
    IFileSinkFilter *pFileWriter;
    IBaseFilter *pSource,*pTransform,*pVDec,*pADec,*pVRenderer,*pARenderer,*pVCod,*pMuxer,*pWriter;
    ISpecifyPropertyPages *pProp;
    LONGLONG duration=NULL;

    bool dshowflag=false,transflag=false;
    HRESULT hr;
private:
    void initUI();
    void initTable();
    void initCombobox();
    void initDatabase();
    void initSys();
    void initSlots();

    int initDirectShow();
    int initPlayerFilter();
    HRESULT initCaptureGraphBuilder(IGraphBuilder **ppGraph, ICaptureGraphBuilder2 **ppBuild);
    HRESULT getUnconnectedPin(IBaseFilter *pFilter,PIN_DIRECTION PinDir,IPin **ppPin);
    HRESULT getCLSID();
    void destroyDShow();
    void destroyTransformFilter();
    void destroyPlayerFilter();
    void connectPlayerFilter();
    void connectTransformFilter();
    void addPlayerFilter();
    void addTransformFilter();
    HRESULT bindwindows();
    void getPages(IBaseFilter *pFilter,ISpecifyPropertyPages *pProp);
    void CreateCompressorFilter(IBaseFilter **pBaseFilter);
    void initTransformFilter();
    void initTransformProgressBar();
    void destoryTransformProgressBar();
    void addMovieVector(QFileInfo fileinfo,int row);
    void removeMovieVector();
    void doTransformMission(MovieInfo *p);

private slots:
    void addMovie();
    void removeMovie();
    void deleteAll();
    void editMovie();
    void editProjection();
    void formatSetting();
    void setPath();
    void openDir();
    void startTransform();
    void playMovie(const QModelIndex & index);
    void updateTransformProgressBar();
};

#endif // MAINWINDOW_H
