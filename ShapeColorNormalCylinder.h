#pragma once
#include "d3dutil.h"
#include "ShapeColorNormal.h"


class ShapeColorNormalCylinder : public ShapeColorNormal
{
public:
	ShapeColorNormalCylinder(float Radius1=1.0f,float Radius2=1.0f, float Length=1.0f,D3DXCOLOR color=D3DXCOLOR(1,1,1,1),int Slices=15, int Stacks=1);
	virtual ~ShapeColorNormalCylinder(void);

	virtual void Tick(float dTime);
	virtual bool Init();
	/**
	* Determinates the color of a mesh
	* @rgb : color ranges from 0 to 1
	*/
	void SetMaterialColor(float r, float g, float b, float a);

protected:
	float m_Radius1, m_Radius2, m_Length;
	//slice is als een stuk taart, 
	//stack is als er taarten op elkaar zouden staan
	int m_Slices, m_Stacks;
	//fanindexen zijn de plaatsen in het vertexarray waar de fan start
	int m_FanIndex1,m_FanIndex2;
	float m_R,m_G,m_B,m_A;


	virtual void BuildVertexBuffer();
	virtual void BuildIndexBuffer();

private:
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	ShapeColorNormalCylinder(const ShapeColorNormalCylinder& t);
	ShapeColorNormalCylinder& operator=(const ShapeColorNormalCylinder& t);

};
