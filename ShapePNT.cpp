#include "StdAfx.h"
#include "ShapePNT.h"
#include "D3D10DeviceWrapper.h"
#include "DXWindow.h"
#include "TextureManager.h"

ShapePNT::ShapePNT(const tstring& TexFileName):
			m_pWorldViewProjectionVariable(0),
			m_pWorldVariable(0),
			m_pDiffuseMapVariabele(0),
			m_pTextureRV(0),
			m_pTechLinear(0)

{
	CreateEffect( _T("./Effect/PNTa.fx"));			//uit Graph3DObject
	GetEffectVariables();
	LoadTexture(TexFileName);
	DefineInputlayout();		
}
ShapePNT::~ShapePNT(void)
{
	//SAFE_RELEASE(m_pTextureRV);
}
void ShapePNT::GetEffectVariables()
{
	//Get Techniques
	m_pTechLinear = m_pEffect->GetTechniqueByName("TechLinear");

	//Set Default Technique
	m_pDefaultTechnique = m_pTechLinear;

	m_pWorldViewProjectionVariable = m_pEffect->GetVariableByName("gWVP")->AsMatrix();
	if(!m_pWorldViewProjectionVariable->IsValid())MessageBox(0,_T("Getting Effectvariable gWVP Failed"),_T("ERROR"),0);

	m_pWorldVariable			= m_pEffect->GetVariableByName("gWorld")->AsMatrix();
	if(!m_pWorldVariable->IsValid())MessageBox(0,_T("Getting Effectvariable gWorld Failed"),_T("ERROR"),0);

	m_pDiffuseMapVariabele = m_pEffect->GetVariableByName( "gDiffuseMap" )->AsShaderResource();
	if(!m_pDiffuseMapVariabele->IsValid())MessageBox(0,_T("Getting Effectvariable gDiffuseMap Failed"),_T("ERROR"),0);
}
void ShapePNT::LoadTexture(const tstring& TexFileName)
{
	if(TexFileName.length()!=0)
	{
		m_pTextureRV = TEXTUREMANAGER.GetTexture(TexFileName);

		//Set never changing params
		HR(m_pDiffuseMapVariabele->SetResource( m_pTextureRV ));
	}
}


void ShapePNT::DefineInputlayout()
{
    // Define the input layout
    D3D10_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },  
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 }, 
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 }
    };
    UINT numElements = sizeof(layout)/sizeof(layout[0]);

    // Create the input layout
    D3D10_PASS_DESC PassDesc;
	// Get the pass decriptor from the effect technique
    m_pDefaultTechnique->GetPassByIndex( 0 )->GetDesc( &PassDesc );
    HR(GETD3D10DEVICE->CreateInputLayout( layout, numElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &m_pVertexLayout ));

	//important set the size of the vertexbuffer, is used in draw base member function
	m_VertexBufferStride = sizeof(VertexPNT);
}
void  ShapePNT::UpdateEffectVariables()const
{
	//
    // Update Shader matrix variables in effect
    //
	//create a matrix containing the multiplication of world, view and projection matrix
	//short: m_pWorldViewProjectionVariable->SetMatrix((float*)&(m_World * m_View * m_Projection));
	//bit longer:
	D3DXMATRIX wvp;
	wvp = m_FinalWorld;
	wvp = wvp * m_View;
	wvp = wvp * m_Projection;
	m_pWorldViewProjectionVariable->SetMatrix((float*)&wvp);
	m_pWorldVariable->SetMatrix((float*)&m_FinalWorld);
	m_pDiffuseMapVariabele->SetResource(m_pTextureRV);
}
