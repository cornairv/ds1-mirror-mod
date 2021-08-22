#include "pch.h"
#include "d3d9.h"
#include "d3d9int.h"
#include "logger.h"

int hkDirect3DCreate9CallCount = 0;

IDirect3D9* APIENTRY hkDirect3DCreate9(UINT SDKVersion)
{
	IDirect3D9* d3d9 = oDirect3DCreate9(SDKVersion);

	if (d3d9 != NULL && hkDirect3DCreate9CallCount < 2) {
		d3d9 = new hkIDirect3D9(d3d9);
		++hkDirect3DCreate9CallCount;
		LOG << "Direct3DCreate9" << LogEndl::INFO;
	}
	return d3d9;
}