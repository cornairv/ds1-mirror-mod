#include "pch.h"
#include "addresses.h"

#define ADDRESS(type, name, address) type name = reinterpret_cast<type>(address)

// rendering
ADDRESS(tSetFloatingRect, oSetLockonRect, 0x00c82a80);
ADDRESS(tSetFloatingRect, oSetEnemyRect, 0x00fae900);
ADDRESS(tSetFloatingRect, oSetPhantomRect, 0x00fa8c90);
ADDRESS(tRenderPostProcess, oRenderPostProcess, 0x00fd3600);

// input
ADDRESS(tSetup_FrpgMenuDlgFeShortcut, oSetup_FrpgMenuDlgFeShortcut, 0x00c82f00);
ADDRESS(tGetMenuState, oGetMenuState, 0x00c2abe0);
ADDRESS(tPropagateKeybinds, oPropagateKeybinds, 0x00f73ff0);
ADDRESS(tVoidFunc, oOncePerFrame, 0x00BAC020);
ADDRESS(byte**, pMenuManager, 0x013786d0);
ADDRESS(byte**, pGameDataManager, 0x01378700);
