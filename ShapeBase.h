#pragma once
#include "d3dutil.h"
#include "Graph3DObject.h"

#include "vertex.h"


class ShapeBase : public Graph3DObject
{
public:
	ShapeBase();
	virtual ~ShapeBase(void);

	virtual void Draw()const;//use when default technique is available
	virtual void Draw(const ID3D10EffectTechnique* pTechnique) const ;//use pTechnique to render
	virtual void OnResize();
	/**
	* Call when the d3d object is constructed.
	*/
	virtual bool Init()=0;
	void EnableAphaBlend(bool enable);


protected:
	
	virtual void GetEffectVariables()=0;
	virtual void UpdateEffectVariables()const = 0;
	virtual void DefineInputlayout()=0;
	virtual void BuildVertexBuffer()=0;
	virtual void BuildIndexBuffer()=0;
	virtual void InitializeMatrices();
	virtual void Cleanup();


	int m_NumVertices,m_NumIndices;

	ID3D10InputLayout*		m_pVertexLayout;
	ID3D10Buffer*			m_pVertexBuffer;
	ID3D10Buffer*			m_pIndexBuffer;

	UINT m_VertexBufferStride;//stride value of vertex buffer
	HRESULT hr;
	bool m_bEnableAlphaBlend;
	

private:
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	ShapeBase(const ShapeBase& t);
	ShapeBase& operator=(const ShapeBase& t);

};
