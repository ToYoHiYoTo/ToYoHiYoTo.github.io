//=======================================
//
//		Meshfield.h
//		name : Hiroto
//		data : 2020/01/06 12:40:32
//
//=======================================

#ifndef _Meshfield_H__
#define _Meshfield_H__

/*** インクルード ***/


/*** 前方宣言 ***/
#define field_X	(3)
#define field_Z	(3)
#define length	(22.0f)

/*** Meshfieldクラス ***/
class Meshfield : public CGameObject
{
private:


	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11Buffer* m_IndexBuffer = NULL;

	CTexture* m_Texture = NULL;

	XMFLOAT3 va, vb;

	XMFLOAT3 n;
	CShader* m_Shader;



public:
	void Init();

	void Uninit();

	void Update();

	void Draw();

	void DrawShadow();

	int VertexCnt, IndexCnt, PrimitiveCnt;
};

#endif // !_Meshfield_H__
