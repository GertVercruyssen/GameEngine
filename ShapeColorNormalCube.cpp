#include "StdAfx.h"
#include "ShapeColorNormalCube.h"
#include "D3D10DeviceWrapper.h"
#include "DXWindow.h"

ShapeColorNormalCube::ShapeColorNormalCube(float width, float height, float depth, D3DXCOLOR color):
	 m_Width(width),m_Height(height),m_Depth(depth),m_Color(color)
{
	BuildVertexBuffer();
	BuildIndexBuffer();
}

ShapeColorNormalCube::~ShapeColorNormalCube(void)
{
}
bool ShapeColorNormalCube::Init()
{
	return true;
}


void ShapeColorNormalCube::BuildVertexBuffer()
{
	m_NumVertices=24;
	// Create vertex array containing three elements in system memory
	VertexPosColNormal *pVertices = new VertexPosColNormal[m_NumVertices];
	int i = 0;
	//front
	//in origin
	pVertices[i++] = VertexPosColNormal( -1.0f,  -1.0f, -1.0f ,		1.0f, 0.0f, 0.0f, 1.0f , 0.0f,0.0f,-1.0f);
	//one unit on y axis (up)
	pVertices[i++] = VertexPosColNormal( -1.0f,  1.0f,  -1.0f ,		1.0f, 0.0f, 0.0f, 1.0f , 0.0f,0.0f,-1.0f);
	//one unit on x axis (right)
	pVertices[i++] = VertexPosColNormal( 1.0f,  -1.0f, -1.0f ,		1.0f, 0.0f, 0.0f, 1.0f , 0.0f,0.0f,-1.0f);
	//one unit on x axis (right) and one unit on y axis (up)
	pVertices[i++] = VertexPosColNormal( 1.0f,  1.0f, -1.0f ,			1.0f, 0.0f, 0.0f, 1.0f , 0.0f,0.0f,-1.0f);
	//back
	//in origin
	pVertices[i++] = VertexPosColNormal( -1.0f,  -1.0f, 1.0f ,		0.0f, 1.0f, 0.0f, 1.0f , 0.0f,0.0f,1.0f);
	//one unit on y axis (up)
	pVertices[i++] = VertexPosColNormal( -1.0f,  1.0f,  1.0f ,		0.0f, 1.0f, 0.0f, 1.0f , 0.0f,0.0f,1.0f);
	//one unit on x axis (right)
	pVertices[i++] = VertexPosColNormal( 1.0f,  -1.0f, 1.0f ,			0.0f, 1.0f, 0.0f, 1.0f , 0.0f,0.0f,1.0f);
	//one unit on x axis (right) and one unit on y axis (up)
	pVertices[i++] = VertexPosColNormal( 1.0f,  1.0f, 1.0f ,			0.0f, 1.0f, 0.0f, 1.0f , 0.0f,0.0f,1.0f);

	//left
	//in origin
	pVertices[i++] = VertexPosColNormal( -1.0f,  -1.0f, -1.0f ,		0.0f, 0.0f, 1.0f, 1.0f , -1.0f,0.0f,0.0f);
	//one unit on y axis (up)
	pVertices[i++] = VertexPosColNormal( -1.0f,  1.0f,  -1.0f ,		0.0f, 0.0f, 1.0f, 1.0f , -1.0f,0.0f,0.0f);
	//one unit on x axis (right)
	pVertices[i++] = VertexPosColNormal( -1.0f,  -1.0f, 1.0f ,		0.0f, 0.0f, 1.0f, 1.0f , -1.0f,0.0f,0.0f);
	//one unit on x axis (right) and one unit on y axis (up)
	pVertices[i++] = VertexPosColNormal( -1.0f,  1.0f, 1.0f ,			0.0f, 0.0f, 1.0f, 1.0f , -1.0f,0.0f,0.0f);

	//right
	//in origin
	pVertices[i++] = VertexPosColNormal( 1.0f,  -1.0f, -1.0f ,		1.0f, 1.0f, 0.0f, 1.0f , 1.0f,0.0f,0.0f);
	//one unit on y axis (up)
	pVertices[i++] = VertexPosColNormal( 1.0f,  1.0f,  -1.0f ,		1.0f, 1.0f, 0.0f, 1.0f , 1.0f,0.0f,0.0f);
	//one unit on x axis (right)
	pVertices[i++] = VertexPosColNormal( 1.0f,  -1.0f, 1.0f ,			1.0f, 1.0f, 0.0f, 1.0f , 1.0f,0.0f,0.0f);
	//one unit on x axis (right) and one unit on y axis (up)
	pVertices[i++] = VertexPosColNormal( 1.0f,  1.0f, 1.0f ,			1.0f, 1.0f, 0.0f, 1.0f , 1.0f,0.0f,0.0f);

	//top
	//in origin
	pVertices[i++] = VertexPosColNormal( -1.0f,  1.0f, 1.0f ,			1.0f, 0.0f, 1.0f, 1.0f , 0.0f,1.0f,0.0f);
	//one unit on y axis (up)
	pVertices[i++] = VertexPosColNormal( -1.0f,  1.0f,  -1.0f ,		1.0f, 0.0f, 1.0f, 1.0f , 0.0f,1.0f,0.0f);
	//one unit on x axis (right)
	pVertices[i++] = VertexPosColNormal( 1.0f,  1.0f, 1.0f ,			1.0f, 0.0f, 1.0f, 1.0f , 0.0f,1.0f,0.0f);
	//one unit on x axis (right) and one unit on y axis (up)
	pVertices[i++] = VertexPosColNormal( 1.0f,  1.0f, -1.0f ,			1.0f, 0.0f, 1.0f, 1.0f , 0.0f,1.0f,0.0f);

	//bottom
	//in origin
	pVertices[i++] = VertexPosColNormal( -1.0f,  -1.0f, 1.0f ,		1.0f, 1.0f, 1.0f, 1.0f , 0.0f,-1.0f,0.0f);
	//one unit on y axis (up)
	pVertices[i++] = VertexPosColNormal( -1.0f,  -1.0f,  -1.0f ,		1.0f, 1.0f, 1.0f, 1.0f , 0.0f,-1.0f,0.0f);
	//one unit on x axis (right)
	pVertices[i++] = VertexPosColNormal( 1.0f,  -1.0f, 1.0f ,			1.0f, 1.0f, 1.0f, 1.0f , 0.0f,-1.0f,0.0f);
	//one unit on x axis (right) and one unit on y axis (up)
	pVertices[i++] = VertexPosColNormal( 1.0f,  -1.0f, -1.0f ,		1.0f, 1.0f, 1.0f, 1.0f , 0.0f,-1.0f,0.0f);

	//recalc according to wanted size and color
	for(int count=0 ; count < i ; ++count)
	{
		pVertices[count].pos.x*=.5f*m_Width;
		pVertices[count].pos.y*=.5f*m_Height;
		pVertices[count].pos.z*=.5f*m_Depth;
		pVertices[count].color=m_Color;
	}

	//fill a buffer description to copy the vertexdata into graphics memory
	D3D10_BUFFER_DESC bd;
	bd.Usage = D3D10_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( VertexPosColNormal ) * m_NumVertices;
	bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pVertices;
	//create a ID3D10Buffer in graphics meemory containing the vertex info
	HR(GETD3D10DEVICE->CreateBuffer( &bd, &InitData, &m_pVertexBuffer ));

	delete[] pVertices;
}
void ShapeColorNormalCube::BuildIndexBuffer()
{
	m_NumIndices=36;
    // Create index buffer
    DWORD indices[] =
    {
        0,1,2,2,1,3,//front
		4,6,5,5,6,7,//back
		8,10,9,9,10,11,//left
		12,13,14,14,13,15,//right
		16,18,17,17,18,19,//top
		20,21,22,22,21,23//bottom
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
void ShapeColorNormalCube::Tick(float dTime)
{
}

