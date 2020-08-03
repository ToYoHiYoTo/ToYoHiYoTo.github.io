
struct LIGHT
{
	float4 Direction;
	float4 Diffuse;
	float4 Ambient;
};



cbuffer LightBuffer : register(b1)
{
	LIGHT Light;
}


Texture2D Texcoord : register(t0);	//モデルのテクスチャ
SamplerState mySampler : register(s0);		//サンプラー

Texture2D tex1 : register(t1);      //トゥーンマップテクスチャー

void main(in float4 inPosition:SV_POSITION,
	in float4 inWorldPosition : POSITION1,
	in float4 inNormal : NORMAL0,
	in float4 inDiffuse : COLOR0,
	in float2 inTexcoord : TEXCOORD0,
	in float inLight : COLOR1,
	out float4 outDiffuse : SV_Target)
{
	
	
	inNormal = normalize(inNormal);
	

	//ライティング設定
	float light = -dot(inNormal, Light.Direction);
	
	light = saturate(light);
	light = light * 0.5 + 0.5;
	light *= light;
	
	outDiffuse = inDiffuse * light;

	outDiffuse = Texcoord.Sample(mySampler, inTexcoord);
	
	outDiffuse.a = 1.0;
}