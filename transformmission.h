#ifndef TRANSFORMMISSION_H
#define TRANSFORMMISSION_H
#include <atlbase.h>
#include <dshow.h>
#include <atlstr.h>
#include <QWidget>
#include <QLayout>

#include "movieinfo.h"
#include "transforminterface.h"

class TransformMission
{
public:
    TransformMission(MovieInfo *p);
    ~TransformMission();
public:
    IMediaSeeking *pSeeking=NULL;
    IMediaEvent   *pEvent = NULL;
    QLayout *dshowLayout;
private:
    MovieInfo *mi;
    QWidget *dshowwidget;
    HWND g_hWnd;
    IVideoWindow *m_pVW;
    CLSID source,transform,vdec,adec,vrenderer,arenderer,vcodec,avimuxer,mp4muxer,writer;
    IGraphBuilder *pGraph=NULL;
    IMediaControl *pControl=NULL;
    ICaptureGraphBuilder2* pGraph2=NULL;

    IFileSourceFilter *pFileSource;
    IFileSinkFilter *pFileWriter;
    IBaseFilter *pSource,*pTransform,*pVDec,*pADec,*pVRenderer,*pARenderer,*pVCod,*pMuxer,*pWriter;

public:
    void setTransform();
    void runTransform();
private:
    HRESULT initDirectShow();
    HRESULT initCaptureGraphBuilder(IGraphBuilder **ppGraph, ICaptureGraphBuilder2 **ppBuild);
    HRESULT getUnconnectedPin(IBaseFilter *pFilter,PIN_DIRECTION PinDir,IPin **ppPin);
    HRESULT getCLSID();
    HRESULT bindwindows();
    void CreateCompressorFilter(IBaseFilter **pBaseFilter);

    HRESULT initTransformFilter();
    HRESULT connectTransformFilter();
    void addTransformFilter();

    void destroyDShow();
    void destroyTransformFilter();
};

#endif // TRANSFORMMISSION_H
