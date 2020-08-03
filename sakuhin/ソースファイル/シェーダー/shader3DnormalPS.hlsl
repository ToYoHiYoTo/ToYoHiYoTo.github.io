cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 CameraPos;
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


Texture2D Texcoord[3] : register(t0);	//モデルのテクスチャ
SamplerState mySampler : register(s0);		//サンプラー

Texture2D tex1 : register(t3);      //トゥーンマップテクスチャー

void main(in float4 inPosition:SV_POSITION,
	in float4 inWorldPosition : POSITION1,
	in float4 inNormal : NORMAL0,
	in float4 inBinormal : BINORMAL0,
	in float4 inTangent : TANGENT0,
	in float4 inDiffuse : COLOR0,
	in float2 inTexcoord : TEXCOORD0,
	in float inLight : COLOR1,
	out float4 outDiffuse : SV_Target)
{
	//視差マッピング
	float3 eyeVec = inWorldPosition - CameraPos;
	eyeVec = normalize(eyeVec);

	matrix mat = { -inBinormal, inTangent, inNormal, float4(0,0,0,1) };
	matrix invmat = transpose(mat);

    float3 invEyev = mul(eyeVec, invmat);

	float hightMap = (Texcoord[1].Sample(mySampler, inTexcoord).r - 0.5) * 0.3;

	inTexcoord += invEyev.xy * hightMap;

	outDiffuse = Texcoord[2].Sample(mySampler, inTexcoord);	//テクスチャ


	//法線マッピング
	float4 normalMap = Texcoord[0].Sample(mySampler, inTexcoord);
	normalMap = (normalMap * 2.0) - 1.0;
	normalMap = (normalMap.x * -inBinormal) + (normalMap.y * inTangent) + (normalMap.z * inNormal);
	normalMap.w = 0.0;

	normalMap = normalize(normalMap);
	//inNormal = normalize(inNormal);


	//ライティング設定	
	float light = -dot(normalMap, Light.Direction);
	light = saturate(light);

	light = light * 0.5 + 0.5;
	light *= light;


	//スペキュラーの設定
	float3 r = reflect(Light.Direction.xyz, normalMap.xyz);
	r = normalize(r);

	float specler = -dot(eyeVec, r);
	specler = saturate(specler);
	specler = pow(specler, 40);


	//outDiffuse *= tex1.Sample(mySampler, float2(light, 0.0f));	//トゥーンテクスチャ
	outDiffuse *= inDiffuse * light;	//ライト
	outDiffuse += specler;	//反射光
	outDiffuse.a = 1.0;
}