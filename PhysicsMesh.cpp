#include "StdAfx.h"
#include "PhysicsMesh.h"
#include "ObjPhysicsReader.h"
#include "D3D10DeviceWrapper.h"
#include "CookingStream.h"
#include "InputMesh.h"
#include "PhysX.h"
#include "DXWindow.h"

PhysicsMesh::PhysicsMesh(tstring filename, tstring physicsFileName, Type type) : Graph3DObject(), 
											m_filename(filename), 
											m_physicsFileName(physicsFileName), 
											m_Type(type), 
											m_pActor(NULL), 
											m_pVertexLayout(NULL),
											m_VertexBufferStride(0),
											m_bEnableAlphaBlend(false),
											m_shouldDraw(false)
{

	if (Init())
	{
		CreateEffect( _T("./Effect/checker.fx"));			//uit Graph3DObject
		GetEffectVariables();	
		DefineInputlayout();
		BuildVertexBuffers();
		BuildIndexBuffers();
		Rotate(-1.57079633f,0,0);
	}
}

PhysicsMesh::~PhysicsMesh(void)
{
	for (UINT i = 0; i<m_Meshes.size();++i)
		delete m_Meshes[i];

	for (UINT i = 0; i<m_pVertexBuffers.size();++i)
		m_pVertexBuffers[i]->Release();

	for (UINT i = 0; i<m_pVertexBuffers.size();++i)
		m_pIndexBuffers[i]->Release();
	
    if( m_pVertexLayout ) m_pVertexLayout->Release();
}

bool PhysicsMesh::Init()
{
	ObjPhysicsReader reader(m_filename);
	if(!reader.Read())
		return false;

	m_Meshes.assign(reader.GetInputMeshBegin(), reader.GetInputMeshEnd());

	if (m_Type == Concave)
		return BuildConcave();
	else
		return BuildConvex();
			
	return false;
}

bool PhysicsMesh::BuildConcave()
{
	string str = string(m_physicsFileName.begin(), m_physicsFileName.end());
	NxTriangleMesh *pTriangleMesh = PHYSX.GetSDK()->createTriangleMesh(UserStream(str.c_str(),true));

	//create triangle mesh instance
	NxTriangleMeshShapeDesc TriangleMeshShapeDesc;

	//calc mass, using AABB volume
	TriangleMeshShapeDesc.mass = 200;
	//set meshdata
	TriangleMeshShapeDesc.meshData  = pTriangleMesh; //See NxTriangleMesh

	NxActorDesc ActorDesc;
	ActorDesc.shapes.push_back(&TriangleMeshShapeDesc);
	ActorDesc.globalPose.t = NxVec3( 0,0,0 );
	ActorDesc.body=0;

	m_pActor = PHYSX.GetScene()->createActor(ActorDesc);

	return m_pActor != 0;
}

bool PhysicsMesh::BuildConvex()
{
	string str = string(m_physicsFileName.begin(), m_physicsFileName.end());
	NxConvexMesh *pConvexMesh = PHYSX.GetSDK()->createConvexMesh(UserStream(str.c_str(),true));

	//create triangle mesh instance
	NxConvexShapeDesc ConvexShapeDesc;

	//calc mass, using AABB volume
	ConvexShapeDesc.mass = 200;
	//set meshdata
	ConvexShapeDesc.meshData  = pConvexMesh; //See NxConvexMesh

	NxActorDesc ActorDesc;
	ActorDesc.shapes.push_back(&ConvexShapeDesc);
	ActorDesc.globalPose.t = NxVec3( 0,0,0 );
	ActorDesc.body=0;

	m_pActor = PHYSX.GetScene()->createActor(ActorDesc);

	return m_pActor != 0;
}

void PhysicsMesh::BuildVertexBuffers()
{
	for (MIT it = m_Meshes.begin(); it < m_Meshes.end();++it)
	{
		D3D10_BUFFER_DESC bd;
		bd.Usage = D3D10_USAGE_DEFAULT;
		bd.ByteWidth = sizeof( float3 ) * (*it)->GetNumVertices();
		bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D10_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = (*it)->GetVertexPointer();
		//create a ID3D10Buffer in graphics memory containing the vertex info
		ID3D10Buffer* pTempVertexBuffer;
		HR(GETD3D10DEVICE->CreateBuffer( &bd, &InitData, &pTempVertexBuffer ));

		m_pVertexBuffers.push_back(pTempVertexBuffer);
	}
}

void PhysicsMesh::BuildIndexBuffers()
{
	for (MIT it = m_Meshes.begin(); it < m_Meshes.end();++it)
	{
		int numIndices = (*it)->GetNumFaces()*3;
		m_numIndicesList.push_back(numIndices);

		D3D10_BUFFER_DESC bd;
		bd.Usage = D3D10_USAGE_DEFAULT;
		bd.ByteWidth = sizeof( int ) * numIndices;        
		bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D10_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = (*it)->GetTrianglesPointer();
		ID3D10Buffer* pTempIndexBuffer;
		HR(GETD3D10DEVICE->CreateBuffer( &bd, &InitData, &pTempIndexBuffer ));
		m_pIndexBuffers.push_back(pTempIndexBuffer);
	}
}

void PhysicsMesh::GetEffectVariables()
{
	m_pDefaultTechnique = m_pEffect->GetTechniqueByName("CheckersTech");
	m_pWorldViewProjectionVariable = m_pEffect->GetVariableByName("gWVP")->AsMatrix();
}

void PhysicsMesh::SetPhysXGlobalPose()
{
	GraphBase::CalculateFinalWorldMatrix();
	SetPhysXGlobalPose(m_FinalWorld);
}

void PhysicsMesh::SetPhysXGlobalPose(const D3DXMATRIX& World)
{
	NxMat34 mat;
	mat.setColumnMajor44(World);
	if(m_pActor !=0) m_pActor->setGlobalPose(mat);
}

void PhysicsMesh::DefineInputlayout()
{
	// Define the input layout
	D3D10_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT numElements = sizeof(layout)/sizeof(layout[0]);

	// Create the input layout
	D3D10_PASS_DESC PassDesc;
	// Get the pass decriptor from the effect technique
	m_pDefaultTechnique->GetPassByIndex(0)->GetDesc( &PassDesc );
	HR(GETD3D10DEVICE->CreateInputLayout(layout, numElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &m_pVertexLayout));

	//important set the size of the vertexbuffer, is used in draw base member function
	m_VertexBufferStride = sizeof(float3);
}

void PhysicsMesh::UpdateEffectVariables()const
{
	m_pWorldViewProjectionVariable->SetMatrix((float*)&(m_World * m_View * m_Projection));
}

void PhysicsMesh::Tick(float dTime)
{
}

void PhysicsMesh::OnResize()
{
	//window client area has changed, recalculate projection matrix
	D3DXMatrixPerspectiveFovLH(&m_Projection,(float)(D3DX_PI *.25), (float)((float)DXWINDOW->GetWidth()/(float)DXWINDOW->GetHeight()),1.0f,100.0f);
}

void PhysicsMesh::Draw(const ID3D10EffectTechnique* pTechnique) const
{
	if(m_shouldDraw)
	{
		ASSERT(m_pVertexBuffers.size() == m_pIndexBuffers.size());
		ASSERT(m_pVertexBuffers.size() == m_numIndicesList.size());

		if(!pTechnique)
		{
			MessageBox(0,_T("No Technique"),_T("ERROR"),0);
			return;
		}
		//if alphablending is enabled, set the state
		D3D10DEVICEWRAPPER->EnableAlphaBlend(m_bEnableAlphaBlend);

		//Every object has it's own set of eg worldmatrix
		UpdateEffectVariables();

		// Set the input layout
		GETD3D10DEVICE->IASetInputLayout( m_pVertexLayout );

		// Set primitive topology
		GETD3D10DEVICE->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

		for (UINT i = 0; i< m_pVertexBuffers.size(); ++i)
		{
			// Set vertex buffer
			UINT offset = 0;
			GETD3D10DEVICE->IASetVertexBuffers( 0, 1, &m_pVertexBuffers[i], &m_VertexBufferStride, &offset );

			// Set index buffer
			GETD3D10DEVICE->IASetIndexBuffer( m_pIndexBuffers[i], DXGI_FORMAT_R32_UINT, 0 );

			//
			// Renders a ShapeBase
			//
			D3D10_TECHNIQUE_DESC techDesc;
			(const_cast<ID3D10EffectTechnique*>(pTechnique))->GetDesc( &techDesc );
			for( UINT p = 0; p < techDesc.Passes; ++p )
			{
				(const_cast<ID3D10EffectTechnique*>(pTechnique))->GetPassByIndex( p )->Apply(0);
				GETD3D10DEVICE->DrawIndexed( m_numIndicesList[i], 0, 0 );
			}
		}
		//disable alpha blending
		D3D10DEVICEWRAPPER->EnableAlphaBlend(false);
	}
}

void PhysicsMesh::Draw()const
{
	if(m_shouldDraw)
		Draw(m_pDefaultTechnique);
}