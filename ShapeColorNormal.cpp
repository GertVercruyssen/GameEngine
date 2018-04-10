#include "StdAfx.h"
#include "ShapeColorNormal.h"
#include "D3D10DeviceWrapper.h"
#include "DXWindow.h"


ShapeColorNormal::ShapeColorNormal():
			m_pWorldViewProjectionVariable(0),
			m_pWorldVariable(0)

{
	CreateEffect( _T("./Effect/ColorNormal.fx"));			//uit Graph3DObject
	GetEffectVariables();
	DefineInputlayout();		
}

ShapeColorNormal::~ShapeColorNormal(void)
{
}

void ShapeColorNormal::GetEffectVariables()
{
	m_pDefaultTechnique = m_pEffect->GetTechniqueByName("ColorNormalTech");
	m_pWorldViewProjectionVariable = m_pEffect->GetVariableByName("gWVP")->AsMatrix();
	m_pWorldVariable = m_pEffect->GetVariableByName("gWorld")->AsMatrix();
}

void ShapeColorNormal::DefineInputlayout()
{
    // Define the input layout
    D3D10_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },  
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D10_INPUT_PER_VERTEX_DATA, 0 }, 
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 }, 
    };
    UINT numElements = sizeof(layout)/sizeof(layout[0]);

    // Create the input layout
    D3D10_PASS_DESC PassDesc;
	// Get the pass decriptor from the effect technique
    m_pDefaultTechnique->GetPassByIndex( 0 )->GetDesc( &PassDesc );
    HR(GETD3D10DEVICE->CreateInputLayout( layout, numElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &m_pVertexLayout ));

	//important set the size of the vertexbuffer, is used in draw base member function
	m_VertexBufferStride = sizeof(VertexPosColNormal);
}
void  ShapeColorNormal::UpdateEffectVariables()const
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
}

