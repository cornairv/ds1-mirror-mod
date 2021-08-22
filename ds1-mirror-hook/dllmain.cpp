// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include "dllmain.h"
#include "logger.h"
#include "hooks.h"
#include <detours/detours.h>
#include <memory>

tDirect3DCreate9 oDirect3DCreate9 = Direct3DCreate9;
tDirectInput8Create oDirectInput8Create;

HRESULT myfail() { return 1; }

bool APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        LOG << "Hooked dinput8.dll" << LogEndl::INFO;

        DisableThreadLibraryCalls(hModule);
        wchar_t DInputDllName[MAX_PATH];
        GetSystemDirectory(DInputDllName, MAX_PATH);
        wcscat_s(DInputDllName, L"\\dinput8.dll");
        HMODULE DInput8DLL = LoadLibrary(DInputDllName);
        if (!DInput8DLL)
        {
            LOG << "Failed to load " << DInputDllName << LogEndl::INFO;
            return false;
        }
        oDirectInput8Create = (tDirectInput8Create)GetProcAddress(DInput8DLL, "DirectInput8Create");
        CHECK(myfail());
        hookD3d9();

        return true;

    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {

    }

    return true;
}

