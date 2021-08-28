#pragma once

#include <XInput.h>

typedef DWORD (WINAPI* tXInputGetState)(DWORD, XINPUT_STATE*);
extern tXInputGetState oXInputGetState;

typedef DWORD (WINAPI* tXInputSetState)(DWORD, XINPUT_VIBRATION*);
extern tXInputSetState oXInputSetState;

DWORD WINAPI hkXInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState);
DWORD WINAPI hkXInputSetState(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration);