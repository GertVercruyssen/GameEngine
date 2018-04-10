#pragma once
#include "d3dutil.h"
#include "Graph3DObject.h"

#include "vertex.h"
#include "ShapeBase.h"

class ShapeColor : public ShapeBase
{
public:
	ShapeColor();
	virtual ~ShapeColor(void);


protected:
	
	virtual void BuildVertexBuffer()=0;
	virtual void BuildIndexBuffer()=0;

	virtual void GetEffectVariables();
	virtual void DefineInputlayout();	
	virtual void UpdateEffectVariables()const;

	ID3D10EffectMatrixVariable* m_pWorldViewProjectionVariable;

private:
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	ShapeColor(const ShapeColor& t);
	ShapeColor& operator=(const ShapeColor& t);

};
