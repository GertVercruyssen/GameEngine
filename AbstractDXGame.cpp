#include "stdafx.h"
#include "AbstractDXGame.h"
#include "DXWindow.h"
#include "D3D10DeviceWrapper.h"

AbstractDXGame::AbstractDXGame(void):m_hWindow(0)
{
}

AbstractDXGame::~AbstractDXGame(void)
{
}
void AbstractDXGame::OnResize(void)
{
	if(GETD3D10DEVICE != 0)
	{
		D3D10DEVICEWRAPPER->OnResize();
	}
}
void AbstractDXGame::SetWindowParams()
{
	DXWINDOW->SetWidth(800);
	DXWINDOW->SetHeight(600);
	DXWINDOW->SetTitle(_T("DirectX 10"));
}
void AbstractDXGame::EnableFullScreenMode(bool enable)
{
	//D3D10DEVICEWRAPPER->EnableFullScreenMode(enable);
}
