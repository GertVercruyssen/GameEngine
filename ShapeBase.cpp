#include "StdAfx.h"
#include "ShapeBase.h"
#include "D3D10DeviceWrapper.h"
#include "DXWindow.h"


ShapeBase::ShapeBase():
			m_pVertexLayout(NULL),
			m_pVertexBuffer(NULL),
			m_pIndexBuffer(NULL),
			m_NumVertices(0),m_NumIndices(0),
			m_VertexBufferStride(0),
			m_bEnableAlphaBlend(false)
{
	//InitializeMatrices();		//uit ShapeBase //moest in commentaar
}

ShapeBase::~ShapeBase(void)
{
	Cleanup();
}


void ShapeBase::InitializeMatrices()
{
    // Initialize the world matrix
    D3DXMatrixIdentity( &m_World );

	// Initialize the view matrix (camera)
	D3DXVECTOR3 Pos(0.0f,1.0f,-3.0f);
	D3DXVECTOR3 Target(0.0f,0.0f,0.0f);
	D3DXVECTOR3 WorldUp(0.0f,0.0f,1.0f);
	D3DXMatrixLookAtLH(&m_View, &Pos,&Target,&WorldUp);

	//Initialize the PerspectiveMatrix also known as ProjectionMatrix
	D3DXMatrixPerspectiveFovLH(
		&m_Projection,//adres van in te vullen matrix
		(float)(D3DX_PI *.25), //field of view 
		(float)((float)DXWINDOW->GetWidth()/(float)DXWINDOW->GetHeight()),//aspect ratio
		1.0f,//near clip plane
		1000.0f);//far clip plane
}
void ShapeBase::OnResize()
{
	//window client area has changed, recalculate projection matrix
	D3DXMatrixPerspectiveFovLH(&m_Projection,(float)(D3DX_PI *.25), (float)((float)DXWINDOW->GetWidth()/(float)DXWINDOW->GetHeight()),1.0f,100.0f);
}

void ShapeBase::Cleanup()
{
	if( m_pIndexBuffer )m_pIndexBuffer->Release();
    if( m_pVertexBuffer ) m_pVertexBuffer->Release();
    if( m_pVertexLayout ) m_pVertexLayout->Release();
}

void ShapeBase::Draw(const ID3D10EffectTechnique* pTechnique) const
{
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

    // Set vertex buffer
    UINT offset = 0;
    GETD3D10DEVICE->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &m_VertexBufferStride, &offset );
   
	// Set index buffer
    GETD3D10DEVICE->IASetIndexBuffer( m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );

    // Set primitive topology
    GETD3D10DEVICE->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    //
    // Renders a ShapeBase
    //
    D3D10_TECHNIQUE_DESC techDesc;
    (const_cast<ID3D10EffectTechnique*>(pTechnique))->GetDesc( &techDesc );
    for( UINT p = 0; p < techDesc.Passes; ++p )
    {
         (const_cast<ID3D10EffectTechnique*>(pTechnique))->GetPassByIndex( p )->Apply(0);
        GETD3D10DEVICE->DrawIndexed( m_NumIndices, 0, 0 ); 
		//GETD3D10DEVICE->Draw(m_NumIndices,0);
    }

	//disable alpha blending
	D3D10DEVICEWRAPPER->EnableAlphaBlend(false);

}

void ShapeBase::Draw()const
{
	Draw(m_pDefaultTechnique);
}
void ShapeBase::EnableAphaBlend(bool enable)
{
	m_bEnableAlphaBlend = enable;
}
