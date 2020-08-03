//=======================================
//
//		Dinput.h
//		name : Hiroto
//		data : 2020/01/23 16:01:08
//
//=======================================

#ifndef _Dinput_H__
#define _Dinput_H__

#define DIRECTINPUT_VERSION		0x0800		// DirectInputのバージョン情報

/*** インクルード ***/
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

/*** 前方宣言 ***/


/*** Dinputクラス ***/
class Dinput 
{
private:
	LPDIRECTINPUT8       g_pDInput = NULL;	// DirectInputオブジェクト

	// マウス用
	LPDIRECTINPUTDEVICE8 g_pDIMouse = NULL;	// マウスデバイス
	

public:
	bool Init(HINSTANCE hInstApp, HWND hWnd);
	bool UnInit(void);
	bool InitMouse(HWND hWnd);
	bool UnInitMouse();
	void GetMouseState(HWND hWnd);

	DIMOUSESTATE g_zdiMouseState;			// マウス状態
};

#endif // !_Dinput_H__
