#pragma once
#include "ShapeColorNormalCube.h"

class NxActor;

class CubePhysX : public ShapeColorNormalCube
{
public:
	CubePhysX(float width=1.0f, float height=1.0f, float depth=1.0f, D3DXCOLOR color=D3DXCOLOR(1,1,1,1),int bodyType=RIGID);
	virtual ~CubePhysX(void);
	/**
	* RIGID: dynamic physics body, STATIC: fixed body, GHOST: no physics
	*/
	static const int RIGID,STATIC,GHOST;
	/**
	* Set the body as rigid body or as static body(fixed)
	*/
	virtual void SetBodyType(int bodyType);
	virtual void Tick(float dTime);
	virtual void OnTriggerEnter(Character* player);

	/**
	* Add a force to the physics object
	*/
	void AddForce(const D3DXVECTOR3& pos);
	/**
	*	Creates a physics entity for the visble object
	*/
	virtual void InitActor(){}
	virtual NxActor* GetActor(){return m_pActor;};

	/**
	* PhysX objects may not retrieve the final world matrix
	* The returned world matrix from PhysX is ok
	*/
	virtual void CalculateFinalWorldMatrix(){m_FinalWorld = m_World;}

protected:
	/**
	*	Creates a physics entity for the visble object
	*/
	virtual void InitActor(int bodytype);
	/**
	* Retrieves the final world matrix 
	* Sets the Physics Global pos.
	*/
	virtual void SetPhysXGlobalPose();
	/**
	* Sets the Physics Global pos.
	* @World: een WorldMatrix
	*/
	virtual void SetPhysXGlobalPose(const D3DXMATRIX& World);
	/**
	* The physics representation of an object
	*/
	NxActor *m_pActor;




private:
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	CubePhysX(const CubePhysX& t);
	CubePhysX& operator=(const CubePhysX& t);
};
