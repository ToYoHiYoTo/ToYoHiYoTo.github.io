
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


Texture2D Texcoord[2] : register(t0); //モデルのテクスチャ

SamplerState mySampler : register(s0); //サンプラー
SamplerState shadowSampler : register(s1); //シャドウサンプラー

Texture2D tex1 : register(t2); //トゥーンマップテクスチャー




void main(in float4 inPosition : SV_POSITION,
	in float4 inWorldPosition : POSITION1,
	in float4 inNormal : NORMAL0,
	in float4 inDiffuse : COLOR0,
	in float4 inTexcoord : TEXCOORD0,
    in float2 innormalTex : TEXCOORD1,
	
	out float4 outDiffuse : SV_Target)
{
    
    inTexcoord /= inTexcoord.w;
    inTexcoord.x = inTexcoord.x * 0.5 + 0.5;
    inTexcoord.y = -inTexcoord.y * 0.5 + 0.5;
    
    outDiffuse.rgb = Texcoord[1].Sample(mySampler, innormalTex);
    
    float depth = Texcoord[0].Sample(shadowSampler, inTexcoord.xy);
    
    outDiffuse.a = 1.0;
    
    if (depth < inTexcoord.z - 0.001)
        outDiffuse.rgb *= 0.5;
    else
        outDiffuse.rgb *= 1.0;
    
	
}