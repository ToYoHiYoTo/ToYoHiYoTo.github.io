//=======================================
//
//		CreateOLModel.h
//		name : Hiroto
//		data : 2019/11/27 16:24:54
//
//=======================================

#ifndef _CreateOLModel_H__
#define _CreateOLModel_H__
/*** インクルード ***/


/*** 前方宣言 ***/
#define MODEL_OUTLINE (0.03f)

/*** CreateOLModelクラス ***/


class CreateOLModel
{
private:

	ID3D11Buffer*	m_VertexBuffer = NULL;
	ID3D11Buffer*	m_IndexBuffer = NULL;

	DX11_SUBSET*	m_SubsetArray = NULL;
	unsigned short	m_SubsetNum;

	void LoadObj(const char *FileName, MODEL *Model);
	void LoadMaterial(const char *FileName, MODEL_MATERIAL **MaterialArray, unsigned short *MaterialNum);

public:

	void Draw();

	void Load(const char *FileName);
	void Unload();

};
#endif // !_CreateOLModel_H__
