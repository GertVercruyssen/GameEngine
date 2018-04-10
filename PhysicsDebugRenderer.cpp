#include "StdAfx.h"
#include "PhysX.h"
#include "PhysicsDebugRenderer.h"
#include "D3D10DeviceWrapper.h"
#include "DXWindow.h"
#include "float3.h"

#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)

PhysicsDebugRenderer::PhysicsDebugRenderer(void) : Graph3DObject(),
													m_pVertexBuffer(NULL),
													m_pVertexLayout(NULL),
													m_pWorldViewProjectionVariable(NULL),
													m_VertexBufferStride(0),
													m_vertexbuffersize(10)
{
	Init();
	PHYSX.GetSDK()->setParameter(NX_VISUALIZATION_SCALE, 1.0f);
	PHYSX.GetSDK()->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
	PHYSX.GetSDK()->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);
	PHYSX.GetSDK()->setParameter(NX_VISUALIZE_BODY_AXES , 1);
	PHYSX.GetSDK()->setParameter(NX_VISUALIZE_BODY_JOINT_GROUPS, 1);
	PHYSX.GetSDK()->setParameter(NX_VISUALIZE_BODY_MASS_AXES, 1);
	PHYSX.GetSDK()->setParameter(NX_VISUALIZE_COLLISION_AABBS, 1);
	PHYSX.GetSDK()->setParameter(NX_VISUALIZE_COLLISION_AXES, 1);
	PHYSX.GetSDK()->setParameter(NX_VISUALIZE_COLLISION_CCD , 1);
	PHYSX.GetSDK()->setParameter(NX_VISUALIZE_COLLISION_COMPOUNDS, 1);
	PHYSX.GetSDK()->setParameter(NX_VISUALIZE_COLLISION_DYNAMIC, 1);
	PHYSX.GetSDK()->setParameter(NX_VISUALIZE_COLLISION_EDGES, 1);
	PHYSX.GetSDK()->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
	PHYSX.GetSDK()->setParameter(NX_VISUALIZE_COLLISION_SPHERES, 1);
	PHYSX.GetSDK()->setParameter(NX_VISUALIZE_COLLISION_STATIC, 1);
	PHYSX.GetSDK()->setParameter(NX_VISUALIZE_FORCE_FIELDS, 1);
	PHYSX.GetSDK()->setParameter(NX_VISUALIZE_JOINT_LOCAL_AXES, 1);
	PHYSX.GetSDK()->setParameter(NX_VISUALIZE_JOINT_LIMITS, 1);
	PHYSX.GetSDK()->setParameter(NX_VISUALIZE_WORLD_AXES, 1);
}

PhysicsDebugRenderer::~PhysicsDebugRenderer(void)
{
	if( m_pVertexLayout ) m_pVertexLayout->Release();
	if( m_pVertexBuffer ) m_pVertexBuffer->Release();
}

bool PhysicsDebugRenderer::Init()
{
	CreateEffect( _T("./Effect/physics.fx"));
	GetEffectVariables();	
	DefineInputlayout();
	BuildVertexBuffer();

	return true;
}

void PhysicsDebugRenderer::BuildVertexBuffer()
{
	m_vertices.reserve(m_vertexbuffersize);
	if (m_pVertexBuffer) m_pVertexBuffer->Release();
	D3D10_BUFFER_DESC bd;
	bd.Usage = D3D10_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof( float3 ) * m_vertexbuffersize;
	bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	//create a ID3D10Buffer in graphics memory containing the vertex info
	HR(GETD3D10DEVICE->CreateBuffer( &bd,NULL, &m_pVertexBuffer ));
}

void PhysicsDebugRenderer::GetEffectVariables()
{
	m_pDefaultTechnique = m_pEffect->GetTechniqueByName("CheckersTech");
	m_pWorldViewProjectionVariable = m_pEffect->GetVariableByName("gWVP")->AsMatrix();
}

void PhysicsDebugRenderer::DefineInputlayout()
{
	// Define the input layout
	D3D10_INPUT_ELEMENT_DESC layout[] =
	{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof (layout) / sizeof (layout[0] );
	// Create the input layout
	D3D10_PASS_DESC PassDesc;
	// Get the pass decriptor from the effect technique
	m_pDefaultTechnique->GetPassByIndex(0)->GetDesc( &PassDesc );
	HR(GETD3D10DEVICE->CreateInputLayout(
	layout,
	numElements,
	PassDesc.pIAInputSignature,
	PassDesc.IAInputSignatureSize,
	&m_pVertexLayout));
	//important set the size of the vertexbuffer, is used in draw base member function
	m_VertexBufferStride = 2*sizeof(float3);
}

void PhysicsDebugRenderer::UpdateEffectVariables()const
{
	m_pWorldViewProjectionVariable->SetMatrix((float*)&(m_World * m_View * m_Projection));
}

void PhysicsDebugRenderer::Tick(float dTime)
{

}

void PhysicsDebugRenderer::OnResize()
{
	//window client area has changed, recalculate projection matrix
	D3DXMatrixPerspectiveFovLH(&m_Projection,(float)(D3DX_PI *.25), (float)((float)DXWINDOW->GetWidth()/(float)DXWINDOW->GetHeight()),1.0f,100.0f);
}

void PhysicsDebugRenderer::Draw(const ID3D10EffectTechnique* pTechnique) const
{
	// Draw the physics in wireframe
	const NxDebugRenderable *dbgRenderable=PHYSX.GetScene()->getDebugRenderable();    

	if (dbgRenderable == NULL)
		return;

	if(!pTechnique)
	{
		MessageBox(0,_T("No Technique"),_T("ERROR"),0);
		return;
	}

	//Every object has it's own set of eg worldmatrix
	UpdateEffectVariables();

	// Set the input layout
	GETD3D10DEVICE->IASetInputLayout( m_pVertexLayout );

	DrawLines(dbgRenderable, pTechnique);           
}

void PhysicsDebugRenderer::Draw() const
{
	Draw(m_pDefaultTechnique);
}

void PhysicsDebugRenderer::DrawLines(const NxDebugRenderable *dbgRenderable, const ID3D10EffectTechnique* pTechnique) const
{
	UINT offset = 0;

	if ( dbgRenderable->getNbLines() == 0) 
	{
		return;
	}
	//
	if ((dbgRenderable->getNbLines()*4) > m_vertexbuffersize) 
	{
		m_vertexbuffersize = (dbgRenderable->getNbLines()*4);

		const_cast<PhysicsDebugRenderer*>(this)->BuildVertexBuffer();
	}

	//Get Lines
	const NxDebugLine* LinesArray = dbgRenderable->getLines();

	//Set primitive topology
	GETD3D10DEVICE->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_LINELIST );

	int NbLines = dbgRenderable->getNbLines();
	float3 *pData = NULL;
	m_vertices.clear();
	for (int i = 0; i<NbLines;++i, ++LinesArray)
	{
		NxU32 color = LinesArray->color;
		float b= NxF32((color)&0xff)/255.0f;
		float g= NxF32((color>>8)&0xff)/255.0f;
		float r= NxF32((color>>16)&0xff)/255.0f;
		m_vertices.push_back (float3(LinesArray->p0.x, LinesArray->p0.y, LinesArray->p0.z));
		m_vertices.push_back (float3(r,g,b));
		m_vertices.push_back(float3(LinesArray->p1.x, LinesArray->p1.y, LinesArray->p1.z));
		m_vertices.push_back (float3(r,g,b));
	} 

	if(SUCCEEDED(m_pVertexBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, reinterpret_cast<void**>(&pData))))
	{
		memcpy(pData, &m_vertices[0], sizeof(float3) * NbLines * 4);
		m_pVertexBuffer->Unmap();
	}

	//To GPU
	GETD3D10DEVICE->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &m_VertexBufferStride, &offset );


	//DRAW
	D3D10_TECHNIQUE_DESC techDesc;
	(const_cast<ID3D10EffectTechnique*>(pTechnique))->GetDesc( &techDesc );

	for( UINT p = 0; p < techDesc.Passes; ++p )
	{
		(const_cast<ID3D10EffectTechnique*>(pTechnique))->GetPassByIndex( p )->Apply(0);
		GETD3D10DEVICE->Draw(NbLines*2,0);
	}
}