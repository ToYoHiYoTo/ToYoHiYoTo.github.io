//=======================================
//
//		field.h
//		name : Hiroto
//		data : 2019/11/18 16:08:06
//
//=======================================

#ifndef _field_H__
#define _field_H__

/*** インクルード ***/
#include "ShaderNormal.h"

/*** 前方宣言 ***/


/*** fieldクラス ***/
class field : public CGameObject
{
private:

	ID3D11Buffer*	m_VertexBuffer = NULL;
	CShader*		m_Shader;
	CShader*		m_nShader;

	CTexture*		m_Texture;


public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void DrawShadow();
	int VertexCnt, IndexCnt, PrimitiveCnt;
};

#endif // !_field_H__
