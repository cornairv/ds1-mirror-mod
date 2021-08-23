#pragma once

#include <dinput.h>

interface hkIDirectInput8 : IDirectInput8W
{
public:
	hkIDirectInput8(IDirectInput8W* pIDirectInput8) : dinput8(pIDirectInput8) { }

    STDMETHOD(QueryInterface)(REFIID riid, LPVOID* ppvObj);
    STDMETHOD_(ULONG, AddRef)();
    STDMETHOD_(ULONG, Release)();
    STDMETHOD(CreateDevice)(REFGUID rguid, LPDIRECTINPUTDEVICE8W* lplpDirectInputDevice, LPUNKNOWN pUnkOuter);
    STDMETHOD(EnumDevices)(DWORD dwDevType, LPDIENUMDEVICESCALLBACKW lpCallback, LPVOID pvRef, DWORD dwFlags);
    STDMETHOD(GetDeviceStatus)(REFGUID rguidInstance);
    STDMETHOD(RunControlPanel)(HWND hwndOwner, DWORD dwFlags);
    STDMETHOD(Initialize)(HINSTANCE hinst, DWORD dwVersion);
    STDMETHOD(FindDevice)(REFGUID rguidClass, LPCWSTR ptszName, LPGUID pguidInstance);
    STDMETHOD(EnumDevicesBySemantics)(LPCWSTR ptszUserName, LPDIACTIONFORMATW lpdiActionFormat, LPDIENUMDEVICESBYSEMANTICSCBW lpCallback, LPVOID pvRef, DWORD dwFlags);
    STDMETHOD(ConfigureDevices)(LPDICONFIGUREDEVICESCALLBACK lpdiCallback, LPDICONFIGUREDEVICESPARAMSW lpdiCDParams, DWORD dwFlags, LPVOID pvRefData);

private:
	IDirectInput8W* dinput8;
};