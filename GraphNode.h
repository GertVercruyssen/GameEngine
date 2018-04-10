#pragma once
#include "GraphBase.h"
#include <vector>

class GraphNode :
	public GraphBase
{
public:
	/**
	* Creates a new GraphNode. Should not be used, this class is meant to be subclassed.
	*/
	GraphNode(void);

	/**
	* Destroys the GraphNode.
	*/
	virtual ~GraphNode(void);

	/**
	* Implement this method to change behaviour according to window size.
	*/
	virtual void OnResize();

	/**
	* Override this method to implement the physics behaviour of this object.
	*/
	virtual void Tick(float dTime);

	/**
	* Calculates the FinalWorldMatrix of itself and every child
	*/
	virtual void CalculateFinalWorldMatrix();

	/**
	* Draw the D3DObject in the scene.
	* Needs definition in derived class
	*/
	virtual void Draw()const;

	/**
	* Draw the D3DObject in the scene with the technique.
	* @param technique the technique to use.
	*/
	virtual void Draw(const ID3D10EffectTechnique* pTechnique) const;

	/**
	* Check key's
	*/
	virtual void KeyPressed(WPARAM cKey);

	/**
	* New: Adds a child object
	*/
	void AddChild(GraphBase *NewGraphElement);

	/**
	* Removes an object from the list of children
	*/
	void RemoveChild(GraphBase * toRemove);

	/**
	* Retrieves the final world matrix 
	* Sets the Physics Global pos.
	*/
	void SetPhysXGlobalPose();
protected:
	///**
	//* New: Sets the parent pointer
	//*/
	//void SetParent(GraphBase *pParent);
	/**
	* New: Child vector
	*/
	vector<GraphBase*>m_ChildNodes;


private:
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	GraphNode(const GraphNode& t);
	GraphNode& operator=(const GraphNode& t);
};
