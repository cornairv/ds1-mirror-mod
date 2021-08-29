#pragma once

#include <XInput.h>
#include <map>

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
	void modifyWeaponIcons(void* obj);
	void pressBoundKey(byte* keyboardState, int bindIndex);

	float* leftWeaponCoords = nullptr;
	float* rightWeaponCoords = nullptr;

	static std::map<byte, byte> keyMap;
	static std::map<byte, byte> modifierKeyMap;

private:
	bool invertDirections = true;
	bool invertAttacks = true;
	bool invertWeaponSwitch = true;

	bool rightClicking = false;
	bool leftClicking = false;
};

typedef int(__fastcall* propagate_keybinds_t)(void*);
extern propagate_keybinds_t oPropagateKeybinds;
int __fastcall hkPropagateKeybinds(void* inputStructure);

typedef void(__thiscall* tSetup_FrpgMenuDlgFeShortcut)(void*, void*, void*, void*);
extern tSetup_FrpgMenuDlgFeShortcut oSetup_FrpgMenuDlgFeShortcut;
void __fastcall hkSetup_FrpgMenuDlgFeShortcut(void* __this, int edx, void* unk1, void* unk2, void* unk3);

bool isMenuOpen();
unsigned int getIngameTime();
void getKeyBinding(int index, byte& key, byte& modifier);
