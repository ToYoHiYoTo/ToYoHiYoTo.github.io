#include "main.h"
#include "game_object.h"
#include "scene.h"
#include "renderer.h"
#include "manager.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "light.h"

#include "DrawModel.h"

void DrawModel::Init()
{

	m_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Rotation = XMFLOAT3(0.0f, 180.0f, 0.0f);
	m_Scale = XMFLOAT3(0.04f, 0.04f, 0.04f);

	m_Shader = new AmodelShader();
	m_Shader->Init("shaderAnimation3DVS.cso", "shaderAnimation3DPS.cso");

	m_ModelManager = new ModelManager();
	m_ModelManager->LoadModel();

	m_Texture = new CTexture();
	m_Texture->Load("data/TEXTURE/toonshadow.tga");
}

void DrawModel::Uninit()
{
	

	m_Texture->Unload();
	delete m_Texture;

	m_ModelManager->Unload();
	delete m_ModelManager;
	
	m_Shader->Uninit();
	delete m_Shader;
}

void DrawModel::Update()
{
	m_ModelManager->Update();
}

void DrawModel::Draw()
{
	
	CRenderer::SetTexture(m_Texture, 2);

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
	
	m_Shader->SetLight();
	m_Shader->Set();
	

	// ラスタライザステート設定
	ID3D11RasterizerState* baseRS;
	CRenderer::GetDeviceContext()->RSGetState(&baseRS);

	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_BACK;
	rd.DepthClipEnable = TRUE;
	rd.MultisampleEnable = FALSE;

	ID3D11RasterizerState *rs;
	CRenderer::GetDevice()->CreateRasterizerState(&rd, &rs);
	CRenderer::GetDeviceContext()->RSSetState(rs);
	
	m_ModelManager->Draw(world);

	CRenderer::GetDeviceContext()->RSSetState(baseRS);
}

void DrawModel::DrawShadow()
{
	Light* light = CManager::GetScene()->GetGameObject<Light>();


	XMMATRIX world;
	world = XMMatrixScaling(m_Scale.x , m_Scale.y , m_Scale.z );
	world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_Rotation.x), XMConvertToRadians(m_Rotation.y), XMConvertToRadians(m_Rotation.z));
	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	XMFLOAT4X4 worldf;
	XMStoreFloat4x4(&worldf, world);
	m_Shader->SetWorldMatrix(&worldf);

	XMFLOAT4X4 viewf;
	XMStoreFloat4x4(&viewf, light->GetViewMatrix());
	m_Shader->SetViewMatrix(&viewf);

	XMFLOAT4X4 projf;
	XMStoreFloat4x4(&projf, light->GetProjectionMatrix());
	m_Shader->SetProjectionMatrix(&projf);

	m_Shader->Set();
	m_Shader->SetLight();

	// ラスタライザステート設定
	ID3D11RasterizerState* baseRS;
	CRenderer::GetDeviceContext()->RSGetState(&baseRS);

	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_BACK;
	rd.DepthClipEnable = TRUE;
	rd.MultisampleEnable = FALSE;

	ID3D11RasterizerState *rs;
	CRenderer::GetDevice()->CreateRasterizerState(&rd, &rs);
	CRenderer::GetDeviceContext()->RSSetState(rs);

	m_ModelManager->DrawShadow(world);

	CRenderer::GetDeviceContext()->RSSetState(baseRS);
}
