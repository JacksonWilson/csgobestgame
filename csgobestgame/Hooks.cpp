#include "Hooks.hpp"

#include "Options.hpp"
#include "Utils.hpp"
#include "XorStr.hpp"

#include "DrawManager.hpp"

#include "ImGUI\imgui.h"
#include "ImGUI\DX9\imgui_impl_dx9.h"

#include "Hooks\HkWndProc.h"
#include "Hooks\HkCreateMove.h"
#include "Hooks\HkDirectx9.h"

std::unique_ptr<VFTableHook> g_pD3DDevice9Hook = nullptr;
std::unique_ptr<VFTableHook> g_pClientModeHook = nullptr;
std::unique_ptr<VFTableHook> g_pMatSurfaceHook = nullptr;
std::unique_ptr<DrawManager> g_pRenderer = nullptr;

HWND g_hWindow = nullptr;

void Initialize()
{
	//AllocConsole();
    //AttachConsole(GetCurrentProcessId());
    //freopen_s(nullptr, "CON", "w", stdout);

    //Builds the netvar database
    NetvarManager::Instance()->CreateDatabase();
    NetvarManager::Instance()->Dump("netvar_dump.txt");

    //Create the virtual table hooks
    g_pD3DDevice9Hook = std::make_unique<VFTableHook>((PDWORD*)**(uint32_t**)(Utils::FindSignature(XorStr("shaderapidx9.dll"), XorStr("A1 ? ? ? ? 50 8B 08 FF 51 0C")) + 1), true);
    g_pClientModeHook = std::make_unique<VFTableHook>((PDWORD*)se::Interfaces::ClientMode(), true);

    //Find CSGO main window
    while(!(g_hWindow = FindWindowA(XorStr("Valve001"), NULL))) Sleep(200);

    //Replace the WindowProc with our own to capture user input
    if(g_hWindow)
        g_pOldWindowProc = (WNDPROC)SetWindowLongPtr(g_hWindow, GWLP_WNDPROC, (LONG_PTR)HkWndProc);

    g_fnOriginalReset = g_pD3DDevice9Hook->Hook(16, (Reset_t)HkReset);
    g_fnOriginalEndScene = g_pD3DDevice9Hook->Hook(42, (EndScene_t)HkEndScene);
    g_fnOriginalCreateMove = g_pClientModeHook->Hook(24, (CreateMove_t)HkCreateMove);
}

void Restore()
{
    //Restore the WindowProc
    SetWindowLongPtr(g_hWindow, GWLP_WNDPROC, (LONG_PTR)g_pOldWindowProc);

    g_pRenderer->InvalidateObjects();

    //Remove the hooks
    g_pD3DDevice9Hook->RestoreTable();
    g_pClientModeHook->RestoreTable();
    g_pMatSurfaceHook->RestoreTable();
}

void GUI_Init(IDirect3DDevice9* pDevice)
{
    //Initializes the GUI and the renderer
    ImGui_ImplDX9_Init(g_hWindow, pDevice);
    g_pRenderer = std::make_unique<DrawManager>(pDevice);
    g_pRenderer->CreateObjects();
    g_bWasInitialized = true;
}