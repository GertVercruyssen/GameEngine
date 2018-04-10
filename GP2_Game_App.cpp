//=============================================================================
// GP2_Game_App.cpp by Bart Uyttenhove (C) 2008 All Rights Reserved.
//
// Demonstrates Direct3D Initialization and spinning GP2_Game_App using the 
// framework code.
//=============================================================================

#include "StdAfx.h"
#include "D3DUtil.h"
#include "GP2_Game_App.h"
#include "D3D10DeviceWrapper.h"
#include "DXWindow.h"
#include <algorithm>

#include "Camera.h"
#include "GraphCamera.h"
#include "Input.h"
#include "PhysicsMiniScene.h"
#include "SkyBox.h"
#include "Character.h"
#ifdef _DEBUG
	#include "PhysicsDebugRenderer.h"
#endif

GP2_Game_App* GP2_Game_App::m_pGP2_Game_App=0;

GP2_Game_App *GP2_Game_App::GetSingleton()
{
	if(m_pGP2_Game_App==0)m_pGP2_Game_App = new GP2_Game_App();
	return m_pGP2_Game_App;
}

GP2_Game_App::GP2_Game_App():
					m_pBaseNode(0),
					m_Angle(0),
					m_pPlayer(0),
					m_pGraphCamera(0),
					m_iCameraNumber(0),
					m_iNumCameras(2),
					m_PostQuad(0),
					m_TexTarget(0)
#ifdef _DEBUG
					,
					m_pPhysicsDebugRenderer(0),
					m_renderstate(Normal)
#endif
{
}

GP2_Game_App::~GP2_Game_App()
{
	SAFE_DELETE(m_pBaseNode);
	SAFE_DELETE(m_pPlayer);
	SAFE_DELETE(m_PostQuad);
	SAFE_DELETE(m_TexTarget);
#ifdef _DEBUG
	SAFE_DELETE(m_pPhysicsDebugRenderer);
#endif
}

void GP2_Game_App::SetWindowParams()
{
	AbstractDXGame::SetWindowParams();
	DXWINDOW->SetWidth(900);
	DXWINDOW->SetHeight(700);
	DXWINDOW->SetTitle(_T("GP2_Game_App"));
}

bool GP2_Game_App::Init()
{
	PHYSX.Init();
	m_pPlayer = new Character(D3DXVECTOR3(0,18,0));
#ifdef _DEBUG
	m_pPhysicsDebugRenderer = new PhysicsDebugRenderer();
#endif

	m_pBaseNode = new GraphNode();
	m_pBaseNode->AddChild(new PhysicsMiniScene());
	m_pBaseNode->AddChild(new SkyBox(_T("./textures/skybox/"),100,100,100));

	m_pGraphCamera = new GraphCamera();
	m_pGraphCamera->LookAt(D3DXVECTOR3(4,4,-4),D3DXVECTOR3(0,2,0),D3DXVECTOR3(0,1,0));
	m_pBaseNode->AddChild(m_pGraphCamera);

	DirLight light;
	light.DirW=D3DXVECTOR3(1,-0.1f,2);
	light.Diffuse = light.Ambient = light.Specular = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	Graph3DObject::SetLight(light);

	m_TexTarget = new DrawableTex2D();
	m_TexTarget->Init(800,600,true,DXGI_FORMAT_R8G8B8A8_UNORM);
	m_PostQuad = new FullscreenQuad(_T("./textures/t1.BMP"));
	m_PostQuad->SetTexture(m_TexTarget->GetColorMap());

	return true;
}

void GP2_Game_App::OnResize()
{
	AbstractDXGame::OnResize();
	m_pBaseNode->OnResize();

	//Use new window client window dimensions to calc new aspect ratio
	m_pPlayer->GetCamera()->OnResize();
	
	m_TexTarget->Init(800,600,true,DXGI_FORMAT_R8G8B8A8_UNORM);
	m_PostQuad->SetTexture(m_TexTarget->GetColorMap());
}

void GP2_Game_App::Paint(float dTime)
{
	if(dTime>0.3)dTime=0;//prevent dTime from getting too large

	//read mouse and keyboard
	INPUT.Tick(dTime);

	//adjust cam position according to user input
	m_pPlayer->Move(dTime);
	//Get view and projection from active camera and set it to static Graph3DObject class
	SetVPMatricesFromActiveCamera();

	//laat physics volgende stap berekenen (in thread)
	PHYSX.Simulate(dTime);
	
	//fetch  results of physics calculations
	PHYSX.FetchResults();

	//PhysX nodes retrieve World matrix from PhysX in Tick()
	UpdateScene(dTime);

	//All nodes calc their Final World Matrix
	m_pBaseNode->CalculateFinalWorldMatrix();

	//Draw the scene
	DrawScene();
}

void GP2_Game_App::UpdateScene(float dTime)
{	
	//update scenegraph
	m_pBaseNode->Tick(dTime);
}

void GP2_Game_App::SetVPMatricesFromActiveCamera()
{
	//Tick Camera
	//camera has calculated new view and projection matrix
	if(m_iCameraNumber==0 && m_pPlayer->GetCamera())
	{
		Graph3DObject::SetView(&m_pPlayer->GetCamera()->GetView());
		Graph3DObject::SetProjection(&m_pPlayer->GetCamera()->GetProj());
	}
	//Get View and Projection from graphcamera
	if(m_iCameraNumber==1 && m_pGraphCamera)
	{
		Graph3DObject::SetView(&m_pGraphCamera->GetView());
		Graph3DObject::SetProjection(&m_pGraphCamera->GetProj());
	}
}

void GP2_Game_App::DrawScene()
{
	m_TexTarget->Begin();

#ifdef _DEBUG
	if (m_renderstate == Normal || m_renderstate == MixedMode)
#endif
		// Draw the scene onto the backbuffer
		m_pBaseNode->Draw();
#ifdef _DEBUG
	if (m_renderstate == DebugPhysics || m_renderstate == MixedMode)
		// Draw the physics as debug lines 
		m_pPhysicsDebugRenderer->Draw();
#endif

#ifdef _DEBUG
	//now draw 2D
	int ypos=10;
	D3D10DEVICEWRAPPER->DrawString(DXWINDOW->GetFrameStats(),10,ypos);
	ypos+=40;

	D3DXVECTOR3 position = m_pPlayer->GetCamera()->GetPos();
	tstringstream spos;
	spos << _T("Position: [") << position.x <<_T(", ")<< position.y << _T(", ") << position.z << _T("]");
	D3D10DEVICEWRAPPER->DrawString(spos.str(),10,ypos);
	ypos+=15;

	D3DXVECTOR3 look = m_pPlayer->GetCamera()->GetLook();
	tstringstream slook;
	slook << _T("Look: [") << look.x <<_T(", ")<< look.y << _T(", ") << look.z << _T("]");
	D3D10DEVICEWRAPPER->DrawString(slook.str(),10,ypos);
#endif
	m_TexTarget->End();


	D3D10DEVICEWRAPPER->ResetRenderTargets();

	// Just clear the backbuffer
	D3D10DEVICEWRAPPER->Clear();
	// Restore default states, input layout and primitive topology 
	// because mFont->DrawText changes them.  Note that we can 
	// restore the default states by passing null.
	GETD3D10DEVICE->OMSetDepthStencilState(0, 0);
	float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
	GETD3D10DEVICE->OMSetBlendState(0, blendFactors, 0xffffffff);

	m_PostQuad->Draw();

	D3D10DEVICEWRAPPER->Present();
}

void GP2_Game_App::KeyPressed(WPARAM cKey)
{
	switch (cKey)
	{
		case 'C':
			++m_iCameraNumber%=m_iNumCameras;
			if(m_iCameraNumber==0)
			{
				m_pPlayer->GetCamera()->SetActive(true);
				m_pGraphCamera->SetActive(false);
			}
			if(m_iCameraNumber==1)
			{
				m_pPlayer->GetCamera()->SetActive(false);
				m_pGraphCamera->SetActive(true);
			}
			break;
#ifdef _DEBUG
		// Toggle debug physics
		case 'P':
			if (m_renderstate == Normal)
				m_renderstate = MixedMode;
			else if (m_renderstate == MixedMode)
				m_renderstate = DebugPhysics;
			else
				m_renderstate = Normal;
			break;
#endif
	}
}	
Character* GP2_Game_App::GetCharacter()
{
	return m_pPlayer;
}