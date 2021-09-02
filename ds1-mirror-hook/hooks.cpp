#include "pch.h"
#include "hooks.h"
#include "d3d9.h"
#include "xinput.h"
#include "fmod.h"
#include "render_manager.h"
#include "input_manager.h"
#include "addresses.h"
#include "logger.h"
#include <detours/detours.h>

#define ATTACH_HOOK(name) CHECK(DetourAttach(&(PVOID&)o##name, hk##name))

void setupHooks()
{
	CHECK(DetourTransactionBegin());
	CHECK(DetourUpdateThread(GetCurrentThread()));

	// rendering
	ATTACH_HOOK(Direct3DCreate9);
	ATTACH_HOOK(RenderPostProcess);
	ATTACH_HOOK(SetLockonRect);
	ATTACH_HOOK(SetEnemyRect);
	ATTACH_HOOK(SetPhantomRect);

	// input
	ATTACH_HOOK(XInputGetState);
	ATTACH_HOOK(XInputSetState);
	ATTACH_HOOK(Setup_FrpgMenuDlgFeShortcut);

	// audio
	ATTACH_HOOK(EventSystem_Create);

	CHECK(DetourTransactionCommit());
}