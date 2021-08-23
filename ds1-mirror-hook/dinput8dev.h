#pragma once

#include <dinput.h>

interface hkIDirectInputDevice8 : public IDirectInputDevice8W
{
    hkIDirectInputDevice8(IDirectInputDevice8W* pDinput8Dev) : dinput8Dev(pDinput8Dev) { }
    IDirectInputDevice8W* dinput8Dev;

    STDMETHOD(QueryInterface)(REFIID riid, LPVOID* ppvObj);
    STDMETHOD_(ULONG, AddRef)();
    STDMETHOD_(ULONG, Release)();
    STDMETHOD(GetCapabilities)(LPDIDEVCAPS lpDIDevCaps);
    STDMETHOD(EnumObjects)(LPDIENUMDEVICEOBJECTSCALLBACKW lpCallback, LPVOID pvRef, DWORD dwFlags);
    STDMETHOD(GetProperty)(REFGUID rguidProp, LPDIPROPHEADER pdiph);
    STDMETHOD(SetProperty)(REFGUID rguidProp, LPCDIPROPHEADER pdiph);
    STDMETHOD(Acquire)();
    STDMETHOD(Unacquire)();
    STDMETHOD(GetDeviceState)(DWORD cbData, LPVOID lpvData);
    STDMETHOD(GetDeviceData)(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags);
    STDMETHOD(SetDataFormat)(LPCDIDATAFORMAT lpdf);
    STDMETHOD(SetEventNotification)(HANDLE hEvent);
    STDMETHOD(SetCooperativeLevel)(HWND hwnd, DWORD dwFlags);
    STDMETHOD(GetObjectInfo)(LPDIDEVICEOBJECTINSTANCEW pdidoi, DWORD dwObj, DWORD dwHow);
    STDMETHOD(GetDeviceInfo)(LPDIDEVICEINSTANCEW pdidi);
    STDMETHOD(RunControlPanel)(HWND hwndOwner, DWORD dwFlags);
    STDMETHOD(Initialize)(HINSTANCE hinst, DWORD dwVersion, REFGUID rguid);
    STDMETHOD(CreateEffect)(REFGUID rguid, LPCDIEFFECT lpeff, LPDIRECTINPUTEFFECT* ppdeff, LPUNKNOWN punkOuter);
    STDMETHOD(EnumEffects)(LPDIENUMEFFECTSCALLBACKW lpCallback, LPVOID pvRef, DWORD dwEffType);
    STDMETHOD(GetEffectInfo)(LPDIEFFECTINFOW pdei, REFGUID rguid);
    STDMETHOD(GetForceFeedbackState)(LPDWORD pdwOut);
    STDMETHOD(SendForceFeedbackCommand)(DWORD dwFlags);
    STDMETHOD(EnumCreatedEffectObjects)(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK lpCallback, LPVOID pvRef, DWORD fl);
    STDMETHOD(Escape)(LPDIEFFESCAPE pesc);
    STDMETHOD(Poll)();
    STDMETHOD(SendDeviceData)(DWORD cbObjectData, LPCDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD fl);
    STDMETHOD(EnumEffectsInFile)(LPCWSTR lpszFileName, LPDIENUMEFFECTSINFILECALLBACK pec, LPVOID pvRef, DWORD dwFlags);
    STDMETHOD(WriteEffectToFile)(LPCWSTR lpszFileName, DWORD dwEntries, LPDIFILEEFFECT rgDiFileEft, DWORD dwFlags);
    STDMETHOD(BuildActionMap)(LPDIACTIONFORMATW lpdiaf, LPCWSTR lpszUserName, DWORD dwFlags);
    STDMETHOD(SetActionMap)(LPDIACTIONFORMATW lpdiActionFormat, LPCWSTR lptszUserName, DWORD dwFlags);
    STDMETHOD(GetImageInfo)(LPDIDEVICEIMAGEINFOHEADERW lpdiDevImageInfoHeader);

};