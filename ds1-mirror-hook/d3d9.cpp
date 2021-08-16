#include "pch.h"
#include "d3d9.h"
#include "d3d9int.h"
#include "logger.h"

int hkDirect3DCreate9CallCount = 0;

IDirect3D9* APIENTRY hkDirect3DCreate9(UINT SDKVersion)
{
	IDirect3D9* d3dint = oDirect3DCreate9(SDKVersion);
	Log << "Direct3DCreate9: " << (d3dint ? "success" : "fail") << LogEndl::INFO;

	if (d3dint != NULL && hkDirect3DCreate9CallCount < 2) {
		d3dint = new hkIDirect3D9(d3dint);
		++hkDirect3DCreate9CallCount;
		Log << "Direct3DCreate9" << LogEndl::INFO;
	}
	return d3dint;
}