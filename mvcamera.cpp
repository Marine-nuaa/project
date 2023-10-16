#include "mvcamera.h"

MvCamera::MvCamera()
{
    m_hDevHandle = MV_NULL;
}

MvCamera::~MvCamera()
{
    if (m_hDevHandle)
    {
        MV_CC_DestroyHandle(m_hDevHandle);
        m_hDevHandle    = MV_NULL;
    }
}

int MvCamera::EnumDevices(unsigned int nTLayerType, MV_CC_DEVICE_INFO_LIST *pstDevList)
{
    return MV_CC_EnumDevices(nTLayerType, pstDevList);
}

int MvCamera::Open(MV_CC_DEVICE_INFO *pstDeviceInfo)
{
    if (MV_NULL == pstDeviceInfo)
    {
        return MV_E_PARAMETER;
    }

    if (m_hDevHandle)
    {
        return MV_E_CALLORDER;
    }

    int nRet  = MV_CC_CreateHandle(&m_hDevHandle, pstDeviceInfo);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    nRet = MV_CC_OpenDevice(m_hDevHandle);
    if (MV_OK != nRet)
    {
        MV_CC_DestroyHandle(m_hDevHandle);
        m_hDevHandle = MV_NULL;
    }

    return nRet;
}

int MvCamera::Close()
{
    if (MV_NULL == m_hDevHandle)
    {
        return MV_E_HANDLE;
    }

    MV_CC_CloseDevice(m_hDevHandle);

    int nRet = MV_CC_DestroyHandle(m_hDevHandle);
    m_hDevHandle = MV_NULL;

    return nRet;
}

int MvCamera::RegisterImageCallBack(void (*cbOutput)(unsigned char *, MV_FRAME_OUT_INFO_EX *, void *), void *pUser)
{
    return MV_CC_RegisterImageCallBackEx(m_hDevHandle, cbOutput, pUser);
}

int MvCamera::StartGrabbing()
{
    return MV_CC_StartGrabbing(m_hDevHandle);
}

int MvCamera::StopGrabbing()
{
    return MV_CC_StopGrabbing(m_hDevHandle);
}

int MvCamera::GetImageBuffer(MV_FRAME_OUT *pFrame, int nMsec)
{
    return MV_CC_GetImageBuffer(m_hDevHandle, pFrame, nMsec);
}

int MvCamera::FreeImageBuffer(MV_FRAME_OUT *pFrame)
{
    return MV_CC_FreeImageBuffer(m_hDevHandle, pFrame);
}

int MvCamera::DisplayOneFrame(MV_DISPLAY_FRAME_INFO *pDisplayInfo)
{
    return MV_CC_DisplayOneFrame(m_hDevHandle, pDisplayInfo);
}

int MvCamera::GetDeviceInfo(MV_CC_DEVICE_INFO *pstDevInfo)
{
    return MV_CC_GetDeviceInfo(m_hDevHandle, pstDevInfo);
}

int MvCamera::SetEnumValue(const char *strKey, unsigned int nValue)
{
    return MV_CC_SetEnumValue(m_hDevHandle, strKey, nValue);
}

int MvCamera::GetFloatValue(const char *strKey, MVCC_FLOATVALUE *pFloatValue)
{
    return MV_CC_GetFloatValue(m_hDevHandle, strKey, pFloatValue);
}

int MvCamera::SetFloatValue(const char *strKey, float fValue)
{
    return MV_CC_SetFloatValue(m_hDevHandle, strKey, fValue);
}

int MvCamera::SetBoolValue(const char *strKey, bool bValue)
{
    return MV_CC_SetBoolValue(m_hDevHandle, strKey, bValue);
}

