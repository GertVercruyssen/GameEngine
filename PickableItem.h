#pragma once

#include "GraphBase.h"
#include "ShapeColorNormalCube.h"
class CubePhysX;
class NxShape;

/**
* This class creates a pickable item by adding a ghost object
* that encapsulates it. When a player character "triggers" the ghost
* object the object will be added to the inventory of the player character.
* @author Koen Samyn
*/
class PickableItem : public GraphBase
{
public:
	/**
	* Creates a new pickable item.
	*/
	PickableItem(int id,D3DXVECTOR3 pos,bool rotate);

	/**
	* Destroys the pickable item.
	*/
	virtual ~PickableItem(void);

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
	* Called when the trigger is entered.
	*/
	virtual void OnTriggerEnter(Character* player);

	/**
	* Called when the trigger is left.
	*/
	virtual void OnTriggerExit(Character* player);

	/**
	* Disable calling OnTriggerEnter and OnTriggerExit.
	*/
	virtual void DisableTrigger();

	int GetID(){return m_ID;}
private:
	/**
	* The object to show
	*/
	ShapeBase* m_pObject;

	/**
	* The trigger object
	*/
	CubePhysX* m_pTrigger;

	/**
	* Rotate the object or not.
	*/
	bool m_Rotate;

	/**
	* The original position
	*/
	D3DXVECTOR3 m_Position;

	/**
	* The rotation of the y axis.
	*/
	float m_yRot;

	/**
	* The id of the pickable item.
	*/
	int m_ID;
};
