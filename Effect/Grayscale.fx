//=============================================================================
//// Shader uses position and texture
//=============================================================================
SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Mirror;
    AddressV = Mirror;
};

Texture2D gTexture;

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
	output.Pos = float4(input.Pos,1);
	output.Tex = input.Tex;

	return output;
}
//--------------------------------------------------------------------------------------
// Pixel XMeshShader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT_STRUCT input): SV_Target
{
	float4 Color = gTexture.Sample( samLinear, input.Tex ) ;
	Color += gTexture.Sample( samLinear, input.Tex+0.001);
	Color += gTexture.Sample( samLinear, input.Tex+0.002);
	Color += gTexture.Sample( samLinear, input.Tex+0.003);
	Color += gTexture.Sample( samLinear, input.Tex+0.004);
	Color += gTexture.Sample( samLinear, input.Tex+0.005);
	Color += gTexture.Sample( samLinear, input.Tex+0.006);
	Color += gTexture.Sample( samLinear, input.Tex+0.007);
	Color += gTexture.Sample( samLinear, input.Tex+0.008);
	Color += gTexture.Sample( samLinear, input.Tex+0.009);
	Color += gTexture.Sample( samLinear, input.Tex+0.010);
	Color += gTexture.Sample( samLinear, input.Tex+0.011);
	Color += gTexture.Sample( samLinear, input.Tex+0.012);
	Color += gTexture.Sample( samLinear, input.Tex+0.013);
	Color += gTexture.Sample( samLinear, input.Tex+0.014);
	Color = Color / 100;
	Color = Color / 3-(Color.r+Color.g+Color.b);
	Color +=gTexture.Sample( samLinear, input.Tex ) ;
    return Color;
}
technique10 Grayscale
{
    pass P0
    {          
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

