#pragma once
#include "GraphNode.h"
#include "PhysX.h"
#include <vector>
#include "LightManager.h"

class CubePhysX;

class PhysicsMiniScene :
	public GraphNode
{
public:
	PhysicsMiniScene(void);
	virtual ~PhysicsMiniScene(void);
	/**
	* Check DirectInput key's
	*/
	virtual void Tick(float dTime);
	
	void OnTriggerEnter(Character* player);
private:
	void Build();

	CubePhysX* m_marioTrigger;
	LightManager* m_Lightmanager;

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	PhysicsMiniScene(const PhysicsMiniScene& t);
	PhysicsMiniScene& operator=(const PhysicsMiniScene& t);

};
