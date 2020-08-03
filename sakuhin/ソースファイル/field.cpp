#include "game_object.h"
#include "manager.h"
#include "scene.h"
#include "shader.h"
#include "light.h"
#include "field.h"
#include "texture.h"
#include "camera.h"

void field::Init()
{
	VERTEX_3D vertex[4];
	vertex[0].Position = XMFLOAT3(-15.0f, 0.0f, 15.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 1.0f,0.0f);
	
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(15.0f, 0.0f, 15.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-15.0f, 0.0f, -15.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(15.0f, 0.0f, -15.0f);
	vertex[3].Normal = XMFLOAT3(0.0f,1.0f,0.0f);
	
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);


	m_nShader = new CShader();
	m_nShader->Init("shader3DShadowVS.cso", "shader3DShadowPS.cso");

	m_Texture = new CTexture();
	m_Texture->Load("data/TEXTURE/grass.tga");
}

void field::Uninit()
{
	m_nShader->Uninit();
	delete m_nShader;

	m_VertexBuffer->Release();

	m_Texture->Unload();
	delete m_Texture;
}

void field::Update()
{
}

void field::Draw()
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	CCamera* camera;
	camera = CManager::GetScene()->GetGameObject<CCamera>();

	CRenderer::SetShadowTexture(0);

	XMMATRIX world;
	world = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	world *= XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	world *= XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	XMFLOAT4X4 worldf;
	XMStoreFloat4x4(&worldf, world);
	m_nShader->SetWorldMatrix(&worldf);

	XMFLOAT4X4 viewf;
	XMStoreFloat4x4(&viewf, camera->GetViewMatrix());
	m_nShader->SetViewMatrix(&viewf);

	XMFLOAT4X4 projf;
	XMStoreFloat4x4(&projf, camera->GetProjectionMatrix());
	m_nShader->SetProjectionMatrix(&projf);

	LIGHT	lightConst;
	Light* light = CManager::GetScene()->GetGameObject<Light>();

	XMFLOAT4X4 lviewf;
	XMStoreFloat4x4(&lviewf, light->GetViewMatrix());
	lightConst.ViewMatrix = lviewf;

	XMFLOAT4X4 lprojf;
	XMStoreFloat4x4(&lprojf, light->GetProjectionMatrix());
	lightConst.ProjectionMatrix = lprojf;


	m_Shader->Set();
	m_Shader->SetLight();
	m_Shader->SetLightMatrix(lightConst);
	

	// プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	CRenderer::GetDeviceContext()->Draw(4, 0);
}

void field::DrawShadow()
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	Light* light;
	light = CManager::GetScene()->GetGameObject<Light>();

	//CRenderer::SetShadowTexture(0);

	XMMATRIX world;
	world = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	world *= XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	world *= XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	XMFLOAT4X4 worldf;
	XMStoreFloat4x4(&worldf, world);
	m_nShader->SetWorldMatrix(&worldf);

	XMFLOAT4X4 viewf;
	XMStoreFloat4x4(&viewf, light->GetViewMatrix());
	m_nShader->SetViewMatrix(&viewf);

	XMFLOAT4X4 projf;
	XMStoreFloat4x4(&projf, light->GetProjectionMatrix());
	m_nShader->SetProjectionMatrix(&projf);


	m_nShader->Set();

	m_nShader->SetLight();

	//CRenderer::SetTexture(m_Texture);

	// プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	CRenderer::GetDeviceContext()->Draw(4, 0);
}
