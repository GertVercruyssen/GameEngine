#include "StdAfx.h"
#include "GraphBase.h"

DirLight GraphBase::m_Light;
GraphBase::GraphBase(void):m_pParent(0)
{
	D3DXMatrixIdentity(&m_World);
	D3DXMatrixIdentity(&m_FinalWorld);
}

GraphBase::~GraphBase(void)
{
}
/**
* Gets the parent object Pointer
*/
GraphBase* GraphBase::GetParent(void) const
{
	return m_pParent;
}
/**
* Sets the parent pointer
*/
void GraphBase::SetParent(GraphBase *pParent)
{
	m_pParent  = pParent;
}
const D3DXMATRIX& GraphBase::GetWorldMatrix() const
{
	//OutputDebugString(_T("GetWorld in Node, returning m_World\n"));
	return m_World;
}
//reset world matrix
void GraphBase::ResetWorldMatrix()
{
	D3DXMatrixIdentity(&m_World);
}
void GraphBase::SetWorldMatrix(const D3DXMATRIX& world)
{
	m_World=world;
}
void GraphBase::SetLight(DirLight& light)
{
	m_Light = light;
}
void GraphBase::CalculateFinalWorldMatrix() 
{
	//calculate final world matrix, using all parents world's
	m_FinalWorld = GetWorldMatrix();
	GraphBase  *pCurrent=m_pParent;
	while (pCurrent!=0)
	{
		m_FinalWorld*=pCurrent->GetWorldMatrix();
		pCurrent = pCurrent->GetParent();
	}	
}
void GraphBase::SetName(const tstring &sName)
{
	m_sNodeName = sName;
}

void GraphBase::Translate(const D3DXVECTOR3& pos)
{
	D3DXMATRIX Translation;
	D3DXMatrixTranslation(&Translation,pos.x,pos.y,pos.z);
	m_World*=Translation;
	SetPhysXGlobalPose();
}
void GraphBase::Translate(float x,float y,float z)
{
	D3DXMATRIX Translation;
	D3DXMatrixTranslation(&Translation,x,y,z);
	m_World*=Translation;
	SetPhysXGlobalPose();
}
void GraphBase::Rotate(const D3DXVECTOR3& angles)
{
	D3DXMATRIX Rotation;
	D3DXMatrixRotationYawPitchRoll(&Rotation,angles.y,angles.x,angles.z);
	m_World*=Rotation;
	SetPhysXGlobalPose();
}
void GraphBase::Rotate(float x,float y,float z)
{
	D3DXMATRIX Rotation;
	D3DXMatrixRotationYawPitchRoll(&Rotation,y,x,z);
	m_World*=Rotation;
	SetPhysXGlobalPose();
}
void GraphBase::Scale(const D3DXVECTOR3& scales)
{
	D3DXMATRIX Scale;
	D3DXMatrixScaling(&Scale,scales.x,scales.y,scales.z);
	m_World*=Scale;
	SetPhysXGlobalPose();
}
void GraphBase::Scale(float x, float y , float z)
{
	D3DXMATRIX Scale;
	D3DXMatrixScaling(&Scale,x,y,z);
	m_World*=Scale;
	SetPhysXGlobalPose();
}
