#include "stdafx.h"
#include "DXWindow.h"
#include "AbstractDXGame.h"
#include "D3D10DeviceWrapper.h"
#include "Input.h"
//-----------------------------------------------------------------
// Static Variable Initialization
//-----------------------------------------------------------------
DXWindow* DXWindow::m_pDXWindow = NULL;

//-----------------------------------------------------------------
// Windows Functions
//-----------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Route all Windows messages to the game engine
	return DXWindow::GetSingleton()->HandleEvent(hWindow, msg, wParam, lParam);
}

DXWindow* DXWindow::GetSingleton(void)
{
	if ( m_pDXWindow == NULL) m_pDXWindow = new DXWindow();
	return m_pDXWindow;
}

DXWindow::DXWindow(void):	m_iWidth(800),m_iHeight(600),
							m_pGame(0),m_AppPaused(false),
							m_WireFrame(false),m_Maximized(false),m_Minimized(false),
							m_Resizing(false),m_bInitReady(false),m_CullMode(1)
{
}

DXWindow::~DXWindow(void)
{
	delete (D3D10DeviceWrapper::GetSingleton());
}

void DXWindow::SetGame(AbstractDXGame* ptrGame)
{
	m_pGame = ptrGame;
}
void DXWindow::SetWidth(int Width)
{
	m_iWidth = Width;
}
void DXWindow::SetHeight(int Height)
{
	m_iHeight = Height;
}
void DXWindow::SetTitle(tstring const& strTitleRef)
{
	m_Title = strTitleRef;
}

bool DXWindow::Run(HINSTANCE hInstance, int iCmdShow)
{
	MSG msg = {0};
	m_Timer.Reset();
	bool bQuit=false;

	
	// check the game engine object, exit if failure
	if (DXWindow::GetSingleton() == NULL) return false;

	// set the instance member variable of the dxengine
	SetInstance(hInstance);

	//game instance sets windowsize and windowtitle
	m_pGame->SetWindowParams();

	// Open the window
	if (!ClassRegister(iCmdShow)) return false;

	//create directX Device Singleton and initialize
	D3D10DeviceWrapper::GetSingleton()->InitDirect3D(m_hWindow, m_iWidth,m_iHeight);

	//set game window handle
	m_pGame->SetWindowHandle(m_hWindow);

	//init the game
	if(!m_pGame->Init())return false;

	m_bInitReady = true;
	// Enter the main message loop
	while (!bQuit)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// Process the message
			if (msg.message == WM_QUIT) break;//leave while loop if quit message
			TranslateMessage(&msg);
			DispatchMessage(&msg);//in x game, messages are processed 
		}
		else
		{
			m_Timer.Tick();
			if(!bQuit )
			{
				if( m_AppPaused )Sleep(20);
				else 
				{
					CalcFPS();
					
					m_pGame->Paint(m_Timer.GetDeltaTime());
				}
			}
		}
	}
	return msg.wParam?true:false;
}
bool DXWindow::ClassRegister(int iCmdShow)
{
	WNDCLASSEX    wndclass;

	// Create the window class for the main window
	wndclass.cbSize         = sizeof(WNDCLASSEX);
	wndclass.style          = CS_CLASSDC;
	wndclass.lpfnWndProc    = WndProc;
	wndclass.cbClsExtra     = 0;
	wndclass.cbWndExtra     = 0;
	wndclass.hInstance      = m_hInstance;
	wndclass.hIcon          = 0;
	wndclass.hIconSm        = 0;
	wndclass.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground  = NULL;//(HBRUSH)(COLOR_WINDOW + 1);
	wndclass.lpszMenuName   = NULL;
	wndclass.lpszClassName  = _T("DXWindow");

	// Register the window class
	if (!RegisterClassEx(&wndclass))
	{
		MessageBox(0, _T("RegisterClass FAILED"), 0, 0);
		PostQuitMessage(0);

		return false;
	}
	// Default to a window with a client area rectangle of m_iWidth x m_iHeight
	RECT R = {0, 0, m_iWidth, m_iHeight};
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);

	//center window
	int iXWindowPos = (GetSystemMetrics(SM_CXSCREEN) - m_iWidth)  / 2;
	int iYWindowPos = (GetSystemMetrics(SM_CYSCREEN) - m_iHeight) / 2;

	// Create the window
	m_hWindow = CreateWindow(	_T("DXWindow"), 
		m_Title.c_str(), 
		WS_OVERLAPPEDWINDOW , 
		iXWindowPos,iYWindowPos, 
		R.right-R.left,R.bottom-R.top, 
		NULL, NULL, 
		m_hInstance, NULL);
	if (!m_hWindow)	
	{
		MessageBox(0, _T("CreateWindow FAILED"), 0, 0);
		PostQuitMessage(0);
		return false;
	}
	// Show and update the window
	ShowWindow(m_hWindow, SW_SHOWDEFAULT);
	UpdateWindow(m_hWindow);

	return true;
}
LRESULT DXWindow::HandleEvent(const HWND hWindow,const  UINT msg,const  WPARAM wParam, const LPARAM lParam)
{
	if(!m_bInitReady)
	{
		return DefWindowProc(hWindow, msg, wParam, lParam);
	}
	POINTS currentpos;
	short zDelta;
	// Route Windows messages to game engine member functions when init is ready
	switch (msg)
	{
	// WM_ACTIVE is sent when the window is activated or deactivated.
	// We pause the game when the main window is deactivated and 
	// unpause it when it becomes active.
	case WM_ACTIVATE:
		if( LOWORD(wParam) == WA_INACTIVE )
		{
			m_AppPaused = true;
			m_Timer.Stop();
		}
		else
		{
			m_AppPaused = false;
			m_Timer.Start();
		}
		return 0;
	
	case WM_CREATE:
		// Set the game window 
		m_hWindow = hWindow;
		// Seed the random number generator
		srand(GetTickCount());
		return 0;

	case WM_DESTROY:
		// End the game and post quit message, so it gets in de main message loop. 
		PostQuitMessage(0);
		return 0;

	case WM_ENTERSIZEMOVE://is sent when user grabs resize bars
		m_AppPaused = true;
		m_Resizing = true;
		m_Timer.Stop();
		return 0;
	
	case WM_EXITSIZEMOVE: //is sent when user releases resize bars
		m_AppPaused = false;
		m_Resizing = false;
		m_Timer.Start();
		m_pGame->OnResize();
		return 0;
	
	case WM_SIZE:
		// Save the new client area dimensions.
		m_iWidth  = LOWORD(lParam);
		m_iHeight = HIWORD(lParam);
		if( wParam == SIZE_MINIMIZED )
		{
			m_AppPaused = true;
			m_Minimized = true;
			m_Maximized = false;
		}
		else if( wParam == SIZE_MAXIMIZED )
		{
			m_AppPaused = false;
			m_Minimized = false;
			m_Maximized = true;
			m_pGame->OnResize();
		}
		else if( wParam == SIZE_RESTORED )
		{
			
			// Restoring from minimized state?
			if( m_Minimized )
			{
				m_AppPaused = false;
				m_Minimized = false;
				m_pGame->OnResize();
			}

			// Restoring from maximized state?
			else if( m_Maximized )
			{
				m_AppPaused = false;
				m_Maximized = false;
				m_pGame->OnResize();
			}
			else if( m_Resizing )
			{
				// If user is dragging the resize bars, we do not resize 
				// the buffers here because as the user continuously 
				// drags the resize bars, a stream of WM_SIZE messages are
				// sent to the window, and it would be pointless (and slow)
				// to resize for each WM_SIZE message received from dragging
				// the resize bars.  So instead, we reset after the user is 
				// done resizing the window and releases the resize bars, which 
				// sends a WM_EXITSIZEMOVE message.
			}
			else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
			{
				m_pGame->OnResize();
			}
		}
		m_AppPaused = false;
		break;
	
	case WM_KEYDOWN:
		switch (wParam)
		{
			case 'W':
				if(m_WireFrame)
				{
					D3D10DeviceWrapper::GetSingleton()->EnableWireframe(false);
					m_WireFrame=false;
				}
				else{
					D3D10DeviceWrapper::GetSingleton()->EnableWireframe(true);
					m_WireFrame=true;
				}
				break;
			case 'X':
				++m_CullMode%=3;
				D3D10DeviceWrapper::GetSingleton()->SetCullMode(m_CullMode);
				break;
		}
		m_pGame->KeyPressed(wParam);
		return 0	;
	
	case WM_MOUSEMOVE:
		currentpos = MAKEPOINTS(lParam);
		INPUT.SetMousePos(currentpos);
		return 0;

	case WM_LBUTTONDOWN:
		INPUT.SetLMousePressed(true);
		return 0;

	case WM_LBUTTONUP:
		INPUT.SetLMousePressed(false);
		return 0;

	case WM_RBUTTONDOWN:
		INPUT.SetRMousePressed(true);
		return 0;

	case WM_RBUTTONUP:
		INPUT.SetRMousePressed(false);
		return 0;

	case WM_MOUSEWHEEL:
		zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		INPUT.SetWheelDelta(zDelta);
		return 0;

	default:
		break;

	}
	return DefWindowProc(hWindow, msg, wParam, lParam);
}
void DXWindow::CalcFPS()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.

	static int	frameCnt = 0;
	static float	t_base = 0.0f;

	frameCnt++;

	// Compute averages over 0.3 second period.
	if( (m_Timer.GetGameTime() - t_base) >= 0.3f )
	{
		float fps = (float)frameCnt/.3f; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		tstringstream outs;   
		outs.precision(6);
		outs << _T("FPS: ") << (int)fps << _T('\n') 
			 << _T("Milliseconds Per Frame: ") << mspf ;
		m_sFrameStats = outs.str();
		// Reset for next average.
		frameCnt = 0;
		t_base  += 0.3f;
	}
}
