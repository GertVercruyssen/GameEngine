#pragma once
#include "Graph3DObject.h"
#include <vector>
class NxDebugRenderable;
class float3;

/**
* Physics debug renderer
* Add this object to your scene in orde to render the physics
* Usage of #ifdef _DEBUG is recommended
* @author Brecht Kets
*/
class PhysicsDebugRenderer : Graph3DObject
{
public:
	PhysicsDebugRenderer(void);
	virtual ~PhysicsDebugRenderer(void);

	/**
	* Draw using the default technique.
	*/
	virtual void Draw() const;

	/**
	* Draw using pTechnique.
	*/
	virtual void Draw(const ID3D10EffectTechnique* pTechnique) const ;
	
	/**
	* Call when the d3d object is constructed.
	* @return success
	*/
	virtual bool Init();

	/**
	* Implementation the physics behaviour of this object.
	*/
	virtual void Tick(float dTime);

	/**
	* On Resize
	*/
	virtual void OnResize();

protected:
	/**
	* Build the vertex buffer.
	* Create it large enough to contain all vertices
	*/
	virtual void BuildVertexBuffer();

	/**
	* Define the input layout. 
	*/
	virtual void DefineInputlayout();

	/**
	* Update the effect variables. 
	*/
	virtual void UpdateEffectVariables() const;

	/**
	* Get the effect variables. 
	*/
	virtual void GetEffectVariables();

private:

	/**
	* Draw the lines received from the debug renderable
	*/
	void DrawLines(const NxDebugRenderable *dbgRenderable, const ID3D10EffectTechnique* pTechnique) const;

	/**
	* The vector layout
	*/
	ID3D10InputLayout* m_pVertexLayout;

	/**
	* The world view projection handle to the shader
	*/
	ID3D10EffectMatrixVariable* m_pWorldViewProjectionVariable;

	/**
	* The stride value of the vertex buffer
	*/
	UINT m_VertexBufferStride;

	/**
	* The vertex buffer
	*/
	ID3D10Buffer* m_pVertexBuffer;

	/**
	* The vertex buffer - mutable to avoid const casting
	*/
	mutable vector<float3> m_vertices;

	/**
	* The vertex buffer size - mutable to avoid const casting
	*/
	mutable UINT m_vertexbuffersize;

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	PhysicsDebugRenderer(const PhysicsDebugRenderer& t);
	PhysicsDebugRenderer& operator=(const PhysicsDebugRenderer& t);
};
