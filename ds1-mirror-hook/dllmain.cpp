#include "pch.h"
#include "hooks.h"
#include "d3d9.h"
#include "xinput.h"
#include "dinput8.h"
#include "config.h"
#include "logger.h"

tDirect3DCreate9 oDirect3DCreate9 = Direct3DCreate9;
tXInputGetState oXInputGetState = XInputGetState;
tXInputSetState oXInputSetState = XInputSetState;
tDirectInput8Create oDirectInput8Create;

bool APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, [[maybe_unused]] LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        configure("dsmirror\\dsmirror.ini");

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

        setupHooks();

        return true;

    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {

    }

    return true;
}

