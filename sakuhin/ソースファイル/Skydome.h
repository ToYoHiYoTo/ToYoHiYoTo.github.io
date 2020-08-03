//=======================================
//
//		Skydome.h
//		name : Hiroto
//		data : 2020/01/06 15:24:23
//
//=======================================

#ifndef _Skydome_H__
#define _Skydome_H__
/*** インクルード ***/


/*** 前方宣言 ***/
#define radius	(32)
#define divisionNum	(30)

/*** Skydomeクラス ***/
class SkyDome : public CGameObject {
public:

	void Init();
	void Uninit();
	void Update();
	void Draw();
	//void DrawShadow();
	
private:
	ID3D11Buffer* vertexBuffer_;
	ID3D11Buffer* indexBuffer_;
	CTexture* texture_;
	UINT vertexNum_;
	int indexNum_;
	UINT primNum_;

	CShader* m_Shader;
};

#endif // !_Skydome_H__
