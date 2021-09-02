#pragma once

#include "pch.h"

// rendering
typedef void(__thiscall* tSetFloatingRect)(void*, float);
extern tSetFloatingRect oSetLockonRect;
extern tSetFloatingRect oSetEnemyRect;
extern tSetFloatingRect oSetPhantomRect;

typedef void(__thiscall* tRenderPostProcess)(void*, void*, void*);
extern tRenderPostProcess oRenderPostProcess;


// input
typedef int(__fastcall* tPropagateKeybinds)(void*);
extern tPropagateKeybinds oPropagateKeybinds;

typedef void(__thiscall* tSetup_FrpgMenuDlgFeShortcut)(void*, void*, void*, void*);
extern tSetup_FrpgMenuDlgFeShortcut oSetup_FrpgMenuDlgFeShortcut;

typedef int(__stdcall* tGetMenuState)();
extern tGetMenuState oGetMenuState;

typedef void(__stdcall* tVoidFunc)();
extern tVoidFunc oOncePerFrame;

extern byte** pMenuManager;
extern byte** pGameDataManager;


// audio
typedef int(__stdcall* tEventSystem_Create)(void**);
extern tEventSystem_Create oEventSystem_Create;

