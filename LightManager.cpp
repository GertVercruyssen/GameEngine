#include "StdAfx.h"
#include "LightManager.h"
#include "d3dUtil.h"
#include "D3D10DeviceWrapper.h"

LightManager::LightManager(void):m_LightCounter(0)
{

}

LightManager::~LightManager(void)
{
	RemoveAllLights();
}

LightManager& LightManager::GetSingleton()
{
	//alternatieve singleton implementatie
	//deze code zorgt ervoor dat de destructor automatisch wordt gecalled en het singleton wordt 
	//verwijderd als het programma exit
	static LightManager oLightManager;
	return oLightManager;
}

SpotLight* LightManager::AddLight(SpotLight* light)
{
	int i = m_LightCounter;
	#if defined (_DEBUG) || defined (_DEBUG)
		tstringstream strstr;
		strstr<<_T("Adding Light ")<<_T("\n");
		OutputDebugString(strstr.str().c_str());
	#endif

	m_Lights.push_back(light);
	++m_LightCounter;

	return m_Lights[i];
}

void LightManager::RemoveLight(const SpotLight& light)
{
	#if defined (_DEBUG) || defined (_DEBUG)
		tstringstream strstr;
		strstr<<_T("Removing Light ")<<_T("\n");
		OutputDebugString(strstr.str().c_str());
	#endif

	vector<SpotLight*>::iterator it;
	for(it=m_Lights.begin(); it != m_Lights.end();++it)
	{
		//if((*it) == light)
		delete (*it);
		(*it) = 0;
	}
}

void LightManager::RemoveAllLights()
{
	#if defined (_DEBUG) || defined (_DEBUG)
		OutputDebugString(_T("Releasing all SpotLights.\n"));
	#endif

	vector<SpotLight*>::iterator it;
	for(it=m_Lights.begin(); it != m_Lights.end();++it)
	{
		delete (*it);
		(*it) = 0;
	}

	m_Lights.clear();
}
