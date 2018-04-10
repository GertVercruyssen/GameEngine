#include "StdAfx.h"
#include "SpotLight.h"

SpotLight::SpotLight(D3DXVECTOR3 position, 
		D3DXVECTOR3 direction,
		D3DXVECTOR3 attenuation,
		D3DXCOLOR color,
		float range,
		float spotPower
		):m_Location(position),m_Direction(direction),m_Attenuation(attenuation),m_Color(color),m_Range(range),m_SpotPower(spotPower) 
{
}