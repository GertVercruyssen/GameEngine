#pragma once
#include "d3dutil.h"
#include "Graph3DObject.h"

#include "vertex.h"
#include "ShapeBase.h"

class ShapePNT : public ShapeBase
{
public:
	ShapePNT(const tstring& TexFileName);
	virtual ~ShapePNT(void);


protected:
	
	virtual void BuildVertexBuffer()=0;
	virtual void BuildIndexBuffer()=0;

	virtual void GetEffectVariables();
	virtual void DefineInputlayout();	
	virtual void UpdateEffectVariables()const;
	virtual void LoadTexture(const tstring& TexFileName);

	ID3D10EffectMatrixVariable* m_pWorldViewProjectionVariable;
	ID3D10EffectMatrixVariable* m_pWorldVariable;
	ID3D10EffectShaderResourceVariable* m_pDiffuseMapVariabele;
	ID3D10ShaderResourceView *m_pTextureRV;

	ID3D10EffectTechnique*  m_pTechLinear;

private:
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	ShapePNT(const ShapePNT& t);
	ShapePNT& operator=(const ShapePNT& t);

};
