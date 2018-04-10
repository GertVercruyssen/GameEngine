#include "StdAfx.h"
#include "D3D10DeviceWrapper.h"
#include "d3dUtil.h"
#include "DXWindow.h"

#include "DXGI.h"
#include "VertexList.h"
#include "IndexBufferMaterial.h"
#include "DX10MaterialIndexBuffer.h"
#include "TextureManager.h"
#include "IndexedGeometry.h"

#pragma comment(lib, "dxgi.lib")

D3D10DeviceWrapper* D3D10DeviceWrapper::m_pDirectXLayer=0;

D3D10DeviceWrapper::D3D10DeviceWrapper(void):m_pD3DDevice(0),m_hWindow(0),
			m_pFont(0),
			m_pSwapChain(0),m_pDepthStencilView(0),m_pDepthStencilBuffer(0),
			m_pRenderTargetView(0),m_pFactory(0),
			m_pRasterizerStateWireframe(0), m_pRasterizerStateSolid(0),
			m_pRasterizerStateCullBackFacing(0),
			m_pRasterizerStateCullFrontFacing(0),
			m_pRasterizerStateCullNone(0),
			m_pTransparantBlendState(0)
{

}

D3D10DeviceWrapper::~D3D10DeviceWrapper(void)
{
	/*	Restore all default device settings; 
		return the device to the state it was in when it was created. 
		This will set all set all input/output resource slots, shaders, 
		input layouts, predications, scissor rectangles, depth-stencil state, 
		rasterizer state, blend state, sampler state, and viewports to NULL. 
		The primitive topology will be set to UNDEFINED.
	*/
	if( m_pD3DDevice ) m_pD3DDevice->ClearState();
	if( m_pDepthStencilBuffer ) m_pDepthStencilBuffer->Release();
	if( m_pDepthStencilView ) m_pDepthStencilView->Release();
	if( m_pRenderTargetView ) m_pRenderTargetView->Release();
	if( m_pSwapChain ) m_pSwapChain->Release();
	if( m_pFont)m_pFont->Release();
	if( m_pD3DDevice ) m_pD3DDevice->Release();
}
D3D10DeviceWrapper* D3D10DeviceWrapper::GetSingleton()
{
	if(m_pDirectXLayer==0){
		m_pDirectXLayer=new D3D10DeviceWrapper();
	}
	return m_pDirectXLayer;
}
ID3D10Device* D3D10DeviceWrapper::Get3DDevice()const
{
	return m_pD3DDevice;
}

HRESULT D3D10DeviceWrapper::InitDirect3D(const HWND hWindow, long width, long height)
{
	//HRESULT hr;

	m_hWindow=hWindow;
	//store dimensions; needed when changing from full screen to windowed mode
	m_WindowedBackbufferWidth = width;
	m_WindowedBackbufferHeight = height;

	//-------------------------------------------------------
	// 4.2.1 Step 1: Create the SwapChain Descriptor struct.

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof(sd) );
    sd.BufferDesc.Width				= width;
    sd.BufferDesc.Height				= height;
    sd.BufferDesc.RefreshRate.Numerator	= 60;
    sd.BufferDesc.RefreshRate.Denominator	 = 1;
    sd.BufferDesc.Format			= DXGI_FORMAT_R8G8B8A8_UNORM;
	//No Multisampling
	sd.SampleDesc.Count			= 1;
	sd.SampleDesc.Quality			= 0;

	sd.BufferUsage						= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount						= 1;
	sd.OutputWindow					= m_hWindow;
	sd.Windowed						= TRUE;
	sd.SwapEffect						= DXGI_SWAP_EFFECT_DISCARD;

	//allow alt enter to change the display resolution
	sd.Flags								= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH ;

	//-------------------------------------------------------
	// 4.2.2 Step 2: Create the Device and SwapChain using the descriptor.

	//if working in debug mode, use device in debug mode
	UINT CreateDeviceFlags = 0;
	#if defined (_DEBUG) || defined (_DEBUG)
		CreateDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
	#endif

	//-------------------------------------------------------
	//Les02 Maak device en swapchain appart aan
	//Maak device
	HR(D3D10CreateDevice(
		NULL,
		D3D10_DRIVER_TYPE_HARDWARE,
		NULL,
		CreateDeviceFlags,
		D3D10_SDK_VERSION,
		&m_pD3DDevice));

	//Because a Direct3D device can be created without creating a swapchain it may be nescessary 
	//to retrieve the factory used to create the device in order to create a swapchain. 
	//This can be accomplished by requesting the IDXGIDevice interface from the Direct3D device and 
	//then using IDXGIObject::GetParent to locate the factory. The following code illustrates the process.

	IDXGIDevice * pDXGIDevice;
	HR(m_pD3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice));
	  
	IDXGIAdapter * pDXGIAdapter;
	HR(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter));

	HR(pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&m_pFactory));

	//Maak met Factory de SwapChain
	HR(m_pFactory->CreateSwapChain(m_pD3DDevice,&sd,&m_pSwapChain));

	//Disable Alt + Enter
	m_pFactory->MakeWindowAssociation(m_hWindow, DXGI_MWA_NO_WINDOW_CHANGES ) ;

   	// The remaining steps that need to be carried out for d3d creation
	// also need to be executed every time the window is resized.  So
	// just call the onResize method here to avoid code duplication.
	OnResize();

	//additional: create the font
	InitFont();
	
	//create states
	CreateRasterizerStates();
	CreateBlendStates();
	return S_OK;
}

void D3D10DeviceWrapper::OnResize()
{
	tstringstream str;
	str << _T("Backbuffer Size: (width, height) ") << DXWINDOW->GetWidth() << _T(" ") << DXWINDOW->GetHeight() << _T("\n");
	OutputDebugString(str.str().c_str());
	//release buffers
	SAFE_RELEASE( m_pRenderTargetView );

	// Resize the swap chain and recreate the render target view.
	HR(m_pSwapChain->ResizeBuffers(
		1, DXWINDOW->GetWidth(), 
		DXWINDOW->GetHeight(), 
		DXGI_FORMAT_R8G8B8A8_UNORM, 
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));//this flag enables mode switch

	ID3D10Texture2D* pBackBuffer;
	HR(m_pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&pBackBuffer)));
	HR(m_pD3DDevice->CreateRenderTargetView(pBackBuffer, 0, &m_pRenderTargetView));
	ReleaseCOM(pBackBuffer);

	// 4.2.4 Create the depth/stencil buffer and view.

	//release buffers
	SAFE_RELEASE( m_pDepthStencilBuffer );
	SAFE_RELEASE( m_pDepthStencilView );

	D3D10_TEXTURE2D_DESC DepthStencilDesc;
	
	DepthStencilDesc.Width					= DXWINDOW->GetWidth();
	DepthStencilDesc.Height				= DXWINDOW->GetHeight();
	DepthStencilDesc.MipLevels			= 1;
	DepthStencilDesc.ArraySize			= 1;
	DepthStencilDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilDesc.SampleDesc.Count	= 1; // multisampling must match
	DepthStencilDesc.SampleDesc.Quality	= 0; // swap chain values.
	DepthStencilDesc.Usage				= D3D10_USAGE_DEFAULT;
	DepthStencilDesc.BindFlags			= D3D10_BIND_DEPTH_STENCIL;
	DepthStencilDesc.CPUAccessFlags	= 0; 
	DepthStencilDesc.MiscFlags			= 0;

	HR(m_pD3DDevice->CreateTexture2D(&DepthStencilDesc, 0, &m_pDepthStencilBuffer));
	HR(m_pD3DDevice->CreateDepthStencilView(m_pDepthStencilBuffer, 0, &m_pDepthStencilView));

	// 4.2.5 Bind the render target view and depth/stencil view to the pipeline.
	m_pD3DDevice->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	

	//4.2.6 Set the viewport transform.

	D3D10_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width    = DXWINDOW->GetWidth();
	vp.Height   = DXWINDOW->GetHeight();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	m_pD3DDevice->RSSetViewports(1, &vp);

}


int D3D10DeviceWrapper::GetBackbufferHeight()const
{
	DXGI_SWAP_CHAIN_DESC sd;
	m_pSwapChain->GetDesc(&sd);
	return sd.BufferDesc.Height;
}

int D3D10DeviceWrapper::GetBackbufferWidth()const
{
	DXGI_SWAP_CHAIN_DESC sd;
	m_pSwapChain->GetDesc(&sd);
	return sd.BufferDesc.Width;
}

void D3D10DeviceWrapper::InitFont()
{
	// 4.4 Text Output
	D3DX10_FONT_DESC fontDesc;
	fontDesc.Height          = 16;
    fontDesc.Width           = 0;
    fontDesc.Weight          = 0;
    fontDesc.MipLevels       = 1;
    fontDesc.Italic          = false;
    fontDesc.CharSet         = DEFAULT_CHARSET;
    fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
    fontDesc.Quality         = DEFAULT_QUALITY;
    fontDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;

	HR(D3DX10CreateFontIndirect(GETD3D10DEVICE, &fontDesc, &m_pFont));

}
void D3D10DeviceWrapper::DrawString(const tstring& Text, int xpos, int ypos)const
{
	if(!m_pFont) return;

	RECT FormatRect;//={xpos,ypos,GetBackbufferWidth()-xpos,GetBackbufferHeight()-ypos};
	SetRectEmpty(&FormatRect);
	//calc size of rect needed
	m_pFont->DrawText(0, Text.c_str(), -1, &FormatRect, DT_LEFT | DT_CALCRECT, D3DXCOLOR(255, 255, 222, 255));
	FormatRect.left+=xpos;
	FormatRect.right+=xpos;
	FormatRect.top+=ypos;
	FormatRect.bottom+=ypos;

	//draw string
	m_pFont->DrawText(0, Text.c_str(), -1, &FormatRect, DT_LEFT, D3DXCOLOR(255, 255, 222, 255));
}

void D3D10DeviceWrapper::Clear()const
{
    //
    // Clear the back buffer
    //
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; //red,green,blue,alpha
	m_pD3DDevice->ClearRenderTargetView( m_pRenderTargetView, ClearColor );
   //
    // Clear the depth buffer to 1.0 (max depth)
    //
    m_pD3DDevice->ClearDepthStencilView( m_pDepthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0 );

}
void D3D10DeviceWrapper::Present()const
{
	m_pSwapChain->Present( 1, 0 );
}
void D3D10DeviceWrapper::CreateRasterizerStates()
{
	D3D10_RASTERIZER_DESC RastDescr;
	RastDescr.AntialiasedLineEnable = false;
	RastDescr.CullMode = D3D10_CULL_BACK;
	RastDescr.FrontCounterClockwise = false;
	RastDescr.DepthBias = false;
	RastDescr.DepthBiasClamp = 0;
	RastDescr.DepthClipEnable = false;
	RastDescr.FillMode = D3D10_FILL_SOLID;
	RastDescr.MultisampleEnable = false;
	RastDescr.ScissorEnable = false;
	RastDescr.SlopeScaledDepthBias = 0;

	// create solid state
	m_pD3DDevice->CreateRasterizerState(&RastDescr, &m_pRasterizerStateSolid);
	// create wireframe state
	RastDescr.FillMode = D3D10_FILL_WIREFRAME;
	m_pD3DDevice->CreateRasterizerState(&RastDescr, &m_pRasterizerStateWireframe);
	
	RastDescr.FillMode = D3D10_FILL_SOLID;

	// create state that hides backfacing triangles
	RastDescr.CullMode = D3D10_CULL_BACK;
	m_pD3DDevice->CreateRasterizerState(&RastDescr, &m_pRasterizerStateCullBackFacing);
	
	// create state that hides frontfacing triangles
	RastDescr.CullMode = D3D10_CULL_FRONT;
	m_pD3DDevice->CreateRasterizerState(&RastDescr, &m_pRasterizerStateCullFrontFacing);

	// create state that renders all triangles
	RastDescr.CullMode = D3D10_CULL_NONE;
	m_pD3DDevice->CreateRasterizerState(&RastDescr, &m_pRasterizerStateCullNone);

	//use the solid state
	m_pD3DDevice->RSSetState(m_pRasterizerStateSolid);
	
}
void D3D10DeviceWrapper::EnableWireframe(bool enable)const
{
	if(enable)m_pD3DDevice->RSSetState(m_pRasterizerStateWireframe);
	else m_pD3DDevice->RSSetState(m_pRasterizerStateSolid);
}
void D3D10DeviceWrapper::SetCullMode(int mode=1)const
{
	switch (mode)
	{
	case 0:
		m_pD3DDevice->RSSetState(m_pRasterizerStateCullNone);
		break;
	case 1:
		m_pD3DDevice->RSSetState(m_pRasterizerStateCullBackFacing);
		break;
	case 2:
		m_pD3DDevice->RSSetState(m_pRasterizerStateCullFrontFacing);
	}
}
void D3D10DeviceWrapper::CreateBlendStates()
{
	D3D10_BLEND_DESC BlendDescr = {0};
	BlendDescr.AlphaToCoverageEnable = FALSE;
	BlendDescr.BlendEnable[0]			= TRUE;
	BlendDescr.SrcBlend					= D3D10_BLEND_SRC_ALPHA;
	BlendDescr.DestBlend				= D3D10_BLEND_INV_SRC_ALPHA;
	BlendDescr.BlendOp					= D3D10_BLEND_OP_ADD;
	BlendDescr.SrcBlendAlpha			= D3D10_BLEND_ONE;
	BlendDescr.DestBlendAlpha			= D3D10_BLEND_ZERO;
	BlendDescr.BlendOpAlpha			= D3D10_BLEND_OP_ADD;
	BlendDescr.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

	HR(m_pD3DDevice->CreateBlendState(&BlendDescr, &m_pTransparantBlendState));

}
void D3D10DeviceWrapper::EnableAlphaBlend(bool enable) const
{
	float blendFactors[]={0.0f,0.0f,0.0f,0.0f};
	if(enable)
	{
		m_pD3DDevice->OMSetBlendState(m_pTransparantBlendState,blendFactors,0xffffffff);
	}
	else
	{
		m_pD3DDevice->OMSetBlendState(NULL,blendFactors,0xffffffff);
	}
}

DX10ObjMesh* D3D10DeviceWrapper::Compile(ObjMesh* mesh)
{
	DX10ObjMesh* result = NULL;
	if(mesh->GetSkeleton() == NULL)
		result = new DX10ObjMesh(_T("./Effect/PNTa.fx"),_T("TechLinear"));
	else
	{
		result = new DX10ObjMesh(_T("./Effect/PNTa.fx"),_T("TechAnim"));
		result->SetSkeleton(mesh->GetSkeleton());
	}

	ID3D10Buffer* vertexBuffer = CompileVertexBuffer(mesh->GetVertexList());
	result->SetVertexBuffer(vertexBuffer,mesh->GetVertexList()->GetSizePerVertexInBytes());
	
	ID3D10InputLayout* layout = CompileInputLayout(mesh->GetVertexList(),result->GetDefaultTechnique());
	result->SetInputLayout(layout);

	IBMIT it;

	for ( it = mesh->GetBegin(); it != mesh->GetEnd(); ++it){
		DX10MaterialIndexBuffer* pIndexBuffer = CompileIndexBuffer(*it,result->GetEffect());
		result->AddIndexBuffer(pIndexBuffer);
	}

	return result;
}

ID3D10Buffer* D3D10DeviceWrapper::CompileVertexBuffer(VertexList* vertexList)
{
	ID3D10Buffer* pVertexBuffer = NULL;
	int size = vertexList->GetSizePerVertexInBytes()*vertexList->GetSize();
	D3D10_BUFFER_DESC bd;
	bd.Usage = D3D10_USAGE_IMMUTABLE;
	bd.ByteWidth = size;
	bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertexList->GetData();

	HR(GETD3D10DEVICE->CreateBuffer( &bd, &InitData, &pVertexBuffer ));

	return pVertexBuffer;
}

ID3D10InputLayout* D3D10DeviceWrapper::CompileInputLayout(
	VertexList* vertexList, 
	ID3D10EffectTechnique*  pTechnique)
{
	//m_pLogger->LogMessage(_T("Compiling vertexlist"));
	int recordSize =vertexList->GetSizePerVertexInBytes();
	int sizeInBytes = vertexList->GetSize() * recordSize;
	// create the vertex declaration
	int nrOfSemantics = vertexList->GetNumberOfSemantics();
	D3D10_INPUT_ELEMENT_DESC* idelements = new D3D10_INPUT_ELEMENT_DESC[nrOfSemantics];
	
	int index = 0;
	
	int offset=0;
	int tcUsageIndex = 0;
	int tangentUsageIndex = 0;
	
	for ( int i = 0 ; i < nrOfSemantics; i++ )
	{	
		D3D10_INPUT_ELEMENT_DESC element;
		element.SemanticIndex = 0;
		switch(vertexList->GetSemantic(i))
		{
			case POSITION:element.SemanticName="POSITION";break;
			case NORMAL:element.SemanticName="NORMAL";break;
			case TEXCOORD:
				element.SemanticName="TEXCOORD";
				element.SemanticIndex=tcUsageIndex++;
				break;
			case TANGENT:
				element.SemanticName="TANGENT";
				element.SemanticIndex=tangentUsageIndex++;
				break;
			case BONEINDEX:
				element.SemanticName="BLENDINDICES";
				break;
			case BONEWEIGHT:
				element.SemanticName="BLENDWEIGHT";
				break;
			case COLOR:
				element.SemanticName="COLOR";break;
		}
		
		switch(vertexList->GetSemanticSize(i))
		{
			case 1: element.Format=DXGI_FORMAT_R32_FLOAT;break;
			case 2: element.Format=DXGI_FORMAT_R32G32_FLOAT;break;
			case 3: element.Format=DXGI_FORMAT_R32G32B32_FLOAT;break;
			case 4: element.Format=DXGI_FORMAT_R32G32B32A32_FLOAT;break;
		}
		
		element.InputSlot = 0;
		element.AlignedByteOffset = D3D10_APPEND_ALIGNED_ELEMENT;
		element.InputSlotClass =D3D10_INPUT_PER_VERTEX_DATA;
		element.InstanceDataStepRate = 0;
		
		idelements[index] = element;
		index++;
	}
	ID3D10InputLayout* pVertexLayout = NULL;
	// Create the input layout
    D3D10_PASS_DESC PassDesc;
	// Get the pass decriptor from the effect technique
	ID3D10EffectPass*  pass = pTechnique->GetPassByIndex( 0 );
    pass->GetDesc( &PassDesc );
    HR(GETD3D10DEVICE->CreateInputLayout( idelements, 
		vertexList->GetNumberOfSemantics(), 
		PassDesc.pIAInputSignature, 
		PassDesc.IAInputSignatureSize, 
		&pVertexLayout ));

	delete[] idelements;
	return pVertexLayout;
}

DX10MaterialIndexBuffer* D3D10DeviceWrapper::CompileIndexBuffer(
	IndexBufferMaterial* material, 
	ID3D10Effect*  pEffect)
{
	DX10MaterialIndexBuffer* result =new DX10MaterialIndexBuffer();

	ID3D10ShaderResourceView * diffuseTextureRV = TEXTUREMANAGER.GetTexture(material->GetMaterial()->GetDiffuseTexture());
	result->SetDiffuseTexture(diffuseTextureRV);

	IndexedGeometry* indexBuffer = material->GetIndexBuffer();
	int numIndices = indexBuffer->GetNumberOfPrimitives()*3;

	D3D10_BUFFER_DESC bd;
	bd.Usage = D3D10_USAGE_DEFAULT;
	bd.ByteWidth = indexBuffer->GetSizeInBytes();        
	bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = indexBuffer->GetData();
	ID3D10Buffer* pTempIndexBuffer;
	HR(GETD3D10DEVICE->CreateBuffer( &bd, &InitData, &pTempIndexBuffer ));
	
	result->SetIndexBuffer(pTempIndexBuffer,indexBuffer->GetNumberOfPrimitives()*3);
	return result;
}
void D3D10DeviceWrapper::ResetRenderTargets()
{
	m_pD3DDevice->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	D3D10_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = DXWINDOW->GetWidth();
	vp.Height = DXWINDOW->GetHeight();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_pD3DDevice->RSSetViewports(1, &vp);
}