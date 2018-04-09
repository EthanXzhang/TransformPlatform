#include "transformmission.h"

TransformMission::TransformMission(MovieInfo *p)
{
    mi=p;
}
TransformMission::~TransformMission()
{

}
void TransformMission::setTransform()
{
    initDirectShow();
    initTransformFilter();
}
void TransformMission::runTransform()
{
    LPCOLESTR input=reinterpret_cast<const wchar_t *>(mi->path.utf16());
    int hr=pSource->QueryInterface(IID_IFileSourceFilter,(void **)&pFileSource);
    pFileSource->Load(input,NULL);
    hr=pWriter->QueryInterface(IID_IFileSinkFilter,(void **)&pFileWriter);
    TransformFilterInterface *pTransformInterface;
    hr=pTransform->QueryInterface(IID_TransformFilterInterface,(void **)&pTransformInterface);
    if(mi->projectflag)
    {
        pTransformInterface->DoSetting(mi->setting.w,mi->setting.h,(int)mi->setting.input_layout,(int)mi->setting.output_layout,
                                       0,mi->setting.cube_edge_length,mi->setting.max_cube_edge_length,mi->setting.interpolation_alg,
                                       mi->setting.enable_low_pass_filter,mi->setting.enable_multi_threading,mi->setting.num_vertical_segments,
                                       mi->setting.num_horizontal_segments);
    }
    switch (mi->muxer)
    {
    case avi:
        mi->outputfile=mi->outputpath+mi->output+'.'+"avi";
        break;
    case mp4:
        mi->outputfile=mi->outputpath+mi->output+'.'+"mp4";
        break;
    default:
        break;
    }
    LPCOLESTR output=reinterpret_cast<const wchar_t *>(mi->outputfile.utf16());
    pFileWriter->SetFileName(output,NULL);
    if(FAILED(hr))
    {
        return ;
    }
    addTransformFilter();
    connectTransformFilter();
    hr = pControl->Run();
}
HRESULT TransformMission::initTransformFilter()
{
    int hr=CoCreateInstance(source, NULL, CLSCTX_ALL, IID_IBaseFilter, (void**)&pSource);
    hr=CoCreateInstance(vdec, NULL, CLSCTX_ALL, IID_IBaseFilter, (void**)&pVDec);
    hr=CoCreateInstance(adec, NULL, CLSCTX_ALL, IID_IBaseFilter, (void**)&pADec);
    hr=CoCreateInstance(transform, NULL, CLSCTX_ALL, IID_IBaseFilter, (void**)&pTransform);
    switch (mi->muxer)
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
    return hr;
}
HRESULT TransformMission::initDirectShow()
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
HRESULT TransformMission::initCaptureGraphBuilder(IGraphBuilder **ppGraph, ICaptureGraphBuilder2 **ppBuild)
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
HRESULT TransformMission::getUnconnectedPin(IBaseFilter *pFilter,PIN_DIRECTION PinDir,IPin **ppPin)//, PIN_INFO *pinInfo
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
HRESULT TransformMission::getCLSID()
{
    int hr = CLSIDFromString(OLESTR("{8F43B7D9-9D6B-4F48-BE18-4D787C795EEA}"), &source);
    hr = CLSIDFromString(OLESTR("{04FE9017-F873-410E-871E-AB91661A4EF7}"), &vdec);
    hr = CLSIDFromString(OLESTR("{0F40E1E5-4F79-4988-B1A9-CC98794E6B55}"), &adec);
    hr = CLSIDFromString(OLESTR("{9E5A9E31-1C34-4873-863D-D5441C645398}"), &transform);
    hr = CLSIDFromString(OLESTR("{E2510970-F137-11CE-8B67-00AA00A3F1A6}"), &avimuxer);
    hr = CLSIDFromString(OLESTR("{5FD85181-E542-4E52-8D9D-5D613C30131B}"), &mp4muxer);
    hr = CLSIDFromString(OLESTR("{8596E5F0-0DA5-11D0-BD21-00A0C911CE86}"), &writer);
    return hr;
}
void TransformMission::CreateCompressorFilter(IBaseFilter **pBaseFilter)
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
            switch (mi->encoder) {
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
HRESULT TransformMission::connectTransformFilter()
{
    IPin *pOut,*pIn;
    getUnconnectedPin(pSource,PINDIR_OUTPUT,&pOut);
    getUnconnectedPin(pVDec,PINDIR_INPUT,&pIn);
    int hr=pGraph->Connect(pOut,pIn);
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
    getUnconnectedPin(pADec,PINDIR_OUTPUT,&pOut);
    getUnconnectedPin(pMuxer,PINDIR_INPUT,&pIn);
    hr=pGraph->Connect(pOut,pIn);
    getUnconnectedPin(pMuxer,PINDIR_OUTPUT,&pOut);
    getUnconnectedPin(pWriter,PINDIR_INPUT,&pIn);
    hr=pGraph->Connect(pOut,pIn);
    return hr;
}
void TransformMission::addTransformFilter()
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
