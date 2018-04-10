//=============================================================================
//// Shader uses position and normal lighting
//=============================================================================
SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Mirror;
    AddressV = Mirror;
};
Texture2D g_TextureDiffuse;

cbuffer cbChangesEveryFrame
{
	matrix g_WorldViewProj;
}

struct VS_INPUT_STRUCT
{
	float3 Pos : POSITION;
    float2 Tex : TEXCOORD;

};
struct PS_INPUT_STRUCT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD1;

};
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT_STRUCT VS( VS_INPUT_STRUCT input )
{
	PS_INPUT_STRUCT output = (PS_INPUT_STRUCT)0;
	output.Pos = mul( float4(input.Pos,1), g_WorldViewProj );
	output.Tex = input.Tex;

	return output;
}
//--------------------------------------------------------------------------------------
// Pixel XMeshShader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT_STRUCT input): SV_Target
{
	float4 color = g_TextureDiffuse.Sample( samLinear, input.Tex ) ;
   	return color;
}
technique10 Render
{
    pass P0
    {          
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

