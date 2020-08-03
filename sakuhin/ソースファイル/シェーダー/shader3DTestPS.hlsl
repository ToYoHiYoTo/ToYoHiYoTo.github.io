
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


Texture2D Texcoord : register(t0);	//���f���̃e�N�X�`��
SamplerState mySampler : register(s0);		//�T���v���[

Texture2D tex1 : register(t1);      //�g�D�[���}�b�v�e�N�X�`���[

void main(in float4 inPosition:SV_POSITION,
	in float4 inWorldPosition : POSITION1,
	in float4 inNormal : NORMAL0,
	in float4 inDiffuse : COLOR0,
	in float2 inTexcoord : TEXCOORD0,
	in float inLight : COLOR1,
	out float4 outDiffuse : SV_Target)
{
	
	
	inNormal = normalize(inNormal);
	

	//���C�e�B���O�ݒ�
	float light = -dot(inNormal, Light.Direction);
	
	light = saturate(light);
	light = light * 0.5 + 0.5;
	light *= light;
	
	outDiffuse = inDiffuse * light;

	outDiffuse = Texcoord.Sample(mySampler, inTexcoord);
	
	outDiffuse.a = 1.0;
}