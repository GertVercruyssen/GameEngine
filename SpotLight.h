#pragma once

#include "D3DX10Core.h"
/**
* This class defines a spotlight.
* @author Koen Samyn
*/
struct SpotLight
{
public:
	/**
	* Creates a new spotlight object.
	* @param position the position for the spotlight
	* @param direction the direction for the spotlight
	* @param attenuation the attenuation for the spotlight.
	* @param color the spotlight color.
	* @param range the range for the spotlight.
	* @param spotPower the value of the power for the spotlight.
	*/
	SpotLight(D3DXVECTOR3 position, 
		D3DXVECTOR3 direction,
		D3DXVECTOR3 attenuation,
		D3DXCOLOR color,
		float range,
		float spotPower
	);
	
	/**
	* Sets the location of the spotlight
	* @param location the new location for the spotlight.
	*/
	void SetLocation(D3DXVECTOR3 location){
		m_Location = location;
	}
	/**
	* Sets the direction for the spotlight.
	* @param direction the new direction for the spotlight.
	*/
	void SetDirection(D3DXVECTOR3 direction){
		m_Direction = direction;
	}
	/**
	* Sets the color for the spotlight.
	* @param color the color for the spotlight.
	*/
	void SetColor(D3DXCOLOR color){
		m_Color = color;
	}
	/**
	* 
	*/ 
private:
	D3DXVECTOR3 m_Location;
	float pad1;
	D3DXVECTOR3 m_Direction;
	float pad2;
	D3DXVECTOR3 m_Attenuation;
	float pad3;
	D3DXCOLOR m_Color;
	float m_SpotPower;
	float m_Range;
};
