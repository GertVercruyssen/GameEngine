#include "StdAfx.h"
#include "DrawableTex2D.h"
#include "D3D10DeviceWrapper.h"

DrawableTex2D::DrawableTex2D():
m_Width(0),m_Height(0),m_ColorMapFormat(DXGI_FORMAT_UNKNOWN),m_pDevice(0),
m_pColorMapSRV(0),m_pColorMapRTV(0),
m_pDepthMapSRV(0),m_pDepthMapDSV(0)
{
	ZeroMemory(&m_ViewPort,sizeof(D3D10_VIEWPORT));
}

DrawableTex2D::~DrawableTex2D(void)
{
	SAFE_RELEASE(m_pColorMapSRV);
	SAFE_RELEASE(m_pDepthMapSRV);
	SAFE_RELEASE(m_pColorMapRTV);
	SAFE_RELEASE(m_pDepthMapDSV);
}

ID3D10ShaderResourceView* DrawableTex2D::GetColorMap(){
	return m_pColorMapSRV;
}

ID3D10ShaderResourceView* DrawableTex2D::GetDepthMap(){
	return m_pDepthMapSRV;
}

void DrawableTex2D::Init(UINT width, UINT height, bool hasColorMap, DXGI_FORMAT colorFormat)
{
	m_Width = width;
	m_Height = height;
	m_ColorMapFormat = colorFormat;

	m_pDevice = GETD3D10DEVICE;

	BuildDepthMap();

	if ( hasColorMap )
		BuildColorMap();

	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_ViewPort.Width = width;
	m_ViewPort.Height = height;
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
}

void DrawableTex2D::BuildDepthMap()
{
	ID3D10Texture2D* depthMap = 0;
	D3D10_TEXTURE2D_DESC texDesc;
	texDesc.Width = m_Width;
	texDesc.Height = m_Height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1 ;
	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D10_USAGE_DEFAULT;
	texDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL | D3D10_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	HR ( m_pDevice->CreateTexture2D(&texDesc,0,&depthMap));

	D3D10_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	HR ( m_pDevice->CreateDepthStencilView ( depthMap, &dsvDesc, &m_pDepthMapDSV ));

	D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	HR ( m_pDevice->CreateShaderResourceView ( depthMap, &srvDesc, &m_pDepthMapSRV) );

	SAFE_RELEASE ( depthMap );
}

void DrawableTex2D::BuildColorMap()
{
	ID3D10Texture2D* colorMap = 0;
	D3D10_TEXTURE2D_DESC texDesc;
	texDesc.Width = m_Width;
	texDesc.Height = m_Height;
	texDesc.MipLevels = 0;
	texDesc.ArraySize = 1;
	texDesc.Format = m_ColorMapFormat;
	texDesc.SampleDesc.Count = 1 ;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D10_USAGE_DEFAULT;
	texDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D10_RESOURCE_MISC_GENERATE_MIPS;

	HR ( m_pDevice->CreateTexture2D(&texDesc,0,&colorMap) );

	HR ( m_pDevice->CreateRenderTargetView(colorMap, 0, &m_pColorMapRTV));
	HR ( m_pDevice->CreateShaderResourceView(colorMap, 0, &m_pColorMapSRV));

	ReleaseCOM(colorMap);
}

void DrawableTex2D::Begin()
{
	ID3D10RenderTargetView* renderTargets[1] = {m_pColorMapRTV};
	m_pDevice->OMSetRenderTargets(1,renderTargets,m_pDepthMapDSV);
	m_pDevice->RSSetViewports(1,&m_ViewPort);

	if ( m_pColorMapRTV)
		m_pDevice->ClearRenderTargetView(m_pColorMapRTV, BLACK);

	m_pDevice->ClearDepthStencilView(m_pDepthMapDSV,D3D10_CLEAR_DEPTH,1.0f,0);

	
}

void DrawableTex2D::End()
{
	if ( m_pColorMapSRV )
		m_pDevice->GenerateMips(m_pColorMapSRV);
}