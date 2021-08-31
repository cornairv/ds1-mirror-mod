#pragma once

#include <XInput.h>
#include <dinput.h>
#include <map>
#include "config.h"

class KeyWrapper
{
public:
	KeyWrapper(int keyCode);
	void update();
	bool down = false;
	bool pressed = false;
	bool released = false;

private:
	int keyCode;
	bool prevDown = false;
};

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

	InputManager() : toggleKey(config.toggleKey) { }

	void modifyDirectInput(DWORD cbData, LPVOID lpvData);
	void modifyGetXInput(XINPUT_STATE* pState);
	void modifySetXInput(XINPUT_VIBRATION* pVibration);
	void modifyWeaponIcons(void* obj);

	bool toggleState = true;

private:
	void modifyMouseInput(DIMOUSESTATE2* state);
	void modifyKeyboardInput(byte* state);
	bool getKeyBinding(int index, byte& key, byte& modifier);

	bool rightClicking = false;
	bool leftClicking = false;
	static std::map<byte, byte> keyMap;
	static std::map<byte, byte> modifierKeyMap;
	float* leftWeaponCoords = nullptr;
	float* rightWeaponCoords = nullptr;
	KeyWrapper toggleKey;
};

int __fastcall hkPropagateKeybinds(void* inputStructure);
void __fastcall hkSetup_FrpgMenuDlgFeShortcut(void* __this, int __edx, void* unk1, void* unk2, void* unk3);

bool isMenuOpen();
unsigned int getIngameTime();
