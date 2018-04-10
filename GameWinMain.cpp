#include "stdafx.h"
#include "GameWinMain.h"
#include "DXWindow.h"

#include "GP2_Game_App.h"


//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------

#define DXWINDOW (DXWindow::GetSingleton())

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory leak check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif


	//create game singleton object
	AbstractDXGame *pGame=GP2_Game_App::GetSingleton();

	if (pGame == NULL) return FALSE;

	// create DXWindow singleton object
	DXWindow *pDXWindow = DXWindow::GetSingleton();
	if (pDXWindow == NULL) return FALSE;

	//copy game pointer into DXWindow singleton
	DXWINDOW->SetGame(pGame);

	// open window, directx singleton, dx init, game init,
	// run the msgloop, in msgloop call game->paint 
	DXWINDOW->Run(hInstance, showCmd);

	//destroy objects
	delete pGame;
	delete DXWINDOW;

	return TRUE;

}