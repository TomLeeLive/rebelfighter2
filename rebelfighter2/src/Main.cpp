#include "_StdAfx.h"


int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;


CGamePlay* g_pGameplay;
CGameMulti* g_pGameMultiplay;

CMain::CMain(void)
{
	g_pGameplay = &m_gameplay;
	g_pGameMultiplay = &m_gamemulti;

	m_pInput = NULL;
	m_nGamePhase = ST_START;
	vcPos = D3DXVECTOR3(0, 0, 0);			//�����ġ
	SetRect(&rc, 0, 0, 800, 600);
	m_pTx = NULL;
	//for(int i=0; i<2; ++i)
	//{
	//	m_pMp[i]=NULL;
	//}
}

CMain::~CMain()
{
}

INT CMain::Init()
{
	
	////////////////////////////////////////////////////////////////////////////////
	memset(m_KeyOld, 0, sizeof(m_KeyOld));
	memset(m_KeyCur, 0, sizeof(m_KeyCur));

	/////////////////////////////////////////////////////////////////////////////////

	//We need Initialize COM(must be defined _WIN32_WINNT	over 0x400)
	if(FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
		return -1;

	HRESULT hr;
	m_text.Create(m_pd3dDevice);
	m_pInput = new CGameInput;

	m_pInput->Create(m_hWnd);

	hr=this->TextureLoad();



	//m_pSound.Play(10, true);
	//m_dTimeBegin	=timeGetTime();

////////////////////////////////////////////////////////////////////////////////
	memset(m_KeyOld, 0, sizeof(m_KeyOld));
	memset(m_KeyCur, 0, sizeof(m_KeyCur));

/////////////////////////////////////////////////////////////////////////////////
	
	m_gamestart.Init();
	m_gameend.Init();
	m_gameplay.Init();
	m_gamehowto.Init();
	m_gamemulti.Init();
	//m_shtgame.SetDevice(&m_pd3dDevice);
	
	
	hr = this->SoundLoad();

	
	GMAIN->m_pSound.Play(SND_MENUBG, true);
	//m_pMp[0] = new CMiMp3;
	//m_pMp[0]->Create("Sound/playbg.mp3");

	//m_pMp[1] = new CMiMp3;
	//m_pMp[1]->Create("Sound/menubg.mp3");

	
	m_nGamePhase = ST_START;

	return hr;
}



void CMain::Destroy()
{
	SAFE_DELETE(m_pInput);
	//SAFE_DELETE(m_pSoundManager);
	SAFE_DELETE(m_pTx);

	m_gamestart.Destroy();
	m_gameend.Destroy();
	m_gameplay.Destroy();
	m_gamehowto.Destroy();
	m_gamemulti.Destroy();


	m_pSound.Release();


	// Release COM
	CoUninitialize();
}


INT CMain::GameFrameMove()
{
	m_pSound.Frame();

	

	// Update Keyboard Data
	memcpy(m_KeyOld, m_KeyCur, sizeof(m_KeyOld));
	memset(m_KeyCur, 0, sizeof(m_KeyCur));

	if(HIBYTE(GetAsyncKeyState(VK_RETURN)))
		m_KeyCur[VK_RETURN] = 1;


	switch(m_nGamePhase)
	{
	case ST_START:
		m_gamestart.Frame();
		break;

	case ST_SINGLEGAME:
		m_gameplay.Frame();
		break;

	case ST_MULTI:
		m_gamemulti.Frame();
		break;

	case ST_END:
		m_gameend.Frame();
		break;
	case ST_HOWTO:
		m_gamehowto.Frame();
		break;
	}

	return 0;
}

INT CMain::GameRender()
{
	if( NULL == m_pd3dDevice )
		return -1;

	m_pd3dDevice->Clear( 0
		, NULL
		, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER
		, D3DCOLOR_XRGB(94, 63, 107)
		, 1.0f
		, 0 );

	if( FAILED( m_pd3dDevice->BeginScene() ) )
		return -1;

	//m_pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND);


	switch(m_nGamePhase)
	{
	case ST_START:
		m_gamestart.Render(m_pd3dDevice, m_pd3dSprite);
		break;

	case ST_SINGLEGAME:
		m_gameplay.Render(m_pd3dDevice, m_pd3dSprite);
		break;

	case ST_MULTI:
		m_gamemulti.Render(m_pd3dDevice, m_pd3dSprite);
		break;

	case ST_END:
		m_gameend.Render(m_pd3dDevice, m_pd3dSprite);
		break;

	case ST_HOWTO:
		m_gamehowto.Render(m_pd3dDevice,m_pd3dSprite);
		break;
	}
	m_pd3dDevice->EndScene();

	return m_pd3dDevice->Present( 0, 0, 0, 0);

}



LRESULT CMain::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if(m_pInput)
		m_pInput->MsgProc(hWnd, uMsg, wParam, lParam);

	switch(uMsg)
	{
	case WM_DSHOW_EVENT:
		{
			CMiMp3* pMp3 = (CMiMp3*)lParam;
			pMp3->ProcessEvent();
			break;
		}

	//case WM_PAINT:
	//	{
	//		PAINTSTRUCT ps;
	//		HDC hDc = BeginPaint(hWnd, &ps);
	//		EndPaint(hWnd,&ps);
	//		break;
	//	}
	//case WM_MOUSEWHEEL:
	//	{
	//		INT c= HIWORD(wParam);
	//		c = short( c );
	//		m_fWheel=c;
	//		break;
	//	}

	}
	return CD3DApplication::MsgProc(hWnd, uMsg, wParam, lParam);
}






int CMain::GameTextureLoad(LPDIRECT3DTEXTURE9& pTex	//�����ʹ� ����(Alias), ����ü�� �ּҷ�
	, D3DXIMAGE_INFO* pImg				//�����ʹ� ����(Alias), ����ü�� �ּҷ�
	, LPDIRECT3DDEVICE9 pDevice
	, TCHAR* sfile
	, DWORD dColor)
{
	//Texture ��ü ����
	HRESULT hr = 0;
	hr = D3DXCreateTextureFromFileEx(
		pDevice
		, sfile //+
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, 1						//MipLevels --> �߿� //3d�ΰ��� ����
		, 0
		, D3DFMT_UNKNOWN
		, D3DPOOL_MANAGED		//<--����Ʈ�� �º���׷��� ī���� ��� �ȵǴ� ��찡 �ִ�.//MANAGED�� �ϸ� �׷��� ī�忡 �������!
		, D3DX_FILTER_NONE		//3d�ΰ��� ����
		, D3DX_FILTER_NONE		//3d�ΰ��� ����
		, dColor//+				//0x00ffffff:�����������  0x0:���İ����� ��� ���� �ø��ڴ�.
		, pImg //+
		, NULL
		, &pTex //+
		);
	return hr;
}
INT	CMain::TextureLoad()
{
	HRESULT hr = 0;
	if (FAILED(GameTextureLoad(m_pGameTex[0].m_pTex
		, &m_pGameTex[0].m_pImg
		, m_pd3dDevice
		, L"Texture/bg2.png", 0x0)))
		hr = -1;

	/*
	if(FAILED(GameTextureLoad(m_pGameTex[1].m_pTex
	,&m_pGameTex[1].m_pImg
	,m_pd3dDevice
	,"Texture2/xwing.png",0xFFFF00FF)))
	hr= -1;
	*/
	if (FAILED(GameTextureLoad(m_pGameTex[1].m_pTex
		, &m_pGameTex[1].m_pImg
		, m_pd3dDevice
		, L"Texture2/playerShip1_red.png", 0xFFFF00FF)))
		hr = -1;


	if (FAILED(GameTextureLoad(m_pGameTex[2].m_pTex
		, &m_pGameTex[2].m_pImg
		, m_pd3dDevice
		, L"Texture/Laser1.png", 0xFFFFFFFF)))
		hr = -1;
	/*
	if (FAILED(GameTextureLoad(m_pGameTex[2].m_pTex
	, &m_pGameTex[2].m_pImg
	, m_pd3dDevice
	, "Texture2/Lasers/laserRed13.png", 0xFFFFFFFF)))
	hr = -1;
	*/
	
	if(FAILED(GameTextureLoad(m_pGameTex[3].m_pTex
	,&m_pGameTex[3].m_pImg
	,m_pd3dDevice
	,L"Texture/Tie.png",0xFFFF00FF)))
	hr= -1;
	/*
	if (FAILED(GameTextureLoad(m_pGameTex[3].m_pTex
		, &m_pGameTex[3].m_pImg
		, m_pd3dDevice
		, "texture2/Enemies/enemyBlack1.png", 0xFFFFFFFF)))
		hr = -1;
	*/
	if (FAILED(GameTextureLoad(m_pGameTex[4].m_pTex
		, &m_pGameTex[4].m_pImg
		, m_pd3dDevice
		, L"Texture/Laser2.png", 0xFFFFFFFF)))
		hr = -1;
	/*
	if (FAILED(GameTextureLoad(m_pGameTex[4].m_pTex
	, &m_pGameTex[4].m_pImg
	, m_pd3dDevice
	, "Texture2/Lasers/laserGreen03.png", 0xFFFFFFFF)))
	hr = -1;
	*/
	if (FAILED(GameTextureLoad(m_pGameTex[5].m_pTex
		, &m_pGameTex[5].m_pImg
		, m_pd3dDevice
		, L"Texture/title2.png", 0x0)))
		hr = -1;

	if (FAILED(GameTextureLoad(m_pGameTex[6].m_pTex
		, &m_pGameTex[6].m_pImg
		, m_pd3dDevice
		, L"Texture/end.png", 0x0)))
		hr = -1;

	if (FAILED(GameTextureLoad(m_pGameTex[7].m_pTex
		, &m_pGameTex[7].m_pImg
		, m_pd3dDevice
		, L"Texture/statusbar2.png", 0x0)))
		hr = -1;
	
	if(FAILED(GameTextureLoad(m_pGameTex[8].m_pTex
	,&m_pGameTex[8].m_pImg
	,m_pd3dDevice
	,L"Texture/Tie2.png",0xFFFF00FF)))
	hr= -1;
	
	/*
	if (FAILED(GameTextureLoad(m_pGameTex[8].m_pTex
		, &m_pGameTex[8].m_pImg
		, m_pd3dDevice
		, "texture2/Enemies/enemyBlack2.png", 0xFFFFFFFF)))
		hr = -1;
	*/

	if (FAILED(GameTextureLoad(m_pGameTex[9].m_pTex
		, &m_pGameTex[9].m_pImg
		, m_pd3dDevice
		, L"Texture/Bomber.png", 0xFFFF00FF)))
		hr = -1;

	if (FAILED(GameTextureLoad(m_pGameTex[10].m_pTex
		, &m_pGameTex[10].m_pImg
		, m_pd3dDevice
		, L"Texture/Bomber2.png", 0xFFFF00FF)))
		hr = -1;

	if (FAILED(GameTextureLoad(m_pGameTex[11].m_pTex
		, &m_pGameTex[11].m_pImg
		, m_pd3dDevice
		, L"Texture/BomberAdv.png", 0xFFFF00FF)))
		hr = -1;

	if (FAILED(GameTextureLoad(m_pGameTex[12].m_pTex
		, &m_pGameTex[12].m_pImg
		, m_pd3dDevice
		, L"Texture/BomberAdv2.png", 0xFFFF00FF)))
		hr = -1;

	if (FAILED(GameTextureLoad(m_pGameTex[13].m_pTex
		, &m_pGameTex[13].m_pImg
		, m_pd3dDevice
		, L"Texture/Intercepter.png", 0xFFFF00FF)))
		hr = -1;

	if (FAILED(GameTextureLoad(m_pGameTex[14].m_pTex
		, &m_pGameTex[14].m_pImg
		, m_pd3dDevice
		, L"Texture/Advanced.png", 0xFFFF00FF)))
		hr = -1;

	if (FAILED(GameTextureLoad(m_pGameTex[15].m_pTex
		, &m_pGameTex[15].m_pImg
		, m_pd3dDevice
		, L"Texture/Boss.png", 0xFFFF00FF)))
		hr = -1;

	if (FAILED(GameTextureLoad(m_pGameTex[16].m_pTex
		, &m_pGameTex[16].m_pImg
		, m_pd3dDevice
		, L"Texture/Ion1.png", 0xFFFFFFFF)))
		hr = -1;

	if (FAILED(GameTextureLoad(m_pGameTex[17].m_pTex
		, &m_pGameTex[17].m_pImg
		, m_pd3dDevice
		, L"Texture/Missile.png", 0xFFFFFFFF)))
		hr = -1;

	if (FAILED(GameTextureLoad(m_pGameTex[18].m_pTex
		, &m_pGameTex[18].m_pImg
		, m_pd3dDevice
		, L"Texture/life.png", 0xFFFFFFFF)))
		hr = -1;

	if (FAILED(GameTextureLoad(m_pGameTex[19].m_pTex
		, &m_pGameTex[19].m_pImg
		, m_pd3dDevice
		, L"Texture/laser.png", 0xFFFFFFFF)))
		hr = -1;

	if (FAILED(GameTextureLoad(m_pGameTex[20].m_pTex
		, &m_pGameTex[20].m_pImg
		, m_pd3dDevice
		, L"Texture/speed.png", 0xFFFFFFFF)))
		hr = -1;

	if (FAILED(GameTextureLoad(m_pGameTex[21].m_pTex
		, &m_pGameTex[21].m_pImg
		, m_pd3dDevice
		, L"Texture/power.png", 0xFFFFFFFF)))
		hr = -1;

	if (FAILED(GameTextureLoad(m_pGameTex[22].m_pTex
		, &m_pGameTex[22].m_pImg
		, m_pd3dDevice
		, L"Texture/hp.png", 0xFFFFFFFF)))
		hr = -1;

	if (FAILED(GameTextureLoad(m_pGameTex[23].m_pTex
		, &m_pGameTex[23].m_pImg
		, m_pd3dDevice
		, L"Texture/bg4.png", 0x0)))
		hr = -1;

	if (FAILED(GameTextureLoad(m_pGameTex[24].m_pTex
		, &m_pGameTex[24].m_pImg
		, m_pd3dDevice
		, L"Texture/bg4.png", 0x0)))
		hr = -1;

	if (FAILED(GameTextureLoad(m_pGameTex[25].m_pTex
		, &m_pGameTex[25].m_pImg
		, m_pd3dDevice
		, L"Texture/howto.png", 0x0)))
		hr = -1;

	if (FAILED(GameTextureLoad(m_pGameTex[26].m_pTex
		, &m_pGameTex[26].m_pImg
		, m_pd3dDevice
		, L"texture2/Lasers/laserRed09.png", 0xFFFFFFFF)))
		hr = -1;

	m_pTx = new CGameTexture;
	if (FAILED(m_pTx->Create(m_pd3dDevice, L"Texture/boom2.png", 0x00FFFFFF)))
	{
		delete m_pTx;
		hr = -1;
	}
	return hr;
}

INT CMain::SoundLoad()
{
	HRESULT hr = 0;

	m_pSound.Init();

	int iIndex = 0;

	iIndex = m_pSound.Load("Sound/alarm.wav");		//�˶� ����
	iIndex = m_pSound.Load("Sound/beam1.wav");		//������ ������� �ߵ� ����
	iIndex = m_pSound.Load("Sound/bonus.wav");		//���ʽ� ����
	iIndex = m_pSound.Load("Sound/cannon1.wav");	//���ΰ� �Ѿ� ����2
	iIndex = m_pSound.Load("Sound/electricityloop2.wav");	//���Ⱑ ������ �Ÿ��� �Ҹ�
	iIndex = m_pSound.Load("Sound/explosion.wav");	//���� ����
	iIndex = m_pSound.Load("Sound/fireby.wav");		//�������� �������� �Ҹ�
	iIndex = m_pSound.Load("Sound/gungexpl.wav");	//�Ѿ� ���� �´� �Ҹ�
	iIndex = m_pSound.Load("Sound/ioncannon.wav");	//�̿� ĳ��
	iIndex = m_pSound.Load("Sound/javjab1.wav");	//���ΰ� ���忡 �������� ������
	iIndex = m_pSound.Load("Sound/menubg.mp3", true);		//���۸޴� bgm
	iIndex = m_pSound.Load("Sound/missile.wav");	//�̻��� �߻�
	iIndex = m_pSound.Load("Sound/misslfly.wav");	//�̻����� ��������
	iIndex = m_pSound.Load("Sound/playbg.mp3",true);		//���� bgm
	iIndex = m_pSound.Load("Sound/score.wav");		//���ھ�, �������� �ڸ� ����
	iIndex = m_pSound.Load("Sound/shokwav2.wav");	//�������߻� ����, �����ߵ� �� �̿��Ұ�
	iIndex = m_pSound.Load("Sound/tie.wav");		//���� ��������
	iIndex = m_pSound.Load("Sound/tie_b.wav");
	iIndex = m_pSound.Load("Sound/tielsr1.wav");	//Ÿ��������1 �Ѿ� ����1
	iIndex = m_pSound.Load("Sound/tielsr2.wav");	//Ÿ��������2 �Ѿ� ����2
	iIndex = m_pSound.Load("Sound/xwlsr1.wav");		//���ΰ� �Ѿ� ����1
	iIndex = m_pSound.Load("Sound/r2_d2.wav");		//���ʽ�: ��������! R2D2 �Ҹ�
	iIndex = m_pSound.Load("Sound/the_force.wav");	//���� ���� bgm
	iIndex = m_pSound.Load("Sound/xwenglp.wav");	//����� ����
	iIndex = m_pSound.Load("Sound/goodinu.wav");	//���ǵ� ��!
	iIndex = m_pSound.Load("Sound/useforce.wav");	//�Ŀ� ��!
	iIndex = m_pSound.Load("Sound/withyou.wav");	//H.P ��!
	iIndex = m_pSound.Load("Sound/wontfail.wav");	//������ ��!
	iIndex = m_pSound.Load("Sound/dark_side.wav");	//���� ���1
	iIndex = m_pSound.Load("Sound/dark_side2_y.wav");//���� ���2
	iIndex = m_pSound.Load("Sound/dont_underestimate.wav");//���� ���3
	iIndex = m_pSound.Load("Sound/dontmake.wav");	//���� ���4
	iIndex = m_pSound.Load("Sound/expecting_you.wav");//���� ���5
	iIndex = m_pSound.Load("Sound/mustobey.wav");	//���� ���6
	iIndex = m_pSound.Load("Sound/your_father.wav");//���� ���7

	return hr;
}
