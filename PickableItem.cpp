#include "StdAfx.h"
#include "PickableItem.h"
#include "ShapeColorNormalCube.h"
#include "CubePhysX.h"
#include "NxActor.h"
#include "Character.h"
#include "GraphNode.h"
#include "NxShape.h"

PickableItem::PickableItem(int id,D3DXVECTOR3 position,bool rotate):m_ID(id),m_Position(position),m_Rotate(rotate),m_yRot(0)
{
	m_pObject = new ShapeColorNormalCube();
	m_pObject->Translate(m_Position);
	m_pTrigger = new CubePhysX(1.0f,1.0f,1.0f,D3DXCOLOR(0.2f,0.5f,0.8f,1.30f),CubePhysX::GHOST);
	m_pTrigger->ResetWorldMatrix();
	m_pTrigger->Translate(position);
	m_pTrigger->GetActor()->userData = this;
}

PickableItem::~PickableItem(void)
{
	delete m_pObject;
	delete m_pTrigger;
}

void PickableItem::CalculateFinalWorldMatrix()
{
	m_FinalWorld = m_World;
	m_pObject->CalculateFinalWorldMatrix();
}

void PickableItem::OnResize()
{
	m_pObject->OnResize();
}

void PickableItem::Tick(float dTime)
{
	m_pObject->ResetWorldMatrix();
	m_pObject->Rotate(0,1.0f*dTime,0);
	m_pObject->Translate(m_Position);
}
	
void PickableItem::Draw() const
{
	m_pObject->Draw();
}
	
void PickableItem::Draw(const ID3D10EffectTechnique* pTechnique) const
{
	m_pObject->Draw(pTechnique);
}

void PickableItem::SetPhysXGlobalPose(const D3DXMATRIX& World)
{
}
void PickableItem::SetPhysXGlobalPose()
{
}

void PickableItem::OnTriggerEnter(Character* player)
{
	player->AddToInventory(this);

	GraphNode * pParent = (GraphNode*)m_pParent;
	pParent->RemoveChild ( this );
	// object mag nu niet meer triggeren.
	DisableTrigger();
}

void PickableItem::OnTriggerExit(Character* player)
{
	
}

void PickableItem::DisableTrigger()
{
	m_pTrigger->GetActor()->raiseActorFlag(NX_AF_DISABLE_COLLISION);
}