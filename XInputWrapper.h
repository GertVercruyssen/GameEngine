// Bart Uyttenhove
// Singleton Class Implementation of XInput used to read data from XBox controller
//
#pragma once
#include "XInput.h"
#pragma comment(lib, "XInput.lib")


#include <map>
//defines for readable code
#define XINPUT (XInputWrapper::GetSingleton())


class XInputWrapper
{
public:
	//static member funtion
	static XInputWrapper&	GetSingleton();

	//Get input from controllers, to be called every frame
	void Tick();
	//has input changed since last call.?
	bool IsInputChanged(int ControllerNumber);

	bool IsControllerConnected(int ControllerNumber);
	//get input from buttons
	bool IsControllerButtonPressed(int ControllerNumber, WORD wButton);
	//get input from thumb sticks
	bool GetInputFromLeftThumbStick(int ControllerNumber,float &magnitude, float &normalizedX, float &normalizedY);
	bool GetInputFromRightThumbStick(int ControllerNumber,float &magnitude, float &normalizedX, float &normalizedY);
	//get input from triggers
	bool GetInputFromLeftTrigger(int ControllerNumber,BYTE& trigger);
	bool GetInputFromRightTrigger(int ControllerNumber,BYTE& trigger);
	//destructor
	virtual ~XInputWrapper(void);

protected:
	static const int MAX_CONTROLLERS = 4;
	int INPUT_DEADZONE;
	XINPUT_STATE m_OldInputState[MAX_CONTROLLERS];
	XINPUT_STATE m_InputState[MAX_CONTROLLERS];
	bool m_bControllerConnected[MAX_CONTROLLERS];
private:
	//private constructor
	XInputWrapper(void);

	XInputWrapper(const XInputWrapper& t);
	XInputWrapper& operator=(const XInputWrapper& t);

};
