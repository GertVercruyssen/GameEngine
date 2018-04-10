// Bart Uyttenhove
// Singleton Class Implementation of DirectX 10 Device
//
#pragma once
#include "d3dUtil.h"
#include "util.h"

#include "SpotLight.h"

#include <vector>
//defines for readable code
#define LIGHTMANAGER (LightManager::GetSingleton())

class LightManager
{
public:
	//static member funtion
	static LightManager& GetSingleton();

	//**Shader Map methods

	/**
	* Adds a Light object with the provided key.
	* @param key the key to add the Light object with.
	* @param Light the XMeshShader object to add.
	*/
	SpotLight* AddLight(SpotLight* light);
	/**
	* Removes a Light object from the map.
	* @param key the key to remove.
	*/
	void RemoveLight(const SpotLight& light);
	/**
	* Removes all Light object from the map.
	*/
	void RemoveAllLights();

	//destructor
	virtual ~LightManager(void);


private:
	//private constructor
	LightManager(void);
	/**
	* Map with the created effects.
	*/
	vector<SpotLight*> m_Lights;
	int m_LightCounter;

private:
	LightManager(const LightManager& t);
	LightManager& operator=(const LightManager& t);

};
