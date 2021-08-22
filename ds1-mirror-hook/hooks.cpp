#include "pch.h"
#include "hooks.h"
#include "d3d9.h"
#include "render_manager.h"
#include <detours/detours.h>

void hookD3d9()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)oDirect3DCreate9, hkDirect3DCreate9);
	DetourAttach(&(PVOID&)oFinalRenderPass, hkFinalRenderPass);
	DetourAttach(&(PVOID&)oSetLockonRect, hkSetLockonRect);
	DetourAttach(&(PVOID&)oSetEnemyRect, hkSetEnemyRect);
	DetourAttach(&(PVOID&)oSetPhantomRect, hkSetPhantomRect);
	DetourTransactionCommit();
}