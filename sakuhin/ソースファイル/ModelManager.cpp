#include "main.h"
#include "scene.h"
#include "renderer.h"
#include "manager.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "ModelManager.h"

void ModelManager::LoadModel()
{
	m_fModel = new FBXmodel();

	m_fModel->Load("data/MODEL/mixamo/SambaDancing2.fbx");

	//m_fModel->Load("data/MODEL/miku/miku.fbx");
}

void ModelManager::LoadOLModel()
{
	m_fOLMoldel = new FBXOLmodel();

	m_fOLMoldel->Load("data/MODEL/mixamo/SambaDancing2.fbx");

	//m_fOLMoldel->Load("data/MODEL/miku/miku.fbx");
}

void ModelManager::Unload()
{
	m_fModel->UnLoad();
	delete m_fModel;
}

void ModelManager::OLUnload()
{
	m_fOLMoldel->UnLoad();
	delete m_fOLMoldel;
}

void ModelManager::Update()
{

}

void ModelManager::Draw(XMMATRIX &Matrix)
{
	m_fModel->Draw(Matrix);
}

void ModelManager::OLDraw(XMMATRIX &Matrix)
{
	m_fOLMoldel->Draw(Matrix);
}

void ModelManager::DrawShadow(XMMATRIX & Matrix)
{
	m_fModel->DrawShadow(Matrix);
}

//void ModelManager::OLDrawShadow(XMMATRIX & Matrix)
//{
//	m_fOLMoldel->DrawShadow(Matrix);
//}
