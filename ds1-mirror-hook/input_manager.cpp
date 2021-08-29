#include "pch.h"
#include "input_manager.h"
#include "addresses.h"
#include "util.h"
#include "logger.h"
#include <map>
#include <dinput.h>

void InputManager::modifyDirectInput(DWORD cbData, LPVOID lpvData)
{
	if (cbData == sizeof(DIMOUSESTATE2))
	{
		DIMOUSESTATE2* state = (DIMOUSESTATE2*)lpvData;
		leftClicking = state->rgbButtons[0];
		rightClicking = state->rgbButtons[1];

		if (invertDirections)
		{
			state->lX *= -1;
		}
		if (invertAttacks && !isMenuOpen())
		{
			state->rgbButtons[0] = 0;
			state->rgbButtons[1] = 0;
		}
	}
	else if (cbData == 256)
	{
		byte* state = (byte*)lpvData;
		if (invertAttacks && !isMenuOpen())
		{
			if (leftClicking)
			{
				pressBoundKey(state, 17); // gets swapped to 19 (block) in modifyKeyBinds
			}
			if (rightClicking)
			{
				pressBoundKey(state, 18); // gets swapped to 20 (parry)
			}
		}
	}
}

void InputManager::pressBoundKey(byte* keyboardState, int bindIndex)
{
	byte key, modifierKey;
	getKeyBinding(bindIndex, key, modifierKey);
	auto it = keyMap.find(key);
	if (it != keyMap.end())
	{
		keyboardState[it->second] = 0x80;
	}
	it = modifierKeyMap.find(modifierKey);
	if (it != keyMap.end())
	{
		keyboardState[it->second] = 0x80;
	}
}

void InputManager::modifyGetXInput(XINPUT_STATE* pState)
{
	bool menu = isMenuOpen();
	if (invertDirections)
	{
		safeNegate(pState->Gamepad.sThumbLX);
		safeNegate(pState->Gamepad.sThumbRX);
	}
	if (invertWeaponSwitch && !menu)
	{
		swapBits(pState->Gamepad.wButtons, XINPUT_GAMEPAD_DPAD_LEFT, XINPUT_GAMEPAD_DPAD_RIGHT);
	}
	if (invertAttacks && !menu)
	{
		swapBits(pState->Gamepad.wButtons, XINPUT_GAMEPAD_LEFT_SHOULDER, XINPUT_GAMEPAD_RIGHT_SHOULDER);
		std::swap(pState->Gamepad.bLeftTrigger, pState->Gamepad.bRightTrigger);
	}
}

void InputManager::modifySetXInput(XINPUT_VIBRATION* pVibration)
{
	std::swap(pVibration->wLeftMotorSpeed, pVibration->wRightMotorSpeed);
}

void InputManager::modifyKeybinds(int* keybinds)
{
	// these are separate keybinds from menu controls, so we don't have to
	// check whether a menu is open

	if (invertDirections)
	{
		std::swap(keybinds[2], keybinds[3]);	// run
		std::swap(keybinds[6], keybinds[7]);	// walk
		std::swap(keybinds[10], keybinds[11]);	// camera
	}
	if (invertWeaponSwitch)
	{
		std::swap(keybinds[13], keybinds[14]);	// switch weapons
	}
	if (invertAttacks)
	{
		std::swap(keybinds[17], keybinds[19]);	// attack/block
		std::swap(keybinds[18], keybinds[20]);	// heavy attack/parry
	}
}

void InputManager::modifyWeaponIcons(void* obj)
{
	if (invertWeaponSwitch)
	{
		byte* bytes = (byte*)obj;
		int id = *(int*)(bytes + 0xf4);
		float* coords = (float*)(bytes + 0x78);
		if (id == 2) // right weapon
		{
			rightWeaponCoords = coords;
		}
		else if (id == 3) // left weapon
		{
			for (int i = 0; i < 4; i++)
			{
				std::swap(coords[i], rightWeaponCoords[i]);
			}
		}
	}
}

void __fastcall hkSetup_FrpgMenuDlgFeShortcut(void* __this, [[maybe_unused]] int __edx, void* unk1, void* unk2, void* unk3)
{
	oSetup_FrpgMenuDlgFeShortcut(__this, unk1, unk2, unk3);
	InputManager::getInstance().modifyWeaponIcons(__this);
}

int __fastcall hkPropagateKeybinds(void* inputStructure)
{
	int* keybinds = (int*)(inputStructure)+1;

	// need to unswap keybinds before original function or else it thinks
	// controls have changed and updates the config file
	InputManager::getInstance().modifyKeybinds(keybinds);

	int ret = oPropagateKeybinds(inputStructure);

	InputManager::getInstance().modifyKeybinds(keybinds);

	return ret;
}

bool isMenuOpen()
{
	// if in-game time is 0, we're at the main menu
	if (getIngameTime() == 0)
	{
		return true;
	}

	// menuManager holds info about all the menus on screen
	byte* menuManager = *pMenuManager;

	// getMenuState builds a bitfield representing currently open menus.
	// Mask out the menus that don't block input
	int menuState = oGetMenuState() & 0x7ffff77f;
	if (menuState == 0)
	{
		return false;
	}
	else if (menuState == 8)
	{
		// One kind of menu has several variations; check for
		// the ones that don't block input
		int menuType = *(int*)(menuManager + 0x60);
		if (menuType == 9 || menuType == 0)
		{
			return false;
		}
	}
	return true;
}

// get amount of time in-game in ms, or 0 if at main menu/loading screen
unsigned int getIngameTime()
{
	byte* gameDataManager = *pGameDataManager;
	return *(unsigned int*)(gameDataManager + 0x68);
}

void getKeyBinding(int index, byte& key, byte& modifier)
{
	byte* gameDataManager = *pGameDataManager;
	short* bindings = (short*)(*(byte**)(gameDataManager + 0x34) + 0x1a);
	key = static_cast<byte>(bindings[index * 2]);
	modifier = static_cast<byte>(bindings[index * 2 + 1]);
}

std::map<byte, byte> InputManager::keyMap =
{
	{VK_BACK, DIK_BACKSPACE},
	{VK_ESCAPE, DIK_ESCAPE},
	{VK_INSERT, DIK_INSERT},
	{VK_DELETE, DIK_DELETE},
	{VK_END, DIK_END},
	{VK_PRIOR, DIK_PGUP},
	{VK_NEXT, DIK_PGDN},
	{VK_TAB, DIK_TAB},
	{VK_CAPITAL, DIK_CAPSLOCK},
	{VK_RETURN, DIK_RETURN},
	{VK_SPACE, DIK_SPACE},
	{VK_LEFT, DIK_LEFT},
	{VK_UP, DIK_UP},
	{VK_RIGHT, DIK_RIGHT},
	{VK_DOWN, DIK_DOWN},
	{0x41, DIK_A},
	{0x42, DIK_B},
	{0x43, DIK_C},
	{0x44, DIK_D},
	{0x45, DIK_E},
	{0x46, DIK_F},
	{0x47, DIK_G},
	{0x48, DIK_H},
	{0x49, DIK_I},
	{0x4a, DIK_J},
	{0x4b, DIK_K},
	{0x4c, DIK_L},
	{0x4d, DIK_M},
	{0x4e, DIK_N},
	{0x4f, DIK_O},
	{0x50, DIK_P},
	{0x51, DIK_Q},
	{0x52, DIK_R},
	{0x53, DIK_S},
	{0x54, DIK_T},
	{0x55, DIK_U},
	{0x56, DIK_V},
	{0x57, DIK_W},
	{0x58, DIK_X},
	{0x59, DIK_Y},
	{0x5a, DIK_Z},
	{VK_NUMPAD0, DIK_NUMPAD0},
	{VK_NUMPAD1, DIK_NUMPAD1},
	{VK_NUMPAD2, DIK_NUMPAD2},
	{VK_NUMPAD3, DIK_NUMPAD3},
	{VK_NUMPAD4, DIK_NUMPAD4},
	{VK_NUMPAD5, DIK_NUMPAD5},
	{VK_NUMPAD6, DIK_NUMPAD6},
	{VK_NUMPAD7, DIK_NUMPAD7},
	{VK_NUMPAD8, DIK_NUMPAD8},
	{VK_NUMPAD9, DIK_NUMPAD9}
};

std::map<byte, byte> InputManager::modifierKeyMap =
{
	{0x04, DIK_LSHIFT},
	{0x08, DIK_RSHIFT},
	{0x01, DIK_LCONTROL},
	{0x02, DIK_RCONTROL},
	{0x10, DIK_LALT},
	{0x20, DIK_RALT}
};