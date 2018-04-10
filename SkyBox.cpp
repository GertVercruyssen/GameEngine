#include "StdAfx.h"
#include "SkyBox.h"
#include "ShapeColor.h"
#include "D3D10DeviceWrapper.h"
#include "DXWindow.h"
#include "TextureManager.h"
#include "EffectManager.h"
//vertex structs
#include "vertex.h"
#include "float3.h"

SkyBox::SkyBox(const tstring& filename) :
			m_pEffect(NULL),
			m_pTechnique(NULL),
			m_pVertexLayout(NULL),
			m_pVertexBuffer(NULL),
			m_pIndexBuffer(NULL),
			m_pViewProjectionVariable(NULL),
			m_pWorldVariable(NULL),
			m_TexFileName(filename),
			m_pCubeMap(NULL)
{
	Init();
}

SkyBox::~SkyBox(void)
{
	Cleanup();
}
bool SkyBox::Init()
{
	LoadTexture();
	CreateEffect();
	DefineInputlayout();
	BuildVertexBuffer();
	BuildIndexBuffer();
	InitializeMatrices();
	return true;
}
void SkyBox::LoadTexture()
{
	m_pCubeMap = TEXTUREMANAGER.GetCubeMap(m_TexFileName);
}
void SkyBox::CreateEffect()//uit Graph3DObject
{
	//Create the effect
	m_pEffect = EFFECTMANAGER.GetEffect(_T("./Effect/SkyBox.fx"));
	// Obtain the technique
	m_pTechnique = m_pEffect->GetTechniqueByName("Render");
	// Obtain the variables
    m_pViewProjectionVariable = m_pEffect->GetVariableByName( "g_ViewProj" )->AsMatrix();
	m_pCubeMapVariable = (ID3D10EffectShaderResourceVariable*) m_pEffect->GetVariableByName("gCubeMap");
    m_pWorldVariable = m_pEffect->GetVariableByName( "g_World" )->AsMatrix();
    //m_pTextureDiffuseVariable = m_pEffect->GetVariableByName( "g_TextureDiffuse" )->AsShaderResource();
}
void SkyBox::DefineInputlayout()
{
    // Define the input layout
	D3D10_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = sizeof(layout)/sizeof(layout[0]);

    // Create the input layout
    D3D10_PASS_DESC PassDesc;
    m_pTechnique->GetPassByIndex( 0 )->GetDesc( &PassDesc );
	HR(GETD3D10DEVICE->CreateInputLayout( layout, numElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &m_pVertexLayout ));
 
}
void SkyBox::BuildVertexBuffer()
{

	float3 v[m_NumVertices];

	int i=0;
	//bottom
	v[i++] = float3(-1.0f, -1.0f, -1.0f);
	v[i++] = float3(1.0f,  -1.0f, -1.0f);
	v[i++] = float3( -1.0f,  1.0f, -1.0f);
	v[i++] = float3( 1.0f, 1.0f, -1.0f);
	//top
	v[i++] = float3( -1.0f, -1.0f,  1.0f);
	v[i++] = float3( 1.0f,  -1.0f,  1.0f);
	v[i++] = float3(-1.0f,  1.0f,  1.0f);
	v[i++] = float3(1.0f, 1.0f,  1.0f);

	//for(int teller=0;teller<m_NumVertices;teller++)
	//{
	//	v[teller].x*=m_Width/2;
	//	v[teller].y*=m_Height/2;
	//	v[teller].z*=m_Depth/2;
	//}
    D3D10_BUFFER_DESC bd;
    bd.Usage = D3D10_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( VertexPT ) * m_NumVertices;
    bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = v;
    hr = GETD3D10DEVICE->CreateBuffer( &bd, &InitData, &m_pVertexBuffer );

}
void SkyBox::BuildIndexBuffer()
{
    DWORD indices[m_NumIndices];


	// Front face.
	indices[0] = 2; indices[1] = 6; indices[2] = 3;
	indices[3] = 6; indices[4] = 7; indices[5] = 3;

	// Back face.
	indices[6] = 1; indices[7]  = 5; indices[8]  = 0;
	indices[9] = 5; indices[10] = 4; indices[11] = 0;

	// Left face.
	indices[12] = 0; indices[13] =4; indices[14] = 2;
	indices[15] = 4; indices[16] = 6; indices[17] = 2;

	// Right face.
	indices[18] = 3; indices[19] = 7; indices[20] = 1;
	indices[21] = 7; indices[22] = 5; indices[23] = 1;

	// Top face.
	indices[24] = 6; indices[25] = 4; indices[26] = 5;
	indices[27] = 6; indices[28] = 5; indices[29] = 7;

	// Bottom face.
	indices[30] = 0; indices[31] = 2; indices[32] = 1;
	indices[33] = 2; indices[34] = 3; indices[35] = 1;

    D3D10_BUFFER_DESC bd;
    bd.Usage = D3D10_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( DWORD ) * m_NumIndices;        // 36 vertices needed for 12 triangles in a triangle list
    bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
       
	D3D10_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = indices;
    hr = GETD3D10DEVICE->CreateBuffer( &bd, &InitData, &m_pIndexBuffer );

}

void SkyBox::Cleanup()
{
	if( m_pIndexBuffer)m_pIndexBuffer->Release();
    if( m_pVertexBuffer ) m_pVertexBuffer->Release();
    if( m_pVertexLayout ) m_pVertexLayout->Release();
}
void SkyBox::Tick(float dTime)
{
	//verplaats de skybox world matrix naar de plaats van de camera
	D3DXMATRIX ViewInverse;
	D3DXMatrixInverse(&ViewInverse, 0, &m_View);

	//Haal de positie van Eye uit de inverse view matrix
	D3DXVECTOR4 Vec4(&ViewInverse[3*4]);
	//verplaats de skybox naar de eye positie
	this->ResetWorldMatrix();
	this->Translate(Vec4.x,Vec4.y,Vec4.z);
	//First calculate final world (includingparent worlds transforms)
	this->CalculateFinalWorldMatrix();
}
void SkyBox::Draw() const
{
	// Set the input layout
    GETD3D10DEVICE->IASetInputLayout( m_pVertexLayout );

    // Set vertex buffer
	UINT stride = sizeof( float3 );
    UINT offset = 0;
    GETD3D10DEVICE->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
   
	// Set index buffer
    GETD3D10DEVICE->IASetIndexBuffer( m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );

    // Set primitive topology
    GETD3D10DEVICE->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    //
    // Update Shader matrix variables
    //
	m_pViewProjectionVariable->SetMatrix((float*)&(m_View*m_Projection));
	m_pWorldVariable->SetMatrix((float*)&m_FinalWorld);
    //
    // Renders a SkyBox
    //
    D3D10_TECHNIQUE_DESC techDesc;
    m_pTechnique->GetDesc( &techDesc );
	//We maken een lus waarin we 6 quads tekenen

	for( UINT p = 0; p < techDesc.Passes; ++p )
	{
		//Gebruik de passende texture
		m_pCubeMapVariable->SetResource(m_pCubeMap );
		//Apply(0) om de effect state toe te passen.
		m_pTechnique->GetPassByIndex( p )->Apply(0);
		//teken een quad, gebruik makend van de parameters van DrawIndexed
		GETD3D10DEVICE->DrawIndexed( 36, 0, 0 ); 
	}
}
void SkyBox::InitializeMatrices()
{
    // Initialize the world matrix
    D3DXMatrixIdentity( &m_World );
}
