#pragma once
#include "d3dUtil.h"
class Character;
class NxShape;

class GraphBase
{
public:
	GraphBase(void);
	virtual ~GraphBase(void);

	/**
	* Gets the parent object Pointer
	*/
	GraphBase *GetParent() const;

	/**
	* Sets the parent pointer
	*/
	void SetParent(GraphBase *pParent);

	/**
	* Sets the name of the node
	*/
	void SetName(const tstring &sName);

	/**
	* Calculates the transformation for this Graph3DObject, with
	* respect for the hierarchy of the object.
	* @return the resulting D3DXMatrix
	*/
	virtual void CalculateFinalWorldMatrix() ;

	/**
	* Reset the World matrix
	*/
	void ResetWorldMatrix();

	/**
	* Gets the world matrix of this object.
	*/
	const D3DXMATRIX& GetWorldMatrix() const;

	/**
	* Sets the world matrix of this object.
	*/
	void SetWorldMatrix(const D3DXMATRIX& world);

	/**
	* Implement this method to change behaviour according to window size.
	*/
	virtual void OnResize()=0;

	/**
	* Override this method to implement the physics behaviour of this object.
	*/
	virtual void Tick(float dTime)=0;

	/**
	* Draw the D3DObject in the scene.
	* Needs definition in derived class
	*/
	virtual void Draw() const =0 ;

	/**
	* Draw the D3DObject in the scene with the technique.
	* @param technique the technique to use.
	*/
	virtual void Draw(const ID3D10EffectTechnique* pTechnique) const = 0;

	/**
	* Check key's
	*/
	virtual void KeyPressed(WPARAM cKey) {}	

	/**
	* Sets the directional light
	*/
	static void SetLight(DirLight& light);

	/**
	* Add a translation to the world matrix of this object.
	* @param pos the translation to add.
	*/
	void Translate(const D3DXVECTOR3& pos);

	/**
	* Adds a translation to the world matrix of this object.
	* @param x the x value for the translation.
	* @param y the y value for the translation.
	* @param z the z value for the translation.
	*/
	void Translate(float x,float y,float z);

	/**
	* Adds a rotation to the world matrix of this object.
	* @param x the rotation around the x axis.
	* @param y the rotatoin around the y axis.
	* @parma z the rotation around the z axis.
	*/
	void Rotate(float x,float y,float z);

	/**
	* Adds a rotation to the world matrix of this object.
	* @param angles the rotation as D3DXVextor with the x,y & z members
	* as the rotation around the respective axises.
	*/
	void Rotate(const D3DXVECTOR3 &angles);

	/**
	* Adds a scaling transformation to the world matrix of this object.
	* @param x scales the x dimension of this object.
	* @param y scales the y dimension of this object.
	* @param z scales the z dimension of this object.
	*/
	void Scale(float x, float y , float z);

	/**
	* Adds a  scaling transformation to the world matrix of this object.
	* @param scales the scaling as a D3DXVECTOR3 with the x,y & z members
	* as the scaling parameters for the 3 dimensions of the object.
	*/
	void Scale(const D3DXVECTOR3& scales);

	/**
	* Sets the Physics Global pos.
	* @World: een WorldMatrix
	*/
	virtual void SetPhysXGlobalPose(const D3DXMATRIX& World){}
	virtual void SetPhysXGlobalPose(){}

	/**
	* Called when this object is triggered by a character.
	*/ 
	virtual void OnTriggerEnter(Character* player){};

	/**
	* Called when the character leaves the trigger area.
	*/
	virtual void OnTriggerExit(Character* player){};

protected:
	/**
	* Holds pointer to parent object
	* equals 0 if no parent is present
	*/
	GraphBase *m_pParent;

	/**
	* Holds World matrix of the element
	*/
	D3DXMATRIX m_World;	

	/**
	* Holds Final World matrix of the element
	*/
	D3DXMATRIX m_FinalWorld;

	/**
	* Holds the directional light properties
	*/
	static DirLight m_Light;

	/**
	* String used to find nodes by name
	*/
	tstring m_sNodeName;

private:
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	GraphBase(const GraphBase& t);
	GraphBase& operator=(const GraphBase& t);
};
