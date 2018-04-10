//=============================================================================
// colornormal.fx by Bart Uyttenhove 2009
//
// 
//=============================================================================

// deze licht variabele wordt de komende weken aangepast
float3 g_LightDir={0,-0.5f,1};

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
	float3 Pos		:POSITION;
	float4 Color	:COLOR;
	float3 Normal	:NORMAL;
};
struct VS_OUT
{
	float4 Pos		:SV_POSITION;
 	float4 Color	:COLOR;
	float3 Normal	:NORMAL;

};

VS_OUT VS(	VS_IN vIn)
{
	VS_OUT vOut;

	// multiply position of vertex and wvp matrix
	vOut.Pos = mul(float4(vIn.Pos, 1.0f), gWVP);
	
	// Pass vertex color into the pixel shader.
    vOut.Color = vIn.Color;

	// rotate normal with world matrix
	vOut.Normal = mul(vIn.Normal, (float3x3)gWorld);

	return vOut;
}

float4 PS(	VS_OUT pIn) : SV_Target
{
	//soms wordt door interpolatie de lengte van de vector aangepast.
   	pIn.Normal=-normalize(pIn.Normal);
   	
	//calculate diffuse light factor, range is van 0 tot 1
	float s = max(dot(g_LightDir,pIn.Normal), 0.0f);
	
   	return float4(s*pIn.Color.rgb, 1.0);
}

technique10 ColorNormalTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}
