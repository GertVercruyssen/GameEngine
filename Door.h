#pragma once

#include "Graph3DObject.h"
class CubePhysX;
class NxRevoluteJoint;

/**
* This class creates a Door object with or without a trigger object.
*/ 
class Door : public GraphBase
{
public:
	/**
	* Creates a new Door object
	*/
	Door(bool useTrigger,D3DXVECTOR3 location, float yRot, float width, float height,float thickness);

	/**
	* Destroys the Door object
	*/
	virtual ~Door(void);

	/**
	* Implement this method to change behaviour according to window size.
	*/
	virtual void OnResize();

	/**
	* Override this method to implement the physics behaviour of this object.
	*/
	virtual void Tick(float dTime);

	/**
	* Draw the D3DObject in the scene.
	* Needs definition in derived class
	*/
	virtual void Draw() const;

	/**
	* Draw the D3DObject in the scene with the technique.
	* @param technique the technique to use.
	*/
	virtual void Draw(const ID3D10EffectTechnique* pTechnique) const;

	/**
	* PhysX objects may not retrieve the final world matrix
	* The returned world matrix from PhysX is ok
	*/
	virtual void CalculateFinalWorldMatrix();

	/**
	* Sets the Physics Global pos.
	* @World: een WorldMatrix
	*/
	virtual void SetPhysXGlobalPose(const D3DXMATRIX& World);
	virtual void SetPhysXGlobalPose();

	/**
	* The trigger has been entered
	*/
	virtual void OnTriggerEnter(Character* player);

	/**
	* The trigger has been exited
	*/
	virtual void OnTriggerExit(Character* player);

private:
	bool m_UseTrigger;
	D3DXVECTOR3 m_Location;
	float m_YRot;
	float m_Width; 
	float m_Height;
	float m_Thickness;

	CubePhysX* m_pDoorMesh;

	CubePhysX* m_pDoorTrigger;

	NxRevoluteJoint* m_pHinge;
};
