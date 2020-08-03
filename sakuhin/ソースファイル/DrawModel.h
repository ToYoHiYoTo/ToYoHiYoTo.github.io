//=======================================
//
//		DrawModel.h
//		name : Hiroto
//		data : 2019/11/21 16:42:25
//
//=======================================

#ifndef _DrawModel_H__
#define _DrawModel_H__
/*** インクルード ***/
#include "ModelManager.h"
#include "AmodelShader.h"

/*** 前方宣言 ***/

/*** DrawModelクラス ***/
class DrawModel : public CGameObject
{
private:

	//ID3D11Buffer*	m_VertexBuffer = NULL;

	
	AmodelShader*		m_Shader;
	ModelManager* m_ModelManager;
	CTexture*		m_Texture;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void DrawShadow();

	auto GetLightDir() { return m_Shader->GetLightDir(); }

	const char* fileName;



	

};

#endif // !_DrawModel_H__
