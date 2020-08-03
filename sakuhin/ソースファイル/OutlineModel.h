//=======================================
//
//		OutlineModel.h
//		name : Hiroto
//		data : 2019/11/26 16:00:58
//
//=======================================

#ifndef _OutlineModel_H__
#define _OutlineModel_H__
/*** �C���N���[�h ***/
#include "ModelManager.h"

/*** �O���錾 ***/


/*** OutlineModel�N���X ***/
class OutlineModel : public CGameObject
{
private:

	ID3D11Buffer*	m_VertexBuffer = NULL;


	CShader*		m_Shader;
	ModelManager*	m_ModelManager;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	//void DrawShadow();
};

#endif // !_OutlineModel_H__
