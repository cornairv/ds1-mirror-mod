#include "pch.h"
#include "xinput.h"
#include "input_manager.h"

DWORD WINAPI hkXInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState)
{
	DWORD ret = oXInputGetState(dwUserIndex, pState);
	InputManager::getInstance().modifyGetXInput(pState);
	return ret;
}

DWORD WINAPI hkXInputSetState(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration)
{
	XINPUT_VIBRATION vibration = *pVibration;
	InputManager::getInstance().modifySetXInput(&vibration);
	return oXInputSetState(dwUserIndex, &vibration);
}