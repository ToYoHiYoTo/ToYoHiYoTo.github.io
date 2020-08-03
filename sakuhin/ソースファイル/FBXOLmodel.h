//=======================================
//
//		FBXOLmodel.h
//		name : Hiroto
//		data : 2019/12/03 18:32:08
//
//=======================================

#ifndef _FBXOLmodel_H__
#define _FBXOLmodel_H__
/*** インクルード ***/
#include <map>
#include <vector>


/*** 前方宣言 ***/
#define OUTLINE (0.3f)

struct aiNode;

/*** FBXOLmodelクラス ***/
class FBXOLmodel 
{

private:

	std::vector<MESH> m_Meshes;
	unsigned int m_MeshNum;
	unsigned int frame,sFrame = 0;

	int m_BoneNum;

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
	//void DrawShadow(XMMATRIX &Matrix);
	void Load(const char *FileName);
	void UnLoad();
	//void Update();
	void DrawMesh(aiNode * Node, XMMATRIX &Matrix);
	//void DrawMeshShadow(aiNode * Node, XMMATRIX &Matrix);
	void CreateBone(aiNode * Node);
	void UpdateBoneMatrix(aiNode* Node, XMMATRIX Matrix);

};

#endif // !_FBXOLmodel_H__
