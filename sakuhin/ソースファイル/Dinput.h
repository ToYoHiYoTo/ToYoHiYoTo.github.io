//=======================================
//
//		Dinput.h
//		name : Hiroto
//		data : 2020/01/23 16:01:08
//
//=======================================

#ifndef _Dinput_H__
#define _Dinput_H__

#define DIRECTINPUT_VERSION		0x0800		// DirectInput�̃o�[�W�������

/*** �C���N���[�h ***/
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

/*** �O���錾 ***/


/*** Dinput�N���X ***/
class Dinput 
{
private:
	LPDIRECTINPUT8       g_pDInput = NULL;	// DirectInput�I�u�W�F�N�g

	// �}�E�X�p
	LPDIRECTINPUTDEVICE8 g_pDIMouse = NULL;	// �}�E�X�f�o�C�X
	

public:
	bool Init(HINSTANCE hInstApp, HWND hWnd);
	bool UnInit(void);
	bool InitMouse(HWND hWnd);
	bool UnInitMouse();
	void GetMouseState(HWND hWnd);

	DIMOUSESTATE g_zdiMouseState;			// �}�E�X���
};

#endif // !_Dinput_H__
