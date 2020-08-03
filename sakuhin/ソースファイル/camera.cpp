
#include "main.h"
#include "renderer.h"
#include "game_object.h"
#include "camera.h"
#include "input.h"

#define CAMERA_MOVE (0.2f)
#define CAMERA_ROT (2.0f)

void CCamera::Init()
{
	m_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_Position = XMFLOAT3( 0.0f, 5.0f, -15.0f );
	m_Rotation = XMFLOAT3( 10.0f, 0.0f, 0.0f );


	m_Viewport.left = 0;
	m_Viewport.top = 0;
	m_Viewport.right = SCREEN_WIDTH;
	m_Viewport.bottom = SCREEN_HEIGHT;

	MState.x = SCREEN_WIDTH / 2;
	MState.y = SCREEN_HEIGHT / 2;
	MState.moveAdd = 2;
	MState.lButton = false;
	MState.rButton = false;
}


void CCamera::Uninit()
{


}


void CCamera::Update()
{
	if (CInput::GetKeyPress('W'))
	{
		m_Position.z += CAMERA_MOVE;
	}
	if (CInput::GetKeyPress('S'))
	{
		m_Position.z-= CAMERA_MOVE;
	}
	if (CInput::GetKeyPress('D'))
	{
		m_Position.x += CAMERA_MOVE;
	}
	if (CInput::GetKeyPress('A'))
	{
		m_Position.x -= CAMERA_MOVE;
	}


	if (CInput::GetKeyPress('Q'))
	{
		m_Position.y += CAMERA_MOVE;
	}
	if (CInput::GetKeyPress('E'))
	{
		m_Position.y -= CAMERA_MOVE;
	}


	if (CInput::GetKeyPress('K'))
	{
		m_Rotation.x += CAMERA_ROT;
	}
	if (CInput::GetKeyPress('I'))
	{
		m_Rotation.x -= CAMERA_ROT;
	}
	if (CInput::GetKeyPress('L'))
	{
		m_Rotation.y += CAMERA_ROT;
	}
	if (CInput::GetKeyPress('J'))
	{
		m_Rotation.y -= CAMERA_ROT;
	}
}


void CCamera::Draw()
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

	CRenderer::GetDeviceContext()->RSSetViewports(1, &dxViewport);

	// ビューマトリクス設定
	m_InvViewMatrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_Rotation.x), XMConvertToRadians(m_Rotation.y), XMConvertToRadians(m_Rotation.z));
	m_InvViewMatrix *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	XMVECTOR det;
	m_ViewMatrix = XMMatrixInverse(&det, m_InvViewMatrix);
	
	// プロジェクションマトリクス設定
	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(1.0f, dxViewport.Width / dxViewport.Height, 1.0f, 1000.0f);

}
