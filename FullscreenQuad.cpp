#include "StdAfx.h"
#include "FullscreenQuad.h"
#include "vertex.h"
#include "D3D10DeviceWrapper.h"
#include "DXWindow.h"
#include "TextureManager.h"
#include "EffectManager.h"

FullscreenQuad::FullscreenQuad(const tstring& TexFileName): m_pTextureMapVariabele(0), 
											m_pTextureRV(0),
											m_pVertexLayout(0), 
											m_pVertexBuffer(0), 
											m_pIndexBuffer(0), 
											m_pEffect(0), 
											m_pDefaultTechnique(0)
{
	CreateEffect( _T("./Effect/Grayscale.fx"));
	GetEffectVariables();
	LoadTexture(TexFileName);
	DefineInputlayout();
	//GetTechniques();
	BuildVertexBuffer();
	BuildIndexBuffer();
}
FullscreenQuad::~FullscreenQuad(void)
{
	//SAFE_DELETE(m_pTextureMapVariabele);
	//m_pTextureRV->Release();
	//m_pVertexLayout->Release();
	//m_pVertexBuffer->Release();
	//m_pIndexBuffer->Release();
}
void FullscreenQuad::Draw()const
{
	Draw(m_pDefaultTechnique);
}
void FullscreenQuad::Draw(const ID3D10EffectTechnique* pTechnique) const
{
	if(!pTechnique)
	{
		MessageBox(0,_T("No Technique"),_T("ERROR"),0);
		return;
	}
	UpdateEffectVariables();
    GETD3D10DEVICE->IASetInputLayout( m_pVertexLayout );

    UINT offset = 0;
    GETD3D10DEVICE->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &m_VertexBufferStride, &offset );
   
    GETD3D10DEVICE->IASetIndexBuffer( m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );

    GETD3D10DEVICE->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    D3D10_TECHNIQUE_DESC techDesc;
    (const_cast<ID3D10EffectTechnique*>(pTechnique))->GetDesc( &techDesc );
    for( UINT p = 0; p < techDesc.Passes; ++p )
    {
         (const_cast<ID3D10EffectTechnique*>(pTechnique))->GetPassByIndex( p )->Apply(0);
        GETD3D10DEVICE->DrawIndexed( m_NumIndices, 0, 0 ); 
		//GETD3D10DEVICE->Draw(m_NumIndices,0);
    }
}

void FullscreenQuad::BuildVertexBuffer()
{
	m_NumVertices = 4;
	// Create vertex array containing three elements in system memory
	VertexPT *pVertices = new VertexPT[4];
	int i = 0;
	//in origin
	pVertices[i++] = VertexPT( -1.0f,  1.0f, 0.0f ,0,0);
	//one unit on y axis (up)
	pVertices[i++] = VertexPT( 1.0f,  1.0f,  0.0f ,1,0);
	//one unit on x axis (right)
	pVertices[i++] = VertexPT( -1.0f,  -1.0f, 0.0f ,0,1);
	//one unit on x axis (right) and one unit on y axis (up)
	pVertices[i++] = VertexPT( 1.0f,  -1.0f, 0.0f , 1,1);

	//fill a buffer description to copy the vertexdata into graphics memory
	D3D10_BUFFER_DESC bd;
	bd.Usage = D3D10_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( VertexPT ) * m_NumVertices;
	bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pVertices;
	//create a ID3D10Buffer in graphics meemory containing the vertex info
	HR(GETD3D10DEVICE->CreateBuffer( &bd, &InitData, &m_pVertexBuffer ));

	delete[] pVertices;
}
void FullscreenQuad::BuildIndexBuffer()
{
	m_NumIndices = 6;
    // Create index buffer
    DWORD indices[] =
    {
        0,1,2,2,1,3,
    };

    D3D10_BUFFER_DESC bd;
    bd.Usage = D3D10_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( DWORD ) * 6;        
    bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
       
	D3D10_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = indices;
    HR(GETD3D10DEVICE->CreateBuffer( &bd, &InitData, &m_pIndexBuffer ));
}
void FullscreenQuad::GetEffectVariables()
{
	m_pDefaultTechnique = m_pEffect->GetTechniqueByName("Grayscale");

	m_pTextureMapVariabele = m_pEffect->GetVariableByName( "gTexture" )->AsShaderResource();
	if(!m_pTextureMapVariabele->IsValid())MessageBox(0,_T("Getting Effectvariable gTexture Failed"),_T("ERROR"),0);
}
void FullscreenQuad::DefineInputlayout()
{
	 // Define the input layout
    D3D10_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },  
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 }
    };
    UINT numElements = sizeof(layout)/sizeof(layout[0]);

    // Create the input layout
    D3D10_PASS_DESC PassDesc;
	// Get the pass decriptor from the effect technique
    m_pDefaultTechnique->GetPassByIndex( 0 )->GetDesc( &PassDesc );
    HR(GETD3D10DEVICE->CreateInputLayout( layout, numElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &m_pVertexLayout ));

	//important set the size of the vertexbuffer, is used in draw base member function
	m_VertexBufferStride = sizeof(VertexPT);
}
void FullscreenQuad::UpdateEffectVariables()const
{
	m_pTextureMapVariabele->SetResource(m_pTextureRV);
}
void FullscreenQuad::LoadTexture(const tstring& TexFileName)
{
	if(TexFileName.length()!=0)
	{
		m_pTextureRV = TEXTUREMANAGER.GetTexture(TexFileName);

		//Set never changing params
		HR(m_pTextureMapVariabele->SetResource( m_pTextureRV ));
	}
}
void FullscreenQuad::CreateEffect(const tstring& EffectFilename)
{
	m_pEffect = EFFECTMANAGER.GetEffect(EffectFilename);
}