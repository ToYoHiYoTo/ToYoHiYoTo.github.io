#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "game_object.h"
#include <cmath>
#include "Mymath.h"
#include "camera.h"
#include "light.h"
#include "texture.h"
#include "shader.h"

#include "Skydome.h"

void SkyDome::Init()
{
	const int slice = divisionNum;
	const int stack = divisionNum * 2;
	const float degree = 360.0f / slice;
	const float degree2 = 360.0f / stack;
	m_Position = { 0.0f, 0.0f, 0.0f };
	m_Rotation = { 0.0f, 0.0f, 0.0f };
	m_Scale = { radius, radius, radius };

	VERTEX_3D* vertex;	// 頂点データ
	WORD* index;		// インデックスデータ

	/*---- 必要なデータの計算 ----*/
	int return_num = slice - 1;
	vertexNum_ = (slice + 1) * (stack + 1);
	indexNum_ = (4 + 2 * stack) * slice + return_num * 2;
	vertex = new VERTEX_3D[vertexNum_];
	index = new WORD[indexNum_];

	/*---- 頂点データの保存 ----*/
	for (int i = 0; i <= slice; i++) {
		for (int j = 0; j <= stack; j++) {
			vertex[i * (slice + 1) + j].Position = { cosf(ToRadian(degree * j)) * sinf(ToRadian(degree2 * i)) ,	// 頂点　X
													cosf(ToRadian(degree2 * i)),								// 頂点　Y
													sinf(ToRadian(degree * j)) * sinf(ToRadian(degree2 * i)) };	// 頂点　Z

			vertex[i * (slice + 1) + j].Normal = { 0.0f, 	// 法線　X
													1.0f,	// 法線　Y
													1.0f };	// 法線　Z

			vertex[i * (slice + 1) + j].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
			vertex[i * (slice + 1) + j].TexCoord = { ((float)1 / stack)*j*4.0f, -((float)1 / slice)*i*2.0f };
		}
	}

	/*---- インデックスデータの保存 ----*/
	for (int i = 0, k = 0; i < slice; i++) {
		for (int j = 0; j < (stack + 1) * 2; j++) {
			index[i * (stack + 1) * 2 + j + k] = (stack + 1) * (i + ((j + 1) % 2)) + j / 2;
		}
		if (i < slice - 1) {
			index[(i * (stack + 1) * 2 + (stack + 1) * 2 + k++)] = (stack + 1) * (i + 1) - 1;
			index[(i * (stack + 1) * 2 + (stack + 1) * 2 + k++)] = (stack + 1) * (i + 2);
		}
	}



	/*---- 頂点・インデックスバッファの生成 ----*/
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA sd;

	/*---- 頂点バッファ ----*/
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * vertexNum_;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;
	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &vertexBuffer_);

	/*---- インデックスバッファ ----*/
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * indexNum_;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	//bd.MiscFlags = 0;
	//bd.StructureByteStride = 0;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = index;
	//sd.SysMemPitch = 0;
	//sd.SysMemSlicePitch = 0;
	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &indexBuffer_);

	/*---- テクスチャの取得 ----*/
	texture_ = new CTexture();
	texture_->Load("data/TEXTURE/sky_bmp.tga");

	m_Shader = new CShader();
	m_Shader->Init("shader3DTestVS.cso", "shader3DTestPS.cso");

	delete[] vertex;
	delete[] index;
}

void SkyDome::Uninit(void)
{
	m_Shader->Uninit();
	delete m_Shader;

	texture_->Unload();
	delete texture_;

	vertexBuffer_->Release();
	vertexBuffer_ = nullptr;
	indexBuffer_->Release();
	indexBuffer_ = nullptr;
}

void SkyDome::Update(void)
{
}

void SkyDome::Draw(void)
{
	XMMATRIX world;
	world = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	world *= XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	
	CCamera* camera;
	camera = CManager::GetScene()->GetGameObject<CCamera>();

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

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
	CRenderer::GetDeviceContext()->IASetIndexBuffer(indexBuffer_, DXGI_FORMAT_R16_UINT, offset);
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CRenderer::SetTexture(texture_);
	CRenderer::GetDeviceContext()->DrawIndexed(indexNum_, 0, 0);

	CRenderer::GetDeviceContext()->RSSetState(baseRS);
}

//void SkyDome::DrawShadow()
//{
//	XMMATRIX world;
//	world = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
//	world *= XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
//	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
//
//	Light* light;
//	light = CManager::GetScene()->GetGameObject<Light>();
//
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
//	UINT stride = sizeof(VERTEX_3D);
//	UINT offset = 0;
//	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
//	CRenderer::GetDeviceContext()->IASetIndexBuffer(indexBuffer_, DXGI_FORMAT_R16_UINT, offset);
//	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
//	CRenderer::SetTexture(texture_);
//	CRenderer::GetDeviceContext()->DrawIndexed(indexNum_, 0, 0);
//
//	CRenderer::GetDeviceContext()->RSSetState(baseRS);
//}
