#include "StdAfx.h"
#include "ShapeColorTriangle.h"
#include "D3D10DeviceWrapper.h"
#include "DXWindow.h"


ShapeColorTriangle::ShapeColorTriangle()
{
	BuildVertexBuffer();
	BuildIndexBuffer();
}

ShapeColorTriangle::~ShapeColorTriangle(void)
{
}
bool ShapeColorTriangle::Init()
{
	return true;
}

void ShapeColorTriangle::BuildVertexBuffer()
{
	m_NumVertices=3;
	// Create vertex array containing three elements in system memory
	VertexPosCol *pVertices = new VertexPosCol[m_NumVertices];
	int i = 0;
	//in origin
	pVertices[i++] = VertexPosCol( 0.0f,  0.0f, 0.0f , 1.0f, 0.0f, 0.0f, 1.0f );
	//one unit on y axis (up)
	pVertices[i++] = VertexPosCol( 0.0f,  1.0f,  0.0f ,  0.0f, 0.0f, 1.0f, 1.0f );
	//one unit on x axis (right)
	pVertices[i++] = VertexPosCol( 1.0f,  0.0f, 0.0f ,  0.0f, 1.0f, 0.0f, 1.0f );
	//one unit on x axis and y axis
	//pVertices[i++] = VertexPosCol( 1.0f,  1.0f, 0.0f ,  1.0f, 1.0f, 1.0f, 1.0f );

	//fill a buffer description to copy the vertexdata into graphics memory
	D3D10_BUFFER_DESC bd;
	bd.Usage = D3D10_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( VertexPosCol ) * m_NumVertices;
	bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pVertices;
	//create a ID3D10Buffer in graphics meemory containing the vertex info
	HR(GETD3D10DEVICE->CreateBuffer( &bd, &InitData, &m_pVertexBuffer ));

	delete pVertices;
}
void ShapeColorTriangle::BuildIndexBuffer()
{
	m_NumIndices=3;
    // Create index buffer
    DWORD indices[] =
    {
        0,1,2
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
void ShapeColorTriangle::Tick(float dTime)
{
}