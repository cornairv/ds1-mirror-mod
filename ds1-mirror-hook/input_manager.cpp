#include "pch.h"
#include "input_manager.h"
#include "addresses.h"
#include "util.h"
#include "logger.h"
#include <map>
#include <dinput.h>
#include <vector>
#include "config.h"

KeyWrapper::KeyWrapper(int keyCode) : keyCode(keyCode)
{
	down = prevDown = GetKeyState(keyCode) & 0x8000;
}

void KeyWrapper::update()
{
	bool state = GetKeyState(keyCode) & 0x8000;
	pressed = state && !down;
	released = !state && down;
	down = state;
}

void InputManager::modifyDirectInput(DWORD cbData, LPVOID lpvData)
{
	if (toggleState)
	{
		if (cbData == sizeof(DIMOUSESTATE2))
		{
			modifyMouseInput((DIMOUSESTATE2*)lpvData);
		}
		else if (cbData == 256)
		{
			modifyKeyboardInput((byte*)lpvData);
		}
	}

	if (cbData == 256)
	{
		// toggle mirror mode
		toggleKey.update();
		if (toggleKey.pressed)
		{
			toggleState = !toggleState;
			for (int i = 0; i < 4; i++)
			{
				std::swap(leftWeaponCoords[i], rightWeaponCoords[i]);
			}
		}
	}
}

void InputManager::modifyMouseInput(DIMOUSESTATE2* state)
{
	leftClicking = state->rgbButtons[0];
	rightClicking = state->rgbButtons[1];

	if (config.invertDirections)
	{
		state->lX *= -1;
	}

	// suppress mouse input
	if (config.invertAttacks && !isMenuOpen())
	{
		state->rgbButtons[0] = 0;
		state->rgbButtons[1] = 0;
	}
}

void InputManager::modifyKeyboardInput(byte* state)
{
	std::vector<byte> keysToPress, keysToRelease;

	if (!isMenuOpen())
	{
		// swap keyboard inputs
		std::vector<std::pair<byte, byte>> keybindSwapPairs; // indices of keybindings to swap
		if (config.invertAttacks)
		{
			keybindSwapPairs.push_back({ 17, 19 }); // attack/block
			keybindSwapPairs.push_back({ 18, 20 }); // heavy attack/parry

			// swap mouse input to keyboard
			byte key, modifier;
			if (leftClicking && getKeyBinding(19, key, modifier)) // block
			{
				keysToPress.push_back(key);
				keysToPress.push_back(modifier);
			}
			if (rightClicking && getKeyBinding(20, key, modifier)) // parry
			{
				keysToPress.push_back(key);
				keysToPress.push_back(modifier);
			}
		}
		if (config.invertDirections)
		{
			keybindSwapPairs.push_back({ 2, 3 });	// run
			keybindSwapPairs.push_back({ 6, 7 });	// walk
			keybindSwapPairs.push_back({ 10, 11 });	// camera
		}
		if (config.invertWeaponSwitch)
		{
			keybindSwapPairs.push_back({ 13, 14 });	// switch weapons
		}
		for (std::pair<byte, byte> swapPair : keybindSwapPairs)
		{
			byte key1, modifier1, key2, modifier2;
			if (getKeyBinding(swapPair.first, key1, modifier1) && getKeyBinding(swapPair.second, key2, modifier2))
			{
				// make sure both key and modifier are pressed if both are defined
				if ((key1 == 0 || (state[key1] & 0x80)) && (modifier1 == 0 || (state[modifier1] & 0x80)))
				{
					if (key1 != 0) keysToRelease.push_back(key1);
					if (modifier1 != 0) keysToRelease.push_back(modifier1);
					if (key2 != 0) keysToPress.push_back(key2);
					if (modifier2 != 0) keysToPress.push_back(modifier2);
				}

				// repeat for other binding
				if ((key2 == 0 || (state[key2] & 0x80)) && (modifier2 == 0 || (state[modifier2] & 0x80)))
				{
					if (key2 != 0) keysToRelease.push_back(key2);
					if (modifier2 != 0) keysToRelease.push_back(modifier2);
					if (key1 != 0) keysToPress.push_back(key1);
					if (modifier1 != 0) keysToPress.push_back(modifier1);
				}
			}
		}
	}
	for (byte keyToRelease : keysToRelease)
	{
		state[keyToRelease] = 0;
	}
	for (byte keyToPress : keysToPress)
	{
		state[keyToPress] = 0x80;
	}
}

void InputManager::modifyGetXInput(XINPUT_STATE* pState)
{
	if (toggleState)
	{
		bool menu = isMenuOpen();
		if (config.invertDirections)
		{
			safeNegate(pState->Gamepad.sThumbLX);
			safeNegate(pState->Gamepad.sThumbRX);
		}
		if (config.invertWeaponSwitch && !menu)
		{
			swapBits(pState->Gamepad.wButtons, XINPUT_GAMEPAD_DPAD_LEFT, XINPUT_GAMEPAD_DPAD_RIGHT);
		}
		if (config.invertAttacks && !menu)
		{
			swapBits(pState->Gamepad.wButtons, XINPUT_GAMEPAD_LEFT_SHOULDER, XINPUT_GAMEPAD_RIGHT_SHOULDER);
			std::swap(pState->Gamepad.bLeftTrigger, pState->Gamepad.bRightTrigger);
		}
	}
}

void InputManager::modifySetXInput(XINPUT_VIBRATION* pVibration)
{
	if (toggleState)
	{
		std::swap(pVibration->wLeftMotorSpeed, pVibration->wRightMotorSpeed);
	}
}

void InputManager::modifyWeaponIcons(void* obj)
{
	if (toggleState && config.invertWeaponSwitch)
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
			leftWeaponCoords = coords;
			for (int i = 0; i < 4; i++)
			{
				std::swap(leftWeaponCoords[i], rightWeaponCoords[i]);
			}
		}
	}
}

void __fastcall hkSetup_FrpgMenuDlgFeShortcut(void* __this, [[maybe_unused]] int __edx, void* unk1, void* unk2, void* unk3)
{
	oSetup_FrpgMenuDlgFeShortcut(__this, unk1, unk2, unk3);
	InputManager::getInstance().modifyWeaponIcons(__this);
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

// get the DirectInput keycodes bound to in-game actions
bool InputManager::getKeyBinding(int index, byte& key, byte& modifier)
{
	byte* gameDataManager = *pGameDataManager;
	short* bindings = (short*)(*(byte**)(gameDataManager + 0x34) + 0x1a);
	byte keyVK = static_cast<byte>(bindings[index * 2]);
	byte modifierVK = static_cast<byte>(bindings[index * 2 + 1]);

	key = 0;
	modifier = 0;
	if (keyVK != 0)
	{
		auto it = keyMap.find(keyVK);
		if (it == keyMap.end())
		{
			LOG << "DirectInput mapping for key 0x" << std::hex << (int)keyVK << " not found" << LogEndl::ERRO;
			return false;
		}
		else
		{
			key = it->second;
		}

	}
	if (modifierVK != 0)
	{
		auto it = modifierKeyMap.find(modifierVK);
		if (it == modifierKeyMap.end())
		{
			LOG << "DirectInput mapping for modifier key 0x" << std::hex << (int)modifierVK << " not found" << LogEndl::ERRO;
			return false;
		}
		else
		{
			modifier = it->second;
		}
	}

	return true;
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