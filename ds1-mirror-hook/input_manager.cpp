#include "pch.h"
#include "input_manager.h"
#include "logger.h"
#include <limits>
#include <dinput.h>

void InputManager::modifyDirectInput(DWORD cbData, LPVOID lpvData)
{
	if (cbData == sizeof(DIMOUSESTATE2))
	{
		DIMOUSESTATE2* state = (DIMOUSESTATE2*)lpvData;
		if (invertDirections)
		{
			state->lX *= -1;
		}
	}
	else if (cbData == sizeof(DIJOYSTATE2))
	{
		DIJOYSTATE2* state = (DIJOYSTATE2*)lpvData;
		// todo
	}
}

void swapButtons(WORD& n, WORD b1, WORD b2)
{
	bool p1 = n & b1;
	bool p2 = n & b2;
	if (p1 != p2)
	{
		n ^= b1;
		n ^= b2;
	}
}

template<typename T>
void safeNegate(T& n)
{
	if (n == std::numeric_limits<T>::min())
	{
		n++;
	}
	n *= -1;
	if (n == std::numeric_limits<T>::min() + 1)
	{
		n--;
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
		swapButtons(pState->Gamepad.wButtons, XINPUT_GAMEPAD_DPAD_LEFT, XINPUT_GAMEPAD_DPAD_RIGHT);
	}
	if (invertAttacks && !menu)
	{
		swapButtons(pState->Gamepad.wButtons, XINPUT_GAMEPAD_LEFT_SHOULDER, XINPUT_GAMEPAD_RIGHT_SHOULDER);
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
		std::swap(keybinds[17], keybinds[19]);	// heavy attack
		std::swap(keybinds[18], keybinds[20]);	// light attack
	}
}

propagate_keybinds_t oPropagateKeybinds = (propagate_keybinds_t)0x00f73ff0;

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

typedef int(__stdcall* get_menu_state_t)();
get_menu_state_t getMenuState = (get_menu_state_t)0x00c2abe0;

bool isMenuOpen()
{
	// if in-game time is 0, we're at the main menu
	if (getIngameTime() == 0)
	{
		return true;
	}

	// menuManager holds info about all the menus on screen
	char* menuManager = *(char**)0x013786d0;

	// getMenuState builds a bitfield representing currently open menus.
	// Mask out the menus that don't block input
	int menuState = getMenuState() & 0x7ffff77f;
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
	char* gameDataManager = *(char**)0x01378700;
	return *(unsigned int*)(gameDataManager + 0x68);
}