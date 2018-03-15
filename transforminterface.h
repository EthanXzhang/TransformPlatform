#ifndef TRANSFORMINTERFACE_H
#define TRANSFORMINTERFACE_H
#pragma once
#include <initguid.h>
#include <combaseapi.h>
// {0C38C5B0-BBBD-47C6-B656-CFD114D0FF01}
DEFINE_GUID(IID_TransformFilterInterface,
    0xc38c5b0, 0xbbbd, 0x47c6, 0xb6, 0x56, 0xcf, 0xd1, 0x14, 0xd0, 0xff, 0x1);

#ifdef __cplusplus
extern "C" {
#endif

    DECLARE_INTERFACE_(TransformFilterInterface, IUnknown)
    {

        STDMETHOD(DoSetting) (THIS_
            int output_w,
            int output_h,
            int inputlayout,
            int outputlayout,
            int frameformat,
            int cubelength,
            int cubemax,
            int interpolationALG,
            BOOL lowpass_enable,
            BOOL lowpass_MT,
            int vs,
            int hs
            ) PURE;
        STDMETHOD(GetSetting) (THIS_
            int *output_w,
            int *output_h,
            int *inputlayout,
            int *outputlayout,
            int *frameformat,
            int *cubelength,
            int *cubemax,
            int *interpolationALG,
            BOOL *lowpass_enable,
            BOOL *lowpass_MT,
            int *vs,
            int *hs
            ) PURE;

    };
#ifdef __cplusplus
}
#endif
#endif // TRANSFORMINTERFACE_H
