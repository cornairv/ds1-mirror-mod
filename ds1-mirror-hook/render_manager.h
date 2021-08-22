#pragma once

#include "d3d9dev.h"
#include <memory>

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


	////////// Flip rendered scene //////////

	IDirect3DTexture9* getFlipTexture();
	IDirect3DSurface9* getFlipSurface();
	LPD3DXEFFECT getFlipEffect();
	IDirect3DStateBlock9* getFlipStateBlock();
	IDirect3DVertexDeclaration9* getScreenQuadVertexDeclaration();
	typedef struct
	{
		float x, y, z, u, v;
	} screen_quad_vertex;
	screen_quad_vertex* getScreenQuad();

private:
	IDirect3DSurface9* flipSurface = nullptr;
	IDirect3DTexture9* flipTexture = nullptr;
	LPD3DXEFFECT flipEffect = nullptr;
	IDirect3DStateBlock9* flipStateBlock = nullptr;
	IDirect3DVertexDeclaration9* screenQuadVertexDeclaration = nullptr;
	std::unique_ptr<screen_quad_vertex*> screenQuad = nullptr;
	const D3DVERTEXELEMENT9 screenQuadVertexElements[3] = {
		{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  0 },
		D3DDECL_END()
	};


private:
	hkIDirect3DDevice9* hkD3dDev = nullptr;
};

typedef void(__stdcall* set_floating_rect_t)();
extern set_floating_rect_t oSetLockonRect;
extern set_floating_rect_t oSetEnemyRect;
extern set_floating_rect_t oSetPhantomRect;
void __stdcall hkSetLockonRect();
void __stdcall hkSetEnemyRect();
void __stdcall hkSetPhantomRect();

typedef void(__stdcall* render_frame_t)(int);
extern render_frame_t oFinalRenderPass;
void __stdcall hkFinalRenderPass(int unk);