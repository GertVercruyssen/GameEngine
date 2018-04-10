#pragma once
#include "d3dutil.h"
#include "ShapeColorNormal.h"



class ShapeColorNormalCube : public ShapeColorNormal
{
public:
	ShapeColorNormalCube(float width=1.0f, float height=1.0f, float depth=1.0f, D3DXCOLOR color=D3DXCOLOR(1,1,1,1));
	virtual ~ShapeColorNormalCube(void);

	virtual void Tick(float dTime);
	virtual bool Init();

protected:
	
	virtual void BuildVertexBuffer();
	virtual void BuildIndexBuffer();
	float m_Width,m_Height,m_Depth;
	D3DXCOLOR m_Color;

private:
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	ShapeColorNormalCube(const ShapeColorNormalCube& t);
	ShapeColorNormalCube& operator=(const ShapeColorNormalCube& t);

};
