#include "_StdAfx.h"

CGameInput::CGameInput()
{
	m_hWnd = NULL;

	memset(m_KeyCur, 0, 256);
	memset(m_KeyOld, 0, 256);
	memset(m_MouseCur, 0, 16);
	memset(m_MouseOld, 0, 16);

	m_MousePos=D3DXVECTOR3(0,0,0);
	m_fWheel =0;
	

	BYTE Key[256]={0};
	SetKeyboardState(Key);
	//memset(m_Keyboard,0,sizeof m_Keyboard);
	//memset(m_Mouse,0,sizeof m_Mouse);
}

CGameInput::~CGameInput()
{
}

INT CGameInput::Create(HWND hWnd)
{
	m_hWnd = hWnd; 
	return 0;
}
INT CGameInput::FrameMove()
{
	memcpy(m_KeyOld, m_KeyCur, 256); memset(m_KeyCur,0,256);
	memcpy(m_MouseOld, m_MouseCur, 16); memset(m_MouseCur,0,16);


	////////////////////////////////////////////////////////////////////////////////
	//키 입력
	//if(short Key =GetAsyncKeyState(VK_RETURN)&0x8000)
	//{
	//	SetWindowText(m_hWnd,"엔터키 입력됨");
	//}
	//else
	//{
	//	SetWindowText(m_hWnd,"엔터키 입력안됨");
	//}
	////////////////////////////////////////////////////////////////////////////////

	BYTE key[256];
	GetKeyboardState(key);
	for(int i=0; i<256; ++i)
	{
		m_KeyCur[i] = key[i] &0x80;
	}

	POINT	MousePos={0};
	m_MouseCur[0] = m_KeyCur[VK_LBUTTON];
	m_MouseCur[1] = m_KeyCur[VK_RBUTTON];
	m_MouseCur[2] = m_KeyCur[VK_MBUTTON];

	memcpy(&m_MousePos,&MousePos,sizeof(POINT));


	//마우스 위치
	GetCursorPos(&MousePos);
	ScreenToClient(m_hWnd, &MousePos);

	m_MousePos.x=FLOAT(MousePos.x);
	m_MousePos.y=FLOAT(MousePos.y);
	m_MousePos.z +=m_fWheel;

	m_fWheel=0;

	//char sMsg[128]={0};
	//sprintf(sMsg, "%.f %.f %.f", m_MousePos.x, m_MousePos.y, m_MousePos.z);

	//SetWindowText(m_hWnd,sMsg);

	//////////////////////////////////////////////////////////////////////////////
	return 0;
}

LRESULT CGameInput::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hDc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd,&ps);
			break;
		}
	case WM_MOUSEWHEEL:
		{
			INT c= HIWORD(wParam);
			c = short( c );
			m_fWheel=float(c);
			break;
		}
	}
	return FALSE;
	/*	return CD3DApplication::MsgProc(hWnd, uMsg, wParam, lParam);*/
}

INT CGameInput::KeyDown(INT Key)
{
	if(m_KeyCur[Key] && 0 ==m_KeyOld[Key])
		return 1;
	return 0;
}

INT CGameInput::KeyUp(INT Key)
{
	if(m_KeyOld[Key] && 0 ==m_KeyCur[Key])
		return 1;
	return 0;
}

INT CGameInput::KeyPress(INT Key)
{
	if(m_KeyOld[Key] && m_KeyCur[Key])
		return 1;
	return 0;
}

INT CGameInput::KeyState(INT Key)
{
	return m_KeyCur[Key];
}



INT CGameInput::ButtonDown(INT Key)
{
	if(m_MouseCur[Key] && 0 ==m_MouseOld[Key])
		return 1;
	return 0;
}

INT CGameInput::ButtonUp(INT Key)
{
	if(m_MouseOld[Key] && 0 ==m_MouseCur[Key])
		return 1;
	return 0;
}

INT CGameInput::ButtonPress(INT Key)
{
	if(m_MouseOld[Key] && m_MouseCur[Key])
		return 1;
	return 0;
}

INT CGameInput::ButtonState(INT Key)
{
	return m_MouseCur[Key];
}
//BYTE			m_KeyCur[256]; //BYTE ==> unsigned char 
//BYTE			m_KeyOld[256];

HRESULT CGameInput::MenuItemClickCheck(void)
{
	RECT              rcWindowClient;
	GetClientRect( GMAIN->m_hWnd, &rcWindowClient );

	//마우스 위치
	POINT	MousePos={0};
	GetCursorPos(&MousePos);
	ScreenToClient(m_hWnd, &MousePos);

	if (KeyDown(VK_LBUTTON))
	{
		if(MousePos.x > rcWindowClient.right*565/800
			&& MousePos.x < rcWindowClient.right*667/800
			&& MousePos.y > rcWindowClient.bottom*359/600
			&& MousePos.y < rcWindowClient.bottom*391/600)
		{
			GMAIN->m_nGamePhase=1;
			OutputDebugString("Start\n");
		}
		else if(MousePos.x > rcWindowClient.right*565/800
			&& MousePos.x < rcWindowClient.right*758/800
			&& MousePos.y > rcWindowClient.bottom*408/600
			&& MousePos.y < rcWindowClient.bottom*442/600)
		{
			GMAIN->m_nGamePhase=4;
			OutputDebugString("Howto\n");
		}
		else if(MousePos.x > rcWindowClient.right*565/800
			&& MousePos.x < rcWindowClient.right*623/800
			&& MousePos.y > rcWindowClient.bottom*508/600
			&& MousePos.y < rcWindowClient.bottom*542/600)
		{
			if(MessageBox(GHWND, "Quit?", "Quit", MB_YESNO)==IDYES)
			{
				PostQuitMessage(0);
				return 0;
			}		
			OutputDebugString("Quit\n");
		}
		else{}
	}

	return S_OK;
}