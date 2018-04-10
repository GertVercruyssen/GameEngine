#pragma once

#define INPUT (Input::GetSingleton())

class Input
{
public:
	static Input& GetSingleton();
	virtual ~Input(void);
	
	bool IsKeyDown(int vKey);
	POINTS GetMouseMovement();
	POINTS GetMousePos();
	void SetMousePos(POINTS pos);
	bool IsLeftMouseButtonDown(){return m_bLMBPressed;}
	bool IsMiddleMouseButtonDown();
	bool IsRightMouseButtonDown(){return m_bRMBPressed;}
	void SetLMousePressed(bool isPressed);
	void SetRMousePressed(bool isPressed);
	void SetWheelDelta(short zDelta);
	short GetWheelDelta();

	void Tick(float dTime);
protected:
		//mouse pos
	POINTS m_MousePos, m_OldMousePos, m_MouseMovement;
	bool m_bLMBPressed,m_bRMBPressed;
	short m_WheelPos, m_OldWheelPos,m_ZDelta;
	BYTE m_keys[256];
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, the declarations are deliberately made without implementation because they should never be used.
	// -------------------------

private:
	Input(void);
	Input(const Input& g);
	Input& operator=(const Input& g);

};
