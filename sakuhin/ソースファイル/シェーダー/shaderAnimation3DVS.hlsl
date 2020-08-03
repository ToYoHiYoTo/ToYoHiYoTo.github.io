
cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 cameraPos;
	
}

struct LIGHT
{
    float4 Direction;
    float4 Diffuse;
    float4 Ambient;
    
    matrix View;
    matrix Projection;
};

cbuffer LightBuffer : register(b1)
{
    LIGHT Light;
}

cbuffer BoneBuffer : register(b3)
{
    matrix Bone[512];
}




void main(in float4 inPosition : POSITION0,
	in float4 inNormal : NORMAL0,
	in float4 inDiffuse : COLOR0,
	in float2 inTexcord : TEXCOORD0,
	in uint4 inBoneIndex : BONE_INDEX0,
	in float4 inBoneWeight : BONEWEIGHT0,

	out float4 outPosition : SV_POSITION,
	out float4 outWorldPosition : POSITION1,
	out float4 outNormal : NORMAL0,
	out float4 outDiffuse : COLOR0,
	out float2 outTexcord : TEXCOORD0,
    out float4 outShadowTex : TEXCOORD1
)
{

	matrix skinTransform = matrix(1.0f, 0.0f, 0.0f, 0.0f,
									  0.0f, 1.0f, 0.0f, 0.0f,
									  0.0f, 0.0f, 1.0f, 0.0f,
									  0.0f, 0.0f, 0.0f, 1.0f);

	//スキニング処理
    skinTransform =  Bone[inBoneIndex.x] * inBoneWeight.x;
    skinTransform += Bone[inBoneIndex.y] * inBoneWeight.y;
    skinTransform += Bone[inBoneIndex.z] * inBoneWeight.z;
    skinTransform += Bone[inBoneIndex.w] * inBoneWeight.w;

	
	//ポジション設定
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
    
    outPosition = mul(inPosition, skinTransform);
    outPosition = mul(outPosition, wvp);
	outWorldPosition = mul(inPosition, World);
	

	//法線ベクトル変換
	inNormal.w = 0.0;
	float4 transNormal = mul(inNormal, skinTransform);
	outNormal = mul(transNormal, World);
    
    //ポジション設定
    matrix lightwvp;
    lightwvp = mul(World, Light.View);
    lightwvp = mul(lightwvp, Light.Projection);
    
    outShadowTex = mul(inPosition, skinTransform);
    outShadowTex = mul(outShadowTex, lightwvp);
    
    	//結果設定
    outDiffuse = inDiffuse;
    outTexcord = inTexcord;
}