
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "shader.h"
#include "game_object.h"
#include "camera.h"
#include "polygon.h"
#include "texture.h"

#define POLYGON_W (SCREEN_WIDTH/3)
#define POLYGON_H (SCREEN_HEIGHT/3)


void CPolygon::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f,0.0f);

	vertex[1].Position = XMFLOAT3(POLYGON_W, 0.0f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(0.0f, POLYGON_H, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(POLYGON_W, POLYGON_H, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( VERTEX_3D ) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory( &sd, sizeof(sd) );
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer( &bd, &sd, &m_VertexBuffer );


	m_Shader = new CShader();
	m_Shader->Init( "shader2DTestVS.cso", "shader2DTestPS.cso" );

	m_Texture = new CTexture();
	m_Texture->Load("data/TEXTURE/grass.tga");

}


void CPolygon::Uninit()
{
	m_Shader->Uninit();
	delete m_Shader;

	m_VertexBuffer->Release();


	m_Texture->Unload();

	delete m_Texture;
}


void CPolygon::Update()
{


}


void CPolygon::Draw()
{
	//CRenderer::SetTexture(m_Texture);

	CRenderer::SetShadowTexture(0);
	
	// 頂点バッファ設定
	UINT stride = sizeof( VERTEX_3D );
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers( 0, 1, &m_VertexBuffer, &stride, &offset );

	XMFLOAT4X4 identity;
	DirectX::XMStoreFloat4x4(&identity, XMMatrixIdentity());

	XMFLOAT4X4 projection;
	DirectX::XMStoreFloat4x4(&projection, XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f));
	m_Shader->SetProjectionMatrix(&projection);

	
	m_Shader->Set();


	// プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	// ポリゴン描画
	CRenderer::GetDeviceContext()->Draw( 4, 0 );

}