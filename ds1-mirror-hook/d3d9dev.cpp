#include "pch.h"
#include "d3d9dev.h"
#include "logger.h"
#include <filesystem>
#include <sstream>

int mainRTuses = 0;
int nrts = 0;
int ndps = 0;
IDirect3DSurface9* mainRT = nullptr;
bool flippedRT = false;
bool didInit = false;
int frames = 0;
bool hudStarted = false;
bool capture = false;
bool renderFinished = false;

hkIDirect3DDevice9::hkIDirect3DDevice9(IDirect3DDevice9* pReturnedDeviceInterface, D3DPRESENT_PARAMETERS* pPresentParam, IDirect3D9* pIDirect3D9) {
	m_pD3Ddev = pReturnedDeviceInterface;
	m_PresentParam = *pPresentParam;
	m_pD3Dint = pIDirect3D9;

}

HRESULT APIENTRY hkIDirect3DDevice9::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) {
	//Log << "Present" << LogEndl::INFO;
	nrts = 0;
	ndps = 0;
	mainRTuses = 0;
	mainRT = nullptr;
	flippedRT = false;
	hudStarted = false;
	renderFinished = false;
	frames++;
	if (std::filesystem::exists("C:\\Users\\Connor\\capture"))
	{
		Log << "Start capture" << LogEndl::INFO;
		std::filesystem::remove("C:\\Users\\Connor\\capture");
		capture = true;
	}
	else {
		capture = false;
	}
	return m_pD3Ddev->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount) {
	return m_pD3Ddev->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

LPD3DXEFFECT flipEffect = nullptr;
LPD3DXBUFFER flipErrors = nullptr;

IDirect3DSurface9* flippedSurface = nullptr;
IDirect3DTexture9* flippedTexture = nullptr;

const D3DVERTEXELEMENT9 vertexElements[3] = {
	{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  0 },
	D3DDECL_END()
};
IDirect3DVertexDeclaration9* vertexDeclaration = nullptr;

IDirect3DVertexDeclaration9* prevVDecl = nullptr;
IDirect3DStateBlock9* prevStateBlock = nullptr;

HRESULT APIENTRY hkIDirect3DDevice9::SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget) {
	nrts++;
	if (nrts == 1)
	{
		mainRT = pRenderTarget;
	}
	

	if (false && capture)
	{
		IDirect3DSurface9* oldRenderTarget;
		HRESULT hr = m_pD3Ddev->GetRenderTarget(0, &oldRenderTarget);
		if (hr == D3D_OK && oldRenderTarget)
		{
			std::wstringstream ss;
			ss << L"capture\\" << std::setfill(L'0') << std::setw(6) << nrts << L".png";
			D3DSURFACE_DESC desc;
			oldRenderTarget->GetDesc(&desc);
			IDirect3DSurface9* convertedSurface;
			m_pD3Ddev->CreateRenderTarget(desc.Width, desc.Height, D3DFMT_X8R8G8B8, D3DMULTISAMPLE_NONE, 0, true, &convertedSurface, NULL);
			D3DXLoadSurfaceFromSurface(convertedSurface, NULL, NULL, oldRenderTarget, NULL, NULL, D3DX_FILTER_POINT, 0);
			D3DXSaveSurfaceToFile(ss.str().c_str(), D3DXIFF_PNG, convertedSurface, NULL, NULL);
			convertedSurface->Release();
			Log << "nrt: " << std::dec << nrts << "; rt: 0x" << std::hex << oldRenderTarget << "; rti: " << std::dec << RenderTargetIndex << LogEndl::INFO;
		}
	}

	if (renderFinished && !flippedRT) { // we just finished rendering the frame (pre-HUD)
		HRESULT hr = D3D_OK;


		if (!prevStateBlock)
		{
			m_pD3Ddev->CreateStateBlock(D3DSBT_ALL, &prevStateBlock);
		}

		IDirect3DSurface9* oldRenderTarget;
		hr = m_pD3Ddev->GetRenderTarget(0, &oldRenderTarget);
		if (hr != D3D_OK) Log << frames << ": GetRenderTarget failed: " << hr << LogEndl::ERRO;

		IDirect3DTexture9* oldRenderTexture;
		hr = oldRenderTarget->GetContainer(IID_IDirect3DTexture9, (void**)&oldRenderTexture);
		if (hr != D3D_OK) Log << frames << ": GetContainer failed: " << hr << LogEndl::ERRO;

		D3DSURFACE_DESC desc;
		hr = oldRenderTarget->GetDesc(&desc);
		if (hr != D3D_OK) Log << frames << ": GetDesc failed: " << hr << LogEndl::ERRO;


		if (!flippedTexture)
		{
			hr = m_pD3Ddev->CreateTexture(desc.Width, desc.Height, 1, D3DUSAGE_RENDERTARGET, desc.Format, desc.Pool, &flippedTexture, NULL);
			if (hr != D3D_OK) Log << frames << ": CreateTexture failed: " << hr << LogEndl::ERRO;

			hr = flippedTexture->GetSurfaceLevel(0, &flippedSurface);
			if (hr != D3D_OK) Log << frames << ": GetSurfaceLevel failed: " << hr << LogEndl::ERRO;
		}

		if (!flipEffect)
		{
			hr = D3DXCreateEffectFromFile(m_pD3Ddev, L"flip.fx", nullptr, NULL, D3DXFX_NOT_CLONEABLE | D3DXSHADER_OPTIMIZATION_LEVEL3, NULL, &flipEffect, &flipErrors);
			if (hr != D3D_OK) Log << frames << ": D3DXCreateEffectFromFile failed: " << flipErrors->GetBufferPointer() << LogEndl::ERRO;
		}

		if (!vertexDeclaration)
		{
			hr = m_pD3Ddev->CreateVertexDeclaration(vertexElements, &vertexDeclaration);
			if (hr != D3D_OK) Log << frames << ": CreateVertexDeclaration failed: " << hr << LogEndl::ERRO;
		}

		prevStateBlock->Capture();
		m_pD3Ddev->GetVertexDeclaration(&prevVDecl);

		m_pD3Ddev->SetVertexDeclaration(vertexDeclaration);
		m_pD3Ddev->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		m_pD3Ddev->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);

		hr = m_pD3Ddev->SetRenderTarget(0, flippedSurface);
		if (hr != D3D_OK) Log << frames << ": SetRenderTarget failed: " << hr << LogEndl::ERRO;

		D3DXHANDLE frameTexHandle = flipEffect->GetParameterByName(NULL, "frameTex2D");
		hr = flipEffect->SetTexture(frameTexHandle, oldRenderTexture);
		if (hr != D3D_OK) Log << frames << ": SetTexture failed: " << hr << LogEndl::ERRO;

		D3DXVECTOR2 pixelSize = D3DXVECTOR2(1.0f / float(desc.Width), 1.0f / float(desc.Height));
		float quad[3][5] = {
			{ -1.0f - pixelSize.x,  1.0f + pixelSize.y, 0.5f, 0.0f, 0.0f },
			{ -1.0f - pixelSize.x, -3.0f + pixelSize.y, 0.5f, 0.0f, 2.0f },
			{  3.0f - pixelSize.x,  1.0f + pixelSize.y, 0.5f, 2.0f, 0.0f }
		};

		UINT passes;
		hr = flipEffect->Begin(&passes, 0);
		if (hr != D3D_OK) Log << frames << ": Begin failed: " << hr << LogEndl::ERRO;
		hr = flipEffect->BeginPass(0);
		if (hr != D3D_OK) Log << frames << ": BeginPass failed: " << hr << LogEndl::ERRO;
		hr = m_pD3Ddev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, quad, sizeof(quad[0]));
		if (hr != D3D_OK) Log << frames << ": DrawPrimitiveUP failed: " << hr << LogEndl::ERRO;
		hr = flipEffect->EndPass();
		if (hr != D3D_OK) Log << frames << ": EndPass failed: " << hr << LogEndl::ERRO;
		hr = flipEffect->End();
		if (hr != D3D_OK) Log << frames << ": End failed: " << hr << LogEndl::ERRO;

		hr = m_pD3Ddev->StretchRect(flippedSurface, nullptr, oldRenderTarget, nullptr, D3DTEXF_NONE);
		if (hr != D3D_OK) Log << frames << ": StretchRect failed: " << hr << LogEndl::ERRO;

		m_pD3Ddev->SetVertexDeclaration(prevVDecl);
		prevStateBlock->Apply();

		prevVDecl->Release();
		oldRenderTarget->Release();
		oldRenderTexture->Release();
		flippedRT = true;
	}

	return m_pD3Ddev->SetRenderTarget(RenderTargetIndex, pRenderTarget);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexShader(IDirect3DVertexShader9* pvShader) {
	return m_pD3Ddev->SetVertexShader(pvShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetViewport(CONST D3DVIEWPORT9* pViewport) {
	return m_pD3Ddev->SetViewport(pViewport);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawIndexedPrimitive(D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount) {
	return m_pD3Ddev->DrawIndexedPrimitive(Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

struct hud_vert {
	short x, y;
	byte r, g, b, a;
	short u, v;
};

struct hud_text_vert {
	float x, y, z;
	float u, v;
	byte r, g, b, a;
};

HRESULT APIENTRY hkIDirect3DDevice9::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) {
	if (renderFinished && capture)
	{
		IDirect3DSurface9* oldRenderTarget;
		HRESULT hr = m_pD3Ddev->GetRenderTarget(0, &oldRenderTarget);
		if (hr == D3D_OK && oldRenderTarget)
		{
			std::wstringstream ss;
			ss << L"capture\\" << std::setfill(L'0') << std::setw(5) << std::dec << nrts << L"_" << std::setfill(L'0') << std::setw(5) << ndps << L".png";
			D3DSURFACE_DESC desc;
			oldRenderTarget->GetDesc(&desc);
			IDirect3DSurface9* convertedSurface;
			m_pD3Ddev->CreateRenderTarget(desc.Width, desc.Height, D3DFMT_X8R8G8B8, D3DMULTISAMPLE_NONE, 0, true, &convertedSurface, NULL);
			D3DXLoadSurfaceFromSurface(convertedSurface, NULL, NULL, oldRenderTarget, NULL, NULL, D3DX_FILTER_POINT, 0);
			D3DXSaveSurfaceToFile(ss.str().c_str(), D3DXIFF_PNG, convertedSurface, NULL, NULL);
			convertedSurface->Release();
			Log << "rt: 0x" << std::hex << oldRenderTarget << "; nrt: " << std::dec << nrts << "; ndps: " << std::dec << ndps << "; DrawIndexedPrimitiveUP: PrimitiveType: " << PrimitiveType << "; " << "PrimitiveCount: " << PrimitiveCount << "; VertexStreamZeroStride: " << VertexStreamZeroStride;

			/*
			if (PrimitiveType == D3DPT_TRIANGLEFAN && PrimitiveCount == 2 && VertexStreamZeroStride == 12)
			{
				Log << "\n";
				struct hud_vert* verts = (struct hud_vert*)pVertexStreamZeroData;
				for (int i = 0; i < 4; i++)
				{
					Log << "v" << i << ": (" << verts[i].x << ", " << verts[i].y << ")";
					if (i < 3) Log << "\n";
				}
			}
			
			IDirect3DVertexDeclaration9 *vdecl;
			m_pD3Ddev->GetVertexDeclaration(&vdecl);
			D3DVERTEXELEMENT9 decl[MAXD3DDECLLENGTH];
			UINT numElements;
			HRESULT hr = vdecl->GetDeclaration(decl, &numElements);
			Log << "\n";
			for (int i = 0; i < numElements; i++)
			{
				Log << "Usage: " << (int)decl[i].Usage << "; Type: " << (int)decl[i].Type << "; Offset: " << (int)decl[i].Offset;
				if (i < numElements - 1) Log << "\n";
			}
			*/
			Log << LogEndl::INFO;
		}
		ndps++;
	}

	if (renderFinished)
	{
		if (PrimitiveType == D3DPT_TRIANGLEFAN && PrimitiveCount == 2 && VertexStreamZeroStride == 12)
		{
			struct hud_vert modifiedVerts[4];
			memcpy(modifiedVerts, pVertexStreamZeroData, sizeof(modifiedVerts));
			for (int i = 0; i < 4; i++)
			{
				modifiedVerts[i].x = (8 * 1280) - modifiedVerts[i].x;
			}
			return m_pD3Ddev->DrawIndexedPrimitiveUP(PrimitiveType, MinIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, modifiedVerts, VertexStreamZeroStride);
		}
	}
	return m_pD3Ddev->DrawIndexedPrimitiveUP(PrimitiveType, MinIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) {
	return m_pD3Ddev->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) {
	if (PrimitiveType == D3DPT_TRIANGLEFAN && PrimitiveCount == 2 && VertexStreamZeroStride == 24)
	{
		IDirect3DSurface9* currentRenderTarget;
		HRESULT hr = m_pD3Ddev->GetRenderTarget(0, &currentRenderTarget);
		if (hr == D3D_OK && mainRT && currentRenderTarget == mainRT)
		{
			if (mainRTuses == 2)
			{
				renderFinished = true;
			}
			mainRTuses++;
		}
	}
	if (renderFinished && capture)
	{
		if (ndps == 13)
		{
			MyDebugBreak();
		}
		IDirect3DSurface9* oldRenderTarget;
		HRESULT hr = m_pD3Ddev->GetRenderTarget(0, &oldRenderTarget);
		if (hr == D3D_OK && oldRenderTarget)
		{
			std::wstringstream ss;
			ss << L"capture\\" << std::setfill(L'0') << std::setw(5) << std::dec << nrts << L"_" << std::setfill(L'0') << std::setw(5) << ndps << L".png";
			D3DSURFACE_DESC desc;
			oldRenderTarget->GetDesc(&desc);
			IDirect3DSurface9* convertedSurface;
			m_pD3Ddev->CreateRenderTarget(desc.Width, desc.Height, D3DFMT_X8R8G8B8, D3DMULTISAMPLE_NONE, 0, true, &convertedSurface, NULL);
			D3DXLoadSurfaceFromSurface(convertedSurface, NULL, NULL, oldRenderTarget, NULL, NULL, D3DX_FILTER_POINT, 0);
			D3DXSaveSurfaceToFile(ss.str().c_str(), D3DXIFF_PNG, convertedSurface, NULL, NULL);
			convertedSurface->Release();
			Log << "rt: 0x" << std::hex << oldRenderTarget << "; nrt: " << std::dec << nrts << "; ndps: " << std::dec << ndps << "; DrawPrimitiveUP: PrimitiveType: " << PrimitiveType << "; " << "PrimitiveCount: " << PrimitiveCount << "; VertexStreamZeroStride: " << VertexStreamZeroStride;
		}

		/*
		if (PrimitiveType == D3DPT_TRIANGLELIST && VertexStreamZeroStride == 24)
		{
			Log << "\n";
			struct hud_text_vert* verts = (struct hud_text_vert*)pVertexStreamZeroData;
			for (int i = 0; i < 6; i++)
			{
				Log << "v" << i << ": (" << verts[i].x << ", " << verts[i].y << ", " << verts[i].z << ")";
				if (i < 5) Log << "\n";
			}
		}

		IDirect3DVertexDeclaration9* vdecl;
		m_pD3Ddev->GetVertexDeclaration(&vdecl);
		D3DVERTEXELEMENT9 decl[MAXD3DDECLLENGTH];
		UINT numElements;
		hr = vdecl->GetDeclaration(decl, &numElements);
		Log << "\n";
		for (int i = 0; i < numElements; i++)
		{
			Log << "Usage: " << (int)decl[i].Usage << "; Type: " << (int)decl[i].Type << "; Offset: " << (int)decl[i].Offset;
			if (i < numElements - 1) Log << "\n";
		}
		*/
		Log << LogEndl::INFO;
		ndps++;
	}

	if (renderFinished)
	{
		if (PrimitiveType == D3DPT_TRIANGLELIST && VertexStreamZeroStride == 24)
		{
			int numVerts = PrimitiveCount * 3;
			std::unique_ptr<struct hud_text_vert[]> modifiedVerts(new struct hud_text_vert[numVerts]);
			memcpy(modifiedVerts.get(), pVertexStreamZeroData, sizeof(struct hud_text_vert) * numVerts);
			for (int i = 0; i < numVerts; i++)
			{
				modifiedVerts[i].x = 1280 - modifiedVerts[i].x;
			}
			return m_pD3Ddev->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, modifiedVerts.get(), VertexStreamZeroStride);
		}
	}
	return m_pD3Ddev->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawRectPatch(UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo) {
	return m_pD3Ddev->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawTriPatch(UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo) {
	return m_pD3Ddev->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetBackBuffer(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer) {
	return m_pD3Ddev->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
}

HRESULT APIENTRY hkIDirect3DDevice9::EndScene() {
	return m_pD3Ddev->EndScene();
}

HRESULT APIENTRY hkIDirect3DDevice9::QueryInterface(REFIID riid, LPVOID* ppvObj) {
	return m_pD3Ddev->QueryInterface(riid, ppvObj);
}

ULONG APIENTRY hkIDirect3DDevice9::AddRef() {
	return m_pD3Ddev->AddRef();
}

HRESULT APIENTRY hkIDirect3DDevice9::BeginScene() {
	return m_pD3Ddev->BeginScene();
}

HRESULT APIENTRY hkIDirect3DDevice9::BeginStateBlock() {
	return m_pD3Ddev->BeginStateBlock();
}

HRESULT APIENTRY hkIDirect3DDevice9::Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil) {
	return m_pD3Ddev->Clear(Count, pRects, Flags, Color, Z, Stencil);
}

HRESULT APIENTRY hkIDirect3DDevice9::ColorFill(IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR color) {
	return m_pD3Ddev->ColorFill(pSurface, pRect, color);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** ppSwapChain) {
	return m_pD3Ddev->CreateAdditionalSwapChain(pPresentationParameters, ppSwapChain);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle) {
	return m_pD3Ddev->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) {

	return m_pD3Ddev->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle) {
	return m_pD3Ddev->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateOffscreenPlainSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) {
	return m_pD3Ddev->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreatePixelShader(CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader) {
	return m_pD3Ddev->CreatePixelShader(pFunction, ppShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateQuery(D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery) {
	return m_pD3Ddev->CreateQuery(Type, ppQuery);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) {
	return m_pD3Ddev->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateStateBlock(D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB) {
	return m_pD3Ddev->CreateStateBlock(Type, ppSB);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle) {
	return m_pD3Ddev->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** VERTexBuffer, HANDLE* pSharedHandle) {
	return m_pD3Ddev->CreateVertexBuffer(Length, Usage, FVF, Pool, VERTexBuffer, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl) {
	return m_pD3Ddev->CreateVertexDeclaration(pVertexElements, ppDecl);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateVertexShader(CONST DWORD* pFunction, IDirect3DVertexShader9** ppShader) {
	return m_pD3Ddev->CreateVertexShader(pFunction, ppShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle) {
	return m_pD3Ddev->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::DeletePatch(UINT Handle) {
	return m_pD3Ddev->DeletePatch(Handle);
}

HRESULT APIENTRY hkIDirect3DDevice9::EndStateBlock(IDirect3DStateBlock9** ppSB) {
	return m_pD3Ddev->EndStateBlock(ppSB);
}

HRESULT APIENTRY hkIDirect3DDevice9::EvictManagedResources() {
	return m_pD3Ddev->EvictManagedResources();
}

UINT APIENTRY hkIDirect3DDevice9::GetAvailableTextureMem() {
	return m_pD3Ddev->GetAvailableTextureMem();
}

HRESULT APIENTRY hkIDirect3DDevice9::GetClipPlane(DWORD Index, float* pPlane) {
	return m_pD3Ddev->GetClipPlane(Index, pPlane);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetClipStatus(D3DCLIPSTATUS9* pClipStatus) {
	return m_pD3Ddev->GetClipStatus(pClipStatus);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS* pParameters) {
	return m_pD3Ddev->GetCreationParameters(pParameters);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetCurrentTexturePalette(UINT* pPaletteNumber) {
	return m_pD3Ddev->GetCurrentTexturePalette(pPaletteNumber);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface) {
	return m_pD3Ddev->GetDepthStencilSurface(ppZStencilSurface);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetDeviceCaps(D3DCAPS9* pCaps) {
	return m_pD3Ddev->GetDeviceCaps(pCaps);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetDirect3D(IDirect3D9** ppD3D9) {
	HRESULT hRet = m_pD3Ddev->GetDirect3D(ppD3D9);
	if (SUCCEEDED(hRet))
		*ppD3D9 = m_pD3Dint;
	return hRet;
}

HRESULT APIENTRY hkIDirect3DDevice9::GetDisplayMode(UINT iSwapChain, D3DDISPLAYMODE* pMode) {
	return m_pD3Ddev->GetDisplayMode(iSwapChain, pMode);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetFrontBufferData(UINT iSwapChain, IDirect3DSurface9* pDestSurface) {
	return m_pD3Ddev->GetFrontBufferData(iSwapChain, pDestSurface);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetFVF(DWORD* pFVF) {
	return m_pD3Ddev->GetFVF(pFVF);
}

void APIENTRY hkIDirect3DDevice9::GetGammaRamp(UINT iSwapChain, D3DGAMMARAMP* pRamp) {
	m_pD3Ddev->GetGammaRamp(iSwapChain, pRamp);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetIndices(IDirect3DIndexBuffer9** ppIndexData) {
	return m_pD3Ddev->GetIndices(ppIndexData);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetLight(DWORD Index, D3DLIGHT9* pLight) {
	return m_pD3Ddev->GetLight(Index, pLight);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetLightEnable(DWORD Index, BOOL* pEnable) {
	return m_pD3Ddev->GetLightEnable(Index, pEnable);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetMaterial(D3DMATERIAL9* pMaterial) {
	return m_pD3Ddev->GetMaterial(pMaterial);
}

float APIENTRY hkIDirect3DDevice9::GetNPatchMode() {
	return m_pD3Ddev->GetNPatchMode();
}

unsigned int APIENTRY hkIDirect3DDevice9::GetNumberOfSwapChains() {
	return m_pD3Ddev->GetNumberOfSwapChains();
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries) {
	return m_pD3Ddev->GetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPixelShader(IDirect3DPixelShader9** ppShader) {
	return m_pD3Ddev->GetPixelShader(ppShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPixelShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount) {
	return m_pD3Ddev->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPixelShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount) {
	return m_pD3Ddev->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPixelShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount) {
	return m_pD3Ddev->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetRasterStatus(UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus) {
	return m_pD3Ddev->GetRasterStatus(iSwapChain, pRasterStatus);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue) {
	return m_pD3Ddev->GetRenderState(State, pValue);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetRenderTarget(DWORD renderTargetIndex, IDirect3DSurface9** ppRenderTarget) {
	return m_pD3Ddev->GetRenderTarget(renderTargetIndex, ppRenderTarget);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetRenderTargetData(IDirect3DSurface9* renderTarget, IDirect3DSurface9* pDestSurface) {
	return m_pD3Ddev->GetRenderTargetData(renderTarget, pDestSurface);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue) {
	return m_pD3Ddev->GetSamplerState(Sampler, Type, pValue);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetScissorRect(RECT* pRect) {
	return m_pD3Ddev->GetScissorRect(pRect);
}

BOOL APIENTRY hkIDirect3DDevice9::GetSoftwareVertexProcessing() {
	return m_pD3Ddev->GetSoftwareVertexProcessing();
}

HRESULT APIENTRY hkIDirect3DDevice9::GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* OffsetInBytes, UINT* pStride) {
	return m_pD3Ddev->GetStreamSource(StreamNumber, ppStreamData, OffsetInBytes, pStride);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetStreamSourceFreq(UINT StreamNumber, UINT* Divider) {
	return m_pD3Ddev->GetStreamSourceFreq(StreamNumber, Divider);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetSwapChain(UINT iSwapChain, IDirect3DSwapChain9** pSwapChain) {
	return m_pD3Ddev->GetSwapChain(iSwapChain, pSwapChain);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetTexture(DWORD Stage, IDirect3DBaseTexture9** ppTexture) {
	return m_pD3Ddev->GetTexture(Stage, ppTexture);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue) {
	return m_pD3Ddev->GetTextureStageState(Stage, Type, pValue);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix) {
	return m_pD3Ddev->GetTransform(State, pMatrix);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl) {
	return m_pD3Ddev->GetVertexDeclaration(ppDecl);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexShader(IDirect3DVertexShader9** ppShader) {
	return m_pD3Ddev->GetVertexShader(ppShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount) {
	return m_pD3Ddev->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount) {
	return m_pD3Ddev->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount) {
	return m_pD3Ddev->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetViewport(D3DVIEWPORT9* pViewport) {
	return m_pD3Ddev->GetViewport(pViewport);
}

HRESULT APIENTRY hkIDirect3DDevice9::LightEnable(DWORD LightIndex, BOOL bEnable) {
	return m_pD3Ddev->LightEnable(LightIndex, bEnable);
}

HRESULT APIENTRY hkIDirect3DDevice9::MultiplyTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix) {
	return m_pD3Ddev->MultiplyTransform(State, pMatrix);
}

void APIENTRY WINAPI D3DPERF_SetOptions(DWORD options) {
	//MessageBox(NULL, "D3DPERF_SetOptions", "D3D9Wrapper", MB_OK);
}

HRESULT APIENTRY hkIDirect3DDevice9::ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags) {
	return m_pD3Ddev->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
}

ULONG APIENTRY hkIDirect3DDevice9::Release() {
	return m_pD3Ddev->Release();
}

HRESULT APIENTRY hkIDirect3DDevice9::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters) {
	return m_pD3Ddev->Reset(pPresentationParameters);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetClipPlane(DWORD Index, CONST float* pPlane) {
	return m_pD3Ddev->SetClipPlane(Index, pPlane);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetClipStatus(CONST D3DCLIPSTATUS9* pClipStatus) {
	return m_pD3Ddev->SetClipStatus(pClipStatus);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetCurrentTexturePalette(UINT PaletteNumber) {
	return m_pD3Ddev->SetCurrentTexturePalette(PaletteNumber);
}

void APIENTRY hkIDirect3DDevice9::SetCursorPosition(int X, int Y, DWORD Flags) {
	m_pD3Ddev->SetCursorPosition(X, Y, Flags);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap) {
	return m_pD3Ddev->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil) {
	return m_pD3Ddev->SetDepthStencilSurface(pNewZStencil);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetDialogBoxMode(BOOL bEnableDialogs) {
	return m_pD3Ddev->SetDialogBoxMode(bEnableDialogs);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetFVF(DWORD FVF) {
	return m_pD3Ddev->SetFVF(FVF);
}

void APIENTRY hkIDirect3DDevice9::SetGammaRamp(UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP* pRamp) {
	m_pD3Ddev->SetGammaRamp(iSwapChain, Flags, pRamp);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetIndices(IDirect3DIndexBuffer9* pIndexData) {
	return m_pD3Ddev->SetIndices(pIndexData);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetLight(DWORD Index, CONST D3DLIGHT9* pLight) {
	return m_pD3Ddev->SetLight(Index, pLight);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetMaterial(CONST D3DMATERIAL9* pMaterial) {
	return m_pD3Ddev->SetMaterial(pMaterial);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetNPatchMode(float nSegments) {
	return m_pD3Ddev->SetNPatchMode(nSegments);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY* pEntries) {
	return m_pD3Ddev->SetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPixelShader(IDirect3DPixelShader9* pShader) {
	return m_pD3Ddev->SetPixelShader(pShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPixelShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount) {
	return m_pD3Ddev->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPixelShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount) {

	return m_pD3Ddev->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPixelShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount) {
	return m_pD3Ddev->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value) {
	return m_pD3Ddev->SetRenderState(State, Value);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value) {
	return m_pD3Ddev->SetSamplerState(Sampler, Type, Value);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetScissorRect(CONST RECT* pRect) {
	return m_pD3Ddev->SetScissorRect(pRect);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetSoftwareVertexProcessing(BOOL bSoftware) {
	return m_pD3Ddev->SetSoftwareVertexProcessing(bSoftware);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride) {
	return m_pD3Ddev->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetStreamSourceFreq(UINT StreamNumber, UINT Divider) {
	return m_pD3Ddev->SetStreamSourceFreq(StreamNumber, Divider);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetTexture(DWORD Stage, IDirect3DBaseTexture9* pTexture) {
	return m_pD3Ddev->SetTexture(Stage, pTexture);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) {
	return m_pD3Ddev->SetTextureStageState(Stage, Type, Value);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix) {
	return m_pD3Ddev->SetTransform(State, pMatrix);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl) {
	return m_pD3Ddev->SetVertexDeclaration(pDecl);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount) {
	return m_pD3Ddev->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount) {
	return m_pD3Ddev->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

BOOL APIENTRY hkIDirect3DDevice9::ShowCursor(BOOL bShow) {
	return m_pD3Ddev->ShowCursor(bShow);
}

HRESULT APIENTRY hkIDirect3DDevice9::StretchRect(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter) {
	return m_pD3Ddev->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
}

HRESULT APIENTRY hkIDirect3DDevice9::TestCooperativeLevel() {
	return m_pD3Ddev->TestCooperativeLevel();
}

HRESULT APIENTRY hkIDirect3DDevice9::UpdateSurface(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestPoint) {
	return m_pD3Ddev->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
}

HRESULT APIENTRY hkIDirect3DDevice9::UpdateTexture(IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture) {
	return m_pD3Ddev->UpdateTexture(pSourceTexture, pDestinationTexture);
}

HRESULT APIENTRY hkIDirect3DDevice9::ValidateDevice(DWORD* pNumPasses) {
	return m_pD3Ddev->ValidateDevice(pNumPasses);
}