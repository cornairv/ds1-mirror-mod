#pragma once

#include <XInput.h>

class InputManager
{
public:

	static InputManager& getInstance()
	{
		static InputManager singleton;
		return singleton;
	}
	InputManager(InputManager const&) = delete;
	void operator=(InputManager const&) = delete;

	InputManager() { }

	void modifyDirectInput(DWORD cbData, LPVOID lpvData);
	void modifyGetXInput(XINPUT_STATE* pState);
	void modifySetXInput(XINPUT_VIBRATION* pVibration);
	void modifyKeybinds(int* keybinds);
private:
	bool invertDirections = true;
	bool invertAttacks = true;
	bool invertWeaponSwitch = true;

	int prevSignL = 1;
	int prevSignR = 1;
};

typedef int(__fastcall* propagate_keybinds_t)(void*);
extern propagate_keybinds_t oPropagateKeybinds;
int __fastcall hkPropagateKeybinds(void* inputStructure);

bool isMenuOpen();
unsigned int getIngameTime();
