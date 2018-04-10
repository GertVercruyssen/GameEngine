#pragma once
#include "d3dx10.h"
class DX10ObjMesh;
/**
* Contains a material and the IndexBuffer that contains the indices
* that use the current vertexbuffer. 
*/
class DX10MaterialIndexBuffer
{
public:
	DX10MaterialIndexBuffer(void);
	virtual ~DX10MaterialIndexBuffer(void);

	void SetDiffuseTexture(ID3D10ShaderResourceView *texture){
			m_pDiffuseTextureRV = texture;
	}

	void SetIndexBuffer(ID3D10Buffer* indexBuffer,int numIndices){
		m_pIndexBuffer = indexBuffer;
		m_NumIndices = numIndices;
	}

	void Draw(const DX10ObjMesh* parent,ID3D10EffectPass* pass) const;

private: 
	/**
	* The buffer with the indices
	*/
	ID3D10Buffer* m_pIndexBuffer;
	/**
	* The number of indices 
	*/
	int m_NumIndices;
	
	/**
	* The texture object
	*/
	ID3D10ShaderResourceView *m_pDiffuseTextureRV;

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	DX10MaterialIndexBuffer(const DX10MaterialIndexBuffer& t);
	DX10MaterialIndexBuffer& operator=(const DX10MaterialIndexBuffer& t);
};
