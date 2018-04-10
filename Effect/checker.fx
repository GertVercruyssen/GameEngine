//=============================================================================
// checker.fx
//
// 
//=============================================================================
cbuffer cbNeverChanges
{
    float4 checkerColor1 = float4(0.2f,0.2f,0.2f,1);
    float4 checkerColor2 = float4(1.0f,0.070588f,0.203921f,1);
};

// deze variabelen worden één keer per frame geupdated.
// ze worden gegroepeerd om efficiëntie redenen.
cbuffer cbPerObject
{
	float4x4 gWVP; 
};

RasterizerState DisableCulling { CullMode = NONE; };

// we gebruiken deze structs omdat ze oa de leesbaarheid verhogen
// POSITION, COLOR en NORMAL zijn semantics en komen overeen met 
// de InputLayout
struct VS_IN
{
	float3 Pos		:POSITION;
};

struct VS_OUT
{
	float4 Pos		:SV_POSITION;
 	float4 Color	:COLOR;
};

VS_OUT VS(VS_IN vIn)
{
	VS_OUT vOut;

	// multiply position of vertex and wvp matrix
	vOut.Pos = mul(float4(vIn.Pos, 1.0f), gWVP);
	
	// Pass position to the shader
    vOut.Color = float4(vIn.Pos,1);

	return vOut;
}

float4 PS(VS_OUT pIn) : SV_Target
{
	int cX = (int)abs(pIn.Color.x);
	int cY = (int)abs(pIn.Color.y);
	int cZ = (int)abs(pIn.Color.z);
	int checker = (cX/2 + cY/2 + cZ/2)%2;
	return lerp(checkerColor1,checkerColor2,checker);
	//return checkerColor1 * checker + (1-checker) * checkerColor2;
}

technique10 CheckersTech
{
    pass P0
    {
    	SetRasterizerState( DisableCulling );
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}
