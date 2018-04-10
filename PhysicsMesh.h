#pragma once
#include <vector>
#include "ShapeBase.h"
class InputMesh;
class NxActor;

/**
* Represents a mesh and its physics mesh
* @author Brecht Kets
*/
class PhysicsMesh : public Graph3DObject
{
public:
	/**
	* Enumeration representing Convex or Concave mesh
	*/
	enum Type { Convex, Concave };

	/**
	* Constructor
	* @param filename the obj file to load.
	* @param physicsFileName the physics file to load.
	* @param type convex or concave.
	*/
	PhysicsMesh(tstring filename, tstring physicsFileName, Type type);

	/**
	* Destructor
	*/
	virtual ~PhysicsMesh(void);

	/**
	* Tick
	* @param dTime the time passed since the last Tick.
	*/
	virtual void Tick(float dTime);

	/**
	* Init
	* Load obj and physics mesh.
	*/
	virtual bool Init();

	/**
	* Draw using the default technique.
	*/
	virtual void Draw() const;

	/**
	* Draw using a custom technique.
	* @param pTechnique the technique to use.
	*/
	virtual void Draw(const ID3D10EffectTechnique* pTechnique) const ;

	/**
	* On Resize
	*/
	virtual void OnResize();

protected:
	/**
	* Build the vertex buffers.
	* Store them in a vector.
	*/
	virtual void BuildVertexBuffers();

	/**
	* Build the index buffers. 
	* Store them in a vector.
	*/
	virtual void BuildIndexBuffers();

	/**
	* Store the default technique and the shader variables
	*/
	virtual void GetEffectVariables();

	/**
	* Defines the input layout
	*/
	virtual void DefineInputlayout();

	/**
	* Updates the shader variables
	*/
	virtual void UpdateEffectVariables() const;

	/**
	* Retrieves the final world matrix 
	* Sets the Physics Global pos.
	*/
	virtual void SetPhysXGlobalPose();

	/**
	* Sets the Physics Global pos.
	* @World: een WorldMatrix
	*/
	virtual void SetPhysXGlobalPose(const D3DXMATRIX& World);

	/**
	* The cooked physics representation of this object
	*/
	NxActor *m_pActor;

private:
	/**
	* Creates a Convex mesh actor and adds it to the physics scene
	*/
	virtual bool BuildConvex(); 

	/**
	* Creates a Concave mesh actor and adds it to the physics scene
	*/
	virtual bool BuildConcave(); 
	
	/**
	* Convex or Concave
	*/
	Type m_Type;

	/**
	* The vector containing all the input meshes
	*/
	vector<InputMesh*>  m_Meshes;

	/**
	* The vector containing all the vertexbuffers for each mesh in m_Meshes
	*/
	vector<ID3D10Buffer*> m_pVertexBuffers;

	/**
	* The vector containing all the indexbuffers for each mesh in m_Meshes
	*/
	vector<ID3D10Buffer*> m_pIndexBuffers;

	/**
	* The vector containing all number of indices for each mesh in m_Meshes
	*/
	vector<int> m_numIndicesList;

	/**
	* The vector layout
	*/
	ID3D10InputLayout*		m_pVertexLayout;

	/**
	* The world view projection handle to the shader
	*/
	ID3D10EffectMatrixVariable* m_pWorldViewProjectionVariable;

	/**
	* The filename of the obj and cooked physics file
	*/
	tstring m_filename, m_physicsFileName;

	/**
	* The stride value of the vertex buffer
	*/
	UINT m_VertexBufferStride;

	/**
	* Alpha blending
	*/
	bool m_bEnableAlphaBlend;

	bool m_shouldDraw;

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	PhysicsMesh(const PhysicsMesh& t);
	PhysicsMesh& operator=(const PhysicsMesh& t);
};
