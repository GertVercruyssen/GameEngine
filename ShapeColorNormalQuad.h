#pragma once
#include "d3dutil.h"
#include "ShapeColorNormal.h"


class ShapeColorNormalQuad: public ShapeColorNormal
{
public:
	ShapeColorNormalQuad();
	virtual ~ShapeColorNormalQuad(void);

	virtual void Tick(float dTime);
	virtual bool Init();

protected:
	
	virtual void BuildVertexBuffer();
	virtual void BuildIndexBuffer();

private:
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	ShapeColorNormalQuad(const ShapeColorNormalQuad& t);
	ShapeColorNormalQuad& operator=(const ShapeColorNormalQuad& t);

};
