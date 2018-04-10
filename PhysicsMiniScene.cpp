#include "StdAfx.h"
#include "PhysicsMiniScene.h"
#include "D3D10DeviceWrapper.h"
#include "CubePhysX.h"
#include "Input.h"
#include "PhysicsMesh.h"
#include "PickableItem.h"
#include "Door.h"

#include "ObjReader.h"
#include "DX10ObjMesh.h"
#include "MeshList.h"
#include "IniFile.h"
#include "LightReader.h"
#include "ShapeColorNormalCylinder.h"
#include "GP2_Game_App.h"
#include "Character.h"
#include "AnimationReader.h"

#define _USE_MATH_DEFINES
#include "math.h"

PhysicsMiniScene::PhysicsMiniScene(void) : GraphNode(), m_marioTrigger(0)
{
	Build();
}

PhysicsMiniScene::~PhysicsMiniScene(void)
{
	if (m_marioTrigger != NULL) delete m_marioTrigger;	
	m_Lightmanager->GetSingleton().RemoveAllLights();
}

void PhysicsMiniScene::Build()
{
	// Load Floor
	CubePhysX *floor = new CubePhysX(20.0f,0.2f,20.0f,D3DXCOLOR(0.9f,0.9f,0.1f,1.0f),CubePhysX::STATIC);
	floor->Translate(0,-0.1f,0);
	AddChild(floor);

	// Load concave model
	PhysicsMesh *concavemesh = new PhysicsMesh(_T("./Models/level.obj"), _T("./Models/level.concavebin"), PhysicsMesh::Concave);
	concavemesh->Translate(0,0,0);
	AddChild(concavemesh);

	// Load convex model
	PhysicsMesh *convexmesh = new PhysicsMesh(_T("./Models/box.obj"), _T("./Models/box.convexbin"), PhysicsMesh::Convex);
	convexmesh->Translate(0,-20,0);
	AddChild(convexmesh);

	// Load box
	CubePhysX *box = new CubePhysX(1,1,1,D3DXCOLOR(0.9f,0.9f,0.1f,1.0f),CubePhysX::RIGID);
	box->Translate(17,15,1);
	AddChild(box);

	// Load door
	Door* door = new Door(true,D3DXVECTOR3(-0.0f,13.2f,1.0f),0,1.5f,3.0f,0.2f);
	AddChild(door);

	// Load green key ( ;-) )
	PickableItem* item = new PickableItem(1,D3DXVECTOR3(5,2,7),true);
	AddChild(item);

	// Load mario trigger
	m_marioTrigger = new CubePhysX(4.5,7,20,D3DXCOLOR(0,0,1,1),CubePhysX::GHOST);
	m_marioTrigger->Translate(D3DXVECTOR3(-0.5f,47,1));
	NxActor* actor = m_marioTrigger->GetActor();
	actor->userData = this;

	IniFile* config = new IniFile();
	config->SetValue(_T("Export"),_T("baseDir"),_T("./Models"));
	config->SetValue(_T("Export"),_T("meshDir"),_T("meshes"));
	config->SetValue(_T("Export"),_T("textureDir"),_T("textures"));
	config->SetValue(_T("Export"),_T("skeletonDir"),_T("rigs"));
	ObjReader reader(config,_T("gebouw.obj"));
	bool success = reader.Read();

	ObjMesh* gebouw = reader.GetMesh(_T("gebouw"));
	DX10ObjMesh* dx10Gebouw = D3D10DEVICEWRAPPER->Compile(gebouw);
	AddChild(dx10Gebouw);

	LightReader lr(_T("./Models/meshes/lights.xml"));
	lr.Read();

	dx10Gebouw->AddSpotLights(lr.GetLights());

	ObjReader reader2(config,_T("characters/Elf08.obj"));
	success = reader2.Read();

	ObjMesh* female = reader2.GetMesh(_T("CHA_Avatar_02"));
	DX10ObjMesh* dx10Female = D3D10DEVICEWRAPPER->Compile(female);
	dx10Female->AddSpotLights(lr.GetLights());
	AddChild(dx10Female);
	dx10Female->Scale(0.1f,0.1f,0.1f);
	dx10Female->Translate(-2,17,-2);

	GP2_Game_App * app = GP2_Game_App::GetSingleton();
	Character* player = app->GetCharacter();
	player->SetCharacterMesh(dx10Female);

	AnimationReader aridle(_T("./Models/anims/characters/elfm/idle.anim"));
	aridle.Read();
	AnimationReader arwalk(_T("./Models/anims/characters/elfm/walk.anim"));
	arwalk.Read();
	AnimationReader arhit(_T("./Models/anims/characters/elfm/hit1.anim"));
	arhit.Read();
	Animation* pWalkAnim = arwalk.GetAnimation();
	Animation* pIdleAnim = aridle.GetAnimation();
	Animation* pHitAnim = arhit.GetAnimation();
	dx10Female->AddAnimation(pIdleAnim);
	dx10Female->AddAnimation(pWalkAnim);
	dx10Female->AddAnimation(pHitAnim);

	ObjReader reader3(config,_T("slijkeling.obj"));
	success = reader3.Read();
	ObjMesh* male = reader3.GetMesh(_T("slijkie"));
	DX10ObjMesh* dx10Male = D3D10DEVICEWRAPPER->Compile(male);
	dx10Male->AddSpotLights(lr.GetLights());
	AddChild(dx10Male);
	dx10Male->Scale(0.15f,0.15f,0.15f);
	dx10Male->Translate(-2.11f,11.6f,-0.8f);
	
	ObjReader reader4(config,_T("emmer.obj"));
	success = reader4.Read();
	if ( success )
	{
		ObjMesh* prop = reader4.GetMesh(_T("Box01"));
		DX10ObjMesh* wineBarrel = D3D10DEVICEWRAPPER->Compile(prop);
		AddChild(wineBarrel);
		wineBarrel->AddSpotLights(lr.GetLights());
		
		wineBarrel->Translate(0,0.5f,0);
		delete prop;
	}

	delete config;
	delete gebouw;
	delete male;
	delete female;
}

void PhysicsMiniScene::Tick(float dTime)
{
	//call baseclass method
	GraphNode::Tick(dTime);
	m_marioTrigger->Tick(dTime);
}

void PhysicsMiniScene::OnTriggerEnter(Character* player)
{
	MessageBox(0, _T("Sorry Mario, but the princess is in another castle!"), _T("SORRY MARIO"), 0);
}