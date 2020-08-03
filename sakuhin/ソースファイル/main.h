#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define NOMINMAX
#include <windows.h>
#undef NOMINMAX

#include <assert.h>
#include <list>
#include <dinput.h>
#include <d3d11.h>
#include <DirectXMath.h>


#include <assimp/cimport.h>
#include <assimp\Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/matrix4x4.h>




using namespace DirectX;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "xaudio2.lib")

#pragma comment (lib,"Dxgi.lib")
#pragma comment (lib,"d3dcompiler.lib")
#pragma comment (lib,"dxguid.lib")
#pragma comment (lib,"assimp.lib")


#define SCREEN_WIDTH	(1536)			// ウインドウの幅
#define SCREEN_HEIGHT	(834)			// ウインドウの高さ


HWND GetWindow();
