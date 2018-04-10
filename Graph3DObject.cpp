//=============================================================================
// Graph3DObject.cpp by Bart Uyttenhove 2008 
//=============================================================================
#include "StdAfx.h"
#include "D3D10DeviceWrapper.h"
#include "Graph3DObject.h"
#include "EffectManager.h"

D3DXMATRIX Graph3DObject::m_View;
D3DXMATRIX Graph3DObject::m_Projection;
D3DXVECTOR3 Graph3DObject::m_CameraPos;

Graph3DObject::Graph3DObject(void):GraphBase(),
			m_pEffect(NULL),
			m_pDefaultTechnique(NULL)

{
}

Graph3DObject::~Graph3DObject(void)
{
//	SAFE_RELEASE(m_pEffect);
}

void Graph3DObject::CreateEffect(const tstring& EffectFilename)
{
	m_pEffect = EFFECTMANAGER.GetEffect(EffectFilename);
}

//static view and projection matrix
void Graph3DObject::SetView(const D3DXMATRIX * View)
{
	m_View = *View;
}
void Graph3DObject::SetProjection(const D3DXMATRIX * Projection)
{
	m_Projection = *Projection;
}


