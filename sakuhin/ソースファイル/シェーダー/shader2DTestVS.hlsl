
cbuffer constantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;

	float4 CameraPosition;
}

void main(in float4 inPosition : POSITION0,
		  in float4	inDiffuse : COLOR0,
		  in float2 inTexcord : TEXCOORD0,
		  out float4 outPosition : SV_POSITION,
		  out float4 outDiffuse : COLOR0,
		  out float4 outTexcord : TEXCOORD0
	)
{
	outPosition = mul(inPosition, Projection);
	outDiffuse = inDiffuse;
	outTexcord.xy = inTexcord.xy;
}