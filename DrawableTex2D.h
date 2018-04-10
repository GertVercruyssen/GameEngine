#pragma once

#include "D3DUtil.h"
/**
* Class to create offscreen textures.
* Generates color map textures as well as depth textures.
*/
class DrawableTex2D
{
public:
	/**
	* Creates a new DrawableTex2D object.
	*/
	DrawableTex2D(void);
	/**
	* Destroys the DrawableTex2D object.
	*/
	virtual ~DrawableTex2D(void);

	/**
	* Initializes this DrawableTex2D object with the given parameters.
	* @param width the width for the offscreen texture.
	* @param height the height for the offscreen texture.
	* @param hasColorMap does this DrawableTex2D object have a color map.
	* @parma colorFormat the storage format for the color map.
	*/
	void Init(UINT width, UINT height, bool hasColorMap, DXGI_FORMAT colorFormat);

	/**
	* Returns the color map from this DrawableTex2D object.
	* @return the colorMap object (can be set as parameter for a shader).
	*/
	ID3D10ShaderResourceView* GetColorMap();
	/**
	* Returns the depth map from this DrawableTex2D object.
	* @return the depthMap object (can be set as parameter for a shader).
	*/
	ID3D10ShaderResourceView* GetDepthMap();

	/**
	* Begin rendering the scene.
	*/
	void Begin();
	/**
	* End the rendering of the scene.
	*/
	void End();

private:
	DrawableTex2D(const DrawableTex2D& dt2d);
	DrawableTex2D& operator=(const DrawableTex2D& dt2d);

	void BuildDepthMap();
	void BuildColorMap();

private:
	UINT m_Width;
	UINT m_Height;
	DXGI_FORMAT m_ColorMapFormat;

	ID3D10Device* m_pDevice;

	ID3D10ShaderResourceView* m_pColorMapSRV;
	ID3D10RenderTargetView* m_pColorMapRTV;

	ID3D10ShaderResourceView* m_pDepthMapSRV;
	ID3D10DepthStencilView* m_pDepthMapDSV;

	D3D10_VIEWPORT m_ViewPort;
};
