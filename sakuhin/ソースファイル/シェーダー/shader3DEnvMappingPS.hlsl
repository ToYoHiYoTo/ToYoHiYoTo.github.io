
cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    float3 cameraPos;
	
}

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

Texture2D tex1 : register(t2); //トゥーンマップテクスチャー

void main(in float4 inPosition : SV_POSITION,
	in float4 inWorldPosition : POSITION1,
	in float4 inNormal : NORMAL0,
	in float4 inDiffuse : COLOR0,
	in float2 inTexcoord : TEXCOORD0,
	out float4 outDiffuse : SV_Target)
{


    inNormal = normalize(inNormal);

    float3 eyeVec = inWorldPosition.xyz - cameraPos.xyz;
    eyeVec = normalize(eyeVec);
    
    float3 r = reflect(eyeVec.xyz, inNormal.xyz);
    r = normalize(r);
    
    float2 envTexCoord;
    envTexCoord.x = -r.x * 0.3 + 0.5;
    envTexCoord.y = -r.y * 0.3 + 0.5;
    
	//ライティング設定
    float light = -dot(inNormal.xyz, Light.Direction.xyz);

    light = saturate(light);
    light = light * 0.5 + 0.5;
    //light *= light;
    
    float3 rif = reflect(Light.Direction.xyz, inNormal.xyz);
    rif = normalize(rif);
    
    float specler = -dot(eyeVec, rif);
    specler = saturate(specler);
    specler = pow(specler, 40);
    
    light -= 0.01;
       

	//outDiffuse = inDiffuse * Light.Diffuse;
	//outDiffuse += inDiffuse * Light.Ambient;
    outDiffuse = inDiffuse * light;
    outDiffuse += specler;
  
    outDiffuse *= Texcoord[0].Sample(mySampler, inTexcoord);
    outDiffuse *= Texcoord[1].Sample(mySampler, envTexCoord);
    //outDiffuse *= tex1.Sample(mySampler, float2(light,0.0));
	
    outDiffuse.a = 1.0;
}