#include "StdAfx.h"
#include "PhysX.h"
#include "NxShape.h"
#include "NxCapsuleController.h"
#include "Graph3DObject.h"
#include "Character.h"
#pragma comment(lib, "PhysXLoader.lib")
#pragma comment(lib, "NxCharacter.lib")


PhysX& PhysX::GetSingleton()
{
	static PhysX oPhysX;
	return oPhysX;
}

PhysX::PhysX():m_pPhysicsSDK(0),m_pScene(0)
{

}

PhysX::~PhysX(void)
{
	NxReleaseControllerManager(m_pControllerManager);
	if (m_pAllocator) delete m_pAllocator;
	if(m_pScene)m_pPhysicsSDK->releaseScene(*m_pScene);
	if(m_pPhysicsSDK)m_pPhysicsSDK->release();
}
bool PhysX::Init(void)
{

	//create PhysicsSDK object
	m_pPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, NULL, NULL);
	if(!m_pPhysicsSDK)
	{
		OutputDebugString(_T("Wrong SDK DLL version?"));
		return false;
	}
	m_pPhysicsSDK->getFoundationSDK().getRemoteDebugger()->connect ("localhost", 5425);
	m_pPhysicsSDK->setParameter(NX_VISUALIZATION_SCALE,1.0f);
    m_pPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES,1);
	//create a scene object
	NxSceneDesc sceneDesc;
	sceneDesc.gravity.set(0,-9.81f,0);
	m_pScene = m_pPhysicsSDK->createScene(sceneDesc);
	if(!m_pScene)
	{
		OutputDebugString(_T("Scene creation failed"));
		return false;
	}
	// Set default material
	NxMaterial* defaultMaterial = m_pScene->getMaterialFromIndex(0);
	defaultMaterial->setRestitution(0.5f);
	defaultMaterial->setStaticFriction(0.6f);
	defaultMaterial->setDynamicFriction(0.2f);

	// Create ground plane
	NxPlaneShapeDesc planeDesc;
	NxActorDesc actorDesc;
	actorDesc.shapes.pushBack(&planeDesc);
	m_pScene->createActor(actorDesc);

	NxReal myTimestep = 1/60.0f;
	m_pScene->setTiming(myTimestep/4.0f,4,NX_TIMESTEP_FIXED);//4 substeps
	m_pScene->setUserTriggerReport(this);

	m_pAllocator = new DAEAllocator();
	m_pControllerManager = NxCreateControllerManager(m_pAllocator);

	return true;
	
}
//fixed is best
void PhysX::Simulate(float dt)
{
	NxReal myTimestep = dt;//1/60.0f;
	m_pScene->simulate(myTimestep);//thread start met rekenen, er is een soort back en frontbuffer
}

void PhysX::FetchResults(void)
{
	m_pScene->flushStream();
	//do stuff met resultaten van vorige berekeningen??
	m_pScene->fetchResults(NX_RIGID_BODY_FINISHED,true);//wachten tot de physX berekeningen gedaan zijn
}

NxController * PhysX::CreateController(Character* player)
{
	NxCapsuleControllerDesc desc;
    desc.radius = player->GetRadius();
	desc.height = player->GetHeight();
	D3DXVECTOR3 pos = player->GetPosition();
	desc.position = NxExtendedVec3( pos.x, pos.y, pos.z);
	desc.climbingMode = CLIMB_EASY ;
	desc.upDirection = NX_Y;

	NxController* c = m_pControllerManager->createController(m_pScene, desc);

	c->setCollision(true);
	c->getActor()->setGroup(1);
	c->getActor()->userData=player;
	c->getActor()->setName("character");

	m_Controllers.push_back(c);
	return c;
}

NxShape *PhysX::GetClosestShape(NxRay& ray,float distance){
	NxRaycastHit hit;
	NxShape* result = m_pScene->raycastClosestShape(ray,NX_STATIC_SHAPES,hit,1,distance);
	return result;
}

void PhysX::onTrigger(NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status)
{
	NxActor& triggerActor = triggerShape.getActor();
	NxActor& otherActor = otherShape.getActor();
	if (triggerActor.userData == NULL || otherActor.userData == NULL)
		return;
	Character* player = (Character*)otherActor.userData;
	GraphBase* object = (GraphBase*)triggerActor.userData;
	
	if(status & NX_TRIGGER_ON_ENTER)
	{	
		object->OnTriggerEnter(player);
	}
	else if(status & NX_TRIGGER_ON_LEAVE)
	{
		object->OnTriggerExit(player);
	}
}
