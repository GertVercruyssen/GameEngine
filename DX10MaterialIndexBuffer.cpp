#include "StdAfx.h"
#include "DX10MaterialIndexBuffer.h"
#include "D3D10DeviceWrapper.h"

DX10MaterialIndexBuffer::DX10MaterialIndexBuffer(void):m_pIndexBuffer(NULL),m_pDiffuseTextureRV(NULL)
{
}

DX10MaterialIndexBuffer::~DX10MaterialIndexBuffer(void)
{
	SAFE_RELEASE(m_pIndexBuffer);
}

void DX10MaterialIndexBuffer::Draw(const DX10ObjMesh* parent,ID3D10EffectPass* pass) const
{
	parent->SetDiffuseTexture(m_pDiffuseTextureRV);
	pass->Apply(NULL);
	GETD3D10DEVICE->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	GETD3D10DEVICE->DrawIndexed(m_NumIndices, 0, 0);
}
