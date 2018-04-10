#pragma once
#include "d3dutil.h"
#include "Graph3DObject.h"
#include <vector>
//Forward declaration
class PosNormShader;

class SkyBox :
	public Graph3DObject
{
public:
	SkyBox(const tstring& fileame);
	virtual ~SkyBox(void);

	virtual void Tick(float dTime);
	virtual void Draw() const;
	virtual void Draw(const tstring& technique)const {};
	virtual void OnResize(){}
protected:
	void BuildVertexBuffer();
	void BuildIndexBuffer();
	bool Init();
	void Cleanup();
	void LoadTexture();
	void CreateEffect();
	void DefineInputlayout();
	void InitializeMatrices();

	static const int m_NumVertices=8,m_NumIndices=36;

	ID3D10Effect*           m_pEffect;
	ID3D10EffectTechnique*  m_pTechnique;
	ID3D10InputLayout*      m_pVertexLayout;
	ID3D10Buffer*           m_pVertexBuffer;
	ID3D10Buffer*           m_pIndexBuffer;

	HRESULT hr;

	ID3D10ShaderResourceView* m_pCubeMap;
	ID3D10EffectShaderResourceVariable* m_pCubeMapVariable;

	ID3D10EffectMatrixVariable* m_pViewProjectionVariable;
	ID3D10EffectMatrixVariable* m_pWorldVariable;
	tstring m_TexFileName;


private:
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	SkyBox(const SkyBox& t);
	SkyBox& operator=(const SkyBox& t);

};
