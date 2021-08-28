#include "pch.h"
#include "hooks.h"
#include "d3d9.h"
#include "xinput.h"
#include "render_manager.h"
#include "input_manager.h"
#include <detours/detours.h>
#include "logger.h"

#define ATTACH_HOOK(name) DetourAttach(&(PVOID&)o##name, hk##name)

void setupHooks()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	// rendering
	ATTACH_HOOK(Direct3DCreate9);
	ATTACH_HOOK(XInputGetState);
	ATTACH_HOOK(XInputSetState);
	ATTACH_HOOK(FinalRenderPass);
	ATTACH_HOOK(SetLockonRect);
	ATTACH_HOOK(SetEnemyRect);
	ATTACH_HOOK(SetPhantomRect);

	//input
	ATTACH_HOOK(PropagateKeybinds);

	DetourTransactionCommit();
}