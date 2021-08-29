#pragma once

#include <d3d9.h>

typedef IDirect3D9* (APIENTRY* tDirect3DCreate9)(UINT);
extern tDirect3DCreate9 oDirect3DCreate9;

IDirect3D9* APIENTRY hkDirect3DCreate9(UINT SDKVersion);