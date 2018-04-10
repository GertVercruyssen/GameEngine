//=============================================================================
// Camera.h by Frank Luna (C) 2004 All Rights Reserved.
// Adapted for GSP framework by Bart Uyttenhove 
//=============================================================================

#pragma once

#include <d3dx10.h>

class Camera
{
public:

	/**
	* Creates a camera at the given position.
	*/
	Camera(D3DXVECTOR3 start);

	// By default, the camera starts out with its basis vectors 
	// aligned with the world space axes, and its origin positioned
	// at the world space origin.
	Camera();
	virtual ~Camera(void);	
	// Read only accessor functions.
	const D3DXMATRIX& GetView() const;
	const D3DXMATRIX& GetProj() const;
	const D3DXMATRIX& GetViewProj() const;

	const D3DXVECTOR3& GetRight() const;
	const D3DXVECTOR3& GetUp() const;
	const D3DXVECTOR3& GetLook() const;
	const D3DXVECTOR3& GetPos() const;

	void SetPos(const D3DXVECTOR3& pos);

	// Our implementation of D3DXMatrixLookAtLH
	void LookAt(const D3DXVECTOR3& pos,const  D3DXVECTOR3& target,const  D3DXVECTOR3& up);

	// Perspective projection parameters.
	void SetLens(float aspect=4.0f/3, float fov=D3DX_PI/4, float nearZ=1.0f, float farZ=10000.0f);
	void SetAspectRatio(float Aspect=4.0f/3);

	// Sets the camera speed.
	void SetSpeed(float s);

	void SetMouseScale(int scale);

	// Updates the camera's basis vectors and origin, relative to 
	// the world space, based on user input.
	virtual void Tick(float dt);

	//boolean setter: if true  camera is to be used as view camera
	void SetActive(bool bIsActive);

	//boolean getter: if true  camera is to be used as view camera
	bool IsActive();

	/**
	* Implement this method to change behaviour according to window size.
	*/
	virtual void OnResize();

	/**
	* Moves the camera to the new position and a new pitch and yAngle value.
	*/
	void Move(D3DXVECTOR3 newPos, bool rotate=false,float pitch=0.0f,float yAngle=0.0f);

protected:
	// Constructs the view matrix based on the camera's basis
	// vectors and origin, relative to the world space
	virtual void BuildViewMatrix();
	virtual void BuildProjectionMatrix();

	// Save camera related matrices.
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
	D3DXMATRIX m_matViewProj;

	// The previous world position
	D3DXVECTOR3 m_PreviousPosW;
	// Camera Position Relative to world space.
	D3DXVECTOR3 m_PosW;
	//unity vector the descrobes the camera's right 
	D3DXVECTOR3 m_RightW;
	//the upvector of the camera
	D3DXVECTOR3 m_UpW;
	//unity vector that describes the direction the camera looks at
	D3DXVECTOR3 m_LookW;

	// Camera speed.
	float m_Speed;
	//Running multiplies speed by factor m_Running
	float m_Running;
	//mousemovement is divided by this scalefactor
	//big makes mouse move slow
	float m_MouseScale;
	//field of view cam be changed by rotating the mouswheel
	float m_FOV;
	// m_AspectRatio is width/height
	float m_AspectRatio;
	//near and far clipping plane
	float m_NearClippingPlane,m_FarClippingPlane;
	//if true, camera listens to input and is movable
	bool m_bIsActive;

private:

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	Camera(const Camera& t);
	Camera& operator=(const Camera& t);
};
