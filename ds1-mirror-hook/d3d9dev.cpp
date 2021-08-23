#include "pch.h"
#include "d3d9dev.h"
#include "logger.h"
#include "render_manager.h"

hkIDirect3DDevice9::hkIDirect3DDevice9(IDirect3DDevice9* pReturnedDeviceInterface, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3D9* pIDirect3D9)
	: d3d9Dev(pReturnedDeviceInterface), presentParams(*pPresentationParameters), hkD3d9(pIDirect3D9)
{
}

HRESULT APIENTRY hkIDirect3DDevice9::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
	return d3d9Dev->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
	return d3d9Dev->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget)
{
	return d3d9Dev->SetRenderTarget(RenderTargetIndex, pRenderTarget);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexShader(IDirect3DVertexShader9* pvShader)
{
	return d3d9Dev->SetVertexShader(pvShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetViewport(CONST D3DVIEWPORT9* pViewport)
{
	return d3d9Dev->SetViewport(pViewport);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawIndexedPrimitive(D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	return d3d9Dev->DrawIndexedPrimitive(Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
	
	return d3d9Dev->DrawIndexedPrimitiveUP(PrimitiveType, MinIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
	return d3d9Dev->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
	return d3d9Dev->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawRectPatch(UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo)
{
	return d3d9Dev->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawTriPatch(UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo)
{
	return d3d9Dev->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetBackBuffer(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer)
{
	return d3d9Dev->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
}

HRESULT APIENTRY hkIDirect3DDevice9::EndScene()
{
	return d3d9Dev->EndScene();
}

HRESULT APIENTRY hkIDirect3DDevice9::QueryInterface(REFIID riid, LPVOID* ppvObj)
{
	return d3d9Dev->QueryInterface(riid, ppvObj);
}

ULONG APIENTRY hkIDirect3DDevice9::AddRef()
{
	return d3d9Dev->AddRef();
}

HRESULT APIENTRY hkIDirect3DDevice9::BeginScene()
{
	return d3d9Dev->BeginScene();
}

HRESULT APIENTRY hkIDirect3DDevice9::BeginStateBlock()
{
	return d3d9Dev->BeginStateBlock();
}

HRESULT APIENTRY hkIDirect3DDevice9::Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
{
	return d3d9Dev->Clear(Count, pRects, Flags, Color, Z, Stencil);
}

HRESULT APIENTRY hkIDirect3DDevice9::ColorFill(IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR color)
{
	return d3d9Dev->ColorFill(pSurface, pRect, color);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** ppSwapChain)
{
	return d3d9Dev->CreateAdditionalSwapChain(pPresentationParameters, ppSwapChain);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle)
{
	return d3d9Dev->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{

	return d3d9Dev->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle)
{
	return d3d9Dev->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateOffscreenPlainSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
	return d3d9Dev->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreatePixelShader(CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader)
{
	return d3d9Dev->CreatePixelShader(pFunction, ppShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateQuery(D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery)
{
	return d3d9Dev->CreateQuery(Type, ppQuery);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
	return d3d9Dev->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateStateBlock(D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB)
{
	return d3d9Dev->CreateStateBlock(Type, ppSB);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle)
{
	return d3d9Dev->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** VERTexBuffer, HANDLE* pSharedHandle)
{
	return d3d9Dev->CreateVertexBuffer(Length, Usage, FVF, Pool, VERTexBuffer, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl)
{
	return d3d9Dev->CreateVertexDeclaration(pVertexElements, ppDecl);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateVertexShader(CONST DWORD* pFunction, IDirect3DVertexShader9** ppShader)
{
	return d3d9Dev->CreateVertexShader(pFunction, ppShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle)
{
	return d3d9Dev->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::DeletePatch(UINT Handle)
{
	return d3d9Dev->DeletePatch(Handle);
}

HRESULT APIENTRY hkIDirect3DDevice9::EndStateBlock(IDirect3DStateBlock9** ppSB)
{
	return d3d9Dev->EndStateBlock(ppSB);
}

HRESULT APIENTRY hkIDirect3DDevice9::EvictManagedResources()
{
	return d3d9Dev->EvictManagedResources();
}

UINT APIENTRY hkIDirect3DDevice9::GetAvailableTextureMem()
{
	return d3d9Dev->GetAvailableTextureMem();
}

HRESULT APIENTRY hkIDirect3DDevice9::GetClipPlane(DWORD Index, float* pPlane)
{
	return d3d9Dev->GetClipPlane(Index, pPlane);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetClipStatus(D3DCLIPSTATUS9* pClipStatus)
{
	return d3d9Dev->GetClipStatus(pClipStatus);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS* pParameters)
{
	return d3d9Dev->GetCreationParameters(pParameters);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetCurrentTexturePalette(UINT* pPaletteNumber)
{
	return d3d9Dev->GetCurrentTexturePalette(pPaletteNumber);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface)
{
	return d3d9Dev->GetDepthStencilSurface(ppZStencilSurface);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetDeviceCaps(D3DCAPS9* pCaps)
{
	return d3d9Dev->GetDeviceCaps(pCaps);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetDirect3D(IDirect3D9** ppD3D9)
{
	HRESULT hr = d3d9Dev->GetDirect3D(ppD3D9);
	if (hr == D3D_OK)
	{
		*ppD3D9 = hkD3d9;
	}
	return hr;
}

HRESULT APIENTRY hkIDirect3DDevice9::GetDisplayMode(UINT iSwapChain, D3DDISPLAYMODE* pMode)
{
	return d3d9Dev->GetDisplayMode(iSwapChain, pMode);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetFrontBufferData(UINT iSwapChain, IDirect3DSurface9* pDestSurface)
{
	return d3d9Dev->GetFrontBufferData(iSwapChain, pDestSurface);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetFVF(DWORD* pFVF)
{
	return d3d9Dev->GetFVF(pFVF);
}

void APIENTRY hkIDirect3DDevice9::GetGammaRamp(UINT iSwapChain, D3DGAMMARAMP* pRamp)
{
	d3d9Dev->GetGammaRamp(iSwapChain, pRamp);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetIndices(IDirect3DIndexBuffer9** ppIndexData)
{
	return d3d9Dev->GetIndices(ppIndexData);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetLight(DWORD Index, D3DLIGHT9* pLight)
{
	return d3d9Dev->GetLight(Index, pLight);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetLightEnable(DWORD Index, BOOL* pEnable)
{
	return d3d9Dev->GetLightEnable(Index, pEnable);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetMaterial(D3DMATERIAL9* pMaterial)
{
	return d3d9Dev->GetMaterial(pMaterial);
}

float APIENTRY hkIDirect3DDevice9::GetNPatchMode()
{
	return d3d9Dev->GetNPatchMode();
}

unsigned int APIENTRY hkIDirect3DDevice9::GetNumberOfSwapChains()
{
	return d3d9Dev->GetNumberOfSwapChains();
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries)
{
	return d3d9Dev->GetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPixelShader(IDirect3DPixelShader9** ppShader)
{
	return d3d9Dev->GetPixelShader(ppShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPixelShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
	return d3d9Dev->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPixelShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
	return d3d9Dev->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPixelShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount)
{
	return d3d9Dev->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetRasterStatus(UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus)
{
	return d3d9Dev->GetRasterStatus(iSwapChain, pRasterStatus);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue)
{
	return d3d9Dev->GetRenderState(State, pValue);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetRenderTarget(DWORD renderTargetIndex, IDirect3DSurface9** ppRenderTarget)
{
	return d3d9Dev->GetRenderTarget(renderTargetIndex, ppRenderTarget);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetRenderTargetData(IDirect3DSurface9* renderTarget, IDirect3DSurface9* pDestSurface)
{
	return d3d9Dev->GetRenderTargetData(renderTarget, pDestSurface);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue)
{
	return d3d9Dev->GetSamplerState(Sampler, Type, pValue);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetScissorRect(RECT* pRect)
{
	return d3d9Dev->GetScissorRect(pRect);
}

BOOL APIENTRY hkIDirect3DDevice9::GetSoftwareVertexProcessing()
{
	return d3d9Dev->GetSoftwareVertexProcessing();
}

HRESULT APIENTRY hkIDirect3DDevice9::GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* OffsetInBytes, UINT* pStride)
{
	return d3d9Dev->GetStreamSource(StreamNumber, ppStreamData, OffsetInBytes, pStride);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetStreamSourceFreq(UINT StreamNumber, UINT* Divider)
{
	return d3d9Dev->GetStreamSourceFreq(StreamNumber, Divider);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetSwapChain(UINT iSwapChain, IDirect3DSwapChain9** pSwapChain)
{
	return d3d9Dev->GetSwapChain(iSwapChain, pSwapChain);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetTexture(DWORD Stage, IDirect3DBaseTexture9** ppTexture)
{
	return d3d9Dev->GetTexture(Stage, ppTexture);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue)
{
	return d3d9Dev->GetTextureStageState(Stage, Type, pValue);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix)
{
	return d3d9Dev->GetTransform(State, pMatrix);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl)
{
	return d3d9Dev->GetVertexDeclaration(ppDecl);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexShader(IDirect3DVertexShader9** ppShader)
{
	return d3d9Dev->GetVertexShader(ppShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
	return d3d9Dev->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
	return d3d9Dev->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount)
{
	return d3d9Dev->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetViewport(D3DVIEWPORT9* pViewport)
{
	return d3d9Dev->GetViewport(pViewport);
}

HRESULT APIENTRY hkIDirect3DDevice9::LightEnable(DWORD LightIndex, BOOL bEnable)
{
	return d3d9Dev->LightEnable(LightIndex, bEnable);
}

HRESULT APIENTRY hkIDirect3DDevice9::MultiplyTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
	return d3d9Dev->MultiplyTransform(State, pMatrix);
}

HRESULT APIENTRY hkIDirect3DDevice9::ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags)
{
	return d3d9Dev->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
}

ULONG APIENTRY hkIDirect3DDevice9::Release()
{
	return d3d9Dev->Release();
}

HRESULT APIENTRY hkIDirect3DDevice9::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	presentParams = *pPresentationParameters;
	RenderManager::getInstance().reset();
	return d3d9Dev->Reset(pPresentationParameters);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetClipPlane(DWORD Index, CONST float* pPlane)
{
	return d3d9Dev->SetClipPlane(Index, pPlane);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetClipStatus(CONST D3DCLIPSTATUS9* pClipStatus)
{
	return d3d9Dev->SetClipStatus(pClipStatus);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetCurrentTexturePalette(UINT PaletteNumber)
{
	return d3d9Dev->SetCurrentTexturePalette(PaletteNumber);
}

void APIENTRY hkIDirect3DDevice9::SetCursorPosition(int X, int Y, DWORD Flags)
{
	d3d9Dev->SetCursorPosition(X, Y, Flags);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap)
{
	return d3d9Dev->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil)
{
	return d3d9Dev->SetDepthStencilSurface(pNewZStencil);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetDialogBoxMode(BOOL bEnableDialogs)
{
	return d3d9Dev->SetDialogBoxMode(bEnableDialogs);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetFVF(DWORD FVF)
{
	return d3d9Dev->SetFVF(FVF);
}

void APIENTRY hkIDirect3DDevice9::SetGammaRamp(UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP* pRamp)
{
	d3d9Dev->SetGammaRamp(iSwapChain, Flags, pRamp);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetIndices(IDirect3DIndexBuffer9* pIndexData)
{
	return d3d9Dev->SetIndices(pIndexData);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetLight(DWORD Index, CONST D3DLIGHT9* pLight)
{
	return d3d9Dev->SetLight(Index, pLight);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetMaterial(CONST D3DMATERIAL9* pMaterial)
{
	return d3d9Dev->SetMaterial(pMaterial);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetNPatchMode(float nSegments)
{
	return d3d9Dev->SetNPatchMode(nSegments);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY* pEntries)
{
	return d3d9Dev->SetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPixelShader(IDirect3DPixelShader9* pShader)
{
	return d3d9Dev->SetPixelShader(pShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPixelShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount)
{
	return d3d9Dev->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPixelShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{

	return d3d9Dev->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPixelShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
{
	return d3d9Dev->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value)
{
	return d3d9Dev->SetRenderState(State, Value);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value)
{
	return d3d9Dev->SetSamplerState(Sampler, Type, Value);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetScissorRect(CONST RECT* pRect)
{
	return d3d9Dev->SetScissorRect(pRect);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetSoftwareVertexProcessing(BOOL bSoftware)
{
	return d3d9Dev->SetSoftwareVertexProcessing(bSoftware);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride)
{
	return d3d9Dev->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetStreamSourceFreq(UINT StreamNumber, UINT Divider)
{
	return d3d9Dev->SetStreamSourceFreq(StreamNumber, Divider);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetTexture(DWORD Stage, IDirect3DBaseTexture9* pTexture)
{
	return d3d9Dev->SetTexture(Stage, pTexture);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
{
	return d3d9Dev->SetTextureStageState(Stage, Type, Value);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
	return d3d9Dev->SetTransform(State, pMatrix);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl)
{
	return d3d9Dev->SetVertexDeclaration(pDecl);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount)
{
	return d3d9Dev->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
{
	return d3d9Dev->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

BOOL APIENTRY hkIDirect3DDevice9::ShowCursor(BOOL bShow)
{
	return d3d9Dev->ShowCursor(bShow);
}

HRESULT APIENTRY hkIDirect3DDevice9::StretchRect(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter)
{
	return d3d9Dev->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
}

HRESULT APIENTRY hkIDirect3DDevice9::TestCooperativeLevel()
{
	return d3d9Dev->TestCooperativeLevel();
}

HRESULT APIENTRY hkIDirect3DDevice9::UpdateSurface(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestPoint)
{
	return d3d9Dev->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
}

HRESULT APIENTRY hkIDirect3DDevice9::UpdateTexture(IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture)
{
	return d3d9Dev->UpdateTexture(pSourceTexture, pDestinationTexture);
}

HRESULT APIENTRY hkIDirect3DDevice9::ValidateDevice(DWORD* pNumPasses)
{
	return d3d9Dev->ValidateDevice(pNumPasses);
}