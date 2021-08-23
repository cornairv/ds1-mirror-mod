#include "pch.h"
#include "dinput8int.h"
#include "dinput8dev.h"
#include "logger.h"

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInput8::QueryInterface(REFIID riid, LPVOID* ppvObj)
{
	return dinput8->QueryInterface(riid, ppvObj);
}

STDMETHODIMP_(ULONG __stdcall) hkIDirectInput8::AddRef()
{
	return dinput8->AddRef();
}

STDMETHODIMP_(ULONG __stdcall) hkIDirectInput8::Release()
{
	return dinput8->Release();
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInput8::CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE8W* lplpDirectInputDevice, LPUNKNOWN pUnkOuter)
{
	HRESULT hr = dinput8->CreateDevice(rguid, lplpDirectInputDevice, pUnkOuter);
	if (hr == 0)
	{
		LOG << "Override IDirectInput8::CreateDevice " << std::hex << lplpDirectInputDevice << LogEndl::INFO;
		hkIDirectInputDevice8* hkDinput8Dev = new hkIDirectInputDevice8(*lplpDirectInputDevice);
		*lplpDirectInputDevice = hkDinput8Dev;
	}
	return hr;
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInput8::EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACKW lpCallback, LPVOID pvRef, DWORD dwFlags)
{
	return dinput8->EnumDevices(dwDevType, lpCallback, pvRef, dwFlags);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInput8::GetDeviceStatus(REFGUID rguidInstance)
{
	return dinput8->GetDeviceStatus(rguidInstance);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInput8::RunControlPanel(HWND hwndOwner, DWORD dwFlags)
{
	return dinput8->RunControlPanel(hwndOwner, dwFlags);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInput8::Initialize(HINSTANCE hinst, DWORD dwVersion)
{
	return dinput8->Initialize(hinst, dwVersion);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInput8::FindDevice(REFGUID rguidClass, LPCWSTR ptszName, LPGUID pguidInstance)
{
	return dinput8->FindDevice(rguidClass, ptszName, pguidInstance);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInput8::EnumDevicesBySemantics(LPCWSTR ptszUserName, LPDIACTIONFORMATW lpdiActionFormat, LPDIENUMDEVICESBYSEMANTICSCBW lpCallback, LPVOID pvRef, DWORD dwFlags)
{
	return dinput8->EnumDevicesBySemantics(ptszUserName, lpdiActionFormat, lpCallback, pvRef, dwFlags);
}

STDMETHODIMP_(HRESULT __stdcall) hkIDirectInput8::ConfigureDevices(LPDICONFIGUREDEVICESCALLBACK lpdiCallback, LPDICONFIGUREDEVICESPARAMSW lpdiCDParams, DWORD dwFlags, LPVOID pvRefData)
{
	return dinput8->ConfigureDevices(lpdiCallback, lpdiCDParams, dwFlags, pvRefData);
}
