
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


Texture2D Texcoord[2] : register(t0);	//モデルのテクスチャ
Texture2D tex1 : register(t2); //トゥーンマップテクスチャー

SamplerState mySampler : register(s0);		//サンプラー
SamplerState shadowSampler : register(s1); //シャドウサンプラー



void main(in float4 inPosition:SV_POSITION,
	in float4 inWorldPosition : POSITION1,
	in float4 inNormal : NORMAL0,
	in float4 inDiffuse : COLOR0,
	in float2 inTexcoord : TEXCOORD0,
    in float4 inShadowTex : TEXCOORD1,

	out float4 outDiffuse : SV_Target)
{
    inNormal = normalize(inNormal);
    
    //ライティング設定
    float light = -dot(inNormal.xyz, Light.Direction.xyz);
    light = saturate(light);
    light = light * 0.5 + 0.5;
    light *= light;
    
    light -= 0.01;
   
    inShadowTex /= inShadowTex.w;
    inShadowTex.x = inShadowTex.x * 0.5 + 0.5;
    inShadowTex.y = -inShadowTex.y * 0.5 + 0.5;
    
    outDiffuse.rgb = Texcoord[1].Sample(mySampler, inTexcoord);
    
    outDiffuse.rgb *= tex1.Sample(mySampler, float2(light, 0.0f));
    
    float depth = Texcoord[0].Sample(shadowSampler, inShadowTex.xy);
    
    outDiffuse.a = 1.0;
    
    if (depth < inShadowTex.z - 0.0005 && light > 0.33)
    {
        outDiffuse.rgb *= 0.5;
    }
    else
    {
        outDiffuse.rgb *= 1.0;
    }
      
    

}