//=======================================
//
//		FBXmodel.h
//		name : Hiroto
//		data : 2019/12/03 15:23:47
//
//=======================================

#ifndef _FBXmodel_H__
#define _FBXmodel_H__
/*** インクルード ***/
#include <map>
#include <vector>
#include "AmodelShader.h"

/*** 前方宣言 ***/

struct aiNode;


/*** 構造体 ***/

struct ANIME_VERTEX
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT4 Diffuse;
	XMFLOAT2 TexCoord;
	unsigned int bone_Index[4];
	float weight[4];
};

/*** FBXmodelクラス ***/
class FBXmodel 
{
private:

	std::vector<MESH> m_Meshes;
	unsigned int m_MeshNum;
	unsigned int frame,sframe = 0;

	int m_BoneNum = 0;

	const aiScene* m_Scene;
	AmodelShader* m_Shader;
	CTexture* m_Texture;


	std::map<std::string, aiQuaternion> m_NodeRotation;
	std::map<std::string, aiVector3D> m_NodePosition;
	std::map<std::string, unsigned int> m_BoneIndex;

	std::map<std::string, unsigned int> m_TextureIdMap;

	std::map<unsigned int, BONE> m_Bone;
	std::vector<DEFORM_VERTEX*> m_DeformVertex;

	XMMATRIX aiMatrixToMatrix(aiMatrix4x4 matrix);
	aiMatrix4x4 MatrixToaiMatrix(XMMATRIX matrix);

public:
	void Draw(XMMATRIX &Matrix);
	void DrawShadow(XMMATRIX &Matrix);
	void Load(const char *FileName);
	void UnLoad();
	//void Update();
	void DrawMesh(aiNode * Node, XMMATRIX &Matrix);
	void DrawMeshShadow(aiNode * Node, XMMATRIX &Matrix);
	void CreateBone(aiNode * Node);
	void UpdateBoneMatrix(aiNode* Node, XMMATRIX Matrix);


	FBXmodel();
	
};

#endif // !_FBXmodel_H__
