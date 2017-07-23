#pragma once

// Endscene
typedef long(__stdcall* EndScene_t)(IDirect3DDevice9* device);
EndScene_t g_fnOriginalEndScene = nullptr;

HRESULT __stdcall HkEndScene(IDirect3DDevice9* pDevice)
{
	return g_fnOriginalEndScene(pDevice);
}


// Reset
typedef long(__stdcall* Reset_t)(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pp);
Reset_t g_fnOriginalReset = nullptr;

HRESULT __stdcall HkReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	return g_fnOriginalReset(pDevice, pPresentationParameters);
}