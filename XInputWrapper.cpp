#include "StdAfx.h"
#include "XInputWrapper.h"
#include "math.h"

XInputWrapper::XInputWrapper(void):INPUT_DEADZONE(10)
{
	for (DWORD i=0; i< MAX_CONTROLLERS; i++ )
	{
		ZeroMemory( &m_InputState[i] , sizeof(XINPUT_STATE) );
		ZeroMemory( &m_OldInputState[i] , sizeof(XINPUT_STATE) );
		m_bControllerConnected[i]=false;
	}
}

XInputWrapper::~XInputWrapper(void)
{
}
XInputWrapper& XInputWrapper::GetSingleton()
{
	//alternatieve singleton implementatie
	//deze code zorgt ervoor dat de destructor automatisch wordt gecalled en het singleton wordt 
	//verwijderd als het programma exit
	static XInputWrapper oXInputSingleton;
	return oXInputSingleton;
}
void XInputWrapper::Tick()
{
	DWORD dwResult;    
	for (DWORD i=0; i< MAX_CONTROLLERS; i++ )
	{
		m_OldInputState[i] = m_InputState[i];
		// Simply get the state of the controllers from XInput.
		dwResult = XInputGetState( i, &m_InputState[i] );

		if( dwResult == ERROR_SUCCESS )
		{ 
			// Controller is connected 
			m_bControllerConnected[i]=true;
		}
		else
		{
			// Controller is not connected 
			m_bControllerConnected[i]=false;
		}
	}
}
bool XInputWrapper::IsInputChanged(int ControllerNumber)
{
	if(IsControllerConnected(ControllerNumber))
	{
		if(m_OldInputState[ControllerNumber].dwPacketNumber!=m_InputState[ControllerNumber].dwPacketNumber)return true;
	}
	return false;
}
bool XInputWrapper::IsControllerConnected(int ControllerNumber)
{
	if(ControllerNumber>=0 && ControllerNumber<=MAX_CONTROLLERS)
	{
		if(m_bControllerConnected[ControllerNumber])
		{
			return true;
		}
	}
	return false;
}
bool XInputWrapper::IsControllerButtonPressed(int ControllerNumber, WORD wButton)
{
	if(!IsControllerConnected(ControllerNumber))return false;

	if(m_InputState[ControllerNumber].Gamepad.wButtons & wButton)return true;

	return false;

}
bool XInputWrapper::GetInputFromLeftThumbStick(int ControllerNumber, float &normalizedMagnitude, float &normalizedX, float &normalizedY)
{
	if(!IsControllerConnected(ControllerNumber))return false;

	XINPUT_STATE state = m_InputState[ControllerNumber];

	float stickX = state.Gamepad.sThumbLX;
	float stickY = state.Gamepad.sThumbLY;

	//determine how far the controller is pushed
	float magnitude = sqrt(stickX*stickX + stickY*stickY);

	//determine the direction the controller is pushed
	normalizedX = stickX / magnitude;
	normalizedY = stickY / magnitude;

	normalizedMagnitude = 0;

	//check if the controller is outside a circular dead zone
	if (magnitude > INPUT_DEADZONE)
	{
	  //clip the magnitude at its expected maximum value
	  if (magnitude > 32767) magnitude = 32767;
	  
	  //adjust magnitude relative to the end of the dead zone
	  magnitude -= INPUT_DEADZONE;

	  //optionally normalize the magnitude with respect to its expected range
	  //giving a magnitude value of 0.0 to 1.0
	  normalizedMagnitude = magnitude / (32767 - INPUT_DEADZONE);
	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		magnitude = 0.0;
		normalizedMagnitude = 0.0;
	}

	return true;
}
bool XInputWrapper::GetInputFromRightThumbStick(int ControllerNumber, float &normalizedMagnitude, float &normalizedX, float &normalizedY)
{
	if(!IsControllerConnected(ControllerNumber))return false;

	XINPUT_STATE state = m_InputState[ControllerNumber];

	float stickX = state.Gamepad.sThumbRX;
	float stickY = state.Gamepad.sThumbRY;

	//determine how far the controller is pushed
	float magnitude = sqrt(stickX*stickX + stickY*stickY);

	//determine the direction the controller is pushed
	normalizedX = stickX / magnitude;
	normalizedY = stickY / magnitude;

	normalizedMagnitude = 0;

	//check if the controller is outside a circular dead zone
	if (magnitude > INPUT_DEADZONE)
	{
	  //clip the magnitude at its expected maximum value
	  if (magnitude > 32767) magnitude = 32767;
	  
	  //adjust magnitude relative to the end of the dead zone
	  magnitude -= INPUT_DEADZONE;

	  //optionally normalize the magnitude with respect to its expected range
	  //giving a magnitude value of 0.0 to 1.0
	  normalizedMagnitude = magnitude / (32767 - INPUT_DEADZONE);
	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		magnitude = 0.0;
		normalizedMagnitude = 0.0;
	}

	return true;
}

//get input from triggers
bool XInputWrapper::GetInputFromLeftTrigger(int ControllerNumber,BYTE& trigger)
{
	if(!IsControllerConnected(ControllerNumber))return false;
	trigger = m_InputState[ControllerNumber].Gamepad.bLeftTrigger;
	return true;
}
bool XInputWrapper::GetInputFromRightTrigger(int ControllerNumber,BYTE& trigger)
{
	if(!IsControllerConnected(ControllerNumber))return false;
	trigger = m_InputState[ControllerNumber].Gamepad.bRightTrigger;
	return true;
}
