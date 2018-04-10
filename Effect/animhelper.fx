struct VS_INPUTSKINNING
{
    float3 Pos : POSITION;
    float3 Norm : NORMAL;
	float2 Tex : TEXCOORD;
	float4 BoneIndices: BLENDINDICES;
	float4 BoneWeights: BLENDWEIGHT;
};

struct PS_INPUTSKINNING
{
	float4 Pos : SV_POSITION;
    float4 PosW : POSITION;
	float3 Norm : TEXCOORD0;
	float2 Tex : TEXCOORD1;
	
};