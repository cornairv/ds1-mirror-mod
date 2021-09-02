#include "pch.h"
#include "fmod.h"
#include "addresses.h"
#include "input_manager.h"
#include "logger.h"
#include <detours/detours.h>

typedef int(__thiscall* tEventSystem_Set3DListenerAttributes)(void*, void*, int, float*, float*, float*, float*);
tEventSystem_Set3DListenerAttributes oEventSystem_Set3DListenerAttributes;

int __fastcall hkEventSystem_Set3DListenerAttributes(void* __this, [[maybe_unused]] int __edx, void* __that, int listener, float* pos, float* vel, float* forward, float* up)
{
	if (InputManager::getInstance().toggleState)
	{
		// flip the up-vector to flip the audio
		for (int i = 0; i < 3; i++)
		{
			up[i] *= -1;
		}
	}
 	return oEventSystem_Set3DListenerAttributes(__this, __that, listener, pos, vel, forward, up);
}

int __stdcall hkEventSystem_Create(void** eventSystem)
{
	int ret = oEventSystem_Create(eventSystem);

	// FMOD_EventSystem_Set3DListenerAttributes isn't exported so grab it here
	byte* vtable = **(byte***)eventSystem;
	oEventSystem_Set3DListenerAttributes = *(tEventSystem_Set3DListenerAttributes*)(vtable + 0x88);
	LOG << "addr: " << std::hex << oEventSystem_Set3DListenerAttributes << LogEndl::INFO;
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)oEventSystem_Set3DListenerAttributes, hkEventSystem_Set3DListenerAttributes);
	DetourTransactionCommit();
	LOG << "Hooked FMOD_EventSystem_Set3DListenerAttributes" << LogEndl::INFO;

	return ret;
}