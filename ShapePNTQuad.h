#pragma once
#include "d3dutil.h"
#include "ShapePNT.h"


class ShapePNTQuad: public ShapePNT
{
public:
	ShapePNTQuad(const tstring& TexFileName);
	virtual ~ShapePNTQuad(void);

	virtual void Tick(float dTime){}
	virtual bool Init();

	void SetTechPoint();
	void SetTechLinear();
	void SetTechAnisotropic();
	void SetTechAlphaBlend();
protected:
	ID3D10EffectTechnique*  m_pTechPoint;
	ID3D10EffectTechnique*  m_pTechAnisotropic;
	ID3D10EffectTechnique*  m_pTechAlphaBlend;
	
	virtual void GetTechniques();
	virtual void BuildVertexBuffer();
	virtual void BuildIndexBuffer();

private:
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	ShapePNTQuad(const ShapePNTQuad& t);
	ShapePNTQuad& operator=(const ShapePNTQuad& t);

};
