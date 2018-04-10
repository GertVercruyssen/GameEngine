#include "StdAfx.h"
#include "TextureManager.h"
#include "d3dUtil.h"
#include "D3D10DeviceWrapper.h"

TextureManager::TextureManager(void)
{

}

TextureManager::~TextureManager(void)
{
	RemoveAllTextures();
}

TextureManager& TextureManager::GetSingleton()
{
	//deze code zorgt ervoor dat de destructor automatisch wordt gecalled en het singleton wordt 
	//verwijderd als het programma exit
	static TextureManager oTextureManager;
	return oTextureManager;

}

void TextureManager::AddTexture(const tstring& key, ID3D10ShaderResourceView* texture)
{
	tstringstream strstr;
	strstr<<_T("Adding Texture ")<<key<<_T("\n");
	OutputDebugString(strstr.str().c_str());

	m_Textures[key]=texture;
}

void TextureManager::RemoveTexture(const tstring& key)
{
	tstringstream strstr;
	strstr<<_T("Removing Texture ")<<key<<_T("\n");
	OutputDebugString(strstr.str().c_str());

	SAFE_RELEASE(m_Textures[key]);
	m_Textures.erase(key);
}

void TextureManager::RemoveAllTextures()
{
	OutputDebugString(_T("Releasing all ID3DTextures.\n"));
	map<tstring,ID3D10ShaderResourceView*>::iterator it;
	for(it=m_Textures.begin(); it != m_Textures.end();++it)
	{
		SAFE_RELEASE((*it).second);
	}
	m_Textures.clear();
}

bool TextureManager::IsTexturePresent(const tstring& key) const
{
	map<tstring,ID3D10ShaderResourceView*>::iterator it;
	return  m_Textures.find(key) != m_Textures.end();
	return false;
}

ID3D10ShaderResourceView* TextureManager::GetTexture(const tstring& Filename) 
{
	if ( IsTexturePresent(Filename))
	{
		OutputDebugString(_T("Using Existing Texture.\n"));
	}
	else
	{
		OutputDebugString(_T("Loading New Texture.\n"));
		ID3D10ShaderResourceView *pTextureRV;
		HRESULT hr = D3DX10CreateShaderResourceViewFromFile( GETD3D10DEVICE, Filename.c_str(), NULL, NULL, &pTextureRV, NULL );
		if(hr!=S_OK)
		{
			tstringstream tstrstr;
			tstrstr << _T("Loading texture ") << Filename << _T(" Failed  !!!");
			MessageBox(0,tstrstr.str().c_str(),_T("ERROR"),0);
			return 0;
		}
		AddTexture(Filename, pTextureRV);
	}
	return m_Textures[Filename];
}
ID3D10ShaderResourceView* TextureManager::GetCubeMap(const tstring& key)
{
	if ( IsTexturePresent(key))
	{
		OutputDebugString(_T("Using Existing Texture.\n"));
	}
	else
	{
		OutputDebugString(_T("Loading New Texture.\n"));

		D3DX10_IMAGE_LOAD_INFO m_imageinfo;
		m_imageinfo.MiscFlags = 4;

		ID3D10Resource *pCubeMap;
		HRESULT hr = D3DX10CreateTextureFromFile( GETD3D10DEVICE, key.c_str(), &m_imageinfo, NULL, &pCubeMap, NULL );
		if(hr!=S_OK)
		{
			D3D10_SHADER_RESOURCE_VIEW_DESC* pViewDesc;
			pViewDesc = pCubeMap->
			pViewDesc->TextureCube.MostDetailedMip = 0;

			ID3D10ShaderResourceView *pTextureRV;
			//HRESULT hr2 = D3DX10CreateShaderResourceView();
			if(hr!=S_OK)
			{
				AddTexture(key, pTextureRV);
				pCubeMap->Release();
			}
		}
	}
	return m_Textures[key];
}