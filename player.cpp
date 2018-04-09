#include "player.h"

Player::Player(MovieInfo *p)
{
    this->mi=p;
    int hr=getCLSID();
}
Player::~Player()
{
    destroyDShow();
    destroyProjectionPlayerFilter();
}
void Player::setPlay()
{
    int hr=initDirectShow();
    if(mi->projectionpreview)
    {
        hr=initProjectionPlayerFilter();
    }else
    {
        hr=initNormalPlayerFilter();
    }

}
void Player::runPlay()
{
    LPCOLESTR str=reinterpret_cast<const wchar_t *>(mi->path.utf16());
    int hr=pSource->QueryInterface(IID_IFileSourceFilter,(void **)&pFileSource);
    pFileSource->Load(str,NULL);
    if(mi->projectionpreview)
    {
        TransformFilterInterface *pTransformInterface;
        hr=pTransform->QueryInterface(IID_TransformFilterInterface,(void **)&pTransformInterface);
        if(mi->projectflag)
        {
            pTransformInterface->DoSetting(mi->setting.w,mi->setting.h,(int)mi->setting.input_layout,(int)mi->setting.output_layout,
                                           0,mi->setting.cube_edge_length,mi->setting.max_cube_edge_length,mi->setting.interpolation_alg,
                                           mi->setting.enable_low_pass_filter,mi->setting.enable_multi_threading,mi->setting.num_vertical_segments,
                                           mi->setting.num_horizontal_segments);
        }
        pTransformInterface->Release();
        if(FAILED(hr))
        {
            return ;
        }
        addProjectionPlayerFilter();
        //getPages(pTransform,pProp);
        connectProjectionPlayerFilter();
    }else
    {
        addNormalPlayerFilter();
        connectNormalPlayerFilter();
    }
    bindwindows();
    hr = pControl->Run();
    if(FAILED(hr))
    {
        return ;
    }
}
HRESULT Player::initDirectShow()
{
    int hr = CoInitialize(NULL);
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
HRESULT Player::initCaptureGraphBuilder(IGraphBuilder **ppGraph, ICaptureGraphBuilder2 **ppBuild)
{
    if (!ppGraph || !ppBuild)
    {
        return NULL;
    }
    IGraphBuilder *pGraph = NULL;
    ICaptureGraphBuilder2 *pBuild = NULL;
    int hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void**)&pBuild);
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
HRESULT Player::getUnconnectedPin(IBaseFilter *pFilter,PIN_DIRECTION PinDir,IPin **ppPin)//, PIN_INFO *pinInfo
{
    *ppPin = 0;
    IEnumPins *pEnum = 0;
    IPin *pPin = 0;
    int hr = pFilter->EnumPins(&pEnum);
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
HRESULT Player::getCLSID()
{
    int hr = CLSIDFromString(OLESTR("{8F43B7D9-9D6B-4F48-BE18-4D787C795EEA}"), &source);
    hr = CLSIDFromString(OLESTR("{04FE9017-F873-410E-871E-AB91661A4EF7}"), &vdec);
    hr = CLSIDFromString(OLESTR("{0F40E1E5-4F79-4988-B1A9-CC98794E6B55}"), &adec);
    hr = CLSIDFromString(OLESTR("{9E5A9E31-1C34-4873-863D-D5441C645398}"), &transform);
    hr = CLSIDFromString(OLESTR("{152F4328-67D0-4B28-A98B-DEEE7D27B63E}"), &vrenderer);
    hr = CLSIDFromString(OLESTR("{79376820-07D0-11CF-A24D-0020AFD79767}"), &arenderer);
    return hr;
}
HRESULT Player::bindwindows()
{
    dshowwidget=new PlayerWidget(NULL,m_pVW);
    dshowwidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    int hr = m_pVW->put_WindowStyle(WS_CHILD);
    hr = m_pVW->put_Visible(OATRUE);
    hr = m_pVW->put_Owner((OAHWND)dshowwidget->winId());
    RECT grc;
    GetClientRect((HWND)dshowwidget->winId(),&grc);
    m_pVW->SetWindowPosition(0,0,420,300);
    dshowLayout->addWidget(dshowwidget);
    return hr;
}
HRESULT Player::initProjectionPlayerFilter()
{
    int hr=CoCreateInstance(source, NULL, CLSCTX_ALL, IID_IBaseFilter, (void**)&pSource);
    hr=CoCreateInstance(vdec, NULL, CLSCTX_ALL, IID_IBaseFilter, (void**)&pVDec);
    hr=CoCreateInstance(adec, NULL, CLSCTX_ALL, IID_IBaseFilter, (void**)&pADec);
    hr=CoCreateInstance(transform, NULL, CLSCTX_ALL, IID_IBaseFilter, (void**)&pTransform);
    hr=CoCreateInstance(vrenderer, NULL, CLSCTX_ALL, IID_IBaseFilter, (void**)&pVRenderer);
    hr=CoCreateInstance(arenderer, NULL, CLSCTX_ALL, IID_IBaseFilter, (void**)&pARenderer);
    return hr;
}
HRESULT Player::initNormalPlayerFilter()
{
    int hr=CoCreateInstance(source, NULL, CLSCTX_ALL, IID_IBaseFilter, (void**)&pSource);
    hr=CoCreateInstance(vdec, NULL, CLSCTX_ALL, IID_IBaseFilter, (void**)&pVDec);
    hr=CoCreateInstance(adec, NULL, CLSCTX_ALL, IID_IBaseFilter, (void**)&pADec);
    hr=CoCreateInstance(transform, NULL, CLSCTX_ALL, IID_IBaseFilter, (void**)&pTransform);
    hr=CoCreateInstance(vrenderer, NULL, CLSCTX_ALL, IID_IBaseFilter, (void**)&pVRenderer);
    hr=CoCreateInstance(arenderer, NULL, CLSCTX_ALL, IID_IBaseFilter, (void**)&pARenderer);
    return hr;
}
HRESULT Player::connectProjectionPlayerFilter()
{
    IPin *pOut,*pIn;
    getUnconnectedPin(pSource,PINDIR_OUTPUT,&pOut);
    getUnconnectedPin(pVDec,PINDIR_INPUT,&pIn);
    int hr=pGraph->Connect(pOut,pIn);
    getUnconnectedPin(pSource,PINDIR_OUTPUT,&pOut);
    getUnconnectedPin(pADec,PINDIR_INPUT,&pIn);
    hr=pGraph->Connect(pOut,pIn);
    hr=pGraph2->RenderStream(NULL,&MEDIATYPE_Video,pVDec,pTransform,pVRenderer);
    hr=pGraph2->RenderStream(NULL,&MEDIATYPE_Video,pADec,NULL,pARenderer);
    return hr;
}
HRESULT Player::connectNormalPlayerFilter()
{
    IPin *pOut,*pIn;
    getUnconnectedPin(pSource,PINDIR_OUTPUT,&pOut);
    getUnconnectedPin(pVDec,PINDIR_INPUT,&pIn);
    int hr=pGraph->Connect(pOut,pIn);
    getUnconnectedPin(pSource,PINDIR_OUTPUT,&pOut);
    getUnconnectedPin(pADec,PINDIR_INPUT,&pIn);
    hr=pGraph->Connect(pOut,pIn);
    hr=pGraph2->RenderStream(NULL,&MEDIATYPE_Video,pVDec,NULL,pVRenderer);
    hr=pGraph2->RenderStream(NULL,&MEDIATYPE_Video,pADec,NULL,pARenderer);
    return hr;
}
void Player::addProjectionPlayerFilter()
{
    pGraph->AddFilter(pSource,L"Source");
    pGraph->AddFilter(pVDec,L"Video Decodec");
    pGraph->AddFilter(pADec,L"Audio Decodec");
    pGraph->AddFilter(pTransform,L"Transform");
    pGraph->AddFilter(pVRenderer,L"Video Renderer");
    pGraph->AddFilter(pARenderer,L"Audio Renderer");
}
void Player::addNormalPlayerFilter()
{
    pGraph->AddFilter(pSource,L"Source");
    pGraph->AddFilter(pVDec,L"Video Decodec");
    pGraph->AddFilter(pADec,L"Audio Decodec");
    pGraph->AddFilter(pVRenderer,L"Video Renderer");
    pGraph->AddFilter(pARenderer,L"Audio Renderer");
}
void Player::destroyDShow()
{
    pGraph->Release();
    pControl->Release();
    pEvent->Release();
    if(pSeeking!=NULL)
    {
        pSeeking->Release();
    }
}
void Player::destroyProjectionPlayerFilter()
{
    pSource->Release();
    pVDec->Release();
    pADec->Release();
    pVRenderer->Release();
    pARenderer->Release();
    pTransform->Release();
    dshowLayout->removeWidget(dshowwidget);
    delete(dshowwidget);
}
void Player::removeWindow()
{
    dshowLayout->removeWidget(dshowwidget);
}
void Player::updateWindow()
{
    dshowLayout->addWidget(dshowwidget);
}
