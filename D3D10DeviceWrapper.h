// Bart Uyttenhove
// Singleton Class Implementation of DirectX 10 Device
//
#pragma once
//Direct3D
#include "d3d10.h"
#include "d3dx10.h"
#include "D3DX10Core.h"

#include "ObjMesh.h"
#include "DX10ObjMesh.h"

class IndexBufferMaterial;

#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3d10.lib")

//defines for readable code
#define GETD3D10DEVICE (D3D10DeviceWrapper::GetSingleton()->Get3DDevice())
#define D3D10DEVICEWRAPPER (D3D10DeviceWrapper::GetSingleton())

class D3D10DeviceWrapper
{
public:
	//destructor
	virtual ~D3D10DeviceWrapper(void);

	//static member function
	static D3D10DeviceWrapper*	GetSingleton();

	/** InitDirect3D
	*	@hWindow: window handle to connect a directx device
	*	@Width: Width in pixels of backbuffer
	*	@Height: Height in pixels of backbuffer
	*/
	HRESULT		InitDirect3D(const HWND hWindow, long Width, long Height);

	// to be called when windowsize has changed: it changes backbuffer size
	void OnResize();

	//draws a text on the backbuffer
	void DrawString(const tstring& Text,int xpos, int ypos)const;

	/*Clears the rendertarget (backbuffer)*/
	void Clear()const;

	// Swaps the back and frontbuffer
	void Present()const;

	// Sets the wireframe state
	void EnableWireframe(bool enable)const;
	// -1: front face culling; 0: no culling; 1: back face culling

	void SetCullMode(int mode)const;
	void EnableAlphaBlend(bool enable)const;
	//getters
	int	GetBackbufferHeight()const;
	int	GetBackbufferWidth()const;
	ID3D10Device* Get3DDevice()const;
	void ResetRenderTargets();

	/**
	* Compile the vertexlist, indexedgeometry and material
	* of an objmesh object into a directx10 obj mesh object.
	*/
	DX10ObjMesh* Compile(ObjMesh* mesh);

private:
	//private constructor
	D3D10DeviceWrapper(void);

	/**
	* Compiles a VertexList into a DirectXBuffer object.
	* @param vertexList the vertex list object with the description
	* of the vertex data.
	*/
	ID3D10Buffer* CompileVertexBuffer(VertexList* vertexList);
	/**
	* Compiles a VertexLayout.
	* @param vertexList the layout of the input vertexlist object.
	* @param pTechnique the technique, used to define the input layout.
	*/
	ID3D10InputLayout* CompileInputLayout(VertexList* vertexList, ID3D10EffectTechnique*  pTechnique);
	/**
	* Compiles the indexbuffer for use in DX10ObjMesh object.
	* @param material the material to compile.
	* @param pTechnique the technique to check if the parameters are ok.
	*/
	DX10MaterialIndexBuffer* CompileIndexBuffer(IndexBufferMaterial* material,ID3D10Effect*  pEffect);

	//create the font
	void InitFont();

	//create the rasterizerstates
	void CreateRasterizerStates();
	//create the blendstates
	void CreateBlendStates();
	//static data member
	static D3D10DeviceWrapper* m_pDirectXLayer;

	//other data members
	HWND							  m_hWindow;
	ID3D10Device					*m_pD3DDevice;
	ID3D10RenderTargetView	*m_pRenderTargetView;
	ID3D10DepthStencilView	*m_pDepthStencilView;
	ID3D10Texture2D			*m_pDepthStencilBuffer;
	IDXGISwapChain				*m_pSwapChain;

	long m_WindowedBackbufferWidth;
	long m_WindowedBackbufferHeight;

	// Font used to draw 2D text
	ID3DX10Font* m_pFont;

	//Les02
	IDXGIFactory *m_pFactory;

	//Les03 States
	ID3D10RasterizerState *m_pRasterizerStateWireframe,*m_pRasterizerStateSolid;
	ID3D10RasterizerState *m_pRasterizerStateCullBackFacing;
	ID3D10RasterizerState *m_pRasterizerStateCullFrontFacing;
	ID3D10RasterizerState *m_pRasterizerStateCullNone;

	//Les05 Blending
	ID3D10BlendState *m_pTransparantBlendState;
private:
	D3D10DeviceWrapper(const D3D10DeviceWrapper& t);
	D3D10DeviceWrapper& operator=(const D3D10DeviceWrapper& t);

};
