#pragma once

typedef struct MOUCE_
{
	int     x;            // X座標
	int     y;            // Y座標
	bool    lButton;      // 左ボタン
	bool    rButton;      // 右ボタン
	int     moveAdd;      // 移動量
}MOUCE;

class CCamera : public CGameObject
{
private:
	RECT m_Viewport;
	XMMATRIX	m_ViewMatrix;
	XMMATRIX	m_ProjectionMatrix;
	XMMATRIX	camMat;

	MOUCE MState;	// マウス情報

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	auto GetViewMatrix() { return m_ViewMatrix; };
	auto GetProjectionMatrix() { return m_ProjectionMatrix; };
};