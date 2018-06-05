#ifndef PLAYER_H
#define PLAYER_H
#include <atlbase.h>
#include <dshow.h>
#include <atlstr.h>
#include <QWidget>
#include <QLayout>

#include "movieinfo.h"
#include "transforminterface.h"
#include "playerwidget.h"
class Player
{
public:
    Player(MovieInfo *p);
    ~Player();
public:
    IMediaSeeking *pSeeking=NULL;
    IMediaEvent   *pEvent = NULL;
    QLayout *dshowLayout;
private:
    MovieInfo *mi;
    PlayerWidget *dshowwidget;
    HWND g_hWnd;
    IVideoWindow *m_pVW;
    CLSID source,transform,vdec,adec,vrenderer,arenderer,vcodec,avimuxer,mp4muxer,writer;
    IGraphBuilder *pGraph=NULL;
    IMediaControl *pControl=NULL;
    ICaptureGraphBuilder2* pGraph2=NULL;
    DWORD   m_dwGraphRegister;

    IFileSourceFilter *pFileSource;
    IFileSinkFilter *pFileWriter;
    IBaseFilter *pSource,*pTransform,*pVDec,*pADec,*pVRenderer,*pARenderer,*pVCod,*pMuxer,*pWriter;

public:
    void setPlay();
    void runPlay();
    void removeWindow();
    void updateWindow();
private:
    HRESULT initDirectShow();
    HRESULT initCaptureGraphBuilder(IGraphBuilder **ppGraph, ICaptureGraphBuilder2 **ppBuild);
    HRESULT getUnconnectedPin(IBaseFilter *pFilter,PIN_DIRECTION PinDir,IPin **ppPin);
    HRESULT getCLSID();
    HRESULT GetAVDevices(IBaseFilter **pBaseFilter);
    HRESULT bindwindows();

    HRESULT AddToRot(IUnknown *pUnkGraph, DWORD *pdwRegister);
    void RemoveFromRot(DWORD pdwRegister);

    HRESULT initProjectionPlayerFilter();
    HRESULT initNormalPlayerFilter();
    HRESULT connectProjectionPlayerFilter();
    HRESULT connectNormalPlayerFilter();
    void addProjectionPlayerFilter();
    void addNormalPlayerFilter();


    void destroyDShow();
    void destroyProjectionPlayerFilter();
};

#endif // PLAYER_H
