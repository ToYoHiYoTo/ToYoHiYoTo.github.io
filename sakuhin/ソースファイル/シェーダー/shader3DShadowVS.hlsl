
cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    
    float3 CameraPosition;
}


struct LIGHT
{
    float4 Direction;
    float4 Diffuse;
    float4 Ambient;
    
    matrix View;
    matrix Projection;
};



cbuffer LightBuffer : register(b1)
{
    LIGHT Light;
}



void main(in float4 inPosition : POSITION0,
	in float4 inNormal : NORMAL0,
	in float4 inDiffuse : COLOR0,
	in float2 inTexcord : TEXCOORD0,
	out float4 outPosition : SV_POSITION,
	out float4 outWorldPosition : POSITION1,
	out float4 outNormal : NORMAL0,
	out float4 outDiffuse : COLOR0,
	out float4 outTexcord : TEXCOORD0,
    out float2 outnormalTex : TEXCOORD1
	)
{
    //�|�W�V�����ݒ�
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    outPosition = mul(inPosition, wvp);
    outWorldPosition = mul(inPosition, World);
	
    //�@���ݒ�
    inNormal.w = 0.0;
    outNormal = mul(inNormal, World);
	
	//���̑��ݒ�
    outDiffuse = inDiffuse;
    outnormalTex = inTexcord;
    
	//���C�g�|�W�V�����ݒ�
    matrix lightwvp;
    lightwvp = mul(World, Light.View);
    lightwvp = mul(lightwvp, Light.Projection);
    
    outTexcord = mul(inPosition, lightwvp);
}