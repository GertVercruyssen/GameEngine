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
#define EFFECTMANAGER (EffectManager::GetSingleton())


class EffectManager
{
public:
	//static member funtion
	static EffectManager&	GetSingleton();

	//**Shader Map methods

	/**
	* Adds a Effect object with the provided key.
	* @param key the key to add the Effect object with.
	* @param Effect the XMeshShader object to add.
	*/
	void AddEffect(const tstring& key, ID3D10Effect* shader);
	/**
	* Removes a Effect object from the map.
	* @param key the key to remove.
	*/
	void RemoveEffect(const tstring& key);
	/**
	* Removes all Effect object from the map.
	*/
	void RemoveAllEffects();
	/**
	* Checks if an Effect is present.
	* @return true if the Effect is present, false otherwise.
	*/
	bool IsEffectPresent(const tstring& key)const;
	/**
	* Gets the Effect with the provided key.
	* @param key the Effect key.
	* @return the Effect if an Effect is stored in the map with the provided key, 
	* NULL otherwise.
	*/
	ID3D10Effect* GetEffect(const tstring& key);

	//get rid of all VRAM resources
	//void OnLostDevice();
	//Device has been reset
	//void OnResetDevice();
	//destructor
	virtual ~EffectManager(void);


private:
	//private constructor
	EffectManager(void);
	/**
	* Map with the created effects.
	*/
	map<tstring,ID3D10Effect*> m_Effects;

private:
	EffectManager(const EffectManager& t);
	EffectManager& operator=(const EffectManager& t);

};
