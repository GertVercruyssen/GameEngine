#pragma once
#include "Graph3DObject.h"
#include "util.h"
#include <vector>
#include "Animation.h"
using namespace std;
#include "Skeleton.h"

class DX10MaterialIndexBuffer;
struct SpotLight;
/**
* Contains the same information as the ObjMesh object but with
* all the internal data as DirectX10 buffers and implements the GraphBase
* interface.
*/
class DX10ObjMesh:public Graph3DObject
{
public:
	/**
	* Creates a new DirectX10 ObjMesh object
	* @param effect the location of the effect file.
	* @param technique the name of the technique to use.
	*/
	DX10ObjMesh(tstring effect,tstring technique);
	/**
	* Delets the DirectX10 ObjMesh object
	*/
	virtual ~DX10ObjMesh(void);

	/**
	* Returns the default technique for this object.
	*/
	ID3D10EffectTechnique* GetDefaultTechnique() const{
		return m_pDefaultTechnique;
	}

	/**
	* Returns the effect that is used by this object.
	*/
	ID3D10Effect* GetEffect() const{
		return m_pEffect;
	}

	/**
	* Commits the effect parameters to the effect.
	*/
	void GetEffectParameters();
	/**
	* Updates the common effect parameters.
	*/
	void UpdateEffectVariables() const;

	/**
	* Sets the vertexbuffer to use with this DX10ObjMesh object.
	* @param buffer the buffer to set.
	*/
	void SetVertexBuffer(ID3D10Buffer* buffer, int stride){
		m_pVertexBuffer = buffer;
		m_VertexBufferStride = stride;
	}

	/**
	* Sets the input layout to use for this DX10ObjMesh object.
	* @param layout the layout to set.
	*/
	void SetInputLayout(ID3D10InputLayout* layout){
		m_pInputLayout = layout;
	}

	/**
	* Adds a DX10MaterialIndexBuffer object to the list of indexbuffers
	* with materials to draw.
	* @param buffer the buffer to add.
	*/
	void AddIndexBuffer(DX10MaterialIndexBuffer* buffer){
		m_IndexBuffers.push_back(buffer);
	}

	/**
	* Adds the spotlights to the vector of spotlights.
	*/
	void AddSpotLights(vector<SpotLight*>& lights){
		m_SpotLights.assign(lights.begin(),lights.end());
	}

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
	/**
	* Sets the current diffuse texture for the rendering.
	* @param texture the texture to use.
	*/
	void SetDiffuseTexture(ID3D10ShaderResourceView * texture) const;

	virtual Skeleton* GetSkeleton();
	virtual void SetSkeleton(Skeleton* skelet);
	virtual bool IsAnimated() const;
	virtual void ActivateAnimation(tstring key,bool looping);
	virtual void AddAnimation(Animation* pAnimation);
private:
	/**
	* The vertex buffer for this mesh object
	*/
	ID3D10Buffer*						m_pVertexBuffer;
	/**
	* The stride of the vertexbuffer.
	*/
	UINT								m_VertexBufferStride;
	/**
	* The input layout for the vertex buffer.
	*/
	ID3D10InputLayout*					m_pInputLayout;
	/**
	* The D3DX10MaterialIndexBuffer objects.
	*/
	vector<DX10MaterialIndexBuffer*>	m_IndexBuffers;
	/**
	* The spotlights to use for this object.
	*/
	vector<SpotLight*> m_SpotLights;

	// the common effect parameters
	ID3D10EffectMatrixVariable* m_pWorldViewProjectionVariable;
	ID3D10EffectMatrixVariable* m_pWorldVariable;
	ID3D10EffectVariable* m_pSpotLight1Variable;
	ID3D10EffectVariable* m_pSpotLight2Variable;
	ID3D10EffectVariable* m_pBoneMatrices;

	ID3D10EffectVectorVariable* m_pEyePosVariable;
	mutable ID3D10EffectShaderResourceVariable* m_pDiffuseMapVariable;

	Skeleton * m_pSkeleton;
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	DX10ObjMesh(const DX10ObjMesh& t);
	DX10ObjMesh& operator=(const DX10ObjMesh& t);
};
