#include "pch.h"
#include "render_manager.h"
#include "addresses.h"
#include "logger.h"

void RenderManager::setD3dDevice(hkIDirect3DDevice9* hkD3dDev_)
{
	if (hkD3dDev_ != nullptr)
	{
		reset();
	}
	hkD3dDev = hkD3dDev_;
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

RenderManager::ScreenQuadVertex* RenderManager::getScreenQuad()
{
	if (!screenQuad)
	{
		int width = hkD3dDev->presentParams.BackBufferWidth;
		int height = hkD3dDev->presentParams.BackBufferHeight;

		D3DXVECTOR2 pixelSize(1.0f / float(width), 1.0f / float(height));
		screenQuad = std::make_unique<RenderManager::ScreenQuadVertex*>(new RenderManager::ScreenQuadVertex[5] {
			{ -1.0f - pixelSize.x,  1.0f + pixelSize.y, 0.5f, 0.0f, 0.0f },
			{ -1.0f - pixelSize.x, -3.0f + pixelSize.y, 0.5f, 0.0f, 2.0f },
			{  3.0f - pixelSize.x,  1.0f + pixelSize.y, 0.5f, 2.0f, 0.0f }
		});
	}
	return *screenQuad;
}


////////// Rendering-related hooks //////////

// correct the position of UI elements attached to characters
void inline hkSetRectImpl(void* __this, float unk, tSetFloatingRect originalFunc)
{
	originalFunc(__this, unk);

	// mirror rect's position
	float* rect = (float*)((byte*)__this + 0x78);
	int width = 1280; // always 1280, even at other resolutions
	rect[0] = width - rect[0];
	rect[2] = width - rect[2];
	std::swap(rect[0], rect[2]);
}

void __fastcall hkSetLockonRect(void* __this, [[maybe_unused]] int __ecx, float unk)
{
	hkSetRectImpl(__this, unk, oSetLockonRect);
}

void __fastcall hkSetPhantomRect(void* __this, [[maybe_unused]] int __ecx, float unk)
{
	hkSetRectImpl(__this, unk, oSetPhantomRect);
}

void __fastcall hkSetEnemyRect(void* __this, [[maybe_unused]] int __ecx, float unk)
{
	hkSetRectImpl(__this, unk, oSetEnemyRect);
}

// intercept the final render pass before UI is drawn and flip the image
void __fastcall hkRenderPostProcess(void* __this, [[maybe_unused]] int __edx, void* unk1, void* unk2)
{
	int processType = *(int*)((byte*)__this + 0x34);
	if (processType == 0x1e)
	{
		// final post processing step before UI is rendered

		RenderManager& rm = RenderManager::getInstance();
		IDirect3DDevice9* d3dDev = rm.getD3dDevice();

		IDirect3DSurface9* renderTarget = nullptr;
		d3dDev->GetRenderTarget(0, &renderTarget);

		// render to our own target
		d3dDev->SetRenderTarget(0, rm.getFlipSurface());

		oRenderPostProcess(__this, unk1, unk2);

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
	}
	else
	{
		oRenderPostProcess(__this, unk1, unk2);
	}
}