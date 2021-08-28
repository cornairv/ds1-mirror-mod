#include "pch.h"
#include "dinput8dev.h"
#include "logger.h"
#include "input_manager.h"

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::QueryInterface(REFIID riid, LPVOID* ppvObj)
{
    return dinput8Dev->QueryInterface(riid, ppvObj);
}

STDMETHODIMP_(ULONG __stdcall) hkIDirectInputDevice8::AddRef()
{
    return dinput8Dev->AddRef();
}

STDMETHODIMP_(ULONG __stdcall) hkIDirectInputDevice8::Release()
{
    return dinput8Dev->Release();
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::GetCapabilities(LPDIDEVCAPS lpDIDevCaps)
{
    return dinput8Dev->GetCapabilities(lpDIDevCaps);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKW lpCallback, LPVOID pvRef, DWORD dwFlags)
{
    return dinput8Dev->EnumObjects(lpCallback, pvRef, dwFlags);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::GetProperty(REFGUID rguidProp, LPDIPROPHEADER pdiph)
{
    return dinput8Dev->GetProperty(rguidProp, pdiph);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::SetProperty(REFGUID rguidProp, LPCDIPROPHEADER pdiph)
{
    return dinput8Dev->SetProperty(rguidProp, pdiph);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::Acquire()
{
    return dinput8Dev->Acquire();
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::Unacquire()
{
    return dinput8Dev->Unacquire();
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::GetDeviceState(DWORD cbData, LPVOID lpvData)
{
    HRESULT hr = dinput8Dev->GetDeviceState(cbData, lpvData);
    InputManager::getInstance().modifyDirectInput(cbData, lpvData);
    return hr;
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::GetDeviceData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags)
{
    return dinput8Dev->GetDeviceData(cbObjectData, rgdod, pdwInOut, dwFlags);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::SetDataFormat(LPCDIDATAFORMAT lpdf)
{
    return dinput8Dev->SetDataFormat(lpdf);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::SetEventNotification(HANDLE hEvent)
{
    return dinput8Dev->SetEventNotification(hEvent);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::SetCooperativeLevel(HWND hwnd, DWORD dwFlags)
{
    return dinput8Dev->SetCooperativeLevel(hwnd, dwFlags);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::GetObjectInfo(LPDIDEVICEOBJECTINSTANCEW pdidoi, DWORD dwObj, DWORD dwHow)
{
    return dinput8Dev->GetObjectInfo(pdidoi, dwObj, dwHow);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::GetDeviceInfo(LPDIDEVICEINSTANCEW pdidi)
{
    return dinput8Dev->GetDeviceInfo(pdidi);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::RunControlPanel(HWND hwndOwner, DWORD dwFlags)
{
    return dinput8Dev->RunControlPanel(hwndOwner, dwFlags);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::Initialize(HINSTANCE hinst, DWORD dwVersion, REFGUID rguid)
{
    return dinput8Dev->Initialize(hinst, dwVersion, rguid);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::CreateEffect(REFGUID rguid, LPCDIEFFECT lpeff, LPDIRECTINPUTEFFECT* ppdeff, LPUNKNOWN punkOuter)
{
    return dinput8Dev->CreateEffect(rguid, lpeff, ppdeff, punkOuter);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::EnumEffects(LPDIENUMEFFECTSCALLBACKW lpCallback, LPVOID pvRef, DWORD dwEffType)
{
    return dinput8Dev->EnumEffects(lpCallback, pvRef, dwEffType);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::GetEffectInfo(LPDIEFFECTINFOW pdei, REFGUID rguid)
{
    return dinput8Dev->GetEffectInfo(pdei, rguid);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::GetForceFeedbackState(LPDWORD pdwOut)
{
    return dinput8Dev->GetForceFeedbackState(pdwOut);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::SendForceFeedbackCommand(DWORD dwFlags)
{
    return dinput8Dev->SendForceFeedbackCommand(dwFlags);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK lpCallback, LPVOID pvRef, DWORD fl)
{
    return dinput8Dev->EnumCreatedEffectObjects(lpCallback, pvRef, fl);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::Escape(LPDIEFFESCAPE pesc)
{
    return dinput8Dev->Escape(pesc);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::Poll()
{
    return dinput8Dev->Poll();
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::SendDeviceData(DWORD cbObjectData, LPCDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD fl)
{
    return dinput8Dev->SendDeviceData(cbObjectData, rgdod, pdwInOut, fl);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::EnumEffectsInFile(LPCWSTR lpszFileName, LPDIENUMEFFECTSINFILECALLBACK pec, LPVOID pvRef, DWORD dwFlags)
{
    return dinput8Dev->EnumEffectsInFile(lpszFileName, pec, pvRef, dwFlags);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::WriteEffectToFile(LPCWSTR lpszFileName, DWORD dwEntries, LPDIFILEEFFECT rgDiFileEft, DWORD dwFlags)
{
    return dinput8Dev->WriteEffectToFile(lpszFileName, dwEntries, rgDiFileEft, dwFlags);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::BuildActionMap(LPDIACTIONFORMATW lpdiaf, LPCWSTR lpszUserName, DWORD dwFlags)
{
    return dinput8Dev->BuildActionMap(lpdiaf, lpszUserName, dwFlags);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::SetActionMap(LPDIACTIONFORMATW lpdiActionFormat, LPCWSTR lptszUserName, DWORD dwFlags)
{
    return dinput8Dev->SetActionMap(lpdiActionFormat, lptszUserName, dwFlags);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInputDevice8::GetImageInfo(LPDIDEVICEIMAGEINFOHEADERW lpdiDevImageInfoHeader)
{
    return dinput8Dev->GetImageInfo(lpdiDevImageInfoHeader);
}
