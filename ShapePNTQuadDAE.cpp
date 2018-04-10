#include "StdAfx.h"
#include "ShapePNTQuadDAE.h"
#include "D3D10DeviceWrapper.h"
#include "DXWindow.h"


ShapePNTQuadDAE::ShapePNTQuadDAE(const tstring& TexFileName):ShapePNT(TexFileName),
			m_pTechPoint(0),
			m_pTechAnisotropic(0)

{
	GetTechniques();
	BuildVertexBuffer();
	BuildIndexBuffer();
}

ShapePNTQuadDAE::~ShapePNTQuadDAE(void)
{
}

void ShapePNTQuadDAE::GetTechniques()
{
	//Get Extra Techniques
	m_pTechPoint = m_pEffect->GetTechniqueByName("TechPoint");
	m_pTechAnisotropic = m_pEffect->GetTechniqueByName("TechAnisotropic");
}
void ShapePNTQuadDAE::SetTechPoint()
{
	m_pDefaultTechnique = m_pTechPoint;
}
void ShapePNTQuadDAE::SetTechLinear()
{
	m_pDefaultTechnique = m_pTechLinear;
}
void ShapePNTQuadDAE::SetTechAnisotropic()
{
	m_pDefaultTechnique = m_pTechAnisotropic;
}

bool ShapePNTQuadDAE::Init()
{
	return true;
}

void ShapePNTQuadDAE::BuildVertexBuffer()
{
	m_NumVertices=4;
	// Create vertex array containing three elements in system memory
	VertexPNT *pVertices = new VertexPNT[m_NumVertices];
	int i = 0;
	//in origin
	pVertices[i++] = VertexPNT( 0.0f,  0.0f, 0.0f , 0.0f,0.0f,-1.0f , 0, 3);
	//one unit on y axis (up)
	pVertices[i++] = VertexPNT( 0.0f,  1.0f,  0.0f , 0.0f,0.0f,-1.0f , 0, 0);
	//one unit on x axis (right)
	pVertices[i++] = VertexPNT( 1.0f,  0.0f, 0.0f ,  0.0f,0.0f,-1.0f , 1, 3);
	//one unit on x axis (right) and one unit on y axis (up)
	pVertices[i++] = VertexPNT( 1.0f,  1.0f, 0.0f ,  0.0f,0.0f,-1.0f , 1, 0);

	//fill a buffer description to copy the vertexdata into graphics memory
	D3D10_BUFFER_DESC bd;
	bd.Usage = D3D10_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( VertexPNT ) * m_NumVertices;
	bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pVertices;
	//create a ID3D10Buffer in graphics meemory containing the vertex info
	HR(GETD3D10DEVICE->CreateBuffer( &bd, &InitData, &m_pVertexBuffer ));

	delete[] pVertices;
}

void ShapePNTQuadDAE::BuildIndexBuffer()
{
	m_NumIndices=6;
    // Create index buffer
    DWORD indices[] =
    {
        0,1,2,2,1,3,
    };

    D3D10_BUFFER_DESC bd;
    bd.Usage = D3D10_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( DWORD ) * m_NumIndices;        
    bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
       
	D3D10_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = indices;
    HR(GETD3D10DEVICE->CreateBuffer( &bd, &InitData, &m_pIndexBuffer ));
}


