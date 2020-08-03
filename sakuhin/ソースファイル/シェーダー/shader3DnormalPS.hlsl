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


Texture2D Texcoord[3] : register(t0);	//���f���̃e�N�X�`��
SamplerState mySampler : register(s0);		//�T���v���[

Texture2D tex1 : register(t3);      //�g�D�[���}�b�v�e�N�X�`���[

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
	//�����}�b�s���O
	float3 eyeVec = inWorldPosition - CameraPos;
	eyeVec = normalize(eyeVec);

	matrix mat = { -inBinormal, inTangent, inNormal, float4(0,0,0,1) };
	matrix invmat = transpose(mat);

    float3 invEyev = mul(eyeVec, invmat);

	float hightMap = (Texcoord[1].Sample(mySampler, inTexcoord).r - 0.5) * 0.3;

	inTexcoord += invEyev.xy * hightMap;

	outDiffuse = Texcoord[2].Sample(mySampler, inTexcoord);	//�e�N�X�`��


	//�@���}�b�s���O
	float4 normalMap = Texcoord[0].Sample(mySampler, inTexcoord);
	normalMap = (normalMap * 2.0) - 1.0;
	normalMap = (normalMap.x * -inBinormal) + (normalMap.y * inTangent) + (normalMap.z * inNormal);
	normalMap.w = 0.0;

	normalMap = normalize(normalMap);
	//inNormal = normalize(inNormal);


	//���C�e�B���O�ݒ�	
	float light = -dot(normalMap, Light.Direction);
	light = saturate(light);

	light = light * 0.5 + 0.5;
	light *= light;


	//�X�y�L�����[�̐ݒ�
	float3 r = reflect(Light.Direction.xyz, normalMap.xyz);
	r = normalize(r);

	float specler = -dot(eyeVec, r);
	specler = saturate(specler);
	specler = pow(specler, 40);


	//outDiffuse *= tex1.Sample(mySampler, float2(light, 0.0f));	//�g�D�[���e�N�X�`��
	outDiffuse *= inDiffuse * light;	//���C�g
	outDiffuse += specler;	//���ˌ�
	outDiffuse.a = 1.0;
}