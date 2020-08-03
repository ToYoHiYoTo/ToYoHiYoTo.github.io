//=======================================
//
//		light.h
//		name : Hiroto
//		data : 2020/02/03 15:00:54
//
//=======================================

#ifndef _light_H__
#define _light_H__
/*** インクルード ***/


/*** 前方宣言 ***/


/*** lightクラス ***/
class Light : public CGameObject
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
	void DrawShadow();

	auto GetViewMatrix() { return m_ViewMatrix; };
	auto GetProjectionMatrix() { return m_ProjectionMatrix; };
};

#endif // !_light_H__
