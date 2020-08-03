
Texture2D Texcord[2] : register(t0);	//モデルのテクスチャ
SamplerState mySampler : register(s0);		//サンプラー

void main(in float4 inPosition:SV_POSITION, 
		  in float4 inDiffuse : COLOR0, 
		  in float4 inTexcord : TEXCOORD0,
		  out float4 outDiffuse : SV_Target)
{
	
	outDiffuse = Texcord[0].Sample(mySampler, inTexcord.xy);

	outDiffuse.a = 1.0;

}