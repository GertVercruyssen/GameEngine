//=============================================================================
// PNT.fx by Bart Uyttenhove 2009
//
// 
//=============================================================================
SamplerState samLinear
{
    //Filter = MIN_MAG_MIP_LINEAR;
    Filter = MIN_MAG_MIP_POINT;
    //Filter = ANISOTROPIC ;
    AddressU = Wrap;// of Mirror of Clamp of Border
    AddressV = Wrap;// of Mirror of Clamp of Border
};
Texture2D gDiffuseMap;

BlendState alphablend
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};
BlendState noblend
{
	BlendEnable[0] = FALSE;
	SrcBlend = SRC_COLOR;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};

// deze licht variabele wordt de komende weken aangepast
float3 g_LightDir={0,-0.2f,1};

// deze variabelen worden één keer per frame geupdated.
// ze worden gegroepeerd om efficiëntie redenen.
cbuffer cbPerObject
{
	float4x4 gWVP; 
	float4x4 gWorld;
};

// we gebruiken deze structs omdat ze oa de leesbaarheid verhogen
// POSITION, COLOR en NORMAL zijn semantics en komen overeen met 
// de InputLayout
struct VS_IN
{
	float3 Pos : POSITION;
	float3 Norm : NORMAL;
    float2 Tex : TEXCOORD;
};
struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float3 Norm : TEXCOORD0;
	float2 Tex : TEXCOORD1;
};

VS_OUT VS(	VS_IN vIn)
{
	VS_OUT vOut = (VS_OUT)0;//fill with 0

	// multiply position of vertex and wvp matrix
	vOut.Pos = mul(float4(vIn.Pos, 1.0f), gWVP);
	
	// rotate normal with world matrix
	vOut.Norm = mul(vIn.Norm, (float3x3)gWorld);

	//Pass texture coordinates
	vOut.Tex = vIn.Tex;
	
	return vOut;
}

float4 PS(	VS_OUT pIn) : SV_Target
{
	//soms wordt door interpolatie de lengte van de vector aangepast.
   	pIn.Norm=-normalize(pIn.Norm);
   	
	//sample alpha value from texture
	float a = gDiffuseMap.Sample(samLinear,pIn.Tex).a;
	
	//calculate diffuse light factor, range is van 0 tot 1
	float s = max(dot(g_LightDir,pIn.Norm), 0.0f);
	
	//sample color value on position pIn.Tex
	float3 color = gDiffuseMap.Sample( samLinear,pIn.Tex ).rgb;
	//do not use s(diffuse light factor) on alpha
   	return float4(color*s,a);
}

technique10 NoBlend
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
 		SetBlendState(noblend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
   }
}
technique10 AlphaBlend
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
 		SetBlendState(alphablend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
   }
}
