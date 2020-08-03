//=======================================
//
//		light.h
//		name : Hiroto
//		data : 2020/02/03 15:00:54
//
//=======================================

#ifndef _light_H__
#define _light_H__
/*** �C���N���[�h ***/


/*** �O���錾 ***/


/*** light�N���X ***/
class Light : public CGameObject
{
private:
	RECT m_Viewport;
	XMMATRIX	m_ViewMatrix;
	XMMATRIX	m_ProjectionMatrix;
	XMMATRIX	camMat;

	MOUCE MState;	// �}�E�X���

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void DrawShadow();

	auto GetViewMatrix() { return m_ViewMatrix; };
	auto GetProjectionMatrix() { return m_ProjectionMatrix; };
};

#endif // !_light_H__
