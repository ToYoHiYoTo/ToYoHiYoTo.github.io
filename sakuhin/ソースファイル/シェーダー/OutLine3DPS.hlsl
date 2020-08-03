

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

	//その他設定	
	outDiffuse = float4(0.0f,0.0f,0.0f,1.0f);


}