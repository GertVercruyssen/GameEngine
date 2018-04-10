#include "StdAfx.h"
#include "Character.h"
#include "PhysX.h"
#include "Input.h"
#include "Camera.h"
#include "PickableItem.h"
#include "NxController.h"
#define _USE_MATH_DEFINES
#include "math.h"

Character::Character(D3DXVECTOR3 start):m_PosW(start),m_RunningMultiplicationFactor(2.5f),m_Speed(10.0f),m_MouseScale(100),m_Radius(0.2f),m_Height(2),m_pCharacterMesh(NULL),m_HitState(false)
{
	m_pCamera = new Camera(start);
	m_pController = PHYSX.CreateController(this);
}

Character::~Character(void)
{
	delete m_pCamera;
	vector<PickableItem*>::iterator it;
	for ( it = m_Inventory.begin(); it != m_Inventory.end(); ++it){
		delete *it;
	}
}

tstring Character::GetContactShape()
{
	NxRay physray(NxVec3(m_PosW.x,m_PosW.y,m_PosW.z),NxVec3(0,-1.0f,0));
	NxShape* shape = PHYSX.GetClosestShape(physray,GetHeight() / 2);

	if(shape == NULL)
		return _T("shape found");
	else
		return _T("No shape");
}

bool Character::HasContact()
{
	NxRay physray(NxVec3(m_PosW.x,m_PosW.y,m_PosW.z),NxVec3(0,-1.0f,0));
	NxShape* shape = PHYSX.GetClosestShape(physray,GetHeight() / 2);

	if(shape == NULL)
		return false;
	else
		return true;
}

void Character::Move(float dt){
	if (!m_pCamera->IsActive())
		return;

	bool bRunning=false;
	// Find the net direction the camera is traveling in (since the
	// camera could be running and strafing).
	D3DXVECTOR3 dir(0.0f, 0.0f, 0.0f);
	if(INPUT.IsKeyDown('Z') )
		dir += m_pCamera->GetLook();
	if(INPUT.IsKeyDown('S') )
		dir -= m_pCamera->GetLook();
	if(INPUT.IsKeyDown('D') )
		dir += m_pCamera->GetRight();
	if(INPUT.IsKeyDown('Q') )
		dir -= m_pCamera->GetRight();
	if(INPUT.IsKeyDown(VK_LSHIFT) )
		bRunning = true;
	if(HasCharacterMesh())
	{
		if(INPUT.IsKeyDown(VK_LCONTROL))
		{
		m_pCharacterMesh->ActivateAnimation(_T("hit1"),0);
		m_HitState = true;
		}
	}
	//if(!m_pCharacterMesh->GetSkeleton()->IsAnimationPlaying()) m_HitState = false;

	
	// Move at mSpeed along net direction.
	D3DXVec3Normalize(&dir, &dir);
	float finalspeed = m_Speed;
	if(bRunning)finalspeed*=m_RunningMultiplicationFactor;
	//use dt so that speed is independent from framerate 
	dir *= finalspeed*dt;
	if ( dir.y > 0 )
		dir.y = 0;

	NxU32 flags;

	if ( dt > 0.001f)
	{
		if ( !HasContact() ){
			// add gravity code.
			D3DXVECTOR3 diff = m_PosW - m_PreviousPosW;

			float vy = diff.y / dt;

			vy += (-9.82f * dt);
			dir.y += vy * dt;
		}
	}

	if (HasCharacterMesh())
	{
		float dx = abs(m_PosW.x - m_PreviousPosW.x);
		float dz = abs(m_PosW.z - m_PreviousPosW.z);
		float dist = sqrt(dx*dx+dz*dz);
		if (!m_HitState)
		{
			if ( dist > .01f)
			{
				m_pCharacterMesh->ActivateAnimation(_T("walk"),0);
			}
			else
			{
				m_pCharacterMesh->ActivateAnimation(_T("idle"),0);
			}
		}
	}

	//** VUL AAN **//

	NxVec3 vector(dir.x,dir.y,dir.z);
	m_pController->move(vector + NxVec3(0,-.02f,0),1,.000001f,flags,0.5f);
	m_PreviousPosW = m_PosW;
	m_PosW.x = m_pController->getActor()->getGlobalPosition().x;
	m_PosW.y = m_pController->getActor()->getGlobalPosition().y;
	m_PosW.z = m_pController->getActor()->getGlobalPosition().z;

	D3DXVECTOR3 look = 2*m_pCamera->GetLook();
	D3DXVECTOR3 camera = D3DXVECTOR3(m_PosW.x-look.x,m_PosW.y+GetHeight()/2-look.y,m_PosW.z-look.z);

	float pitch = 0;
	float yAngle = 0;

	if(INPUT.IsLeftMouseButtonDown())
	{
		POINTS mouseMovement = INPUT.GetMouseMovement();
		float pitch = (float)mouseMovement.y / m_MouseScale;
		float yAngle = (float)mouseMovement.x / m_MouseScale;
		m_pCamera->Move(camera,true,pitch,yAngle);
	}
	else m_pCamera->Move(camera);

	if(HasCharacterMesh())
	{
		m_pCharacterMesh->ResetWorldMatrix();
		m_pCharacterMesh->Scale(0.1f,0.1f,0.1f);
		m_pCharacterMesh->Rotate(-(float)M_PI/2.0,0,0);

		D3DXVECTOR3 look = m_pCamera->GetLook();
		float yAngle = -atan2(look.z,look.x) - (float)M_PI/2;
		m_pCharacterMesh->Rotate(0,yAngle,0);

		D3DXVECTOR3 pos = m_PosW;
		pos.y -= GetHeight()/2;
		m_pCharacterMesh->Translate(pos);
	}
}

void Character::SetMouseScale(int scale)
{
	if(scale>1 && scale<1000)
		m_MouseScale = (float)scale;
}

bool Character::HasItem(int id)
{
	for (UINT i = 0; i<m_Inventory.size();++i)
	{
		if(m_Inventory[i]->GetID() == id)
			return true;
	}
	return false;
}
void Character::SetCharacterMesh(DX10ObjMesh* inputmesh)
{
	m_pCharacterMesh = inputmesh;
}
bool Character::HasCharacterMesh()
{
	if(m_pCharacterMesh == NULL)
		return false;
	else
		return true;
}