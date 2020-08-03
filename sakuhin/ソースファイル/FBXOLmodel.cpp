
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "light.h"
#include "shader.h"
#include "model.h"
#include "texture.h"
#include "FBXmodel.h"



XMMATRIX FBXOLmodel::aiMatrixToMatrix(aiMatrix4x4 matrix)
{
	XMMATRIX m;
	m.r[0] = XMLoadFloat4(&XMFLOAT4(matrix.a1, matrix.a2, matrix.a3, matrix.a4));
	m.r[1] = XMLoadFloat4(&XMFLOAT4(matrix.b1, matrix.b2, matrix.b3, matrix.b4));
	m.r[2] = XMLoadFloat4(&XMFLOAT4(matrix.c1, matrix.c2, matrix.c3, matrix.c4));
	m.r[3] = XMLoadFloat4(&XMFLOAT4(matrix.d1, matrix.d2, matrix.d3, matrix.d4));

	return m;
}

aiMatrix4x4 FBXOLmodel::MatrixToaiMatrix(XMMATRIX matrix)
{
	aiMatrix4x4 m;
	m.a1 = XMVectorGetX(matrix.r[0]);
	m.a2 = XMVectorGetY(matrix.r[0]);
	m.a3 = XMVectorGetZ(matrix.r[0]);
	m.a4 = XMVectorGetW(matrix.r[0]);

	m.b1 = XMVectorGetX(matrix.r[1]);
	m.b2 = XMVectorGetY(matrix.r[1]);
	m.b3 = XMVectorGetZ(matrix.r[1]);
	m.b4 = XMVectorGetW(matrix.r[1]);

	m.c1 = XMVectorGetX(matrix.r[2]);
	m.c2 = XMVectorGetY(matrix.r[2]);
	m.c3 = XMVectorGetZ(matrix.r[2]);
	m.c4 = XMVectorGetW(matrix.r[2]);

	m.d1 = XMVectorGetX(matrix.r[3]);
	m.d2 = XMVectorGetY(matrix.r[3]);
	m.d3 = XMVectorGetZ(matrix.r[3]);
	m.d4 = XMVectorGetW(matrix.r[3]);

	return m;
}



void FBXOLmodel::Load(const char *FileName)
{
	m_Shader = new AmodelShader();

	m_Shader->Init("shaderAnimation3DVS.cso", "OutLine3DPS.cso");

	//モデルの読み込み
	m_Scene = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality);

	assert(m_Scene);

	CreateBone(m_Scene->mRootNode);

	for (int m = 0; m < m_Scene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_Scene->mMeshes[m];
		unsigned int vertex_Num = mesh->mNumVertices;
		ANIME_VERTEX* temp_Vertex = new ANIME_VERTEX[mesh->mNumVertices];
		//DEFORM_VERTEX* temp_DeformVertex = new DEFORM_VERTEX[mesh->mNumVertices];

		std::vector<DEFORM_VERTEX*> temp_DeformVertex;
		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			temp_DeformVertex.emplace_back(new DEFORM_VERTEX());
		}

		ID3D11Buffer* vertex_Buffer;



		// 頂点バッファ生成
		ANIME_VERTEX* vertex = new ANIME_VERTEX[mesh->mNumVertices];

		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			vertex[i].Position = XMFLOAT3(mesh->mVertices[i].x + mesh->mNormals[i].x * OUTLINE,
				mesh->mVertices[i].y + mesh->mNormals[i].y* OUTLINE,
				mesh->mVertices[i].z + mesh->mNormals[i].z* OUTLINE);

			vertex[i].Normal = XMFLOAT3(mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z);
			vertex[i].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[i].TexCoord = XMFLOAT2(0.0f, 1.0f);


			temp_DeformVertex[i]->Position = mesh->mVertices[i];
			temp_DeformVertex[i]->Normal = mesh->mVertices[i];
			temp_DeformVertex[i]->DefromPosition = mesh->mVertices[i];
			temp_DeformVertex[i]->DefromNormal = mesh->mVertices[i];
			temp_DeformVertex[i]->BoneNum = 0;

			temp_Vertex[i].Position = vertex[i].Position;
			temp_Vertex[i].Normal = vertex[i].Normal;
			temp_Vertex[i].Diffuse = vertex[i].Diffuse;
			temp_Vertex[i].TexCoord = vertex[i].TexCoord;

			for (int b = 0; b < 4; b++)
			{
				temp_Vertex[i].weight[b] = 0.0f;
				temp_Vertex[i].bone_Index[b] = 511;

				temp_DeformVertex[i]->BoneName[b] = "";
				temp_DeformVertex[i]->BoneWeight[b] = 0.0f;
			}
		}



		for (unsigned int b = 0; b < mesh->mNumBones; b++)
		{

			aiBone* bone = mesh->mBones[b];
			m_Bone[m_BoneIndex[bone->mName.C_Str()]].OffsetMatrix = aiMatrixToMatrix(bone->mOffsetMatrix);

			for (unsigned int w = 0; w < bone->mNumWeights; w++)
			{
				aiVertexWeight weight = bone->mWeights[w];
				temp_Vertex[weight.mVertexId].bone_Index[temp_DeformVertex[weight.mVertexId]->BoneNum] = m_BoneIndex[bone->mName.C_Str()];
				temp_DeformVertex[weight.mVertexId]->BoneName[temp_DeformVertex[weight.mVertexId]->BoneNum] = bone->mName.C_Str();
				temp_DeformVertex[weight.mVertexId]->BoneWeight[temp_DeformVertex[weight.mVertexId]->BoneNum] = weight.mWeight;
				temp_Vertex[weight.mVertexId].weight[temp_DeformVertex[weight.mVertexId]->BoneNum] = weight.mWeight;
				temp_DeformVertex[weight.mVertexId]->BoneNum++;
			}
		}


		m_DeformVertex = temp_DeformVertex;

		{

			//改造頂点バッファ生成
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(ANIME_VERTEX) * mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bd.MiscFlags = 0;

			CRenderer::GetDevice()->CreateBuffer(&bd, NULL, &vertex_Buffer);

			D3D11_MAPPED_SUBRESOURCE ms;
			CRenderer::GetDeviceContext()->Map(vertex_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
			memcpy(ms.pData, temp_Vertex, sizeof(ANIME_VERTEX)* mesh->mNumVertices);
			CRenderer::GetDeviceContext()->Unmap(vertex_Buffer, 0);
		}


		// インデックスバッファ生成
		int index_Num;
		ID3D11Buffer* index_Beffer;
		{
			std::vector<int> index;
			for (int f = 0; f < mesh->mNumFaces; f++)
			{
				aiFace* face = &mesh->mFaces[f];
				for (unsigned int i = 0; i < face->mNumIndices; i++)
				{
					index.push_back(face->mIndices[i]);
				}
			}

			index_Num = index.size();
			unsigned short* temp_index = new unsigned short[index_Num];
			for (int i = 0; i < index_Num; i++)
			{
				temp_index[i] = (unsigned short)index[i];
			}


			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(unsigned short) * index_Num;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = temp_index;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &index_Beffer);

			delete[] temp_index;
		}

		MESH temp_mesh;
		temp_mesh.VertexBuffer = vertex_Buffer;
		temp_mesh.IndexBuffer = index_Beffer;
		temp_mesh.VertexNum = vertex_Num;
		temp_mesh.IndexNum = index_Num;
		temp_mesh.deform_Vertex = temp_DeformVertex;

		m_Meshes.push_back(temp_mesh);



		delete[] temp_Vertex;

		delete vertex;
	}
}

void FBXOLmodel::UnLoad()
{
	for (auto vertex : m_DeformVertex)
	{
		delete vertex;
	}
	m_DeformVertex.clear();


	for (int m = 0; m < m_MeshNum; m++)
	{
		m_Meshes[m].VertexBuffer->Release();
		m_Meshes[m].IndexBuffer->Release();
	}
	m_Meshes.clear();

	aiReleaseImport(m_Scene);

	delete m_Texture;

	m_Shader->Uninit();
	delete m_Shader;
}


void FBXOLmodel::Draw(XMMATRIX &Matrix)
{
	frame++;
	if (0 < m_Scene->mNumAnimations)
	{
		aiAnimation* animation = m_Scene->mAnimations[0];
		for (unsigned int c = 0; c < animation->mNumChannels; c++)
		{
			aiNodeAnim* nodeAnim = animation->mChannels[c];
			BONE* bone = &m_Bone[m_BoneIndex[nodeAnim->mNodeName.C_Str()]];

			int f = frame % nodeAnim->mNumRotationKeys;
			aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

			f = frame % nodeAnim->mNumPositionKeys;
			aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

			bone->AnimationMatrix = aiMatrixToMatrix(aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos));
		}
	}

	XMMATRIX Identity = XMMatrixIdentity();

	UpdateBoneMatrix(m_Scene->mRootNode, Identity);


	XMFLOAT4X4 bonef[512];

	for (int bf = 0; bf < 512; bf++)
	{
		bonef[bf] = XMFLOAT4X4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	for (int b = 0; b < m_BoneNum; b++)
	{
		XMStoreFloat4x4(&bonef[b], m_Bone[b].Matrix);
	}

	m_Shader->SetBoneMatrix(bonef);

	DrawMesh(m_Scene->mRootNode, Matrix);
}

//void FBXOLmodel::DrawShadow(XMMATRIX & Matrix)
//
//{
//	sFrame++;
//	if (0 < m_Scene->mNumAnimations)
//	{
//		aiAnimation* animation = m_Scene->mAnimations[0];
//		for (unsigned int c = 0; c < animation->mNumChannels; c++)
//		{
//			aiNodeAnim* nodeAnim = animation->mChannels[c];
//			BONE* bone = &m_Bone[m_BoneIndex[nodeAnim->mNodeName.C_Str()]];
//
//			int f = sFrame % nodeAnim->mNumRotationKeys;
//			aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;
//
//			f = sFrame % nodeAnim->mNumPositionKeys;
//			aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;
//
//			bone->AnimationMatrix = aiMatrixToMatrix(aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos));
//		}
//	}
//
//	XMMATRIX Identity = XMMatrixIdentity();
//
//	UpdateBoneMatrix(m_Scene->mRootNode, Identity);
//
//
//	XMFLOAT4X4 bonef[512];
//
//	for (int bf = 0; bf < 512; bf++)
//	{
//		bonef[bf] = XMFLOAT4X4(
//			1.0f, 0.0f, 0.0f, 0.0f,
//			0.0f, 1.0f, 0.0f, 0.0f,
//			0.0f, 0.0f, 1.0f, 0.0f,
//			0.0f, 0.0f, 0.0f, 1.0f);
//	}
//
//	for (int b = 0; b < m_BoneNum; b++)
//	{
//		XMStoreFloat4x4(&bonef[b], m_Bone[b].Matrix);
//	}
//
//	m_Shader->SetBoneMatrix(bonef);
//
//	DrawMeshShadow(m_Scene->mRootNode, Matrix);
//}



void FBXOLmodel::DrawMesh(aiNode * Node, XMMATRIX &Matrix)
{
	XMMATRIX world;
	world = XMMatrixTranspose(aiMatrixToMatrix(Node->mTransformation));
	world *= Matrix;

	XMFLOAT4X4 worldf;
	XMStoreFloat4x4(&worldf, world);

	m_Shader->SetWorldMatrix(&worldf);

	CCamera* camera = CManager::GetScene()->GetGameObject<CCamera>();

	for (int n = 0; n < Node->mNumMeshes; n++)
	{
		unsigned int m = Node->mMeshes[n];
		auto mesh = m_Meshes[m];

		XMFLOAT4X4 viewf;
		XMStoreFloat4x4(&viewf, camera->GetViewMatrix());
		m_Shader->SetViewMatrix(&viewf);

		XMFLOAT4X4 projf;
		XMStoreFloat4x4(&projf, camera->GetProjectionMatrix());
		m_Shader->SetProjectionMatrix(&projf);

		m_Shader->SetLight();
		m_Shader->Set();
	

		CRenderer::SetVertexBuffers(m_Meshes[m].VertexBuffer, sizeof(ANIME_VERTEX));
		CRenderer::SetIndexBuffer(m_Meshes[m].IndexBuffer);

		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		CRenderer::DrawIndexed(m_Meshes[m].IndexNum, 0, 0);
	}

	for (unsigned int i = 0; i < Node->mNumChildren; i++)
	{
		DrawMesh(Node->mChildren[i], world);
	}
}

//void FBXOLmodel::DrawMeshShadow(aiNode * Node, XMMATRIX & Matrix)
//
//{
//	XMMATRIX world;
//	world = XMMatrixTranspose(aiMatrixToMatrix(Node->mTransformation));
//	world *= Matrix;
//
//	XMFLOAT4X4 worldf;
//	XMStoreFloat4x4(&worldf, world);
//
//	m_Shader->SetWorldMatrix(&worldf);
//
//	//通常
//	/*XMMATRIX world;
//	aiQuaternion aiQuat = m_NodeRotation[Node->mName.C_Str()];
//	XMVECTOR quat = XMLoadFloat4(&XMFLOAT4(aiQuat.x, aiQuat.y, aiQuat.z, aiQuat.w));
//	world = XMMatrixRotationQuaternion(quat);
//	aiVector3D aiPos = m_NodePosition[Node->mName.C_Str()];
//	XMMATRIX tran = XMMatrixTranslation(aiPos.x, aiPos.y, aiPos.z);
//	world *= tran;
//	world *= Matrix;
//	XMFLOAT4X4 worldf;
//	XMStoreFloat4x4(&worldf, world);
//	m_Shader->SetWorldMatrix(&worldf);*/
//
//	for (int n = 0; n < Node->mNumMeshes; n++)
//	{
//		unsigned int m = Node->mMeshes[n];
//		auto mesh = m_Meshes[m];
//
//		Light* light = CManager::GetScene()->GetGameObject<Light>();
//
//		XMFLOAT4X4 viewf;
//		XMStoreFloat4x4(&viewf, light->GetViewMatrix());
//		m_Shader->SetViewMatrix(&viewf);
//
//		XMFLOAT4X4 projf;
//		XMStoreFloat4x4(&projf, light->GetProjectionMatrix());
//		m_Shader->SetProjectionMatrix(&projf);
//
//
//		m_Shader->Set();
//		m_Shader->SetLight();
//
//		CRenderer::SetVertexBuffers(m_Meshes[m].VertexBuffer, sizeof(ANIME_VERTEX));
//		CRenderer::SetIndexBuffer(m_Meshes[m].IndexBuffer);
//
//		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//		CRenderer::DrawIndexed(m_Meshes[m].IndexNum, 0, 0);
//	}
//
//	for (unsigned int i = 0; i < Node->mNumChildren; i++)
//	{
//		DrawMeshShadow(Node->mChildren[i], world);
//	}
//}

void FBXOLmodel::CreateBone(aiNode * Node)
{
	BONE bone;
	bone.Matrix = XMMatrixIdentity();
	bone.AnimationMatrix = XMMatrixIdentity();
	bone.OffsetMatrix = XMMatrixIdentity();

	m_BoneIndex[Node->mName.C_Str()] = m_BoneNum;

	m_Bone[m_BoneNum] = bone;

	m_BoneNum++;

	for (unsigned int i = 0; i < Node->mNumChildren; i++)
	{
		CreateBone(Node->mChildren[i]);
	}
}

void FBXOLmodel::UpdateBoneMatrix(aiNode * Node, XMMATRIX Matrix)
{
	BONE* bone = &m_Bone[m_BoneIndex[Node->mName.C_Str()]];
	XMMATRIX world;
	XMMATRIX matrix, aMatrix;

	matrix = Matrix;
	aMatrix = bone->AnimationMatrix;

	world = matrix * aMatrix;

	bone->Matrix = world * bone->OffsetMatrix;

	for (unsigned int i = 0; i < Node->mNumChildren; i++)
	{
		UpdateBoneMatrix(Node->mChildren[i], world);
	}
}