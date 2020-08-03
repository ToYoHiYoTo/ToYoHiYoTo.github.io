//=======================================
//
//		ModelManager.h
//		name : Hiroto
//		data : 2019/11/26 19:55:27
//
//=======================================

#ifndef _ModelManager_H__
#define _ModelManager_H__

/*** インクルード ***/
#include "FBXmodel.h"
#include "FBXOLmodel.h"

/*** 前方宣言 ***/


/*** ModelManagerクラス ***/
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
