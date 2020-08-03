#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "input.h"
#include "shader.h"
#include "AmodelShader.h"
#include "camera.h"


#include <io.h>

void AmodelShader::Init(const char * VertexShader, const char * PixelShader)
{

	// 頂点シェーダ生成
	{
		FILE* file;
		long int fsize;

		file = fopen(VertexShader, "rb");
		fsize = _filelength(_fileno(file));
		unsigned char* buffer = new unsigned char[fsize];
		fread(buffer, fsize, 1, file);
		fclose(file);

		//頂点シェーダー作成
		CRenderer::GetDevice()->CreateVertexShader(buffer, fsize, NULL, &m_VertexShader);


		// 入力レイアウト生成
		{
			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

				{ "BONE_INDEX",   0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "BONEWEIGHT",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

			};
			UINT numElements = ARRAYSIZE(layout);

			CRenderer::GetDevice()->CreateInputLayout(layout,
				numElements,
				buffer,
				fsize,
				&m_VertexLayout);
		}

		delete[] buffer;
	}



	// ピクセルシェーダ生成
	{
		FILE* file;
		long int fsize;

		file = fopen(PixelShader, "rb");
		fsize = _filelength(_fileno(file));
		unsigned char* buffer = new unsigned char[fsize];
		fread(buffer, fsize, 1, file);
		fclose(file);

		//ピクセルシェーダー作成
		CRenderer::GetDevice()->CreatePixelShader(buffer, fsize, NULL, &m_PixelShader);

		delete[] buffer;
	}


	

	// 定数バッファ生成
	{
		D3D11_BUFFER_DESC hBufferDesc;
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		hBufferDesc.MiscFlags = 0;
		hBufferDesc.StructureByteStride = sizeof(float);

		hBufferDesc.ByteWidth = sizeof(CONSTANT);
		CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, NULL, &m_ConstantBuffer);
		

		hBufferDesc.ByteWidth = sizeof(LIGHT);
		CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, NULL, &m_LightBuffer);
		
		lightDir = XMFLOAT4(0.0f, -10.0f, 0.0f, 0.0f);

		m_Light.Direction = XMFLOAT4(lightDir.x, lightDir.y, lightDir.z, lightDir.w);
		XMVECTOR vec = XMLoadFloat4(&m_Light.Direction);
		vec = XMVector4Normalize(vec);
		XMStoreFloat4(&m_Light.Direction, vec);
		m_Light.Diffuse = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_Light.Ambient = COLOR(1.0f, 1.0f, 1.0f, 1.0f);


		hBufferDesc.ByteWidth = sizeof(MATERIAL);
		CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, NULL, &m_MaterialBuffer);
		m_Material.Diffuse = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_Material.Ambient = COLOR(1.0f, 1.0f, 1.0f, 1.0f);


		hBufferDesc.ByteWidth = sizeof(BONE_DATA);
		CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, NULL, &m_BoneBuffer);
		
	}

}

void AmodelShader::Uninit()
{
	if (m_ConstantBuffer)	m_ConstantBuffer->Release();

	if (m_VertexLayout)		m_VertexLayout->Release();
	if (m_VertexShader)		m_VertexShader->Release();
	if (m_PixelShader)		m_PixelShader->Release();
	if (m_LightBuffer)		m_LightBuffer->Release();
	if (m_MaterialBuffer)	m_MaterialBuffer->Release();
	if (m_BoneBuffer)	m_BoneBuffer->Release();
}

void AmodelShader::Set()
{

	// シェーダ設定
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


	// 入力レイアウト設定
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);


	// 定数バッファ更新
	CRenderer::GetDeviceContext()->UpdateSubresource(m_ConstantBuffer, 0, NULL, &m_Constant, 0, 0);

	CRenderer::GetDeviceContext()->UpdateSubresource(m_LightBuffer, 0, NULL, &m_Light, 0, 0);

	CRenderer::GetDeviceContext()->UpdateSubresource(m_MaterialBuffer, 0, NULL, &m_Material, 0, 0);

	CRenderer::GetDeviceContext()->UpdateSubresource(m_BoneBuffer, 0, NULL, &m_BoneData, 0, 0);

	// 定数バッファ設定
	CRenderer::GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	CRenderer::GetDeviceContext()->PSSetConstantBuffers(0, 1, &m_ConstantBuffer);

	CRenderer::GetDeviceContext()->VSSetConstantBuffers(1, 1, &m_LightBuffer);
	CRenderer::GetDeviceContext()->PSSetConstantBuffers(1, 1, &m_LightBuffer);

	CRenderer::GetDeviceContext()->VSSetConstantBuffers(2, 1, &m_MaterialBuffer);
	CRenderer::GetDeviceContext()->PSSetConstantBuffers(2, 1, &m_MaterialBuffer);

	CRenderer::GetDeviceContext()->VSSetConstantBuffers(3, 1, &m_BoneBuffer);
	CRenderer::GetDeviceContext()->PSSetConstantBuffers(3, 1, &m_BoneBuffer);

	XMFLOAT3 cameraPos = CManager::GetScene()->GetGameObject<CCamera>()->GetPosition();

	m_Constant.CameraPos.x = cameraPos.x;
	m_Constant.CameraPos.y = cameraPos.y;
	m_Constant.CameraPos.z = cameraPos.z;
}


void AmodelShader::SetLight()
{
	m_Light.Direction = XMFLOAT4(lightDir.x, lightDir.y, lightDir.z, lightDir.w);

	lightDir.w = 1.0f;


	if (CInput::GetKeyPress('X'))
	{
		lightDir.x -= 0.1f;
	}

	if (CInput::GetKeyPress('Z'))
	{
		lightDir.x += 0.1f;
	}


	if (CInput::GetKeyTrigger('V'))
	{
		lightDir.y *= -1.0f;
	}


	if (CInput::GetKeyPress('N'))
	{
		lightDir.z -= 0.1f;
	}

	if (CInput::GetKeyPress('B'))
	{
		lightDir.z += 0.1f;
	}
}
