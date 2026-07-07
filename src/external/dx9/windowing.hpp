#pragma once
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "sdk/vec3.hpp"
#include <d3d9.h>
#include <tchar.h>
namespace dx9 {

// Data
static LPDIRECT3D9 g_pD3D = nullptr;
static LPDIRECT3DDEVICE9 g_pd3dDevice = nullptr;
static bool g_DeviceLost = false;
static UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
static D3DPRESENT_PARAMETERS g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
int setup_dx9_context();
} // namespace dx9
