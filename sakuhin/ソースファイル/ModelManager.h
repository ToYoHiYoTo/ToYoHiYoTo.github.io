//=======================================
//
//		ModelManager.h
//		name : Hiroto
//		data : 2019/11/26 19:55:27
//
//=======================================

#ifndef _ModelManager_H__
#define _ModelManager_H__

/*** �C���N���[�h ***/
#include "FBXmodel.h"
#include "FBXOLmodel.h"

/*** �O���錾 ***/


/*** ModelManager�N���X ***/
class ModelManager 
{
private:

public:
	void LoadModel();
	void LoadOLModel();
	void Unload();
	void OLUnload();
	void Update();
	void Draw(XMMATRIX &Matrix);
	void OLDraw(XMMATRIX &Matrix);

	void DrawShadow(XMMATRIX &Matrix);
	//void OLDrawShadow(XMMATRIX &Matrix);

	FBXmodel* m_fModel;
	FBXOLmodel* m_fOLMoldel;

	CTexture* m_Texture;
};

#endif // !_ModelManager_H__
