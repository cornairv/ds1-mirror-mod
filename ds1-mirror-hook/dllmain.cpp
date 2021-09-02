#include "pch.h"
#include "hooks.h"
#include "d3d9.h"
#include "xinput.h"
#include "dinput8.h"
#include "config.h"
#include "logger.h"
#include <wincrypt.h>
#include <DbgHelp.h>

tDirect3DCreate9 oDirect3DCreate9 = Direct3DCreate9;
tXInputGetState oXInputGetState = XInputGetState;
tXInputSetState oXInputSetState = XInputSetState;
tDirectInput8Create oDirectInput8Create;

bool APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, [[maybe_unused]] LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        
        configure("dsmirror\\dsmirror.ini");

        LOG << "Hooked dinput8.dll" << LogEndl::INFO;

        HMODULE dinputDll;
        std::string dinputDllName;
        if (config.loadDll.empty())
        {
            char dinputDllPath[MAX_PATH];
            GetSystemDirectoryA(dinputDllPath, MAX_PATH);
            dinputDllName = std::string(dinputDllPath) + "\\dinput8.dll";
        }
        else
        {
            dinputDllName = config.loadDll;
        }
        LOG << "Loading DLL: " << dinputDllName.c_str() << LogEndl::INFO;
        dinputDll = LoadLibraryA(dinputDllName.c_str());
        if (!dinputDll)
        {
            LOG << "Failed to load " << dinputDllName.c_str() << LogEndl::INFO;
            return false;
        }

        oDirectInput8Create = (tDirectInput8Create)GetProcAddress(dinputDll, "DirectInput8Create");

        setupHooks();

        return true;

    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {

    }

    return true;
}

