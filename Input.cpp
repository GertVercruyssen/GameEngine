#include "StdAfx.h"
#include "Input.h"

Input& Input::GetSingleton()
{
	//alternatieve singleton implementatie
	//deze code zorgt ervoor dat de destructor automatisch wordt gecalled en het singleton wordt 
	//verwijderd als het programma exit
	static Input oInput;
	return oInput;
}
Input::Input(void) :
	m_bLMBPressed(false),
	m_bRMBPressed(false),
	m_WheelPos(0), 
	m_OldWheelPos(0),
	m_ZDelta(0)
{
	m_MouseMovement.x=0;
	m_MouseMovement.y=0;
	m_OldMousePos.x=0;
	m_OldMousePos.y=0;
	m_MousePos.x=0;
	m_MousePos.y=0;
}

Input::~Input(void)
{
}
void Input::Tick(float dTime)
{
	//calc changes since last visit

	//position
	m_MouseMovement.x = m_MousePos.x - m_OldMousePos.x;
	m_MouseMovement.y = m_MousePos.y - m_OldMousePos.y;
	m_OldMousePos = m_MousePos;

	//mousewheel
	m_ZDelta = (m_WheelPos-m_OldWheelPos);
	m_OldWheelPos = m_WheelPos = 0;

	//get status of all keys
	BOOL b = GetKeyboardState(m_keys);
}
bool Input::IsKeyDown(int vKey)
{
	if(m_keys[vKey] & 0x80)return true;
	return false;
}
POINTS Input::GetMouseMovement()
{
	return m_MouseMovement;
}
void Input::SetMousePos(POINTS pos)
{
	m_MousePos=pos;
}
POINTS Input::GetMousePos()
{
	return m_MousePos;
}
void Input::SetLMousePressed(bool isPressed)
{
	m_bLMBPressed = isPressed;
}
void Input::SetRMousePressed(bool isPressed)
{
	m_bRMBPressed = isPressed;
}
void Input::SetWheelDelta(short zDelta)
{
	m_WheelPos = zDelta;
}
short  Input::GetWheelDelta()
{
	return m_ZDelta;
}
