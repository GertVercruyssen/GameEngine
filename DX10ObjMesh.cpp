#include "StdAfx.h"
#include "DX10ObjMesh.h"

#include "DX10MaterialIndexBuffer.h"
#include "D3D10DeviceWrapper.h"
#include "SpotLight.h"

DX10ObjMesh::DX10ObjMesh(tstring effect,tstring technique):m_pVertexBuffer(NULL),m_pInputLayout(NULL),m_pSkeleton(NULL),m_pBoneMatrices(NULL)
{
	CreateEffect( effect );

	string tname(technique.begin(),technique.end());
	m_pDefaultTechnique = m_pEffect->GetTechniqueByName(tname.c_str());

	GetEffectParameters();
	Rotate(-1.57079633f,0,0);
	Translate(0,0,0);
}
DX10ObjMesh::~DX10ObjMesh(void)
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pInputLayout);
	vector<DX10MaterialIndexBuffer*>::iterator it;
	for (it = m_IndexBuffers.begin(); it != m_IndexBuffers.end(); ++it )
	{
		delete *it;
	}
}

void DX10ObjMesh::GetEffectParameters()
{
	m_pWorldViewProjectionVariable = m_pEffect->GetVariableByName("gWVP")->AsMatrix();
	m_pWorldVariable = m_pEffect->GetVariableByName("gWorld")->AsMatrix();
	m_pDiffuseMapVariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
	//Checken of het valid is telkens op extra regel!
	m_pSpotLight1Variable = m_pEffect->GetVariableByName("gLight1");
	m_pSpotLight2Variable = m_pEffect->GetVariableByName("gLight2");
	m_pEyePosVariable = m_pEffect->GetVariableByName("EyePos")->AsVector();
}

void  DX10ObjMesh::UpdateEffectVariables()const
{
	//
    // Update Shader matrix variables in effect
    //
	//create a matrix containing the multiplication of world, view and projection matrix
	//short: m_pWorldViewProjectionVariable->SetMatrix((float*)&(m_World * m_View * m_Projection));
	//bit longer:
	D3DXMATRIX wvp;
	wvp = m_FinalWorld;
	wvp = wvp * m_View;
	wvp = wvp * m_Projection;
	m_pWorldViewProjectionVariable->SetMatrix((float*)&wvp);
	m_pWorldVariable->SetMatrix((float*)&m_FinalWorld);

	int spotlightsize = sizeof(SpotLight);
	if ( m_SpotLights.size()>0 )
		m_pSpotLight1Variable->SetRawValue((void*)m_SpotLights[0],0,sizeof(SpotLight));
	if ( m_SpotLights.size()>1)
		m_pSpotLight2Variable->SetRawValue((void*)m_SpotLights[1],0,sizeof(SpotLight));

	m_pEyePosVariable->SetRawValue(&m_CameraPos,0,sizeof(D3DXVECTOR3));

	if(IsAnimated())
		m_pBoneMatrices->SetRawValue((void*)m_pSkeleton->GetValuePointer(),0,m_pSkeleton->GetSizeInBytes());
}
void DX10ObjMesh::SetDiffuseTexture(ID3D10ShaderResourceView *texture) const
{
	m_pDiffuseMapVariable->SetResource(texture);
}
bool DX10ObjMesh::Init()
{
	return true;
}
void DX10ObjMesh::Tick(float dTime)
{
	if(IsAnimated()) m_pSkeleton->Animate(dTime*20);
}
void DX10ObjMesh::Draw() const
{
	Draw(m_pDefaultTechnique);
}
void DX10ObjMesh::Draw(const ID3D10EffectTechnique* pTechnique) const
{
	if(!pTechnique)
	{
		MessageBox(0,_T("No Technique"),_T("ERROR"),0);
		return;
	}
	//if alphablending is enabled, set the state
	D3D10DEVICEWRAPPER->EnableAlphaBlend(false);

	D3D10DEVICEWRAPPER->SetCullMode(2);

	//Every object has it's own set of eg worldmatrix
	UpdateEffectVariables();

	// Set the input layout
	GETD3D10DEVICE->IASetInputLayout( m_pInputLayout );

	// Set vertex buffer
	UINT offset = 0;
	GETD3D10DEVICE->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &m_VertexBufferStride, &offset );

	// Set primitive topology
	GETD3D10DEVICE->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	//
	// Renders a ShapeBase
	//
	D3D10_TECHNIQUE_DESC techDesc;
	(const_cast<ID3D10EffectTechnique*>(pTechnique))->GetDesc( &techDesc );
	for( UINT p = 0; p < techDesc.Passes; ++p )
	{
		vector<DX10MaterialIndexBuffer*>::const_iterator it;
		for (it=m_IndexBuffers.begin(); it != m_IndexBuffers.end(); ++it)
		{
			ID3D10EffectPass * pass = (const_cast<ID3D10EffectTechnique*>(pTechnique))->GetPassByIndex( p );
			(*it)->Draw( this, pass );
		}
	}

	//disable alpha blending
	D3D10DEVICEWRAPPER->EnableAlphaBlend(false);
	D3D10DEVICEWRAPPER->SetCullMode(1);
}
void DX10ObjMesh::OnResize()
{
}
Skeleton* DX10ObjMesh::GetSkeleton()
{
	return m_pSkeleton;
}
void DX10ObjMesh::SetSkeleton(Skeleton* skelet)
{
	m_pSkeleton = skelet;
	if(m_pSkeleton != NULL && m_pBoneMatrices == NULL)
	{
		m_pBoneMatrices = m_pEffect->GetVariableByName("gBoneMatrices")->AsMatrix();
		if(!m_pBoneMatrices->IsValid())MessageBox(0,_T("Getting Effectvariable gBoneMatrices Failed"),_T("ERROR"),0);
	}
}
bool DX10ObjMesh::IsAnimated() const
{
	if(m_pSkeleton == NULL)
		return false;
	else
		return true;
}
void DX10ObjMesh::ActivateAnimation(tstring key,bool looping)
{
	if(IsAnimated())
	{
		m_pSkeleton->ActivateAnimation(key);
	}
}
void DX10ObjMesh::AddAnimation(Animation* pAnimation)
{
	if(IsAnimated())
		m_pSkeleton->AddAnimation(pAnimation);
}