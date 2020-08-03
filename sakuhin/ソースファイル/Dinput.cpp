#include "Dinput.h"

bool Dinput::Init(HINSTANCE hInstApp, HWND hWnd)
{
	HRESULT ret = DirectInput8Create(hInstApp, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pDInput, NULL);
	if (FAILED(ret)) {
		return false;	// 作成に失敗
	}

	return true;
}

bool Dinput::UnInit(void)
{
	// DirectInputのデバイスを開放
	if (g_pDInput) {
		g_pDInput->Release();
		g_pDInput = NULL;
	}

	return true;
}

bool Dinput::InitMouse(HWND hWnd)
{
	HRESULT ret = S_FALSE;
	if (g_pDInput == NULL) {
		return false;
	}

	// マウス用にデバイスオブジェクトを作成
	ret = g_pDInput->CreateDevice(GUID_SysMouse, &g_pDIMouse, NULL);
	if (FAILED(ret)) {
		// デバイスの作成に失敗
		return false;
	}

	// データフォーマットを設定
	ret = g_pDIMouse->SetDataFormat(&c_dfDIMouse);	// マウス用のデータ・フォーマットを設定
	if (FAILED(ret)) {
		// データフォーマットに失敗
		return false;
	}

	// モードを設定（フォアグラウンド＆非排他モード）
	ret = g_pDIMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(ret)) {
		// モードの設定に失敗
		return false;
	}

	// デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）

	ret = g_pDIMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(ret)) {
		// デバイスの設定に失敗
		return false;
	}

	// 入力制御開始
	g_pDIMouse->Acquire();

	return true;
}

bool Dinput::UnInitMouse()
{
	// DirectInputのデバイスを開放
	if (g_pDIMouse) {
		g_pDIMouse->Release();
		g_pDIMouse = NULL;
	}

	return true;
}

void Dinput::GetMouseState(HWND hWnd)
{
	if (g_pDIMouse == NULL) {
		// オブジェクト生成前に呼ばれたときはここで生成させる
		InitMouse(hWnd);
	}

	// 読取前の値を保持します
	DIMOUSESTATE g_zdiMouseState_bak;	// マウス情報(変化検知用)
	memcpy(&g_zdiMouseState_bak, &g_zdiMouseState, sizeof(g_zdiMouseState_bak));

		// マウスの状態を取得します
	HRESULT	hr = g_pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &g_zdiMouseState);
	if (hr == DIERR_INPUTLOST) {
		g_pDIMouse->Acquire();
		hr = g_pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &g_zdiMouseState);
	}
}
