// Bart Uyttenhove
// Singleton implementation of DXWindow class
//
#pragma once
#include <windows.h>
#include <tchar.h>
#include <string>
#include "GameTimer.h"

using namespace std;

//defines for readable code
#define DXWINDOW (DXWindow::GetSingleton())

//-----------------------------------------------------------------
// Forward Class Declarations
//-----------------------------------------------------------------
class AbstractDXGame;

//-----------------------------------------------------------------
// Windows Procedure Declarations
//-----------------------------------------------------------------
LRESULT CALLBACK	WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);



class DXWindow
{
public:
	//Destructor
	virtual ~DXWindow(void);
	// Static Method returns singleton pointer
	static DXWindow* GetSingleton(void);

	//sets game object pointer
	void		SetGame(AbstractDXGame* ptrGame);
	// opens window
	bool		ClassRegister(int iCmdShow);
	// init DirectX, init Game, enter messageloop
	bool		Run(HINSTANCE hInstance, int iCmdShow);
	// Handles Windows Events
	LRESULT HandleEvent(const HWND hWindow, const UINT msg, const WPARAM wParam,const  LPARAM lParam);

	// Getters and Setters
	int GetWidth()const {return m_iWidth;}
	int GetHeight()const {return m_iHeight;}
	void SetWidth(int Width);
	void SetHeight(int Height);
	void SetTitle(tstring const& strTitleRef);
	HINSTANCE 	GetInstance() const { return m_hInstance; };
	HWND GetWindowHandle()const {return m_hWindow;};

	// Returns tstring holding fps and frametime
	tstring GetFrameStats(){return m_sFrameStats;};


private:
	// singleton has private constructor
	DXWindow(void);

	// Private Methods	
	void SetInstance(HINSTANCE hInstance) { m_hInstance = hInstance; };
	void CalcFPS();

	// Private member variables
	static DXWindow*	m_pDXWindow;
	//holds GamePointer
	AbstractDXGame*	m_pGame;
	//GameTimer Object on Stack
	GameTimer m_Timer;

	HINSTANCE		m_hInstance;
	HWND			m_hWindow;
	int					m_iWidth, m_iHeight;
	tstring			m_Title;
	bool				m_AppPaused;
	bool				m_WireFrame;
	int					m_CullMode;
	bool			m_Minimized;
	bool			m_Maximized;
	bool			m_Resizing;
	bool			m_bInitReady;
	tstring		m_sFrameStats;

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, the declarations are deliberately made without implementation because they should never be used.
	// -------------------------

	DXWindow(const DXWindow& g);
	DXWindow& operator=(const DXWindow& g);
};
