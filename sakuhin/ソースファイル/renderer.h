#pragma once
#include <vector>




// 頂点構造体
struct VERTEX_3D
{
    XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT4 Diffuse;
	XMFLOAT2 TexCoord;
};


struct VERTEX_3D_NORMAL
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT3 Binormal;
	XMFLOAT3 Tangent;
	XMFLOAT4 Diffuse;
	XMFLOAT2 TexCoord;
};

struct DEFORM_VERTEX
{
	aiVector3D Position;
	aiVector3D DefromPosition;
	aiVector3D Normal;
	aiVector3D DefromNormal;

	std::string BoneName[4];
	float BoneWeight[4];

	int BoneNum = 0;

};

struct MESH
{
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;
	int VertexNum;
	int IndexNum;

	std::vector<DEFORM_VERTEX*> deform_Vertex;
};

struct BONE
{
	XMMATRIX Matrix          = XMMatrixIdentity();
	XMMATRIX AnimationMatrix = XMMatrixIdentity();
	XMMATRIX OffsetMatrix    = XMMatrixIdentity();
};

struct CONSTANT
{
	XMFLOAT4X4 WorldMatrix;
	XMFLOAT4X4 ViewMatrix;
	XMFLOAT4X4 ProjectionMatrix;

	XMFLOAT4	CameraPos;
};

struct BONE_DATA
{
	XMFLOAT4X4 BoneMatrix[512] = { {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f,
	}};
};



// 色構造体
struct COLOR
{
	COLOR(){}
	COLOR( float _r, float _g, float _b, float _a )
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	float r;
	float g;
	float b;
	float a;
};

// マテリアル構造体
struct MATERIAL
{
	COLOR		Ambient;
	COLOR		Diffuse;
	COLOR		Specular;
	COLOR		Emission;
	float		Shininess;
	float		Dummy[3];//16bit境界用
};


// マテリアル構造体
struct DX11_MODEL_MATERIAL
{
	MATERIAL		Material;
	class CTexture*	Texture;
};


// 描画サブセット構造体
struct DX11_SUBSET
{
	unsigned short	StartIndex;
	unsigned short	IndexNum;
	DX11_MODEL_MATERIAL	Material;
};

struct LIGHT
{
	XMFLOAT4	Direction;
	COLOR		Diffuse;
	COLOR		Ambient;

	XMFLOAT4X4	ViewMatrix;
	XMFLOAT4X4	ProjectionMatrix;
};




class CVertexBuffer;
class CIndexBuffer;
class CTexture;


class CRenderer
{
private:

	static D3D_FEATURE_LEVEL       m_FeatureLevel;

	static ID3D11Device*           m_D3DDevice;
	static ID3D11DeviceContext*    m_ImmediateContext;
	static IDXGISwapChain*         m_SwapChain;
	static ID3D11RenderTargetView* m_RenderTargetView;
	static ID3D11DepthStencilView* m_DepthStencilView;

	static ID3D11DepthStencilView* m_DepthStencilView_Shadow;

	static ID3D11ShaderResourceView* m_ShaderResoce_Shadow;

	static ID3D11Buffer*			m_ViewBuffer;
	static ID3D11Buffer*			m_ProjectionBuffer;


/*
	static XMMATRIX				m_WorldMatrix;
	static XMMATRIX				m_ViewMatrix;
	static XMMATRIX				m_ProjectionMatrix;
*/
	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;


public:
	static void Init();
	static void Uninit();
	static void Begin();
	static void End();

	static void BeginShadow();
	static ID3D11ShaderResourceView* GetShadow();

	static void SetShadowTexture(unsigned int Slot);

	static void SetDepthEnable(bool Enable);
	static void SetVertexBuffers( ID3D11Buffer* VertexBuffer );
	static void SetVertexBuffers(ID3D11Buffer* VertexBuffer, UINT stride);
	static void SetIndexBuffer( ID3D11Buffer* IndexBuffer );
	static void SetTexture( CTexture* Texture , int Index = 0);
	
	
	static void DrawIndexed( unsigned int IndexCount, unsigned int StartIndexLocation, int BaseVertexLocation );

	static ID3D11Device* GetDevice( void ){ return m_D3DDevice; }
	static ID3D11DeviceContext* GetDeviceContext( void ){ return m_ImmediateContext; }


 
	
	//bool CreateGeoOutline(const Object& obj, Mesh& out_mesh);
};
