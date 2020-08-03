#pragma once

typedef struct MOUCE_
{
	int     x;            // X���W
	int     y;            // Y���W
	bool    lButton;      // ���{�^��
	bool    rButton;      // �E�{�^��
	int     moveAdd;      // �ړ���
}MOUCE;

class CCamera : public CGameObject
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

	auto GetViewMatrix() { return m_ViewMatrix; };
	auto GetProjectionMatrix() { return m_ProjectionMatrix; };
};