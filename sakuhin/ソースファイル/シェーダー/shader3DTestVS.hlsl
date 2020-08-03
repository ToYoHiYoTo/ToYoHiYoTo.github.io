
cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
    
    float3 CameraPosition;
}



void main(in float4 inPosition : POSITION0,
	in float4 inNormal : NORMAL0,
	in float4 inDiffuse : COLOR0,
	in float2 inTexcord : TEXCOORD0,
	out float4 outPosition : SV_POSITION,
	out float4 outWorldPosition : POSITION1,
	out float4 outNormal : NORMAL0,
	out float4 outDiffuse : COLOR0,
	out float2 outTexcord : TEXCOORD0
	)
{
	//ポジション設定
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	outPosition = mul(inPosition, wvp);
	outWorldPosition = mul(inPosition, World);
	
	//法線設定
	inNormal.w = 0.0;
	outNormal = mul(inNormal, World);
	
	//その他設定
	outDiffuse = inDiffuse;
	outTexcord = inTexcord;
}