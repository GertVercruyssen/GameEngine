#pragma once

////PhysX:you must suppress its definition of the macros min and max, as these are common C++ method names in the SDK
//#define NOMINMAX  --> move to stdafx.h !!
//#include <windows.h>

#include "NxPhysics.h"
#include "ControllerManager.h"
//#include "NxUserTriggerReport.h"
using namespace std;
#include <vector>

class Character;

#define PHYSX (PhysX::GetSingleton())

class DAEAllocator : public NxUserAllocator
{
public:        
    void * malloc(NxU32 size)
    {
        return ::malloc(size);
    }
    
    void * mallocDEBUG(NxU32 size,const char *fileName, int line)
    {
        return ::_malloc_dbg(size,_NORMAL_BLOCK, fileName, line);
    }
    
    void * realloc(void * memory, NxU32 size)
    {
        return ::realloc(memory,size);
    }
    
    void free(void * memory)
    {
		::free(memory);
    }
};

class PhysX : public NxUserTriggerReport
{
private:
public:
	virtual ~PhysX( );


public:
	// Static methods
	static PhysX&  GetSingleton();
	bool Init(void);
	void Simulate(float dt);
	void FetchResults(void);

	/**
	* Get the pointer of the physics scene
	*/
	NxScene *GetScene() const {return m_pScene;}

	/**
	* Get the pointer of the physics sdk
	*/
	NxPhysicsSDK* GetSDK() const {return m_pPhysicsSDK;}

	/**
	* Creates a new character controller at the specified location
	* @param player the player character.
	*/
	NxController *CreateController(Character* player);

	/**
	* Gets the closest shape 
	* @param ray the ray to check.
	* @param distance the maximum distance for the shape.
	* @return the found shape
	*/
	NxShape *GetClosestShape(NxRay& ray,float distance);

	/**
	* Implementation of the NxUserTriggerReport class.
	* Called when a shape is triggered.
	* @param triggerShape the shape of the trigger.
	* @param otherShape the shape that triggered the trigger.
	* @param status the kind of trigger event.
	*/
	virtual void onTrigger(NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status);

private:
	PhysX();//singleton constructor

	NxPhysicsSDK *m_pPhysicsSDK;
	NxScene *m_pScene;

	NxControllerManager* m_pControllerManager;
	DAEAllocator* m_pAllocator;
	vector<NxController*> m_Controllers;

private:
	PhysX(const PhysX& t);
	PhysX& operator=(const PhysX& t);

};
