#include "StdAfx.h"
#include "CubePhysX.h"
#include "PhysX.h"
const int CubePhysX::RIGID=1,CubePhysX::STATIC=2,CubePhysX::GHOST=0;

CubePhysX::CubePhysX(float width, float height, float depth, D3DXCOLOR color,int bodyType):
	ShapeColorNormalCube(width,height,depth,color),
	m_pActor(0)
{
	SetBodyType(bodyType);
}

CubePhysX::~CubePhysX(void)
{
}

void CubePhysX::AddForce(const D3DXVECTOR3& pos)
{
	NxVec3 v(pos.x,pos.y,pos.z);
	m_pActor->addForce(v);
}

void CubePhysX::Tick(float dTime)
{
	//Get world matrix from PhysX engine 
	if(m_pActor != 0)
		m_pActor->getGlobalPose().getColumnMajor44(m_World);
	//call baseclass tick
	ShapeColorNormalCube::Tick(dTime);
}

void CubePhysX::SetPhysXGlobalPose()
{
	GraphBase::CalculateFinalWorldMatrix();
	SetPhysXGlobalPose(m_FinalWorld);
}

void CubePhysX::SetPhysXGlobalPose(const D3DXMATRIX& World)
{
	NxMat34 mat;
	mat.setColumnMajor44(World);
	if(m_pActor !=0 )m_pActor->setGlobalPose(mat);
}

void CubePhysX::SetBodyType(int bodyType)
{
	switch(bodyType)
	{
	case RIGID:
		InitActor(bodyType);
		break;
	case STATIC:
		InitActor(bodyType);
		break;
	case GHOST:
		InitActor(bodyType);
		break;
	default:
		;
	}
}

void CubePhysX::InitActor(int BodyType)
{
	//release actor 
	if(m_pActor) PHYSX.GetScene()->releaseActor(*m_pActor);
	NxActorDesc ActorDesc;

	NxBoxShapeDesc BoxShapeDesc;
	BoxShapeDesc.dimensions=NxVec3(m_Width/2,m_Height/2,m_Depth/2);//is waarde van HALVE lengte!!
	BoxShapeDesc.mass = m_Width*m_Height*m_Depth;
	ActorDesc.shapes.push_back(&BoxShapeDesc);	
	
	if ( BodyType == GHOST ){
		BoxShapeDesc.shapeFlags |= NX_TRIGGER_ENABLE;
	}else{
		if(BodyType==RIGID)
		{
		//bodyNx
			NxBodyDesc bodyDesc;
			bodyDesc.angularDamping	= 0.9f;
			bodyDesc.linearVelocity = NxVec3(0,0,0);
			ActorDesc.body = &bodyDesc;
		}
		else ActorDesc.body =0;
		// density only needed for static or rigid physic boxes
		ActorDesc.density = 10.0f;	
	}

	//actor
	NxMat34 mat;
	mat.setColumnMajor44(m_World);
	ActorDesc.globalPose = mat;
	ActorDesc.density = 10.0f;
	m_pActor = PHYSX.GetScene()->createActor(ActorDesc);
}
void CubePhysX::OnTriggerEnter(Character* player)
{
	
}