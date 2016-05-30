
#include "_StdAfx.h"

CMain* g_pApp;

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	CMain mainApp;

	g_pApp = &mainApp;

	if(FAILED(mainApp.Create(hInst)))
		return -1;

	return mainApp.Run();
}

