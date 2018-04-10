#pragma once
#include "windows.h"
#include "d3dUtil.h"


class AbstractDXGame
{
public:
	virtual ~AbstractDXGame(void);

	// Framework methods.  Derived client class overrides these methods to 
	// implement specific application requirements.

	virtual void UpdateScene(float dt)=0;
	virtual void DrawScene()=0;
	virtual void EnableFullScreenMode(bool enable);
	virtual bool Init()=0;
	virtual void Paint(float dTime)=0;
	virtual void KeyPressed(WPARAM cKey) {}	
	virtual void OnResize();
	virtual void SetWindowParams();
	void SetWindowHandle(HWND hWindow){m_hWindow=hWindow;};

protected:
	HWND				m_hWindow;
	AbstractDXGame(void);

private:
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------

	AbstractDXGame(const AbstractDXGame& t);
	AbstractDXGame& operator=(const AbstractDXGame& t);

};
