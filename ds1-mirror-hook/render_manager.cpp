#include "pch.h"
#include "render_manager.h"
#include "input_manager.h"
#include "addresses.h"
#include "logger.h"

void RenderManager::mirrorScreen(void* __this, void* unk1, void* unk2)
{
	// final post processing step before UI is rendered

	IDirect3DSurface9* renderTarget = nullptr;
	CHECK(hkD3dDev->GetRenderTarget(0, &renderTarget));

	// render to our own target
	CHECK(hkD3dDev->SetRenderTarget(0, getFlipSurface()));

	oRenderPostProcess(__this, unk1, unk2);

	// save state info
	CHECK(getFlipStateBlock()->Capture());
	IDirect3DVertexDeclaration9* prevVertDecl;
	CHECK(hkD3dDev->GetVertexDeclaration(&prevVertDecl));

	CHECK(hkD3dDev->SetVertexDeclaration(getScreenQuadVertexDeclaration()));
	CHECK(hkD3dDev->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE));
	CHECK(hkD3dDev->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE));

	CHECK(hkD3dDev->SetRenderTarget(0, renderTarget));

	D3DXHANDLE frameTexHandle = getFlipEffect()->GetParameterByName(NULL, "frameTex2D");
	CHECK_TRUE(frameTexHandle != nullptr);
	CHECK(getFlipEffect()->SetTexture(frameTexHandle, getFlipTexture()));

	// draw flipped image to original render target
	UINT passes;
	CHECK(getFlipEffect()->Begin(&passes, 0));
	CHECK(getFlipEffect()->BeginPass(0));
	CHECK(hkD3dDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, getScreenQuad(), sizeof(getScreenQuad()[0])));
	CHECK(getFlipEffect()->EndPass());
	CHECK(getFlipEffect()->End());

	// restore state info
	CHECK(hkD3dDev->SetVertexDeclaration(prevVertDecl));
	CHECK(getFlipStateBlock()->Apply());

	// clean up
	renderTarget->Release();
}

void RenderManager::mirrorRectPosition(void* __this)
{
	// fix the position of floating UI elements like enemy health bars and the lock-on target
	float* rect = (float*)((byte*)__this + 0x78);
	int width = 1280; // always 1280, even at other resolutions
	rect[0] = width - rect[0];
	rect[2] = width - rect[2];
	std::swap(rect[0], rect[2]);
}

void RenderManager::setD3dDevice(hkIDirect3DDevice9* hkD3dDev_)
{
	if (hkD3dDev_ != nullptr)
	{
		reset();
	}
	hkD3dDev = hkD3dDev_;
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
		CHECK(hkD3dDev->GetRenderTarget(0, &renderTarget));
		D3DSURFACE_DESC desc;
		CHECK(renderTarget->GetDesc(&desc));

		CHECK(hkD3dDev->CreateTexture(desc.Width, desc.Height, 1, D3DUSAGE_RENDERTARGET, desc.Format, desc.Pool, &flipTexture, NULL));

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
		CHECK(flipTexture->GetSurfaceLevel(0, &flipSurface));
	}
	return flipSurface;
}

LPD3DXEFFECT RenderManager::getFlipEffect()
{
	if (!flipEffect)
	{
		LPD3DXBUFFER flipErrors = nullptr;
		CHECK(D3DXCreateEffectFromFile(hkD3dDev, L"dsmirror\\mirror.fx", nullptr, NULL, D3DXFX_NOT_CLONEABLE | D3DXSHADER_OPTIMIZATION_LEVEL3, NULL, &flipEffect, &flipErrors));
	}
	return flipEffect;
}

IDirect3DStateBlock9* RenderManager::getFlipStateBlock()
{
	if (!flipStateBlock)
	{
		CHECK(hkD3dDev->CreateStateBlock(D3DSBT_ALL, &flipStateBlock));
	}
	return flipStateBlock;
}

IDirect3DVertexDeclaration9* RenderManager::getScreenQuadVertexDeclaration()
{
	if (!screenQuadVertexDeclaration)
	{
		CHECK(hkD3dDev->CreateVertexDeclaration(screenQuadVertexElements, &screenQuadVertexDeclaration));
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

	if (InputManager::getInstance().toggleState)
	{
		RenderManager::getInstance().mirrorRectPosition(__this);
	}
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
	if (InputManager::getInstance().toggleState)
	{
		int processType = *(int*)((byte*)__this + 0x34);
		if (processType == 0x1e)
		{
			RenderManager::getInstance().mirrorScreen(__this, unk1, unk2);
			return;
		}
	}

	oRenderPostProcess(__this, unk1, unk2);
}