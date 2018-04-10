#pragma once

#include "d3dutil.h"

class FullscreenQuad
{

public:
	FullscreenQuad(const tstring& TexFileName);
	virtual ~FullscreenQuad(void);
	virtual void Draw()const;
	virtual void Draw(const ID3D10EffectTechnique* pTechnique) const;
	virtual void SetTexture(ID3D10ShaderResourceView * texture){
	m_pTextureRV = texture;
}
protected:
	virtual void BuildVertexBuffer();
	virtual void BuildIndexBuffer();
	virtual void GetEffectVariables();
	virtual void DefineInputlayout();
	virtual void UpdateEffectVariables()const;
	virtual void LoadTexture(const tstring& TexFileName);
	// Loads and compiles the effect and gets the technique
	virtual void CreateEffect(const tstring& EffectFilename);
	int m_NumVertices,m_NumIndices;
	UINT m_VertexBufferStride;//stride value of vertex buffer
	ID3D10EffectShaderResourceVariable* m_pTextureMapVariabele;
	ID3D10ShaderResourceView * m_pTextureRV;
	ID3D10InputLayout* m_pVertexLayout;
	ID3D10Buffer* m_pVertexBuffer;
	ID3D10Buffer* m_pIndexBuffer;
	ID3D10Effect* m_pEffect;
	ID3D10EffectTechnique* m_pDefaultTechnique;
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	FullscreenQuad(const FullscreenQuad& t);
	FullscreenQuad& operator=(const FullscreenQuad& t);

};