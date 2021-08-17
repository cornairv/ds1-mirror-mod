#include "pch.h"
#include "hooks.h"
#include "d3d9.h"
#include "d3d9int.h"
#include "d3d9dev.h"
#include "logger.h"
#include <detours/detours.h>

void AfterRenderFrame()
{

}

void hookD3d9()
{
	LONG res;
	res = DetourTransactionBegin();
	if (res != NO_ERROR) Log << "DetourTransactionBegin failed: " << res << LogEndl::INFO;
	res = DetourUpdateThread(GetCurrentThread());
	if (res != NO_ERROR) Log << "DetourUpdateThread failed: " << res << LogEndl::INFO;
	res = DetourAttach(&(PVOID&)oDirect3DCreate9, hkDirect3DCreate9);
	if (res != NO_ERROR) Log << "DetourAttach failed: " << res << LogEndl::INFO;
	res = DetourTransactionCommit();
	if (res != NO_ERROR) Log << "DetourTransactionCommit failed: " << res << LogEndl::INFO;


	res = DetourTransactionBegin();
	if (res != NO_ERROR) Log << "DetourTransactionBegin 2 failed: " << res << LogEndl::INFO;
	res = DetourUpdateThread(GetCurrentThread());
	if (res != NO_ERROR) Log << "DetourUpdateThread 2 failed: " << res << LogEndl::INFO;
	res = DetourAttach(&(PVOID&)renderFunc, MyRenderFrame);
	if (res != NO_ERROR) Log << "DetourAttach 2 failed: " << res << LogEndl::INFO;
	res = DetourTransactionCommit();
	if (res != NO_ERROR) Log << "DetourTransactionCommit 2 failed: " << res << LogEndl::INFO;


	//void* retPtr = (void*)0x00d81724;
	//*((int*)retPtr) = 0xdeadbeef;
}