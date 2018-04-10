//=============================================================================
// GraphCamera.cpp by Frank Luna (C) 2004 All Rights Reserved.
//=============================================================================
#include "StdAfx.h"
#include "GraphCamera.h"
#include "ShapeColorNormalCube.h"
#include "Input.h"
#include "D3D10DeviceWrapper.h"

GraphCamera::GraphCamera()
{

	m_PosW   = D3DXVECTOR3(0.0f, 0.0f,0.0f);
	m_RightW = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_UpW    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_LookW  = D3DXVECTOR3(0.0f, 0.0f, 1.0f );

	////Use small cube to locate the camera
	//ShapeColorNormalCube *pCubeCamera = new ShapeColorNormalCube(.1f,.1f,.1f);
	//AddChild(pCubeCamera);

}
GraphCamera::~GraphCamera()
{
}
void GraphCamera::Tick(float dTime)
{

	//tick child objects
	GraphNode::Tick(dTime);

	//do camera updates
	Camera::Tick(dTime);
}


/**
* Construct the view matrix for this camera.
*/ 
void GraphCamera::BuildViewMatrix()
{
	// in a graph, we first calculate the final transform matrix
	//next, pos vector is multiplied with finalworldtransform
	GraphBase::CalculateFinalWorldMatrix();

	D3DXVECTOR3 PosW,UpW,LookW,RightW;
	D3DXVECTOR4 Out;

	D3DXVec3Transform(&Out,&m_PosW,&m_FinalWorld);
	PosW=D3DXVECTOR3(Out.x,Out.y,Out.z);
	//directional vectors are only rotated
	m_FinalWorld._41=0;
	m_FinalWorld._42=0;
	m_FinalWorld._43=0;
	D3DXVec3Transform(&Out,&m_UpW,&m_FinalWorld);
	UpW=D3DXVECTOR3(Out.x,Out.y,Out.z);
	D3DXVec3Transform(&Out,&m_LookW,&m_FinalWorld);
	LookW=D3DXVECTOR3(Out.x,Out.y,Out.z);
	D3DXVec3Transform(&Out,&m_RightW,&m_FinalWorld);
	RightW=D3DXVECTOR3(Out.x,Out.y,Out.z);

	// Set up our view matrix. A view matrix can be defined given an eye point,
    // a point to lookat, and a direction for which way is up.
	// Keep camera's axes orthogonal to each other and of unit length.
	D3DXVec3Normalize(&LookW, &LookW);

	D3DXVec3Cross(&UpW, &LookW, &RightW);
	D3DXVec3Normalize(&UpW, &UpW);

	D3DXVec3Cross(&RightW, &UpW, &LookW);
	D3DXVec3Normalize(&RightW, &RightW);

    D3DXMatrixLookAtLH( &m_matView, &PosW, &(LookW+PosW), &UpW );

}

