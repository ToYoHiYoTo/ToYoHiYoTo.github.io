#include "main.h"
#include "game_object.h"
#include "scene.h"
#include "renderer.h"
#include "manager.h"
#include "shader.h"
#include "model.h"
#include "texture.h"
#include "camera.h"
#include "ModelManager.h"
#include "DrawModel.h"
#include "OutlineModel.h"
#include "light.h"

void OutlineModel::Init()
{
	m_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Rotation = XMFLOAT3(0.0f, 180.0f, 0.0f);
	m_Scale = XMFLOAT3(0.04f, 0.04f, 0.04f);

	m_Shader = new CShader();
	m_Shader->Init("shaderAnimation3DVS.cso", "OutLine3DPS.cso");

	m_ModelManager = new ModelManager();
	m_ModelManager->LoadOLModel();
}
	
void OutlineModel::Uninit()
{
	m_ModelManager->OLUnload();
	

	m_Shader->Uninit();

	delete m_ModelManager;
	delete m_Shader;
}

void OutlineModel::Update()
{
}

void OutlineModel::Draw()
{
	CCamera* camera = CManager::GetScene()->GetGameObject<CCamera>();

	XMMATRIX world;
	world = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_Rotation.x), XMConvertToRadians(m_Rotation.y), XMConvertToRadians(m_Rotation.z));
	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	
	XMFLOAT4X4 worldf;
	XMStoreFloat4x4(&worldf, world);
	m_Shader->SetWorldMatrix(&worldf);

	XMFLOAT4X4 viewf;
	XMStoreFloat4x4(&viewf, camera->GetViewMatrix());
	m_Shader->SetViewMatrix(&viewf);

	XMFLOAT4X4 projf;
	XMStoreFloat4x4(&projf, camera->GetProjectionMatrix());
	m_Shader->SetProjectionMatrix(&projf);


	m_Shader->Set();

	// ラスタライザステート設定
	ID3D11RasterizerState* baseRS;
	CRenderer::GetDeviceContext()->RSGetState(&baseRS);

	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_FRONT;
	rd.DepthClipEnable = TRUE;
	rd.MultisampleEnable = FALSE;

	ID3D11RasterizerState *rs;
	CRenderer::GetDevice()->CreateRasterizerState(&rd, &rs);
	CRenderer::GetDeviceContext()->RSSetState(rs);

	m_ModelManager->OLDraw(world);

	CRenderer::GetDeviceContext()->RSSetState(baseRS);
}

//void OutlineModel::DrawShadow()
//{
//	Light* light = CManager::GetScene()->GetGameObject<Light>();
//
//	XMMATRIX world;
//	world = XMMatrixScaling(m_Scale.x - 0.01f, m_Scale.y - 0.01f, m_Scale.z - 0.01f);
//	world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_Rotation.x), XMConvertToRadians(m_Rotation.y), XMConvertToRadians(m_Rotation.z));
//	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
//	XMFLOAT4X4 worldf;
//	XMStoreFloat4x4(&worldf, world);
//	m_Shader->SetWorldMatrix(&worldf);
//
//	XMFLOAT4X4 viewf;
//	XMStoreFloat4x4(&viewf, light->GetViewMatrix());
//	m_Shader->SetViewMatrix(&viewf);
//
//	XMFLOAT4X4 projf;
//	XMStoreFloat4x4(&projf, light->GetProjectionMatrix());
//	m_Shader->SetProjectionMatrix(&projf);
//
//
//	m_Shader->Set();
//
//	// ラスタライザステート設定
//	ID3D11RasterizerState* baseRS;
//	CRenderer::GetDeviceContext()->RSGetState(&baseRS);
//
//	D3D11_RASTERIZER_DESC rd;
//	ZeroMemory(&rd, sizeof(rd));
//	rd.FillMode = D3D11_FILL_SOLID;
//	rd.CullMode = D3D11_CULL_FRONT;
//	rd.DepthClipEnable = TRUE;
//	rd.MultisampleEnable = FALSE;
//
//	ID3D11RasterizerState *rs;
//	CRenderer::GetDevice()->CreateRasterizerState(&rd, &rs);
//	CRenderer::GetDeviceContext()->RSSetState(rs);
//
//	m_ModelManager->OLDrawShadow(world);
//
//	CRenderer::GetDeviceContext()->RSSetState(baseRS);
//}
