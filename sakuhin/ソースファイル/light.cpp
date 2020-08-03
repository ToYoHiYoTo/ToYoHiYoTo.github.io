#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "game_object.h"
#include "camera.h"
#include "input.h"
#include "DrawModel.h"
#include "light.h"

#define MOVE	(0.23f)

void Light::Init()
{
	

	m_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_Position = XMFLOAT3(0.01f, 23.0f,0.0f);
	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);


	m_Viewport.left = 0;
	m_Viewport.top = 0;
	m_Viewport.right = 2048;
	m_Viewport.bottom = 2048;

	MState.x = SCREEN_WIDTH / 2;
	MState.y = SCREEN_HEIGHT / 2;
	MState.moveAdd = 2;
	MState.lButton = false;
	MState.rButton = false;
}

void Light::Uninit()
{
}

void Light::Update()
{
	if (CInput::GetKeyPress('X'))
	{
		m_Position.x += MOVE;
	}

	if (CInput::GetKeyPress('Z'))
	{
		m_Position.x -= MOVE;
	}



	if (CInput::GetKeyPress('N'))
	{
		m_Position.z += MOVE;
	}

	if (CInput::GetKeyPress('B'))
	{
		m_Position.z -= MOVE;
	}
}

void Light::Draw()
{

}

void Light::DrawShadow()
{
	XMMATRIX	m_InvViewMatrix;

	// ビューポート設定
	D3D11_VIEWPORT dxViewport;
	dxViewport.Width = (float)(m_Viewport.right - m_Viewport.left);
	dxViewport.Height = (float)(m_Viewport.bottom - m_Viewport.top);
	dxViewport.MinDepth = 0.0f;
	dxViewport.MaxDepth = 1.0f;
	dxViewport.TopLeftX = (float)m_Viewport.left;
	dxViewport.TopLeftY = (float)m_Viewport.top;

	//CRenderer::GetDeviceContext()->RSSetViewports(1, &dxViewport);

	
	
	XMVECTOR vecLight,vecFocus,vecUp;

	vecLight = XMLoadFloat3(&m_Position);
	vecFocus = XMVectorSet(0.0f,0.0f,0.0f,0.0f);
	vecUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	

	// ビューマトリクス設定
	//m_InvViewMatrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_Rotation.x), XMConvertToRadians(m_Rotation.y), XMConvertToRadians(m_Rotation.z));

	//m_InvViewMatrix *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	m_InvViewMatrix = XMMatrixLookAtLH(vecLight, vecFocus, vecUp);

	XMVECTOR det;
	//m_ViewMatrix = XMMatrixInverse(NULL, m_InvViewMatrix);
	m_ViewMatrix = m_InvViewMatrix;

	

	// プロジェクションマトリクス設定
	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(90), dxViewport.Width / dxViewport.Height, 1.0f, 100.0f);
}
