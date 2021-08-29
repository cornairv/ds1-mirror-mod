#pragma once

#include "d3d9dev.h"
#include <memory>
#include <d3dx9.h>

class RenderManager
{
public:

	static RenderManager& getInstance()
	{
		static RenderManager singleton;
		return singleton;
	}
	RenderManager(RenderManager const&) = delete;
	void operator=(RenderManager const&) = delete;

	RenderManager() { }

	void setD3dDevice(hkIDirect3DDevice9* hkD3dDev);
	hkIDirect3DDevice9* getD3dDevice();
	void reset();

	// lazy initialize resources
	typedef struct
	{
		float x, y, z, u, v;
	} ScreenQuadVertex;
	ScreenQuadVertex* getScreenQuad();
	IDirect3DVertexDeclaration9* getScreenQuadVertexDeclaration();
	IDirect3DTexture9* getFlipTexture();
	IDirect3DSurface9* getFlipSurface();
	LPD3DXEFFECT getFlipEffect();
	IDirect3DStateBlock9* getFlipStateBlock();

private:
	IDirect3DSurface9* flipSurface = nullptr;
	IDirect3DTexture9* flipTexture = nullptr;
	LPD3DXEFFECT flipEffect = nullptr;
	IDirect3DStateBlock9* flipStateBlock = nullptr;
	IDirect3DVertexDeclaration9* screenQuadVertexDeclaration = nullptr;
	std::unique_ptr<ScreenQuadVertex*> screenQuad = nullptr;
	const D3DVERTEXELEMENT9 screenQuadVertexElements[3] = {
		{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  0 },
		D3DDECL_END()
	};

	hkIDirect3DDevice9* hkD3dDev = nullptr;
};

void __fastcall hkSetLockonRect(void* __this, int __ecx, float unk);
void __fastcall hkSetEnemyRect(void* __this, int __ecx, float unk);
void __fastcall hkSetPhantomRect(void* __this, int __ecx, float unk);
void __fastcall hkRenderPostProcess(void* __this, int __edx, void* unk1, void* unk2);
