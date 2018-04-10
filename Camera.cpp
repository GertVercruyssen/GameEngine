//=============================================================================
// Camera.cpp by Frank Luna (C) 2004 All Rights Reserved.
//=============================================================================
#include "StdAfx.h"
#include "Camera.h"
#include "Input.h"
#include "d3dUtil.h"
#include "D3D10DeviceWrapper.h"


Camera::Camera(D3DXVECTOR3 start):m_PosW(start),m_FOV(3.1415f/4.0f),
	m_AspectRatio(4.0f/3), m_NearClippingPlane(1.0f), m_FarClippingPlane(1000.0f),m_bIsActive(1)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
	D3DXMatrixIdentity(&m_matViewProj);

	
	m_PreviousPosW = m_PosW;
	m_RightW = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_UpW    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_LookW  = D3DXVECTOR3(0.0f, -0.4f, 1.0f );

	// Client should adjust to a value that makes sense for application's
	// unit scale, and the object the camera is attached to--e.g., car, jet,
	// human walking, etc.

	//Use window client window dimensions to calc new aspect ratio
	float Aspect = (float)D3D10DEVICEWRAPPER->GetBackbufferWidth() / D3D10DEVICEWRAPPER->GetBackbufferHeight();
	SetAspectRatio(Aspect);
}

Camera::Camera():m_Running(10.0f),m_Speed(5.0f),m_MouseScale(100),m_FOV(3.1415f/4.0f),
	m_AspectRatio(4.0f/3), m_NearClippingPlane(1.0f), m_FarClippingPlane(1000.0f),m_bIsActive(1)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
	D3DXMatrixIdentity(&m_matViewProj);

	m_PosW   = D3DXVECTOR3(0.0f, 7.0f,-18.0f);
	m_RightW = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_UpW    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_LookW  = D3DXVECTOR3(0.0f, -0.4f, 1.0f );

	// Client should adjust to a value that makes sense for application's
	// unit scale, and the object the camera is attached to--e.g., car, jet,
	// human walking, etc.

	//Use window client window dimensions to calc new aspect ratio
	float Aspect = (float)D3D10DEVICEWRAPPER->GetBackbufferWidth() / D3D10DEVICEWRAPPER->GetBackbufferHeight();
	SetAspectRatio(Aspect);
}
Camera::~Camera()
{
}
const D3DXMATRIX& Camera::GetView() const
{
	return m_matView;
}

const D3DXMATRIX& Camera::GetProj() const
{
	return m_matProj;
}

const D3DXMATRIX& Camera::GetViewProj() const
{
	return m_matViewProj;
}

const D3DXVECTOR3& Camera::GetRight() const
{
	return m_RightW;
}

const D3DXVECTOR3& Camera::GetUp() const
{
	return m_UpW;
}

const D3DXVECTOR3& Camera::GetLook() const
{
	return m_LookW;
}

const D3DXVECTOR3& Camera::GetPos()const
{
	return m_PosW;
}

/**
* Create the camera with the specified position , target and up vector.
* This method calls the buildView method internally.
* @param pos the position for the camera.
* @param target the target for the camera.
* @param up the up vector for the camera.
*/
void Camera::LookAt(const D3DXVECTOR3& pos, const D3DXVECTOR3& target,const  D3DXVECTOR3& up)
{
	D3DXVECTOR3 Lookat = target - pos;
	D3DXVec3Normalize(&Lookat, &Lookat);

	D3DXVECTOR3 Right;
	D3DXVec3Cross(&Right, &up, &Lookat);
	D3DXVec3Normalize(&Right, &Right);

	D3DXVECTOR3 Up;
	D3DXVec3Cross(&Up, &Lookat, &Right);
	D3DXVec3Normalize(&Up, &Up);

	m_PosW   = pos;
	m_RightW = Right;
	m_UpW    = Up;
	m_LookW  = Lookat;

	BuildViewMatrix();

	m_matViewProj = m_matView * m_matProj;
}

/**
* Sets the perspective parameters for this camera.
* @param fov the field of view for this camera.
* @param aspect the aspect ratio (it is best to let this value correspond with the aspect
* ration of the viewport.
* @param nearZ the near clipping plane. 
* @param farZ the far clipping plane.
*/
void Camera::SetLens(float aspect, float fov, float nearZ, float farZ)
{
	m_FOV = fov;
	m_AspectRatio = aspect;
	m_NearClippingPlane = nearZ;
	m_FarClippingPlane = farZ;
	BuildProjectionMatrix();
}

void Camera::SetAspectRatio(float Aspect)
{
	m_AspectRatio = Aspect;
	BuildProjectionMatrix();
}

/**
* Sets the speed of the camera.
* @param s the speed for this camera.
*/
void Camera::SetSpeed(float s)
{
	m_Speed = s;
}

void Camera::SetMouseScale(int scale)
{
	if(scale>1 && scale<1000)
		m_MouseScale = (float)scale;
}

void Camera::SetPos(const D3DXVECTOR3& pos)
{
	m_PreviousPosW = pos;
	m_PosW = pos;
	// Rebuild the view matrix to reflect changes.
	BuildViewMatrix();
	m_matViewProj = m_matView * m_matProj;

}

/**
* Updates the camera ( for first person camera for example) 
* @param dt the delta time.
*/
void Camera::Tick(float dt)
{
	//only move when camera is onair
	if(!m_bIsActive)return;

	bool bRunning=false;
	// Find the net direction the camera is traveling in (since the
	// camera could be running and strafing).
	D3DXVECTOR3 dir(0.0f, 0.0f, 0.0f);
	if(INPUT.IsKeyDown('Z') )
		dir += m_LookW;
	if(INPUT.IsKeyDown('S') )
		dir -= m_LookW;
	if(INPUT.IsKeyDown('D') )
		dir += m_RightW;
	if(INPUT.IsKeyDown('Q') )
		dir -= m_RightW;
	if(INPUT.IsKeyDown(VK_LSHIFT) )
		bRunning = true;
	
	// Move at mSpeed along net direction.
	D3DXVec3Normalize(&dir, &dir);
	float finalspeed = m_Speed;
	if(bRunning)finalspeed*=m_Running;
	//use dt so that speed is independent from framerate 
	m_PosW += dir*finalspeed*dt;
	
	//get mousewheel info to change field of view
	float angle  = (float)INPUT.GetWheelDelta() / 120/10;//120 not magic see docs
	if(angle!=0)
	{
		m_FOV+=angle;
		BuildProjectionMatrix();
	}

	if(INPUT.IsLeftMouseButtonDown())//Left mouse button
	{
		// We rotate at a fixed speed.
		POINTS mouseMovement = INPUT.GetMouseMovement();
		float pitch  = (float)mouseMovement.y / m_MouseScale;
		float yAngle =  (float)mouseMovement.x / m_MouseScale;

		// Rotate camera's look and up vectors around the camera's right vector.
		//R is resulting matrix, second param is axis to turn around, pitch is angle
		D3DXMATRIX R;
		D3DXMatrixRotationAxis(&R, &m_RightW, pitch);
		D3DXVec3TransformCoord(&m_LookW, &m_LookW, &R);
		D3DXVec3TransformCoord(&m_UpW, &m_UpW, &R);


		// Rotate camera axes about the world's y-axis.
		D3DXMatrixRotationY(&R, yAngle);
		D3DXVec3TransformCoord(&m_RightW, &m_RightW, &R);
		D3DXVec3TransformCoord(&m_UpW, &m_UpW, &R);
		D3DXVec3TransformCoord(&m_LookW, &m_LookW, &R);

	}
	// Rebuild the view matrix to reflect changes.
	BuildViewMatrix();
	m_matViewProj = m_matView * m_matProj;
}

void Camera::Move(D3DXVECTOR3 newPos, bool rotate,float pitch,float yAngle){
	m_PosW.x = newPos.x;
	m_PosW.y = newPos.y;
	m_PosW.z = newPos.z;
	if ( rotate )
	{
		// Rotate camera's look and up vectors around the camera's right vector.
		// R is resulting matrix, second param is axis to turn around, pitch is angle
		D3DXMATRIX R;
		D3DXMatrixRotationAxis(&R, &m_RightW, pitch);
		D3DXVec3TransformCoord(&m_LookW, &m_LookW, &R);
		D3DXVec3TransformCoord(&m_UpW, &m_UpW, &R);


		// Rotate camera axes about the world's y-axis.
		D3DXMatrixRotationY(&R, yAngle);
		D3DXVec3TransformCoord(&m_RightW, &m_RightW, &R);
		D3DXVec3TransformCoord(&m_UpW, &m_UpW, &R);
		D3DXVec3TransformCoord(&m_LookW, &m_LookW, &R);
	}
	// Rebuild the view matrix to reflect changes.
	BuildViewMatrix();
	m_matViewProj = m_matView * m_matProj;
}

/**
* Construct the view matrix for this camera.
*/ 
void Camera::BuildViewMatrix()
{
	// Set up our view matrix. A view matrix can be defined given an eye point,
    // a point to lookat, and a direction for which way is up.
	// Keep camera's axes orthogonal to each other and of unit length.
	D3DXVec3Normalize(&m_LookW, &m_LookW);

	D3DXVec3Cross(&m_UpW, &m_LookW, &m_RightW);
	D3DXVec3Normalize(&m_UpW, &m_UpW);

	D3DXVec3Cross(&m_RightW, &m_UpW, &m_LookW);
	D3DXVec3Normalize(&m_RightW, &m_RightW);

    D3DXMatrixLookAtLH( &m_matView, &m_PosW, &(m_LookW+m_PosW), &m_UpW );

}

/**
* Construct the projection matrix for this camera.
*/ 
 void Camera::BuildProjectionMatrix()
{
	//limit FOV
	if(m_FOV>D3DX_PI/5*4.0f) m_FOV = static_cast<float>(D3DX_PI/5*4.0f) ;
	if(m_FOV<D3DX_PI/30.0f)m_FOV= static_cast<float>(D3DX_PI/30.0f);

	D3DXMatrixPerspectiveFovLH(&m_matProj, m_FOV, m_AspectRatio, m_NearClippingPlane, m_FarClippingPlane);
	m_matViewProj = m_matView * m_matProj;
}

void Camera::SetActive(bool bIsActive)
{
	m_bIsActive = bIsActive;
}

bool Camera::IsActive()
{
	return m_bIsActive;
}

 void Camera::OnResize()
 {
	//Use new window client window dimensions to calc new aspect ratio
	float Aspect = (float)D3D10DEVICEWRAPPER->GetBackbufferWidth() / D3D10DEVICEWRAPPER->GetBackbufferHeight();
	SetAspectRatio(Aspect);

 }