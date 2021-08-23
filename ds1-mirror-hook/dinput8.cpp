#include "pch.h"
#include "dinput8.h"
#include "dinput8int.h"
#include "logger.h"

HRESULT WINAPI DirectInput8Create(HINSTANCE inst_handle, DWORD version, const IID& r_iid, LPVOID* out_wrapper, LPUNKNOWN p_unk) {
    HRESULT hr = oDirectInput8Create(inst_handle, version, r_iid, out_wrapper, p_unk);

    if (hr == 0)
    {
        LOG << "DirectInput8Create" << LogEndl::INFO;
        *out_wrapper = new hkIDirectInput8(*(IDirectInput8**)out_wrapper);
    }

    return hr;
}
