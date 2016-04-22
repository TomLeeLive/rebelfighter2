#include "_StdAfx.h"

static CD3DApplication* g_pD3DApp;

CD3DApplication::CD3DApplication()
{
	g_pD3DApp=this;

	strcpy(m_sCls, "Rebel Fighter V17.8 Worked by Lee Jae Joon [serialkk@naver.com]");
	m_hInst		= NULL;
	m_hWnd		= NULL;
	m_dWinStyle	= WS_OVERLAPPEDWINDOW| WS_VISIBLE;
	m_dScnX		= 800;			// Screen Width
	m_dScnY		= 600;			// Screen Height
	m_bShowCusor= TRUE;			// Show Cusor

	m_pD3D     = NULL;  // D3D
	m_pd3dDevice = NULL; // Direct3D Device
	m_pd3dSprite  = NULL;// D3DX Sprite
}

CD3DApplication::~CD3DApplication()
{
}


INT CD3DApplication::Create( HINSTANCE hInst)
{
	m_hInst	= hInst;

	WNDCLASS wc =								// Register the window class
	{
		CS_CLASSDC
		, WndProc
		, 0L
		, 0L
		, m_hInst
		, NULL
		, LoadCursor(NULL,IDC_ARROW)
		, (HBRUSH)GetStockObject(LTGRAY_BRUSH)
		, NULL
		, m_sCls
	};

	RegisterClass( &wc );

	RECT rc;									//Create the application's window

	SetRect( &rc, 0, 0, m_dScnX, m_dScnY);
	AdjustWindowRect( &rc, m_dWinStyle, FALSE );

	int iScnSysW = ::GetSystemMetrics(SM_CXSCREEN);
	int iScnSysH = ::GetSystemMetrics(SM_CYSCREEN);

	m_hWnd = CreateWindow( m_sCls
		, m_sCls
		, m_dWinStyle
		, (iScnSysW - (rc.right - rc.left))/2
		, (iScnSysH - (rc.bottom - rc.top))/2
		, (rc.right - rc.left)
		, (rc.bottom - rc.top)
		, GetDesktopWindow()
		, NULL
		, m_hInst
		, NULL );

	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );
	::ShowCursor(m_bShowCusor);


	////////////////////////////////////////////////////////////////////////////
	//

	// 여기에 DX를 초기화 할 것임

	//
	////////////////////////////////////////////////////////////////////////////
	//
	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if(NULL==m_pD3D)
		return -1;
	HRESULT hr;

	D3DPRESENT_PARAMETERS d3dpp={0};  //구조체는 반드시 초기화를 해야한다.
	ZeroMemory( &d3dpp, sizeof(d3dpp) );

	d3dpp.Windowed			= TRUE;  //윈도우 모드에서 실행!
	d3dpp.SwapEffect		= D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil	= TRUE;
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; //모니터 해상도!

	if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL
		, m_hWnd
		, D3DCREATE_MIXED_VERTEXPROCESSING
		, &d3dpp
		, &m_pd3dDevice ) ) ) //HAL은 hardware 가속을 하겠다는 이야기
	{
		if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL
			, m_hWnd
			, D3DCREATE_SOFTWARE_VERTEXPROCESSING
			, &d3dpp
			, &m_pd3dDevice ) ) )
		{
			m_pD3D->Release();
			return -1;
		}
	}
	//3.스프라이트 객체 생성
	hr = D3DXCreateSprite(m_pd3dDevice,&m_pd3dSprite);
	if(FAILED(hr))
	{
		m_pd3dDevice->Release();
		m_pD3D->Release();
		return -1;
	}
	//4.게임데이터 생성 초기화
	if(FAILED(Init()))
	{
		m_pd3dSprite->Release();
		m_pd3dDevice->Release();
		m_pD3D->Release();
		return -1;
	}

	return 0;




}


void CD3DApplication::Cleanup()
{
	////////////////////////////////////////////////////////////////////////////
	//

	// 게임 데이터 + DX 해제

	//
	////////////////////////////////////////////////////////////////////////////
	Destroy();

	//com객체의 해제는 release()
	if(m_pd3dSprite){m_pd3dSprite->Release();m_pd3dSprite=NULL;}
	if(m_pd3dDevice){m_pd3dDevice->Release(); m_pd3dDevice =NULL;}
	if(m_pd3dDevice){m_pd3dDevice->Release();m_pd3dDevice=NULL;}
}

LRESULT WINAPI CD3DApplication::WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return g_pD3DApp->MsgProc(hWnd, msg, wParam, lParam);
}



LRESULT CD3DApplication::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_KEYDOWN:
		{
			switch(wParam)
			{
			case VK_ESCAPE:
				{
					SendMessage(hWnd, WM_DESTROY, 0,0);
					break;
				}
			}

			return 0;

		}

	case WM_DESTROY:
		{
			Cleanup();
			PostQuitMessage( 0 );
			return 0;
		}
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}



INT CD3DApplication::Run()
{
	// Enter the message loop
	MSG msg;
	memset( &msg, 0, sizeof(msg) );

	while( msg.message!=WM_QUIT )
	{
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			////////////////////////////////////////////////////////////////////
			//

			// 게임 루프( 함수 호출하면 보기 좋다!)

			//
			////////////////////////////////////////////////////////////////////

			if(FAILED(GameFrameMove()))
			{
				PostQuitMessage(0);
				continue;
			}
			if(FAILED(GameRender()))
			{
				PostQuitMessage(0);
				
				continue;
			}
			UpdateFrame();
		}
	}
	
	

	UnregisterClass( m_sCls, m_hInst);

	return 0;
}




void CD3DApplication::UpdateFrame()
{
	static int		iCnt =0;
	static DWORD		dB = timeGetTime();
	DWORD			dE = timeGetTime();

	++iCnt;

	if(iCnt>30)		// 30Frame을 기준
	{
		double dDelta = DOUBLE(dE-dB);

		m_deltatime= dDelta/iCnt;
		m_fFps = iCnt*1000.f/dDelta;

		m_movingdist = float(0.3*m_deltatime);


		iCnt = 0;
		dB	= dE;
	}
}