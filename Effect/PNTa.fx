//=============================================================================
// PNT.fx by Bart Uyttenhove 2009
//
// 
//=============================================================================
#include "lighthelper.fx"
#include "animhelper.fx"

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;// of Mirror of Clamp of Border
    AddressV = Wrap;// of Mirror of Clamp of Border
};
SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Clamp;// of Mirror of Clamp of Border
    AddressV = Clamp;// of Mirror of Clamp of Border
};
SamplerState samAnisotropic
{
	Filter = ANISOTROPIC ;
    AddressU = Border;// of Mirror of Clamp of Border
    AddressV = Border;// of Mirror of Clamp of Border
    BorderColor = float4(0.0f,0.0f,1.0f,1.0f);
};

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


Texture2D gDiffuseMap;

// deze licht variabele wordt de komende weken aangepast
float3 g_LightDir={0.2f,-1.0f,0.2f};

// deze variabelen worden één keer per frame geupdated.
// ze worden gegroepeerd om efficiëntie redenen.
cbuffer cbPerObject
{
	float4x4 gBoneMatrices[40];
	float4x4 gWVP; 
	float4x4 gWorld;
	Light gLight1;
	Light gLight2;
	float3 EyePos;
	
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
	float4 PosW : POSITION;
	float3 Norm : TEXCOORD0;
	float2 Tex : TEXCOORD1;
};

VS_OUT VS(	VS_IN vIn)
{
	VS_OUT vOut = (VS_OUT)0;//fill with 0
	vOut.PosW = mul(float4(vIn.Pos,1.0f),gWorld);
	// multiply position of vertex and wvp matrix
	vOut.Pos = mul(float4(vIn.Pos, 1.0f), gWVP);
	
	// rotate normal with world matrix
	vOut.Norm = mul(vIn.Norm, (float3x3)gWorld);

	//Pass texture coordinates
	vOut.Tex = vIn.Tex;
	return vOut;
}

PS_INPUTSKINNING VS_Anim(VS_INPUTSKINNING vIn)
{
	PS_INPUTSKINNING vOut = (PS_INPUTSKINNING)0;
	
	float4 vertex = float4(vIn.Pos,1);
	float4 tVertex = 0;
	float3 tNormal =0;
	float totalWeight =0;
	for (int i =0; i < 4 ;i++)
	{
		int index = round(vIn.BoneIndices[i])-1;
	
		if ( index > -1 )
		{
			float weight = vIn.BoneWeights[i];
			totalWeight+= weight;
			float4x4 boneMatrix = gBoneMatrices[index];
			tVertex += weight *  mul( boneMatrix,vertex);
			tNormal += weight *  mul( boneMatrix,float4(vIn.Norm,0)).xyz;
    	 }
	}
	tVertex.w = 1.0f;
	vOut.PosW = mul(tVertex,gWorld);
	// multiply position of vertex and wvp matrix
	vOut.Pos = mul(tVertex, gWVP);
		
	// rotate normal with world matrix
	vOut.Norm = mul(float4(tNormal,0), gWorld);
	
	//Pass texture coordinates
	vOut.Tex = vIn.Tex;
	
	return vOut;
}

float4 PSPoint(	VS_OUT pIn) : SV_Target
{
	//soms wordt door interpolatie de lengte van de vector aangepast.
   	pIn.Norm=-normalize(pIn.Norm);
   	
	//sample alpha value from texture
	float a = gDiffuseMap.Sample(samLinear,pIn.Tex).a;
	//discard pixel if texture alpha < 0.25f. Note that we do this test as soon as possible so that we can potentially exit the shader early, thereby skipping the rest of the shader code.
	clip(a-0.25f);
	
	//calculate diffuse light factor, range is van 0 tot 1
	float s = max(dot(g_LightDir,pIn.Norm), 0.0f);
	
	//sample color value on position pIn.Tex
	float3 color = gDiffuseMap.Sample( samPoint,pIn.Tex ).rgb;
	//do not use s(diffuse light factor) on alpha
   	return float4(color*s,a);
}
float4 PSLinear(	VS_OUT pIn) : SV_Target
{
	//soms wordt door interpolatie de lengte van de vector aangepast.
   	pIn.Norm=-normalize(pIn.Norm);
   	
	//sample alpha value from texture
	float a = gDiffuseMap.Sample(samLinear,pIn.Tex).a;
	//discard pixel if texture alpha < 0.25f. Note that we do this test as soon as possible so that we can potentially exit the shader early, thereby skipping the rest of the shader code.
	//clip(a-0.15f);
	
	//calculate diffuse light factor, range is van 0 tot 1
	float s = max(dot(g_LightDir,pIn.Norm), 0.0f);
	
	//sample color value on position pIn.Tex
	float3 color = gDiffuseMap.Sample( samLinear,pIn.Tex ).rgb;
	//do not use s(diffuse light factor) on alpha
	SurfaceInfo forLight1 = {pIn.PosW.xyz,pIn.Norm,float4(color,1),float4(1,1,1,1)};
	float3 colorFromLight1 = SpotLight(forLight1,gLight1,EyePos);
	float3 colorFromLight2 = SpotLight(forLight1,gLight2,EyePos);
   	
   	return float4( color*(s) + colorFromLight1 + colorFromLight2,a) + float4(0.1f,0.1f,0.1f,1);
}
float4 PSAnisotropic(	VS_OUT pIn) : SV_Target
{
	//soms wordt door interpolatie de lengte van de vector aangepast.
   	pIn.Norm=-normalize(pIn.Norm);
   	
	//sample alpha value from texture
	float a = gDiffuseMap.Sample(samLinear,pIn.Tex).a;
	//discard pixel if texture alpha < 0.25f. Note that we do this test as soon as possible so that we can potentially exit the shader early, thereby skipping the rest of the shader code.
	//clip(a-0.001);
	//calculate diffuse light factor, range is van 0 tot 1
	float s = max(dot(g_LightDir,pIn.Norm), 0.0f);
	
	//sample color value on position pIn.Tex
	float3 color = gDiffuseMap.Sample( samAnisotropic, pIn.Tex ).rgb;
	//do not use s(diffuse light factor) on alpha
   	return float4(color*s,a);
}

technique10 TechPoint
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSPoint() ) );
   }
}
technique10 TechLinear
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSLinear() ) );
   }
}

technique10 TechAnim
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_4_0, VS_Anim() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PSLinear() ) );
	}
}
technique10 TechAnisotropic
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSAnisotropic() ) );
  }
}

technique10 TechAlphaBlend
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSLinear() ) );
 		SetBlendState(alphablend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
   }
}
