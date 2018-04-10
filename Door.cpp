#include "StdAfx.h"
#include "Door.h"
#include "PhysX.h"
#include "CubePhysX.h"
#include "NxRevoluteJoint.h"
#include "Character.h"

Door::Door(bool trigger,D3DXVECTOR3 location, float yRot, float width, float height,float thickness):
m_UseTrigger(trigger),
m_Location(location),
m_YRot(yRot),
m_Width(width),
m_Height(height)

{
	m_pDoorMesh = new CubePhysX(width,height,thickness,D3DXCOLOR(1,1,1,1),CubePhysX::RIGID);
	m_pDoorMesh->Rotate(0,m_YRot,0);
	m_pDoorMesh->Translate(location);
	m_pDoorTrigger = new CubePhysX(width,height,20,D3DXCOLOR(0,0,1,1),CubePhysX::GHOST);
	m_pDoorTrigger->Translate(location);
	m_pDoorTrigger->Rotate(0,yRot,0);
	NxActor* actor = m_pDoorTrigger->GetActor();
	actor->userData = this;

	NxVec3 axisLocation(location.x - width/2,location.y + height/2,location.z);
	NxVec3 axisDirection(0,1,0);
	NxRevoluteJointDesc revDesc;
	revDesc.actor[0] = m_pDoorMesh->GetActor();
	revDesc.actor[1] = 0;
	revDesc.setGlobalAxis(axisDirection);
	//The direction of the axis the bodies revolve around.
	revDesc.setGlobalAnchor(axisLocation);
	revDesc.name = "hinge";
	revDesc.motor.maxForce = 1000;
	revDesc.motor.velTarget = 0;
	revDesc.limit.low.value = 0.0f;
	revDesc.limit.high.value = NxPi/2.0f;
	revDesc.flags |= NX_RJF_MOTOR_ENABLED | NX_RJF_LIMIT_ENABLED;
	m_pHinge = (NxRevoluteJoint *)PHYSX.GetScene()->createJoint(revDesc);
}

Door::~Door(void)
{
	delete m_pDoorMesh;
	delete m_pDoorTrigger;
}

void Door::CalculateFinalWorldMatrix()
{
	m_pDoorMesh->CalculateFinalWorldMatrix();
	m_pDoorTrigger->CalculateFinalWorldMatrix();
}

void Door::OnResize()
{
	m_pDoorMesh->OnResize();
	m_pDoorTrigger->OnResize();
}

void Door::Tick(float dTime)
{
	m_pDoorMesh->Tick(dTime);
	m_pDoorTrigger->Tick(dTime);
}
	
void Door::Draw() const
{
	m_pDoorMesh->Draw();
	//m_pDoorTrigger->Draw();
}
	
void Door::Draw(const ID3D10EffectTechnique* pTechnique) const
{
	m_pDoorMesh->Draw(pTechnique);
	//m_pDoorTrigger->Draw(pTechnique);
}

/**
* Sets the Physics Global pos.
* @World: een WorldMatrix
*/
void Door::SetPhysXGlobalPose(const D3DXMATRIX& World)
{
}
void Door::SetPhysXGlobalPose()
{
}

void Door::OnTriggerEnter(Character* player)
{
	if ( player->HasItem(1) ) {
	NxMotorDesc desc;
	desc.velTarget= 1.20f;
	desc.maxForce = 1000;
	m_pHinge->setMotor(desc);
	}
}
void Door::OnTriggerExit(Character* player)
{
	if (player->HasItem(1)) {
	NxMotorDesc desc;
	desc.velTarget= -1.20f;
	desc.maxForce = 1000;
	m_pHinge->setMotor(desc);
	}
}