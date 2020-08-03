#include "game_object.h"
#include "manager.h"
#include "scene.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "light.h"
#include "Meshfield.h"

VERTEX_3D* vertex;
WORD* index;

void Meshfield::Init()
{
	//フィールドのY座標の高さ(フィールドのX,Zを変えたときはここも変える)
	

	//頂点数計算
	VertexCnt = (field_X + 1)*(field_Z + 1);

	//インデックス数計算
	IndexCnt = ((field_X + 1)*(field_Z * 2)) + (field_Z - 1) * 2;

	//プリミティブ数計算
	PrimitiveCnt = (field_X * field_Z) * 2 + (field_Z - 1) * 4;


	vertex = new VERTEX_3D[VertexCnt];
	index = new WORD[IndexCnt];


	//バーテックスループ
	for (int i = 0; i < (field_Z + 1); i++)
	{
		for (int j = 0; j < (field_X + 1); j++)
		{
			vertex[i * (field_X + 1) + j].Position.x = -((field_X * length) / 2) + j * length;
			vertex[i * (field_X + 1) + j].Position.z = ((field_Z* length) / 2) - i * length;
			vertex[i * (field_X + 1) + j].Position.y = 0.0f;
			vertex[i * (field_X + 1) + j].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertex[i * (field_X + 1) + j].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[i * (field_X + 1) + j].TexCoord = XMFLOAT2(i, j);
		}
	}



	//法線ベクトルの外積計算
	for (int z = 1; z < field_Z; z++)
	{
		for (int x = 1; x < field_X; x++)
		{
			va.x = vertex[(z + 1) * (field_X + 1) + x].Position.x - vertex[(z - 1) * (field_X + 1) + x].Position.x;
			va.y = vertex[(z + 1) * (field_X + 1) + x].Position.y - vertex[(z - 1) * (field_X + 1) + x].Position.y;
			va.z = vertex[(z + 1) * (field_X + 1) + x].Position.z - vertex[(z - 1) * (field_X + 1) + x].Position.z;

			vb.x = vertex[z * (field_X + 1) + (x - 1)].Position.x - vertex[z * (field_X + 1) + (x + 1)].Position.x;
			vb.y = vertex[z * (field_X + 1) + (x - 1)].Position.y - vertex[z * (field_X + 1) + (x + 1)].Position.y;
			vb.z = vertex[z * (field_X + 1) + (x - 1)].Position.z - vertex[z * (field_X + 1) + (x + 1)].Position.z;

			//n = va X vb 外積計算
			n.x = va.y*vb.z - va.z*vb.y;
			n.y = va.z*vb.x - va.x*vb.z;
			n.z = va.x*vb.y - va.y*vb.x;

			//nの長さ
			float _length = sqrtf(n.x*n.x + n.y*n.y + n.z*n.z);

			//正規化(長さを1にする)
			n.x = n.x / _length;
			n.y = n.y / _length;
			n.z = n.z / _length;

			vertex[z * (field_X + 1) + x].Normal = n;
		}
	}


	//インデックスループ
	for (int i = 0; i < field_Z; i++)
	{
		for (int j = 0; j < (field_X + 1); j++)
		{
			index[((field_X + 2) * 2 * i + 2 * j) + 1] = (field_X + 1) * i + j;
			index[(field_X + 2) * 2 * i + 2 * j] = (field_X + 1) * (i + 1) + j;
		}
	}

	//縮退ポリゴンのインデックス
	for (int i = 0; i < field_Z - 1; i++)
	{
		index[(field_X + 2) * 2 * (i + 1) - 2] = field_X * (i + 1) + i;
		index[(field_X + 2) * 2 * (i + 1) - 1] = (field_X * (i + 1) + i) + (field_X + 2);
	}


	/*頂点バッファの生成*/
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * VertexCnt;	//数値は頂点数に合わせる
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//生成する種類
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);	//頂点バッファ生成


	/*インデックスバッファの生成*/
	D3D11_BUFFER_DESC id;
	ZeroMemory(&id, sizeof(id));
	id.Usage = D3D11_USAGE_DEFAULT;
	id.ByteWidth = sizeof(WORD) * IndexCnt;	//数値は頂点数に合わせる
	id.BindFlags = D3D11_BIND_INDEX_BUFFER;	//生成する種類
	id.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA isd;
	ZeroMemory(&sd, sizeof(isd));
	isd.pSysMem = index;

	CRenderer::GetDevice()->CreateBuffer(&id, &isd, &m_IndexBuffer);	//インデックスバッファ生成

	m_Shader = new CShader();
	m_Shader->Init("shader3DShadowVS.cso", "shader3DShadowPS.cso");

	/*テクスチャ読み込み*/
	m_Texture = new CTexture();
	m_Texture->Load("data/TEXTURE/grass.tga");

}

void Meshfield::Uninit()
{
	m_Shader->Uninit();
	delete m_Shader;

	if (m_VertexBuffer) {
		m_VertexBuffer->Release();
		m_VertexBuffer = NULL;
	}

	if (m_IndexBuffer) {
		m_IndexBuffer->Release();
		m_IndexBuffer = NULL;
	}

	delete vertex;

	delete index;

	delete m_Texture;
}

void Meshfield::Update()
{
}

void Meshfield::Draw()
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//インデックスバッファ設定
	CRenderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R16_UINT, offset);


	CCamera* camera;
	camera = CManager::GetScene()->GetGameObject<CCamera>();

	CRenderer::SetShadowTexture(0);

	CRenderer::SetTexture(m_Texture, 1);

	XMMATRIX world;
	world = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	world *= XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	world *= XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	XMFLOAT4X4 worldf;
	XMStoreFloat4x4(&worldf, world);
	m_Shader->SetWorldMatrix(&worldf);

	XMFLOAT4X4 viewf;
	XMStoreFloat4x4(&viewf, camera->GetViewMatrix());
	m_Shader->SetViewMatrix(&viewf);

	XMFLOAT4X4 projf;
	XMStoreFloat4x4(&projf, camera->GetProjectionMatrix());
	m_Shader->SetProjectionMatrix(&projf);


	LIGHT	lightConst;
	Light* light = CManager::GetScene()->GetGameObject<Light>();

	XMFLOAT4X4 lviewf;
	XMStoreFloat4x4(&lviewf, light->GetViewMatrix());
	lightConst.ViewMatrix = lviewf;

	XMFLOAT4X4 lprojf;
	XMStoreFloat4x4(&lprojf, light->GetProjectionMatrix());
	lightConst.ProjectionMatrix = lprojf;


	m_Shader->SetLight();
	m_Shader->SetLightMatrix(lightConst);
	m_Shader->Set();

	//CRenderer::SetTexture(m_Texture);

	// プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	CRenderer::GetDeviceContext()->DrawIndexed(IndexCnt,0, 0);
}


void Meshfield::DrawShadow()
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//インデックスバッファ設定
	CRenderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R16_UINT, offset);

	Light* light = CManager::GetScene()->GetGameObject<Light>();

	XMMATRIX world;
	world = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	world *= XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	world *= XMMatrixTranslation(0.0f, 0.0f, 0.0f);

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

	//CRenderer::SetTexture(m_Texture);

	// プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	CRenderer::GetDeviceContext()->DrawIndexed(IndexCnt, 0, 0);
}
