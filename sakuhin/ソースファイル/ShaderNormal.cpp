#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "input.h"
#include "shader.h"
#include "ShaderNormal.h"

#include <io.h>

void ShaderNormal::Init(const char* VertexShader, const char* PixelShader)
{
	// ���_�V�F�[�_����
	{
		FILE* file;
		long int fsize;

		file = fopen(VertexShader, "rb");
		fsize = _filelength(_fileno(file));
		unsigned char* buffer = new unsigned char[fsize];
		fread(buffer, fsize, 1, file);
		fclose(file);

		//���_�V�F�[�_�[�쐬
		CRenderer::GetDevice()->CreateVertexShader(buffer, fsize, NULL, &m_VertexShader);


		// ���̓��C�A�E�g����
		{
			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "BINORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TANGENT",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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



	// �s�N�Z���V�F�[�_����
	{
		FILE* file;
		long int fsize;

		file = fopen(PixelShader, "rb");
		fsize = _filelength(_fileno(file));
		unsigned char* buffer = new unsigned char[fsize];
		fread(buffer, fsize, 1, file);
		fclose(file);

		//�s�N�Z���V�F�[�_�[�쐬
		CRenderer::GetDevice()->CreatePixelShader(buffer, fsize, NULL, &m_PixelShader);

		delete[] buffer;
	}

	//���C�g�����ݒ�
	lightPos = XMFLOAT4(0.0f, -1.0f, 0.0f, 1.0f);

	// �萔�o�b�t�@����
	{
		D3D11_BUFFER_DESC hBufferDesc;
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		hBufferDesc.MiscFlags = 0;
		hBufferDesc.StructureByteStride = sizeof(float);

		hBufferDesc.ByteWidth = sizeof(CONSTANT);
		CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, NULL, &m_ConstantBuffer);

		hBufferDesc.ByteWidth = sizeof(MATERIAL);
		CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, NULL, &m_MaterialBuffer);

		hBufferDesc.ByteWidth = sizeof(LIGHT);
		CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, NULL, &m_LightBuffer);

		m_Light.Direction = XMFLOAT4(lightPos.x, lightPos.y, lightPos.z,lightPos.w);
		XMVECTOR vec = XMLoadFloat4(&m_Light.Direction);
		vec = XMVector4Normalize(vec);
		XMStoreFloat4(&m_Light.Direction, vec);
		m_Light.Diffuse = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_Light.Ambient = COLOR(0.3f, 0.3f, 0.3f, 1.0f);
	}

	
}


void ShaderNormal::Uninit()
{
	if (m_ConstantBuffer)	m_ConstantBuffer->Release();

	if (m_VertexLayout)		m_VertexLayout->Release();
	if (m_VertexShader)		m_VertexShader->Release();
	if (m_PixelShader)		m_PixelShader->Release();

	if (m_MaterialBuffer)	m_MaterialBuffer->Release();
	if (m_LightBuffer)		m_LightBuffer->Release();
}


void ShaderNormal::Set()
{

	// �V�F�[�_�ݒ�
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


	// ���̓��C�A�E�g�ݒ�
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);


	// �萔�o�b�t�@�X�V
	CRenderer::GetDeviceContext()->UpdateSubresource(m_ConstantBuffer, 0, NULL, &m_Constant, 0, 0);

	CRenderer::GetDeviceContext()->UpdateSubresource(m_LightBuffer, 0, NULL, &m_Light, 0, 0);

	CRenderer::GetDeviceContext()->UpdateSubresource(m_MaterialBuffer, 0, NULL, &m_Material, 0, 0);

	XMFLOAT3 cameraPos = CManager::GetScene()->GetGameObject<CCamera>()->GetPosition();

	m_Constant.CameraPos.x = cameraPos.x;
	m_Constant.CameraPos.y = cameraPos.y;
	m_Constant.CameraPos.z = cameraPos.z;

	// �萔�o�b�t�@�ݒ�
	CRenderer::GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	CRenderer::GetDeviceContext()->PSSetConstantBuffers(0, 1, &m_ConstantBuffer);

	CRenderer::GetDeviceContext()->VSSetConstantBuffers(1, 1, &m_LightBuffer);
	CRenderer::GetDeviceContext()->PSSetConstantBuffers(1, 1, &m_LightBuffer);

	CRenderer::GetDeviceContext()->VSSetConstantBuffers(2, 1, &m_MaterialBuffer);
	CRenderer::GetDeviceContext()->PSSetConstantBuffers(2, 1, &m_MaterialBuffer);

}

void ShaderNormal::SetLight()
{
	m_Light.Direction = XMFLOAT4(lightPos.x, lightPos.y, lightPos.z, lightPos.w);

	lightPos.w = 1.0f;


	if (CInput::GetKeyPress('X'))
	{
		lightPos.x -= 0.1f;
	}

	if (CInput::GetKeyPress('Z'))
	{
		lightPos.x += 0.1f;
	}

	if (CInput::GetKeyTrigger('V'))
	{
		lightPos.y *= -1.0f;
	}


	if (CInput::GetKeyPress('N'))
	{
		lightPos.z -= 0.1f;
	}

	if (CInput::GetKeyPress('B'))
	{
		lightPos.z += 0.1f;
	}
}
