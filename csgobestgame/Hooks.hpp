#pragma once

#include <Windows.h>
#include <d3d9.h>
//#include <d3dx9.h>
#include <memory> //unique_ptr & make_unique

#include "SourceEngine/SDK.hpp"
#include "VFTableHook.hpp"

//Link with the D3D9 implementations
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

void Initialize();
void Restore();
void GUI_Init(IDirect3DDevice9* pDevice);

