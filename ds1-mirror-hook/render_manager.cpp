#include "pch.h"
#include "render_manager.h"
#include "logger.h"
#include <algorithm>

void RenderManager::setD3dDevice(hkIDirect3DDevice9* hkD3dDev)
{
	if (hkD3dDev != nullptr)
	{
		reset();
	}
	this->hkD3dDev = hkD3dDev;
}

hkIDirect3DDevice9* RenderManager::getD3dDevice()
{
	return hkD3dDev;
}

void RenderManager::reset()
{
	if (flipSurface) flipSurface->Release();
	flipSurface = nullptr;
	if (flipTexture) flipTexture->Release();
	flipTexture = nullptr;
	screenQuad = nullptr;
}

IDirect3DTexture9* RenderManager::getFlipTexture()
{
	if (!flipTexture)
	{
		IDirect3DSurface9* renderTarget;
		hkD3dDev->GetRenderTarget(0, &renderTarget);
		D3DSURFACE_DESC desc;
		renderTarget->GetDesc(&desc);

		hkD3dDev->CreateTexture(desc.Width, desc.Height, 1, D3DUSAGE_RENDERTARGET, desc.Format, desc.Pool, &flipTexture, NULL);

		renderTarget->Release();
	}
	return flipTexture;
}

IDirect3DSurface9* RenderManager::getFlipSurface()
{
	if (!flipSurface)
	{
		if (!flipTexture)
		{
			getFlipTexture();
		}
		flipTexture->GetSurfaceLevel(0, &flipSurface);
	}
	return flipSurface;
}

LPD3DXEFFECT RenderManager::getFlipEffect()
{
	if (!flipEffect)
	{
		LPD3DXBUFFER flipErrors = nullptr;
		D3DXCreateEffectFromFile(hkD3dDev, L"flip.fx", nullptr, NULL, D3DXFX_NOT_CLONEABLE | D3DXSHADER_OPTIMIZATION_LEVEL3, NULL, &flipEffect, &flipErrors);
	}
	return flipEffect;
}

IDirect3DStateBlock9* RenderManager::getFlipStateBlock()
{
	if (!flipStateBlock)
	{
		hkD3dDev->CreateStateBlock(D3DSBT_ALL, &flipStateBlock);
	}
	return flipStateBlock;
}

IDirect3DVertexDeclaration9* RenderManager::getScreenQuadVertexDeclaration()
{
	if (!screenQuadVertexDeclaration)
	{
		hkD3dDev->CreateVertexDeclaration(screenQuadVertexElements, &screenQuadVertexDeclaration);
	}
	return screenQuadVertexDeclaration;
}

RenderManager::screen_quad_vertex* RenderManager::getScreenQuad()
{
	if (!screenQuad)
	{
		int width = hkD3dDev->presentParams.BackBufferWidth;
		int height = hkD3dDev->presentParams.BackBufferHeight;

		D3DXVECTOR2 pixelSize = D3DXVECTOR2(1.0f / float(width), 1.0f / float(height));
		screenQuad = std::make_unique<RenderManager::screen_quad_vertex*>(new RenderManager::screen_quad_vertex[5] {
			{ -1.0f - pixelSize.x,  1.0f + pixelSize.y, 0.5f, 0.0f, 0.0f },
			{ -1.0f - pixelSize.x, -3.0f + pixelSize.y, 0.5f, 0.0f, 2.0f },
			{  3.0f - pixelSize.x,  1.0f + pixelSize.y, 0.5f, 2.0f, 0.0f }
		});
	}
	return *screenQuad;
}


////////// Rendering-related hooks //////////

set_floating_rect_t oSetLockonRect = (set_floating_rect_t)0x00c82770;
set_floating_rect_t oSetEnemyRect = (set_floating_rect_t)0x00fadfd0;
set_floating_rect_t oSetPhantomRect = (set_floating_rect_t)0x00fa86f0;
render_frame_t oFinalRenderPass = (render_frame_t)0x00d81140;

// correct the position of UI elements attached to characters
void inline hkSetRectImpl(set_floating_rect_t originalFunc)
{
	originalFunc();

	int ebx_bak, esi_bak, edi_bak;
	float* rect;

	// original function doesn't follow calling conventions
	// so back up registers that shouldn't get overwritten
	_asm
	{
		mov ebx_bak, ebx
		mov esi_bak, esi
		mov edi_bak, edi

		// rect is left in ecx+0x78
		mov rect, ecx
		add rect, 0x78
	}

	// mirror rect's position
	int width = RenderManager::getInstance().getD3dDevice()->presentParams.BackBufferWidth;
	rect[0] = width - rect[0];
	rect[2] = width - rect[2];
	std::swap(rect[0], rect[2]);

	_asm
	{
		mov ebx, ebx_bak
		mov esi, esi_bak
		mov edi, edi_bak
	}
}

void __stdcall hkSetLockonRect()
{
	hkSetRectImpl(oSetLockonRect);
}

void __stdcall hkSetPhantomRect()
{
	hkSetRectImpl(oSetPhantomRect);
}

void __stdcall hkSetEnemyRect()
{
	hkSetRectImpl(oSetEnemyRect);
}

// intercept the final render pass before UI is drawn and flip the image
void __stdcall hkFinalRenderPass(int unk)
{
	int edi_bak;

	// crashes if edi gets overwritten
	_asm
	{
		mov edi_bak, edi
	}

	RenderManager& rm = RenderManager::getInstance();
	IDirect3DDevice9* d3dDev = rm.getD3dDevice();

	IDirect3DSurface9* renderTarget = nullptr;
	d3dDev->GetRenderTarget(0, &renderTarget);

	// render to our own target
	d3dDev->SetRenderTarget(0, rm.getFlipSurface());

	_asm
	{
		xchg edi, edi_bak
	}
	oFinalRenderPass(unk);
	_asm
	{
		xchg edi, edi_bak
	}

	// save state info
	rm.getFlipStateBlock()->Capture();
	IDirect3DVertexDeclaration9* prevVertDecl;
	d3dDev->GetVertexDeclaration(&prevVertDecl);

	d3dDev->SetVertexDeclaration(rm.getScreenQuadVertexDeclaration());
	d3dDev->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	d3dDev->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);

	d3dDev->SetRenderTarget(0, renderTarget);

	D3DXHANDLE frameTexHandle = rm.getFlipEffect()->GetParameterByName(NULL, "frameTex2D");
	rm.getFlipEffect()->SetTexture(frameTexHandle, rm.getFlipTexture());

	// draw flipped image to original render target
	UINT passes;
	rm.getFlipEffect()->Begin(&passes, 0);
	rm.getFlipEffect()->BeginPass(0);
	d3dDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, rm.getScreenQuad(), sizeof(rm.getScreenQuad()[0]));
	rm.getFlipEffect()->EndPass();
	rm.getFlipEffect()->End();

	// restore state info
	d3dDev->SetVertexDeclaration(prevVertDecl);
	rm.getFlipStateBlock()->Apply();


	// clean up
	renderTarget->Release();

	_asm
	{
		mov edi, edi_bak
	}
}