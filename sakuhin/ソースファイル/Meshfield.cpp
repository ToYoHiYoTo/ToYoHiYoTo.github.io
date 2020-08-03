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
	//�t�B�[���h��Y���W�̍���(�t�B�[���h��X,Z��ς����Ƃ��͂������ς���)
	

	//���_���v�Z
	VertexCnt = (field_X + 1)*(field_Z + 1);

	//�C���f�b�N�X���v�Z
	IndexCnt = ((field_X + 1)*(field_Z * 2)) + (field_Z - 1) * 2;

	//�v���~�e�B�u���v�Z
	PrimitiveCnt = (field_X * field_Z) * 2 + (field_Z - 1) * 4;


	vertex = new VERTEX_3D[VertexCnt];
	index = new WORD[IndexCnt];


	//�o�[�e�b�N�X���[�v
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



	//�@���x�N�g���̊O�όv�Z
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

			//n = va X vb �O�όv�Z
			n.x = va.y*vb.z - va.z*vb.y;
			n.y = va.z*vb.x - va.x*vb.z;
			n.z = va.x*vb.y - va.y*vb.x;

			//n�̒���
			float _length = sqrtf(n.x*n.x + n.y*n.y + n.z*n.z);

			//���K��(������1�ɂ���)
			n.x = n.x / _length;
			n.y = n.y / _length;
			n.z = n.z / _length;

			vertex[z * (field_X + 1) + x].Normal = n;
		}
	}


	//�C���f�b�N�X���[�v
	for (int i = 0; i < field_Z; i++)
	{
		for (int j = 0; j < (field_X + 1); j++)
		{
			index[((field_X + 2) * 2 * i + 2 * j) + 1] = (field_X + 1) * i + j;
			index[(field_X + 2) * 2 * i + 2 * j] = (field_X + 1) * (i + 1) + j;
		}
	}

	//�k�ރ|���S���̃C���f�b�N�X
	for (int i = 0; i < field_Z - 1; i++)
	{
		index[(field_X + 2) * 2 * (i + 1) - 2] = field_X * (i + 1) + i;
		index[(field_X + 2) * 2 * (i + 1) - 1] = (field_X * (i + 1) + i) + (field_X + 2);
	}


	/*���_�o�b�t�@�̐���*/
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * VertexCnt;	//���l�͒��_���ɍ��킹��
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//����������
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);	//���_�o�b�t�@����


	/*�C���f�b�N�X�o�b�t�@�̐���*/
	D3D11_BUFFER_DESC id;
	ZeroMemory(&id, sizeof(id));
	id.Usage = D3D11_USAGE_DEFAULT;
	id.ByteWidth = sizeof(WORD) * IndexCnt;	//���l�͒��_���ɍ��킹��
	id.BindFlags = D3D11_BIND_INDEX_BUFFER;	//����������
	id.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA isd;
	ZeroMemory(&sd, sizeof(isd));
	isd.pSysMem = index;

	CRenderer::GetDevice()->CreateBuffer(&id, &isd, &m_IndexBuffer);	//�C���f�b�N�X�o�b�t�@����

	m_Shader = new CShader();
	m_Shader->Init("shader3DShadowVS.cso", "shader3DShadowPS.cso");

	/*�e�N�X�`���ǂݍ���*/
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
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//�C���f�b�N�X�o�b�t�@�ݒ�
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

	// �v���~�e�B�u�g�|���W�ݒ�
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	CRenderer::GetDeviceContext()->DrawIndexed(IndexCnt,0, 0);
}


void Meshfield::DrawShadow()
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//�C���f�b�N�X�o�b�t�@�ݒ�
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

	// �v���~�e�B�u�g�|���W�ݒ�
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	CRenderer::GetDeviceContext()->DrawIndexed(IndexCnt, 0, 0);
}
