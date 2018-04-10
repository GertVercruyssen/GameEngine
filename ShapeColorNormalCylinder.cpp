#include "StdAfx.h"
#include "ShapeColorNormalCylinder.h"
#include "D3D10DeviceWrapper.h"
#include "DXWindow.h"
//vertex structs
#include "vertex.h"

ShapeColorNormalCylinder::ShapeColorNormalCylinder(float Radius1,float Radius2, float Length, D3DXCOLOR color, int Slices, int Stacks):	
						m_Radius1(Radius1),m_Radius2(Radius2),m_Length(Length),
						m_Slices(Slices),m_Stacks(Stacks),
						m_FanIndex1(0),m_FanIndex2(0),
						m_R(color.r),m_G(color.g),m_B(color.b),m_A(color.a)
{
	BuildVertexBuffer();
	BuildIndexBuffer();
}

ShapeColorNormalCylinder::~ShapeColorNormalCylinder(void)
{
}
bool ShapeColorNormalCylinder::Init()
{
	return true;
}
void ShapeColorNormalCylinder::BuildVertexBuffer()
{
	//de eerste en laatste vertex bevinden zich op dezelfde plaats
	//must voor trianglefan, en eenvoudiger voor indexbuffer
	m_NumVertices=(m_Slices+1)*4*m_Stacks+2;
	// Create vertex array containing three elements in system memory
	VertexPosColNormal *pVertices = new VertexPosColNormal[m_NumVertices];
	int i = 0;

	//eerst de driehoeken in lengte met normal in het xz vlak
	//onderste cirkel
	for(int teller=0;teller<=m_Slices;++teller)
	{
		float angle=static_cast<float>(D3DX_PI*2/m_Slices*teller);
		pVertices[i++] = VertexPosColNormal(
			m_Radius1*sinf(angle), -m_Length/2, m_Radius1*cosf(angle),
			m_R,m_G,m_B,m_A,
			sinf(angle),0,cosf(angle));
	}
	//bovenste
	for(int teller=0;teller<=m_Slices;++teller)
	{
		float angle=static_cast<float>(D3DX_PI*2/m_Slices*teller);
		pVertices[i++] = VertexPosColNormal(
			m_Radius2*sinf(angle), m_Length/2, m_Radius2*cosf(angle),
			m_R,m_G,m_B,m_A,
			sinf(angle),0,cosf(angle));
	}
	//nu nog eens de twee cirkels maar met normal naar boven en beneden voor de trianglefan
	//eerste punt is midden omdat we dan een trianglefan kunnen renderen
	//middelpunt van de onderste cirkel
	m_FanIndex1=i;
	pVertices[i++] = VertexPosColNormal(0, -m_Length/2, 0,
			m_R,m_G,m_B,m_A,
			0,-1,0);
	//onderste cirkel
	for(int teller=0;teller<m_Slices;++teller)
	{
		float angle=static_cast<float>(D3DX_PI*2/m_Slices*(m_Slices-teller));
		pVertices[i++] = VertexPosColNormal(
			m_Radius1*sinf(angle), -m_Length/2, m_Radius1*cosf(angle),
			m_R,m_G,m_B,m_A,
			0,-1,0);
	}
	//eerste punt is midden omdat we dan een trianglefan kunnen renderen
	//middelpunt van de bovenste cirkel
	m_FanIndex2=i;
	pVertices[i++] = VertexPosColNormal(0, +m_Length/2, 0,
			m_R,m_G,m_B,m_A,
			0,+1,0);
	//bovenste schijf
	for(int teller=0;teller<m_Slices;++teller)
	{
		//om winding goed te hebben, cirkel in andere zin berekenen
		float angle=static_cast<float>(D3DX_PI*2/m_Slices*teller);
		pVertices[i++] = VertexPosColNormal(m_Radius2*sinf(angle), m_Length/2, m_Radius2*cosf(angle),
			m_R,m_G,m_B,m_A,
			0,1,0);
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
void ShapeColorNormalCylinder::BuildIndexBuffer()
{
	//sides + top and bottom triangles
	m_NumIndices=m_Slices*6*m_Stacks + m_Slices*3*2;
	// Create index array containing three elements in system memory
	DWORD *pIndices = new DWORD[m_NumIndices];
	int i = 0;

	for(int teller=0;teller<m_Slices;++teller)
	{
		//laatste twee driehoeken moeten verbonden worden met eerste vertices
		//+1= is om de eerste vertex over te slaan (is middelpunt)
		pIndices[i++]=teller;
		pIndices[i++]=teller+1;
		pIndices[i++]=teller+1+m_Slices;
		pIndices[i++]=teller+1;
		pIndices[i++]=(1+teller)+(m_Slices+1);
		pIndices[i++]=teller+1+m_Slices;
	}	
	//onderste vlak
	for(int teller=0;teller<m_Slices;++teller)
	{
		pIndices[i++]=m_FanIndex1;
		pIndices[i++]=m_FanIndex1+1+teller;
		pIndices[i++]=m_FanIndex1+1+(1+teller)%m_Slices;
	}
	//bovenste vlak
	for(int teller=0;teller<m_Slices;++teller)
	{
		pIndices[i++]=m_FanIndex2;
		pIndices[i++]=m_FanIndex2+1+teller;
		pIndices[i++]=m_FanIndex2+1+(1+teller)%m_Slices;
	}
	
	
	D3D10_BUFFER_DESC bd;
    bd.Usage = D3D10_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( DWORD ) * m_NumIndices;        
    bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
       
	D3D10_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pIndices;
    HR(GETD3D10DEVICE->CreateBuffer( &bd, &InitData, &m_pIndexBuffer ));

	//delete system memory array
	delete[] pIndices;
}
void ShapeColorNormalCylinder::SetMaterialColor(float r, float g, float b, float a)
{
	m_R = r;
	m_G = g;
	m_B = b;
	m_A = a;
}
void ShapeColorNormalCylinder::Tick(float dTime)
{
}


