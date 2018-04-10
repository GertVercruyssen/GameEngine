// Bart Uyttenhove
// Singleton Class Implementation of DirectX 10 Device
//
#pragma once
//Direct3D
#include "d3d10.h"
#include "d3dx10.h"
#include "D3DX10Core.h"
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3d10.lib")

#include <map>
//defines for readable code
#define TEXTUREMANAGER (TextureManager::GetSingleton())


class TextureManager
{
public:
	//static member funtion
	static TextureManager&	GetSingleton();

	//**Shader Map methods

	/**
	* Adds a Texture object with the provided key.
	* @param key the key to add the Texture object with.
	* @param Texture the XMeshShader object to add.
	*/
	void AddTexture(const tstring &key, ID3D10ShaderResourceView* texture);
	/**
	* Removes a Texture object from the map.
	* @param key the key to remove.
	*/
	void RemoveTexture(const tstring &key);
	/**
	* Removes all Texture object from the map.
	*/
	void RemoveAllTextures();
	/**
	* Checks if an Texture is present.
	* @return true if the Texture is present, false otherwise.
	*/
	bool IsTexturePresent(const tstring &key)const;
	/**
	* Gets the Texture with the provided key.
	* @param key the Texture key.
	* @return the Texture if an Texture is stored in the map with the provided key, 
	* NULL otherwise.
	*/
	ID3D10ShaderResourceView* GetTexture(const tstring& key);


	//get rid of all VRAM resources
	//void OnLostDevice();
	//Device has been reset
	//void OnResetDevice();
	//destructor
	virtual ~TextureManager(void);

	ID3D10ShaderResourceView* GetCubeMap(const tstring& key);


private:
	//private constructor
	TextureManager(void);
	/**
	* Map with the created textures.
	*/
	map<tstring,ID3D10ShaderResourceView*> m_Textures;

private:
	TextureManager(const TextureManager& t);
	TextureManager& operator=(const TextureManager& t);

};
