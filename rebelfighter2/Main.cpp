#include "_StdAfx.h"
#include "CDrawText.h"

int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;

D3DXVECTOR3 boom;			//������ġ
CDrawText g_text;

CMain::CMain(void)
{
	m_pInput=NULL;
	ioncnt=0;
	lasercnt=0;
	missilecnt=0;
	curioncnt=0;
	curlasercnt=0;
	curmissilecnt=0;

	SetRect(&rc,0,0,800,600);

	stage = 1;			//ó�� ���۽� �������� 1
	curstage;
	score = 0;			//����

	vcPos= D3DXVECTOR3(0,0,0);			//�����ġ
	vcPos1= D3DXVECTOR3(-75,0,0);		//�����ġ
	vcPos2= D3DXVECTOR3(-75,-600,0);	//�����ġ

	m_pTx = NULL;
	SetRect(&m_ImgRc2,0,0,0, 70);		// RECT �ִ� �̹���

	m_nGamePhase=0;

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
	//We need Initialize COM(must be defined _WIN32_WINNT	over 0x400)
	if(FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
		return -1;

	HRESULT hr;
	g_text.Create(m_pd3dDevice);
	m_pInput = new CGameInput;

	m_pInput->Create(m_hWnd);

	hr=this->TextureLoad();

	hr=this->SoundLoad();

	//m_dTimeBegin	=timeGetTime();

////////////////////////////////////////////////////////////////////////////////
	memset(m_KeyOld, 0, sizeof(m_KeyOld));
	memset(m_KeyCur, 0, sizeof(m_KeyCur));

/////////////////////////////////////////////////////////////////////////////////
	
	
	//m_pMp[0] = new CMiMp3;
	//m_pMp[0]->Create("Sound/playbg.mp3");

	//m_pMp[1] = new CMiMp3;
	//m_pMp[1]->Create("Sound/menubg.mp3");

	

	return hr;
}



void CMain::Destroy()
{
	SAFE_DELETE(m_pInput);
	//SAFE_DELETE(m_pSoundManager);
	SAFE_DELETE(m_pTx);

	//���ΰ� �Ѿ� �Ҹ�κ�
	std::vector<CLaserData*>::iterator _F=pvLaser0.begin();
	std::vector<CLaserData*>::iterator _L=pvLaser0.end();
	for(; _F != _L; ++_F) { if (*_F != 0) delete (*_F); }
	pvLaser0.clear();

	//�� �Ѿ� �Ҹ�κ�
	std::vector<CLaserData*>::iterator _F1=pvLaser1.begin();
	std::vector<CLaserData*>::iterator _L1=pvLaser1.end();
	for(; _F1 != _L1; ++_F1) { if (*_F1 != 0) delete (*_F1); }
	pvLaser1.clear();

	//�� �ɸ��� �Ҹ�κ�
	std::vector<CCharacterData*>::iterator _FT=pvTie0.begin();
	std::vector<CCharacterData*>::iterator _LT=pvTie0.end();
	for(; _FT != _LT; ++_FT) { if (*_FT != 0) delete (*_FT); }
	pvTie0.clear();


	m_pSound.Release();

	// Release COM
	CoUninitialize();
}


INT CMain::GameFrameMove()
{
	int j=0;

	if(pvTie0.size() ==0)
	{
		srand((int)time(NULL));
	}

	// Update Keyboard Data
	memcpy(m_KeyOld, m_KeyCur, sizeof(m_KeyOld));
	memset(m_KeyCur, 0, sizeof(m_KeyCur));

	if(HIBYTE(GetAsyncKeyState(VK_RETURN)))
		m_KeyCur[VK_RETURN] = 1;

	switch(m_nGamePhase)
	{
	case 0:	
////////////////////////////////////////////////////////////////////////////////
		score=0;
		curstage=0;
		stage=1;
		m_xwing.xval=325;
		m_xwing.yval=500;
		m_xwing.hp=100;
		m_xwing.dead=FALSE;
		m_xwing.laserhit=FALSE;
		m_xwing.laserability=3;
		m_xwing.m_killcount=0;
		m_xwing.m_life=2;
		m_xwing.m_laserable=3;
		m_xwing.m_speed=1;
		m_xwing.m_power=1;

		pvLaser0.clear();
		pvLaser1.clear();
		pvTie0.clear();

		curmissilecnt=0;
		curlasercnt=0;
		curioncnt=0;
		
		m_gamebegin=timeGetTime();
////////////////////////////////////////////////////////////////////////////////
		StartFrameMove();
		break;

	case 1:

		if(curstage != stage)
		{
			if(pvTie0.size() ==0)
			{	
				lasercnt=0;
				ioncnt=0;
				missilecnt=0;

				pvLaser0.clear();
				curstage = stage;
				
				enemylevel=(50+stage*7)/50;
				
				if(stage !=0 && stage%10==0 && (50+stage*7)-50*m_xwing.m_power >0)
				{//10�������� ���� �Ŀ������� ����
					pvTie0.push_back(new CCharacterData(13,int(rand()%360),305,float(rand()%50+50-300),FALSE,0,50,int(rand()%2),FALSE));
				}
				if(stage !=0 && stage%5==0)
				{

					//pvTie0.push_back(new CCharacterData("Boss",int(rand()%360),305,float(rand()%50-300),FALSE,5,500,0,FALSE));
					pvTie0.push_back(new CCharacterData(9,int(rand()%360),305,float(rand()%50+50-300),FALSE,5,stage,int(rand()%2),FALSE));//���� ����
					pvTie0.push_back(new CCharacterData(int(rand()%9),int(rand()%360),305,float(rand()%50+50-300),FALSE,2,stage,int(rand()%2),FALSE));
					pvTie0.push_back(new CCharacterData(int(rand()%9),int(rand()%360),305,float(rand()%50+100-300),FALSE,2,stage,int(rand()%2),FALSE));
					pvTie0.push_back(new CCharacterData(int(rand()%9),int(rand()%360),305,float(rand()%50+150-300),FALSE,2,stage,int(rand()%2),FALSE));
					pvTie0.push_back(new CCharacterData(int(rand()%9),int(rand()%360),305,float(rand()%50+200-300),FALSE,2,stage,int(rand()%2),FALSE));
					pvTie0.push_back(new CCharacterData(int(rand()%9),int(rand()%360),305,float(rand()%50+250-300),FALSE,2,stage,int(rand()%2),FALSE));

					if(rand()%6==3&& stage%10!=0)
					{
						switch(rand()%7)
						{
						case 0://10. ������ ������ ����
							if(m_xwing.m_life <3)
								pvTie0.push_back(new CCharacterData(10,int(rand()%360),305,float(rand()%50+50-300),FALSE,0,50,int(rand()%2),FALSE));
							break;

						case 1://11. ������ ������ ����
							if(m_xwing.m_laserable <6)
								pvTie0.push_back(new CCharacterData(11,int(rand()%360),305,float(rand()%50+50-300),FALSE,0,50,int(rand()%2),FALSE));
							break;

						case 2://12. ���ǵ� ������ ����
							pvTie0.push_back(new CCharacterData(12,int(rand()%360),305,float(rand()%50+50-300),FALSE,0,50,int(rand()%2),FALSE));
							break;

						case 3://13. �Ŀ� ������ ����
							if( (50+stage*7)-50*m_xwing.m_power >0)
								pvTie0.push_back(new CCharacterData(13,int(rand()%360),305,float(rand()%50+50-300),FALSE,0,50,int(rand()%2),FALSE));
							break;

						case 4://14. HP ������ ����
							pvTie0.push_back(new CCharacterData(14,int(rand()%360),305,float(rand()%50+50-300),FALSE,0,50,int(rand()%2),FALSE));
							break;

						case 5://12. ���ǵ� ������ ����
							pvTie0.push_back(new CCharacterData(12,int(rand()%360),305,float(rand()%50+50-300),FALSE,0,50,int(rand()%2),FALSE));
							break;

						default://11. ������ ������ ����
							if(m_xwing.m_laserable <6)
								pvTie0.push_back(new CCharacterData(11,int(rand()%360),305,float(rand()%50+50-300),FALSE,0,50,int(rand()%2),FALSE));
							break;
						}
					}
				}
				else
				{
					pvTie0.push_back(new CCharacterData(int(rand()%9),int(rand()%360),305,float(rand()%50-300),FALSE,2,stage,int(rand()%2),FALSE));
					pvTie0.push_back(new CCharacterData(int(rand()%9),int(rand()%360),305,float(rand()%50+50-300),FALSE,2,stage,int(rand()%2),FALSE));
					pvTie0.push_back(new CCharacterData(int(rand()%9),int(rand()%360),305,float(rand()%50+100-300),FALSE,2,stage,int(rand()%2),FALSE));
					pvTie0.push_back(new CCharacterData(int(rand()%9),int(rand()%360),305,float(rand()%50+150-300),FALSE,2,stage,int(rand()%2),FALSE));
					pvTie0.push_back(new CCharacterData(int(rand()%9),int(rand()%360),305,float(rand()%50+200-300),FALSE,2,stage,int(rand()%2),FALSE));
					pvTie0.push_back(new CCharacterData(int(rand()%9),int(rand()%360),305,float(rand()%50+250-300),FALSE,2,stage,int(rand()%2),FALSE));

					if(rand()%6==3)
					{
						switch(rand()%7)
						{
						case 0://10. ������ ������ ����
							if(m_xwing.m_life <3)
								pvTie0.push_back(new CCharacterData(10,int(rand()%360),305,float(rand()%50+50-300),FALSE,0,50,int(rand()%2),FALSE));
							break;

						case 1://11. ������ ������ ����
							if(m_xwing.m_laserable <6)
								pvTie0.push_back(new CCharacterData(11,int(rand()%360),305,float(rand()%50+50-300),FALSE,0,50,int(rand()%2),FALSE));
							break;

						case 2://12. ���ǵ� ������ ����
							pvTie0.push_back(new CCharacterData(12,int(rand()%360),305,float(rand()%50+50-300),FALSE,0,50,int(rand()%2),FALSE));
							break;

						case 3://13. �Ŀ� ������ ����
							if( (50+stage*7)-50*m_xwing.m_power >0)
								pvTie0.push_back(new CCharacterData(13,int(rand()%360),305,float(rand()%50+50-300),FALSE,0,50,int(rand()%2),FALSE));
							break;

						case 4://14. HP ������ ����
							pvTie0.push_back(new CCharacterData(14,int(rand()%360),305,float(rand()%50+50-300),FALSE,0,50,int(rand()%2),FALSE));
							break;

						case 5://12. ���ǵ� ������ ����
							pvTie0.push_back(new CCharacterData(12,int(rand()%360),305,float(rand()%50+50-300),FALSE,0,50,int(rand()%2),FALSE));
							break;

						default://11. ������ ������ ����
							if(m_xwing.m_laserable <6)
								pvTie0.push_back(new CCharacterData(11,int(rand()%360),305,float(rand()%50+50-300),FALSE,0,50,int(rand()%2),FALSE));
							break;
						}
					}
				}
				
				//////////////////////////////////////////////////////////////////
				std::vector<CCharacterData*>::iterator _FC=pvTie0.begin(); //������ ���� ī��Ʈ�� �ݺ���
				std::vector<CCharacterData*>::iterator _LC=pvTie0.end();
				for( ; _FC != _LC ; _FC++)
				{
					if((*_FC)->laserable==TRUE)
					{
						lasercnt=lasercnt+(*_FC)->laserability;
					}
					if((*_FC)->ionable==TRUE)
					{
						ioncnt=ioncnt+(*_FC)->ionability;
					}
					if((*_FC)->missileable==TRUE)
					{
						missilecnt=missilecnt+(*_FC)->missileability;
					}
				}
				/////////////////////////////////////////////////////////////////////////			
			}
		}
		if(m_xwing.dead != TRUE)
		{
			//m_pSndEngine->Play();
			//m_pSndEngine->SetVolume(-1000);
		}
		PlayFrameMove();
		break;

	case 2:
		EndFrameMove();
		break;


	case 4:
		HowtoFrameMove();
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
		, 0x00000000
		, 1.0f
		, 0 );

	if( FAILED( m_pd3dDevice->BeginScene() ) )
		return -1;

	//m_pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND);

	switch(m_nGamePhase)
	{
	case 0:
		StartRender();
		break;

	case 1:
		PlayRender();
		break;

	case 2:
		EndRender();
		break;

	case 4:
		HowtoRender();
		break;
	}

	//m_pd3dSprite->End();
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


//�浹üũ �Լ�1: ���Ѿ��� ���ΰ��� �¾Ҵ��� üũ
INT CMain::ColCheck()
{
	D3DXVECTOR3	TempPos;
	INT bColl=0;

	std::vector<CLaserData*>::iterator _F=pvLaser1.begin();
	std::vector<CLaserData*>::iterator _L=pvLaser1.end();

	for(;_F != _L; ++_F)
	{
		TempPos = D3DXVECTOR3((*_F)->laserxval,(*_F)->laseryval,0);
		//TempPos.x=(*_F)->laserxval;
		//TempPos.y=(*_F)->laseryval;
		//TempPos.z=0;

		RECT rcCol1;
		RECT rcCol2;
		RECT rcColSound;
		
		if((*_F)->name=="Missile")
		{
			SetRect(&rcCol1 , INT(TempPos.x+4)
				, INT(TempPos.y)
				, INT(TempPos.x+4) + 5	//m_pGameTex[4].GetImageWidth()
				, INT(TempPos.y) + 30	//m_pGameTex[4].GetImageHeight()
				);

			SetRect(&rcCol2 , INT(m_xwing.vcPosC.x+22)
				, INT(m_xwing.vcPosC.y)
				, INT(m_xwing.vcPosC.x+22) + 11	//m_pGameTex[1].GetImageWidth()
				, INT(m_xwing.vcPosC.y) + 56	//m_pGameTex[1].GetImageHeight()
				);

			SetRect(&rcColSound , INT(m_xwing.vcPosC.x-112)
				, INT(m_xwing.vcPosC.y)
				, INT(m_xwing.vcPosC.x-112) + 275	//m_pGameTex[1].GetImageWidth()
				, INT(m_xwing.vcPosC.y+56)			//+ m_pGameTex[1].GetImageHeight()
				);
		}
		else
		{
		SetRect(&rcCol1 , INT(TempPos.x)
			, INT(TempPos.y)
			, INT(TempPos.x) + m_pGameTex[4].GetImageWidth()
			, INT(TempPos.y) + m_pGameTex[4].GetImageHeight()
			);

		SetRect(&rcCol2 , INT(m_xwing.vcPosC.x+22)
			, INT(m_xwing.vcPosC.y)
			, INT(m_xwing.vcPosC.x+22) + 11		//m_pGameTex[1].GetImageWidth()
			, INT(m_xwing.vcPosC.y) + 56		//m_pGameTex[1].GetImageHeight()
			);
		SetRect(&rcColSound , INT(m_xwing.vcPosC.x-112)
			, INT(m_xwing.vcPosC.y)
			, INT(m_xwing.vcPosC.x-112) + 275	//m_pGameTex[1].GetImageWidth()
			, INT(m_xwing.vcPosC.y+56)			//+ m_pGameTex[1].GetImageHeight()
			);
		}
		///////////////////////////////////////////////////////////////////////
		
		if((*_F)->laserhit==TRUE)
			continue;

		if( rcCol1.left <= rcCol2.right &&
			rcCol1.right>= rcCol2.left  &&

			rcCol1.top   <= rcCol2.bottom &&
			rcCol1.bottom >= rcCol2.top )
		{
			bColl =1;
			//SetWindowText(m_hWnd, "Collision");

			//���� ����
			//m_pSndExplo->Reset();
			//m_pSndExplo->Play();
			//m_pSndExplo->SetVolume(0);
			//���� ��


			m_xwing.hp=m_xwing.hp-(*_F)->damage; //���ΰ� ĳ���� ü�� - (*_F)->damage
			if(m_xwing.hp<=0)
			{
				m_xwing.dead=TRUE;

				//���ΰ��� ������ ��� ���� ȭ�� ������� �̵�

				std::vector<CCharacterData*>::iterator _FT=pvTie0.begin();
				std::vector<CCharacterData*>::iterator _LT=pvTie0.end();

				for(; _FT!=_LT ; ++_FT)
				{
					if (*_FT == 0)
						continue;

					(*_FT)->yval=(*_FT)->yval-600;
				}
			}
			(*_F)->laserhit=TRUE;
			m_xwing.laserhit=TRUE;
		}

		else if (rcCol1.left <= rcColSound.right &&
			rcCol1.right>= rcColSound.left  &&

			rcCol1.top   <= rcColSound.bottom &&
			rcCol1.bottom >= rcColSound.top && (*_F)->flybysound==FALSE)
		{
			//SetWindowText(m_hWnd, "Collision");

			if((*_F)->name=="Laser")
			{
				//m_pSndLaserby->Reset();
				//m_pSndLaserby->Play();
				//m_pSndLaserby->SetVolume(0);
			}
			else if((*_F)->name=="Ion")
			{
				//m_pSndLaserby->Reset();
				//m_pSndLaserby->Play();
				//m_pSndLaserby->SetVolume(0);
			}
			else if((*_F)->name=="Missile")
			{
				//m_pSndMisslby->Reset();
				//m_pSndMisslby->Play();
				//m_pSndMisslby->SetVolume(0);
			}
			else{}
			(*_F)->flybysound=TRUE;
		}

		if(0==bColl)
		{
			//SetWindowText(m_hWnd, "Press Left, Right, Up, Down Key!");
			(*_F)->vcPosC = TempPos;
		}
	}
	return bColl;
}

//�浹üũ �Լ�2: ���ΰ��Ѿ��� ���� �¾Ҵ��� üũ
INT CMain::ColCheck2()
{
	D3DXVECTOR3	TempPos;
	INT bColl=0;

	std::vector<CLaserData*>::iterator _F=pvLaser0.begin();
	std::vector<CLaserData*>::iterator _L=pvLaser0.end();

	for(;_F != _L; ++_F)
	{
		TempPos=D3DXVECTOR3((*_F)->laserxval,(*_F)->laseryval,0);

		RECT rcCol1;
		RECT rcCol2;

		std::vector<CCharacterData*>::iterator _FT=pvTie0.begin();
		std::vector<CCharacterData*>::iterator _LT=pvTie0.end();

		for(; _FT!=_LT ; ++_FT)
		{
			if ((*_FT)->dead==TRUE)
				continue;

			if((*_FT)->name=="Boss")
			{
				SetRect(&rcCol1 , INT(TempPos.x)
					, INT(TempPos.y)
					, INT(TempPos.x) + m_pGameTex[2].GetImageWidth()
					, INT(TempPos.y) + m_pGameTex[2].GetImageHeight()
					);

				SetRect(&rcCol2 , INT((*_FT)->vcPosC.x+29)
					, INT((*_FT)->vcPosC.y+61)
					, INT((*_FT)->vcPosC.x+29) + 41	//m_pGameTex[3].GetImageWidth()
					, INT((*_FT)->vcPosC.y+61) + 44	//m_pGameTex[3].GetImageHeight()
					);
			}
			else
			{
				SetRect(&rcCol1 , INT(TempPos.x)
					, INT(TempPos.y)
					, INT(TempPos.x) + m_pGameTex[2].GetImageWidth()
					, INT(TempPos.y) + m_pGameTex[2].GetImageHeight()
					);

				SetRect(&rcCol2 , INT((*_FT)->vcPosC.x)
					, INT((*_FT)->vcPosC.y)
					, INT((*_FT)->vcPosC.x) + m_pGameTex[3].GetImageWidth()
					, INT((*_FT)->vcPosC.y) + m_pGameTex[3].GetImageHeight()
					);
			}


			if( rcCol1.left <= rcCol2.right &&
				rcCol1.right>= rcCol2.left  &&

				rcCol1.top   <= rcCol2.bottom &&
				rcCol1.bottom >= rcCol2.top )
			{
				bColl =1;
				//SetWindowText(m_hWnd, "Collision");

				//���� ����
				//m_pSndExplo->Reset();
				//m_pSndExplo->Play();
				//m_pSndExplo->SetVolume(0);
				//���� ��

				(*_FT)->hp=(*_FT)->hp-50*m_xwing.m_power;		//���ɸ��� ü�� -50

				if((*_FT)->hp<=0)				//HP�� 0�ϰ�� ��ĳ�� ���
				{
					(*_FT)->dead=TRUE;
				}
				else
				{
				}
				(*_F)->laserhit=TRUE;
				(*_FT)->laserhit=TRUE;

			}


			if(0==bColl)
			{
				//SetWindowText(m_hWnd, "Press Left, Right, Up, Down Key!");
				(*_F)->vcPosC = TempPos;
			}
		}
	}

	return bColl;
}

//�浹üũ �Լ�3: ���� ���ΰ��� �浹 üũ
INT CMain::ColCheck3()
{
	D3DXVECTOR3	TempPos;
	INT bColl=0;

	std::vector<CCharacterData*>::iterator _FT=pvTie0.begin();
	std::vector<CCharacterData*>::iterator _LT=pvTie0.end();


	for(;_FT != _LT; ++_FT)
	{
		if (*_FT == 0 )
			continue;

		TempPos=D3DXVECTOR3((*_FT)->xval,(*_FT)->yval,0);

		RECT rcCol1;
		RECT rcCol2;
		RECT rcCol3;	//���� �浹 üũ��
		
		SetRect(&rcCol1, INT(TempPos.x), INT(TempPos.y)
				,INT(TempPos.x)+m_pGameTex[3].GetImageWidth()
				,INT(TempPos.y)+m_pGameTex[3].GetImageHeight());

		SetRect(&rcCol2, INT(m_xwing.vcPosC.x-112),INT(m_xwing.vcPosC.y-112)
				,INT(m_xwing.vcPosC.x-112)+275
				,INT(m_xwing.vcPosC.y)+112);

		SetRect(&rcCol3 , INT(m_xwing.vcPosC.x+22)
			, INT(m_xwing.vcPosC.y)
			, INT(m_xwing.vcPosC.x+22) + 11//m_pGameTex[1].GetImageWidth()
			, INT(m_xwing.vcPosC.y) + 56//m_pGameTex[1].GetImageHeight()
			);

		if( rcCol1.left <= rcCol2.right &&
			rcCol1.right>= rcCol2.left  &&

			rcCol1.top   <= rcCol2.bottom &&
			rcCol1.bottom >= rcCol2.top &&(*_FT)->flybysound==FALSE)
		{
			bColl =1;
			//SetWindowText(m_hWnd, "Collision");
			if((*_FT)->name !="Life"||(*_FT)->name !="Power"||(*_FT)->name !="Speed"||(*_FT)->name !="Laser"||(*_FT)->name !="Hp")
			{
			//�� �������� ����

			//m_pSndTie->Play();
			//m_pSndTie->SetVolume(0);
			//���� ��
			if((*_FT)->name =="Boss"&&rand()%2==0)
			{
				//m_pSndBossline1->Reset();
				//m_pSndBossline1->Stop();
				//m_pSndBossline2->Reset();
				//m_pSndBossline2->Stop();
				//m_pSndBossline3->Reset();
				//m_pSndBossline3->Stop();
				//m_pSndBossline4->Reset();
				//m_pSndBossline4->Stop();
				//m_pSndBossline5->Reset();
				//m_pSndBossline5->Stop();
				//m_pSndBossline6->Reset();
				//m_pSndBossline6->Stop();
				//m_pSndBossline7->Reset();
				//m_pSndBossline7->Stop();

				switch(rand()%7)
				{
				case 0:
					//m_pSndBossline1->Reset();
					//m_pSndBossline1->Play();
					//m_pSndBossline1->SetVolume(5000);
					break;
				case 1:
					//m_pSndBossline2->Reset();
					//m_pSndBossline2->Play();
					//m_pSndBossline2->SetVolume(5000);
					break;
				case 2:
					//m_pSndBossline3->Reset();
					//m_pSndBossline3->Play();
					//m_pSndBossline3->SetVolume(5000);
					break;
				case 3:
					//m_pSndBossline4->Reset();
					//m_pSndBossline4->Play();
					//m_pSndBossline4->SetVolume(5000);
					break;
				case 4:
					//m_pSndBossline5->Reset();
					//m_pSndBossline5->Play();
					//m_pSndBossline5->SetVolume(5000);
					break;
				case 5:
					//m_pSndBossline6->Reset();
					//m_pSndBossline6->Play();
					//m_pSndBossline6->SetVolume(5000);
					break;
				case 6:
					//m_pSndBossline7->Reset();
					//m_pSndBossline7->Play();
					//m_pSndBossline7->SetVolume(5000);
					break;
				default:
					//m_pSndBossline1->Reset();
					//m_pSndBossline1->Play();
					//m_pSndBossline1->SetVolume(5000);
					break;
				}
			}
			}
			else{}

			(*_FT)->flybysound=TRUE;
		}

		if(0==bColl)
		{
			//SetWindowText(m_hWnd, "Press Left, Right, Up, Down Key!");
			(*_FT)->vcPosC = TempPos;
		}
		
	}
	return bColl;
}

////////////////////////////////////////////////////////////////////////////////
//�޴����� ���� �޴�, ���� ����, ���ھ� 

int		CMain::StartFrameMove()				// ���� ����
{
	m_pInput->FrameMove();

	//m_pSndTie->Reset();
	//m_pSndTie->Stop();
	//m_pMp[0]->Reset();//m_pSndGameBg->Reset();
	//m_pMp[1]->Play();//m_pSndGameMenu->Play();
	//m_pMp[1]->SetVolume(9000L);//m_pSndGameMenu->SetVolume(1000);
	//m_pSound.Stop(29);
	//m_pSound.Play(10, true);

	if(0 == m_KeyOld[VK_RETURN] && m_KeyCur[VK_RETURN] )
	{
		m_nGamePhase = 1;
		ImgArrayInit();
		return 0;
	}
	
	//09-08-28 �߰�
	m_pInput->MenuItemClickCheck();

	return 0;
}


void	CMain::StartRender()					// ���� ������
{
	m_pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_pd3dSprite->Draw(m_pGameTex[5].m_pTex,&rc,NULL,&vcPos,D3DXCOLOR(1,1,1,1.f));
	m_pd3dSprite->End();
}


void	CMain::StartRndBck()					// ���� Back buffer ������
{

}


int		CMain::PlayFrameMove()				// �÷��� ����
{
	//m_pMp[1]->Reset();//m_pSndGameMenu->Reset();
	//m_pMp[1]->Stop();//m_pSndGameMenu->Stop();
	//m_pMp[0]->Play();//m_pSndGameBg->Play(0, DSBPLAY_LOOPING);
	//m_pSound.Stop(10);
	//m_pSound.Play(13, true);

	//this->BoomSprite();
	this->InputMove();
	
	m_alphatime=timeGetTime()-m_gamebegin;

	this->CharacterMove();


////////////////////////////////////////////////////////////////////////////////
	std::vector<CCharacterData*>::iterator _FT=pvTie0.begin();
	std::vector<CCharacterData*>::iterator _LT=pvTie0.end();

	char* attackname = "";
	
	
	for(; _FT!=_LT ; ++_FT)
	{
		if (*_FT == 0)
			continue;

		if((*_FT)->xval<=m_xwing.xval+0.3 && (*_FT)->xval>=m_xwing.xval-0.3 && (*_FT)->yval >=0 && (*_FT)->yval <=600)
		{
			if(pvLaser1.size() <=((*_FT)->laserability+1)*pvTie0.size())
			{
				m_dTimeEnd	= timeGetTime();

				if( (m_dTimeEnd-m_dTimeBegin)>100)
				{
					if((*_FT)->dead==FALSE)
					{
						////////////////////////////////////////////////////////////////////////////////////////
						if((*_FT)->laserable==TRUE&&(*_FT)->ionable==TRUE&&(*_FT)->missileable==TRUE)
						{
							switch(rand()%3)
							{
							case 0:
								if(lasercnt != curlasercnt)
								{
									attackname="Laser";
									pvLaser1.push_back(new CLaserData(attackname,(*_FT)->xval+15, (*_FT)->yval+30));
									curlasercnt++;
								}
								break;

							case 1:
								if(ioncnt != curioncnt)
								{
									attackname="Ion";
									pvLaser1.push_back(new CLaserData(attackname,(*_FT)->xval+15, (*_FT)->yval+30));
									curioncnt++;
								}
								break;

							case 2:
								if(missilecnt !=curmissilecnt)
								{
									attackname="Missile";
									pvLaser1.push_back(new CLaserData(attackname,(*_FT)->xval+15, (*_FT)->yval+30));
									curmissilecnt++;
								}
								break;

							default:
								attackname="Missile";
								break;
							}
						}
						else if((*_FT)->laserable==TRUE&&(*_FT)->ionable==TRUE)
						{
							switch(rand()%2)
							{
							case 0:
								if(lasercnt != curlasercnt)
								{
									attackname="Laser";
									pvLaser1.push_back(new CLaserData(attackname,(*_FT)->xval+15, (*_FT)->yval+30));
									curlasercnt++;
								}
								break;

							case 1:
								if(ioncnt != curioncnt)
								{
									attackname="Ion";
									pvLaser1.push_back(new CLaserData(attackname,(*_FT)->xval+15, (*_FT)->yval+30));
									curioncnt++;
								}
								break;
							}
						}
						else if((*_FT)->laserable==TRUE&&(*_FT)->missileable==TRUE)
						{
							switch(rand()%2)
							{
							case 0:
								if(lasercnt != curlasercnt)
								{
									attackname="Laser";
									pvLaser1.push_back(new CLaserData(attackname,(*_FT)->xval+15, (*_FT)->yval+30));
									curlasercnt++;
								}
								break;

							case 1:
								if(missilecnt !=curmissilecnt)
								{
									attackname="Missile";
									pvLaser1.push_back(new CLaserData(attackname,(*_FT)->xval+15, (*_FT)->yval+30));
									curmissilecnt++;
								}
								break;
							}
						}
						else if((*_FT)->ionable==TRUE&&(*_FT)->missileable==TRUE)
						{
							switch(rand()%2)
							{
							case 0:
								if(ioncnt != curioncnt)
								{
									attackname="Ion";
									pvLaser1.push_back(new CLaserData(attackname,(*_FT)->xval+15, (*_FT)->yval+30));
									curioncnt++;
								}
								break;

							case 1:
								if(missilecnt !=curmissilecnt)
								{
									attackname="Missile";
									pvLaser1.push_back(new CLaserData(attackname,(*_FT)->xval+15, (*_FT)->yval+30));
									curmissilecnt++;
								}
								break;
							}

						}
						else if((*_FT)->missileable==TRUE)
						{
							if(missilecnt !=curmissilecnt)
							{
								attackname="Missile";
								pvLaser1.push_back(new CLaserData(attackname,(*_FT)->xval+15, (*_FT)->yval+30));
								curmissilecnt++;
							}
						}
						else if((*_FT)->laserable==TRUE)
						{
							if(lasercnt != curlasercnt)
							{
								attackname="Laser";
								pvLaser1.push_back(new CLaserData(attackname,(*_FT)->xval+15, (*_FT)->yval+30));
								curlasercnt++;
							}
						}
						else{}
						////////////////////////////////////////////////////////////////////////////////////////

						//�� �Ѿ� ����
						if(attackname=="Laser")
						{
							switch(rand()%1)
							{
							case 0:
								//m_pSndTLsr1->Reset();
								//m_pSndTLsr1->Play();
								//m_pSndTLsr1->SetVolume(0);
								break;
							case 1:
								//m_pSndTLsr2->Reset();
								//m_pSndTLsr2->Play();
								//m_pSndTLsr2->SetVolume(0);
								break;
							default:
								//m_pSndTLsr1->Reset();
								//m_pSndTLsr1->Play();
								//m_pSndTLsr1->SetVolume(0);
								break;
							}
						}
						else if(attackname=="Missile")
						{
							//m_pSndMissl->Reset();
							//m_pSndMissl->Play();
							//m_pSndMissl->SetVolume(0);
						}
						else if(attackname=="Ion")
						{
							//m_pSndIon->Reset();
							//m_pSndIon->Play();
							//m_pSndIon->SetVolume(0);
						}
						else
						{
							//char buf[80];
							//sprintf(buf, "%s\n", m_alphatime);
 							//OutputDebugString(buf);
							//attackname = "";
						}
						//���� ��

						/////////////////////////////////////////////////////////////////////////

						std::vector<CLaserData*>::iterator _FL=pvLaser1.begin();
						std::vector<CLaserData*>::iterator _LL=pvLaser1.begin();

						for(;_FL!=_LL ;_FL++)
						{
							if((*_FL)->name=="Laser")
							{
								curlasercnt++;
							}
							else if((*_FL)->name=="Ion")
							{
								curioncnt++;
							}
							else if((*_FL)->name=="Missile")
							{
								curmissilecnt++;
							}
							else
							{}
						}
						/////////////////////////////////////////////////////////////////////////
					}
						m_dTimeBegin = m_dTimeEnd;
				}
	
			}
		}
	}
	attackname = "";

	//char sbuf[80];
	//sprintf(sbuf, "%d\n", m_alphatime);
	//OutputDebugString(sbuf);
	//attackname = "";
////////////////////////////////////////////////////////////////////////////////
return 0;
}


void CMain::PlayRender()				// �÷��� ������
{
	m_pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND);

	vcBar= D3DXVECTOR3(650,0,0);							//���¹���ġ
	m_xwing.AssignVal(m_xwing.xval,540);				//���ΰ���ġ

	m_pd3dSprite->Draw(m_pGameTex[23].m_pTex,&rc,NULL,&vcPos1,D3DXCOLOR(1,1,1,1.f)); //���
	m_pd3dSprite->Draw(m_pGameTex[24].m_pTex,&rc,NULL,&vcPos2,D3DXCOLOR(1,1,1,1.f)); //���
	//D3DXCOLOR ������ �Ű������� ���İ��� �ǹ��ϰ� �� �Լ��� �� �Ű������� RGBA�����μ� ���� 0f~1f�� ���� ������.
	if(vcPos1.y >= 600){vcPos1.y= -600;}else{vcPos1.y+=m_movingdist/20;}
	if(vcPos2.y >= 600){vcPos2.y= -600;}else{vcPos2.y+=m_movingdist/20;}
	
	
	if(m_xwing.dead ==TRUE)
	{
		pvLaser1.clear();
		curlasercnt=0;
		curioncnt=0;
		curmissilecnt=0;
	}
	
	if(m_xwing.dead !=TRUE)
	{
	m_pd3dSprite->Draw(m_pGameTex[1].m_pTex,&rc,NULL,&m_xwing.vcPosC,D3DXCOLOR(1,1,1,1.f)); //���ΰ�ĳ����
	}
	
	/////////////////////////////////////////////////////////////////////////////
	//���ΰ� �Ѿ�

	std::vector<CLaserData*>::iterator _F=pvLaser0.begin();
	std::vector<CLaserData*>::iterator _L=pvLaser0.end();

	for(; _F!=_L ; ++_F)
	{
		if (*_F == 0)
			continue;

		(*_F)->vcPosC = D3DXVECTOR3((*_F)->laserxval,(*_F)->laseryval,0);
		m_pd3dSprite->Draw(m_pGameTex[2].m_pTex,&rc,NULL,&((*_F)->vcPosC),D3DXCOLOR(1,1,1,1.f));

		if((*_F)->laseryval <= -30 || (*_F)->laserhit==TRUE)
		{
			if (*_F != 0)
			{				
				delete (*_F);
				*_F = 0;
			}
		}

		else
		{
			(*_F)->laseryval=(*_F)->laseryval=(*_F)->laseryval-m_movingdist;
		}
	}

	_F=pvLaser0.begin();
	while (_F != pvLaser0.end() )
	{
		if (*_F == 0)
			_F = pvLaser0.erase(_F);
		else
			_F++;
	}
	/////////////////////////////////////////////////////////////////////////////
	//��ĳ����
	std::vector<CCharacterData*>::iterator _FT=pvTie0.begin();
	std::vector<CCharacterData*>::iterator _LT=pvTie0.end();

	for(; _FT!=_LT ; ++_FT)
	{
		if (*_FT == 0)
			continue;

		(*_FT)->vcPosC=D3DXVECTOR3((*_FT)->xval,(*_FT)->yval,0);
		if((*_FT)->dead !=TRUE)
		{
			(*_FT)->CharacterDraw((*_FT)->name,rc, _FT);
		}

	}

	_FT=pvTie0.begin();
	while (_FT != pvTie0.end() )
	{
		if (*_FT == 0)
			_FT = pvTie0.erase(_FT);
		else
			_FT++;
	}
	/////////////////////////////////////////////////////////////////////////////
	//�� �Ѿ�

	std::vector<CLaserData*>::iterator _F1=pvLaser1.begin();
	std::vector<CLaserData*>::iterator _L1=pvLaser1.end();

	for(; _F1!=_L1 ; ++_F1)
	{
		if (*_F1 == 0)
			continue;

		(*_F1)->vcPosC=D3DXVECTOR3((*_F1)->laserxval,(*_F1)->laseryval,0);

		(*_F1)->LaserDraw((*_F1)->name,rc,_F1);

		if((*_F1)->laseryval >= 630 || (*_F1)->laserhit==TRUE)
		{
			if (*_F1 != 0)
			{		
				//////////////////////////////////////////////////////////////////
					if((*_F1)->name=="Laser")
					{
						curlasercnt--;
					}
					else if((*_F1)->name=="Ion")
					{
						curioncnt--;
					}
					else if((*_F1)->name=="Missile")
					{
						curmissilecnt--;
					}
					else
					{}
				//////////////////////////////////////////////////////////////////
				delete (*_F1);
				*_F1 = 0;
			}
		}
		else
		{
			(*_F1)->laseryval=(*_F1)->laseryval=(*_F1)->laseryval+m_movingdist/(*_F1)->speed;
		}
	}

	_F1=pvLaser1.begin();
	while (_F1 != pvLaser1.end() )
	{
		if (*_F1 == 0)
			_F1 = pvLaser1.erase(_F1);
		else
			_F1++;
	}
	/////////////////////////////////////////////////////////////////////////////

	LPDIRECT3DTEXTURE9 pTex = (LPDIRECT3DTEXTURE9)m_pTx->GetTexture();
////////////////////////////////////////////////////////////////////////////////
//���� : ���ΰ��� �� �Ѿ� ������ �浹 �Ǿ��°�?
	ColCheck2();  // ���ΰ� -> �� �浹üũ
	_FT=pvTie0.begin();
	_LT=pvTie0.end();

	for(; _FT!=_LT ; ++_FT)
	{
		if(*_FT == 0)
			continue;

		if((*_FT)->laserhit==TRUE)
		{
			if((*_FT)->name=="Boss")
			{
				boom.x=(*_FT)->vcPosC.x-15+29;
				boom.y=(*_FT)->vcPosC.y-13+61;
			}
			else
			{
				boom.x=(*_FT)->vcPosC.x-15;
				boom.y=(*_FT)->vcPosC.y-13;
			}
			//////////////////////////////////////////////////////////////////////
			(*_FT)->m_dTimeEnd	= timeGetTime();

			if( ((*_FT)->m_dTimeEnd-(*_FT)->m_dTimeBegin)>50)
			{
				m_ImgRc2.left +=70;

				if(m_ImgRc2.left +70 >=1120)
				{
					m_ImgRc2.left = 0;
					(*_FT)->laserhit=FALSE;
				}

				m_ImgRc2.right =m_ImgRc2.left +70;
				(*_FT)->m_dTimeBegin = (*_FT)->m_dTimeEnd;

			}
			//////////////////////////////////////////////////////////////////////
			if (*_FT != 0)
			{
			m_pd3dSprite->Draw(pTex, &m_ImgRc2, NULL, &boom, D3DXCOLOR(1,1,1,1.F)); //&m_vcPosImg2
			}
			//////////////////////////////////////////////////////////////////////
		}
		if((*_FT)->dead==TRUE)
		{
			(*_FT)->m_dTimeEnd	= timeGetTime();
			if( ((*_FT)->m_dTimeEnd-(*_FT)->m_dTimeBegin)>50)
			{
				m_ImgRc2.left +=70;

				if(m_ImgRc2.left +70 >=1120)
				{
					m_ImgRc2.left = 0;
					if (*_FT != 0)
					{
						if((*_FT)->laserable==TRUE){lasercnt-=(*_FT)->laserability;}
						if((*_FT)->ionable==TRUE){ioncnt-=(*_FT)->ionability;}
						if((*_FT)->missileable==TRUE){missilecnt-=(*_FT)->missileability;}
						
						if((*_FT)->name=="Life"||(*_FT)->name=="Power"||(*_FT)->name=="Speed"||(*_FT)->name=="Laser"||(*_FT)->name=="Hp")
						{
							if((*_FT)->name=="Life")
							{
								m_xwing.m_life++;
								//m_pSndBonus->Reset();
								//m_pSndBonus->Play();
								//m_pSndBonus->SetVolume(0);
								//m_pSndR2->Reset();
								//m_pSndR2->Play();
								//m_pSndR2->SetVolume(5000);
							}
							else if((*_FT)->name=="Laser")
							{
								m_xwing.m_laserable++;
								//m_pSndBonus->Reset();
								//m_pSndBonus->Play();
								//m_pSndBonus->SetVolume(0);
								//m_pSndLaserup->Reset();
								//m_pSndLaserup->Play();
								//m_pSndLaserup->SetVolume(5000);
							}
							else if((*_FT)->name=="Power")
							{
								m_xwing.m_power++;
								//m_pSndBonus->Reset();
								//m_pSndBonus->Play();
								//m_pSndBonus->SetVolume(0);
								//m_pSndPowerup->Reset();
								//m_pSndPowerup->Play();
								//m_pSndPowerup->SetVolume(5000);
							}
							else if((*_FT)->name=="Hp")
							{
								m_xwing.hp=100;
								//m_pSndBonus->Reset();
								//m_pSndBonus->Play();
								//m_pSndBonus->SetVolume(0);
								//m_pSndHpup->Reset();
								//m_pSndHpup->Play();
								//m_pSndHpup->SetVolume(5000);
							}
							else if((*_FT)->name=="Speed")
							{
								m_xwing.m_speed++;
								//m_pSndBonus->Reset();
								//m_pSndBonus->Play();
								//m_pSndBonus->SetVolume(0);
								//m_pSndSpeedup->Reset();
								//m_pSndSpeedup->Play();
								//m_pSndSpeedup->SetVolume(5000);
							}
							else{}

							delete (*_FT);
							*_FT = 0;
						}
						else
						{
							delete (*_FT);
							*_FT = 0;
							score=score+10;
							m_xwing.m_killcount++;
						}
						if(score !=0 && score%1000==0)
						{
							m_xwing.m_life++;
							//m_pSndBonus->Reset();
							//m_pSndBonus->Play();
							//m_pSndBonus->SetVolume(0);
							//m_pSndR2->Reset();
							//m_pSndR2->Play();
							//m_pSndR2->SetVolume(5000);
						}

						if((m_xwing.m_killcount !=0) && (m_xwing.m_killcount%6==0))
						{
							stage++;
						}
					}
				}
				m_ImgRc2.right =m_ImgRc2.left +70;
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////
	//���� : ������ �� �Ѿ� ���ΰ����� �浹 �Ǿ��°�?
	if(m_xwing.dead !=TRUE)
	{
	ColCheck();	// �� -> ���ΰ� �浹üũ
	}
	if(m_xwing.laserhit==TRUE)
	{
		boom.x=m_xwing.vcPosC.x-8;
		boom.y=m_xwing.vcPosC.y-7;
		////////////////////////////////////////////////////////////////////////////////
		m_xwing.m_dTimeEnd	= timeGetTime();

		if( (m_xwing.m_dTimeEnd-m_xwing.m_dTimeBegin)>100)
		{
			m_ImgRc2.left +=70;

			if(m_ImgRc2.left +70 >=1120)
			{
				m_ImgRc2.left = 0;
				m_xwing.laserhit=FALSE;
			}

			m_ImgRc2.right =m_ImgRc2.left +70;
			m_xwing.m_dTimeBegin = m_xwing.m_dTimeEnd;

		}
		////////////////////////////////////////////////////////////////////////////////
		m_pd3dSprite->Draw(pTex, &m_ImgRc2, NULL, &boom, D3DXCOLOR(1,1,1,1.F)); //&m_vcPosImg2
	}
	if(m_xwing.dead==TRUE)
	{
		m_xwing.m_dTimeEnd	= timeGetTime();

		if( (m_xwing.m_dTimeEnd-m_xwing.m_dTimeBegin)>100)
		{
			m_ImgRc2.left +=70;

			if(m_ImgRc2.left +70 >=1120)
			{
				m_ImgRc2.left = 0;
				//m_nGamePhase=2;
			}
			/////////////////////////////////////////////////////////////////////////////
			if(m_xwing.dead==TRUE && m_xwing.m_life !=0)
			{
				m_xwing.xval=325;
				m_xwing.yval=500;
				m_xwing.hp=100;
				m_xwing.dead=FALSE;
				m_xwing.laserhit=FALSE;
				m_xwing.laserability=3;
				--m_xwing.m_life;
				pvLaser0.clear();
				pvLaser1.clear();
				curmissilecnt=0;
				curlasercnt=0;
				curioncnt=0;
			}
			else
			{
				m_nGamePhase=2;
				//m_pMp[0]->Reset();//m_pSndGameBg->Reset();
				//m_pMp[0]->Stop();//m_pSndGameBg->Stop();
				//m_pSndGameover->Reset();
				//m_pSndGameover->Play();
				//m_pSndGameover->SetVolume(0);
				//m_pSound.Stop(13);
				//m_pSound.Play(29, true);
			}
			////////////////////////////////////////////////////////////////////////////
			m_ImgRc2.right =m_ImgRc2.left +70;
			m_xwing.m_dTimeBegin = m_xwing.m_dTimeEnd;
		}
		if(m_xwing.dead != TRUE)
		{
			m_pd3dSprite->Draw(pTex, &m_ImgRc2, NULL, &boom, D3DXCOLOR(1,1,1,1.F)); //&m_vcPosImg2
		}
	}
	ColCheck3(); //�浹üũ �Լ�3: ���� ���ΰ��� �浹 üũ
////////////////////////////////////////////////////////////////////////////////
	m_pd3dSprite->Draw(m_pGameTex[7].m_pTex,&rc,NULL,&vcBar,D3DXCOLOR(1,1,1,1.f)); //���¹�
	m_pd3dSprite->End();
////////////////////////////////////////////////////////////////////////////////
	g_text.Begin();
	
	hh=m_alphatime/3600000;
	mm=m_alphatime%3600000;
	mm=mm/60000;
	ss=m_alphatime%3600000%60000/1000;

	
	sprintf(scoreBuf, " %d", score);
	sprintf(killcount, " %d", m_xwing.m_killcount);
	sprintf(enemyBuf,"%d : Enemy",pvTie0.size());
	sprintf(timeBuf," %d : %d : %d",hh, mm, ss);
	sprintf(ioncntBuf,    "%d : Ion", ioncnt);
	sprintf(lasercntBuf,  "%d : Laser", lasercnt);
	sprintf(missilecntBuf,"%d : Missile", missilecnt);
	sprintf(curioncntBuf,    "%d : Ion Cnt", curioncnt);
	sprintf(curlasercntBuf,  "%d : Laser Cnt", curlasercnt);
	sprintf(curmissilecntBuf,"%d : Missile Cnt", curmissilecnt);
	sprintf(vectorsize, "%d : Attack Cnt", pvLaser1.size());
	sprintf(stageBuf, " %d", stage);
	sprintf(enemylevelBuf, " %d", enemylevel);
	sprintf(lifeBuf, " Life : %d", m_xwing.m_life);
	sprintf(phpBuf, " H.P : %d", m_xwing.hp);
	sprintf(playerabilBuf, " Laser : %d", m_xwing.m_laserable);
	sprintf(powerBuf, " Power : %d", m_xwing.m_power);
	sprintf(speedBuf, " Speed : %d", m_xwing.m_speed);

	g_text.Draw("Score", 660, 10);
	g_text.Draw(scoreBuf, 660, 30);

	g_text.Draw("KillCount", 660, 50);
	g_text.Draw(killcount, 660, 70);
	g_text.Draw("FlightTime",660,90);
	g_text.Draw(timeBuf,660,110);
	g_text.Draw("------------------",660,150);
	g_text.Draw(enemyBuf,660,160);
	g_text.Draw("------------------",660,170);
	g_text.Draw(lasercntBuf,660,180);
	g_text.Draw(ioncntBuf,660,200);
	g_text.Draw(missilecntBuf,660,220);
	g_text.Draw("------------------",660,230);
	g_text.Draw(curlasercntBuf,660,240);
	g_text.Draw(curioncntBuf,660,260);
	g_text.Draw(curmissilecntBuf,660,280);
	g_text.Draw("------------------",660,290);
	g_text.Draw(vectorsize, 660, 300);
	g_text.Draw("------------------",660,310);

	/////////////////////////////////////////////////////////////////////////////
	//FPS ȭ�鿡 ���
	sprintf(fpsBuf, "FPS: %4.1f", m_fFps);
	g_text.Draw(fpsBuf, 660, 340);
	/////////////////////////////////////////////////////////////////////////////

	g_text.Draw("Stage", 660, 370);
	g_text.Draw(stageBuf, 660, 390);
	g_text.Draw("Level", 660, 410);
	g_text.Draw(enemylevelBuf, 660, 430);

	g_text.Draw("Player", 660, 480);
	g_text.Draw(lifeBuf, 660, 500);
	g_text.Draw(phpBuf, 660, 520);
	g_text.Draw(playerabilBuf, 660, 540);
	g_text.Draw(powerBuf,	660, 560);
	g_text.Draw(speedBuf, 660, 580);

	g_text.End();
}


void	CMain::PlayRndBck()				// �÷��� Back buffer ������
{

}


int		CMain::ScoreFrameMove()			// ���ھ� ����
{
return 0;
}


void	CMain::ScoreRender()				// ���ھ� ������
{

}


void	CMain::ScoreRndBck()				// ���ھ� Back buffer ������
{

}

int		CMain::EndFrameMove()						// ���ӿ��� ����
{
	if(0 == m_KeyOld[VK_RETURN] && m_KeyCur[VK_RETURN] )
	{
		m_nGamePhase = 0;
		ImgArrayInit();
		//m_pSndExplo->Stop();
		//m_pSndTie->Reset();
		//m_pSndTie->Stop();
		//m_pSndGameover->Reset();
		//m_pSndGameover->Stop();
		return 0;
	}

	return 0;
}


void	CMain::EndRender()						// ���ӿ��� ������
{
	m_pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_pd3dSprite->Draw(m_pGameTex[6].m_pTex,&rc,NULL,&vcPos,D3DXCOLOR(1,1,1,1.f));
////////////////////////////////////////////////////////////////////////////////
	m_pd3dSprite->End();

	g_text.Begin();
	char	scoreBuf[80];
	TCHAR	fpsBuf[128];

	sprintf(scoreBuf, "%d", score);

	g_text.Draw("Game Over", 355, 250,D3DXCOLOR(0,0,0,1));

	g_text.Draw("Total Score", 355, 300,D3DXCOLOR(0,0,0,1));
	g_text.Draw(scoreBuf, 355, 320,D3DXCOLOR(0,0,0,1));

	g_text.Draw("Press Enter for going back to Menu", 250, 400,D3DXCOLOR(0,0,0,1));

////////////////////////////////////////////////////////////////////////////////
//FPS ȭ�鿡 ���
	sprintf(fpsBuf, "FPS: %4.1f", m_fFps);
	g_text.Draw(fpsBuf, 700, 300);

	g_text.End();
}


void	CMain::EndRndBck()				// ���ӿ��� Back buffer ������
{
}


void	CMain::ScoreRead()				// ���ھ� �б�
{

}


void	CMain::ScoreWrite()				// ���ھ� ����
{

}


int		CMain::ImgArrayInit()					// Image Array Init
{
return 0;
}


void	CMain::ImgArrayDestroy()				// Image Array Delete
{

}


void	CMain::ImgArrayShuffle()				// Image Array Shuffle
{

}

void	CMain::InputMove()
{
	m_pInput->FrameMove();

	////////////////////////////////////////////////////////////////////////////////
	//SetWindowText(m_hWnd,"");
	//if(m_pInput->ButtonPress(0))
	//{
	//SetWindowText(m_hWnd, "���ʹ�ư�� �������ϴ�");
	//}
	//if(m_pInput->ButtonPress(1))
	//{
	//SetWindowText(m_hWnd, "�����ʹ�ư�� �������ϴ�");
	//}
	////////////////////////////////////////////////////////////////////////////////
	if(m_xwing.dead !=TRUE)
	{
		if(m_pInput->KeyPress(VK_RIGHT))
		{
			m_xwing.xval=m_xwing.xval+m_movingdist*m_xwing.m_speed;
			//++m_xwing.xval;
			if(m_xwing.xval>593)
			{
				m_xwing.xval=593;
			}
		}
		if(m_pInput->KeyPress(VK_LEFT))
		{
			m_xwing.xval=m_xwing.xval-m_movingdist*m_xwing.m_speed;
			if(m_xwing.xval<0)
			{
				m_xwing.xval=0;
			}
		}
		if(m_pInput->KeyDown(VK_SPACE))
		{
			if(pvLaser0.size() < m_xwing.m_laserable)
			{
				pvLaser0.push_back(new CLaserData(m_xwing.xval, m_xwing.yval,FALSE)); // ���ΰ� �Ѿ� �߻�
				//////////////////////////////////////////////////////////////////////
				//m_pSndXwLsr1->Reset();

				//m_pSndXwLsr1->Play();

				//m_pSndXwLsr1->SetVolume(0);
				//////////////////////////////////////////////////////////////////////
			}
		}
	}
}

void CMain::CharacterMove()
{
	
	std::vector<CCharacterData*>::iterator _FT=pvTie0.begin();
	std::vector<CCharacterData*>::iterator _LT=pvTie0.end();
	
	for(; _FT!=_LT ; ++_FT)
	{
		if (*_FT == 0)
			continue;

		if ((*_FT)->dead == TRUE)
			continue;
		
		(*_FT)->xval += sinf((m_alphatime+290000) * 0.0015f+(*_FT)->position);

		////(*_FT)->yval=(*_FT)->yval+movingdist;
		////(*_FT)->yval=+cosf(timeGetTime()	 * 0.001f) * 300.f+300;

		if((*_FT)->yval>=600)
		{
			if((*_FT)->name=="Boss")
			{
					(*_FT)->yval=(*_FT)->yval=-108;
					(*_FT)->flybysound=FALSE;
			}
			else
			{
				if((*_FT)->name=="Life"||(*_FT)->name=="Power"||(*_FT)->name=="Speed"||(*_FT)->name=="Laser"||(*_FT)->name=="Hp")
				{
					delete (*_FT);
					*_FT = 0;
				}
				else
				{
					(*_FT)->yval=(*_FT)->yval=-44;
					(*_FT)->flybysound=FALSE;
				}
			}
		}
		else
		{
			(*_FT)->yval=(*_FT)->yval+m_movingdist/(*_FT)->speed;
			//(*_FT)->yval=(*_FT)->yval+movingdist/10;
		}
	}
}

int CMain::GameTextureLoad(LPDIRECT3DTEXTURE9& pTex	//�����ʹ� ����(Alias), ����ü�� �ּҷ�
									,D3DXIMAGE_INFO* pImg				//�����ʹ� ����(Alias), ����ü�� �ּҷ�
									,LPDIRECT3DDEVICE9 pDevice
									,char* sfile
									,DWORD dColor)
{
	//Texture ��ü ����
	HRESULT hr = 0;
	hr = D3DXCreateTextureFromFileEx(
		pDevice
		,sfile //+
		,D3DX_DEFAULT
		,D3DX_DEFAULT
		,1						//MipLevels --> �߿� //3d�ΰ��� ����
		,0
		,D3DFMT_UNKNOWN
		,D3DPOOL_MANAGED		//<--����Ʈ�� �º���׷��� ī���� ��� �ȵǴ� ��찡 �ִ�.//MANAGED�� �ϸ� �׷��� ī�忡 �������!
		,D3DX_FILTER_NONE		//3d�ΰ��� ����
		,D3DX_FILTER_NONE		//3d�ΰ��� ����
		,dColor//+				//0x00ffffff:�����������  0x0:���İ����� ��� ���� �ø��ڴ�.
		,pImg //+
		,NULL
		,&pTex //+
		);
	return hr;
}


INT	CMain::TextureLoad()
{
	HRESULT hr=0;
	if(FAILED(GameTextureLoad(m_pGameTex[0].m_pTex
		,&m_pGameTex[0].m_pImg
		,m_pd3dDevice
		,"Texture/bg2.png",0x0)))
		hr= -1;

	if(FAILED(GameTextureLoad(m_pGameTex[1].m_pTex
		,&m_pGameTex[1].m_pImg
		,m_pd3dDevice
		,"Texture/xwing.png",0xFFFF00FF)))
		hr= -1;

	if(FAILED(GameTextureLoad(m_pGameTex[2].m_pTex
		,&m_pGameTex[2].m_pImg
		,m_pd3dDevice
		,"Texture/Laser1.png",0xFFFFFFFF)))
		hr= -1;

	if(FAILED(GameTextureLoad(m_pGameTex[3].m_pTex
		,&m_pGameTex[3].m_pImg
		,m_pd3dDevice
		,"Texture/Tie.png",0xFFFF00FF)))
		hr= -1;

	if(FAILED(GameTextureLoad(m_pGameTex[4].m_pTex
		,&m_pGameTex[4].m_pImg
		,m_pd3dDevice
		,"Texture/Laser2.png",0xFFFFFFFF)))
		hr= -1;

	if(FAILED(GameTextureLoad(m_pGameTex[5].m_pTex
		,&m_pGameTex[5].m_pImg
		,m_pd3dDevice
		,"Texture/title.png",0x0)))
		hr= -1;

	if(FAILED(GameTextureLoad(m_pGameTex[6].m_pTex
		,&m_pGameTex[6].m_pImg
		,m_pd3dDevice
		,"Texture/end.png",0x0)))
		hr= -1;

	if(FAILED(GameTextureLoad(m_pGameTex[7].m_pTex
		,&m_pGameTex[7].m_pImg
		,m_pd3dDevice
		,"Texture/statusbar.png",0x0)))
		hr= -1;

	if(FAILED(GameTextureLoad(m_pGameTex[8].m_pTex
		,&m_pGameTex[8].m_pImg
		,m_pd3dDevice
		,"Texture/Tie2.png",0xFFFF00FF)))
		hr= -1;

	if(FAILED(GameTextureLoad(m_pGameTex[9].m_pTex
		,&m_pGameTex[9].m_pImg
		,m_pd3dDevice
		,"Texture/Bomber.png",0xFFFF00FF)))
		hr= -1;

	if(FAILED(GameTextureLoad(m_pGameTex[10].m_pTex
		,&m_pGameTex[10].m_pImg
		,m_pd3dDevice
		,"Texture/Bomber2.png",0xFFFF00FF)))
		hr= -1;

	if(FAILED(GameTextureLoad(m_pGameTex[11].m_pTex
		,&m_pGameTex[11].m_pImg
		,m_pd3dDevice
		,"Texture/BomberAdv.png",0xFFFF00FF)))
		hr= -1;

	if(FAILED(GameTextureLoad(m_pGameTex[12].m_pTex
		,&m_pGameTex[12].m_pImg
		,m_pd3dDevice
		,"Texture/BomberAdv2.png",0xFFFF00FF)))
		hr= -1;

	if(FAILED(GameTextureLoad(m_pGameTex[13].m_pTex
		,&m_pGameTex[13].m_pImg
		,m_pd3dDevice
		,"Texture/Intercepter.png",0xFFFF00FF)))
		hr= -1;

	if(FAILED(GameTextureLoad(m_pGameTex[14].m_pTex
		,&m_pGameTex[14].m_pImg
		,m_pd3dDevice
		,"Texture/Advanced.png",0xFFFF00FF)))
		hr= -1;

	if(FAILED(GameTextureLoad(m_pGameTex[15].m_pTex
		,&m_pGameTex[15].m_pImg
		,m_pd3dDevice
		,"Texture/Boss.png",0xFFFF00FF)))
		hr= -1;

	if(FAILED(GameTextureLoad(m_pGameTex[16].m_pTex
		,&m_pGameTex[16].m_pImg
		,m_pd3dDevice
		,"Texture/Ion1.png",0xFFFFFFFF)))
		hr= -1;

	if(FAILED(GameTextureLoad(m_pGameTex[17].m_pTex
		,&m_pGameTex[17].m_pImg
		,m_pd3dDevice
		,"Texture/Missile.png",0xFFFFFFFF)))
		hr= -1;

	if(FAILED(GameTextureLoad(m_pGameTex[18].m_pTex
		,&m_pGameTex[18].m_pImg
		,m_pd3dDevice
		,"Texture/life.png",0xFFFFFFFF)))
		hr= -1;

	if(FAILED(GameTextureLoad(m_pGameTex[19].m_pTex
		,&m_pGameTex[19].m_pImg
		,m_pd3dDevice
		,"Texture/laser.png",0xFFFFFFFF)))
		hr= -1;

	if(FAILED(GameTextureLoad(m_pGameTex[20].m_pTex
		,&m_pGameTex[20].m_pImg
		,m_pd3dDevice
		,"Texture/speed.png",0xFFFFFFFF)))
		hr= -1;

	if(FAILED(GameTextureLoad(m_pGameTex[21].m_pTex
		,&m_pGameTex[21].m_pImg
		,m_pd3dDevice
		,"Texture/power.png",0xFFFFFFFF)))
		hr= -1;

	if(FAILED(GameTextureLoad(m_pGameTex[22].m_pTex
		,&m_pGameTex[22].m_pImg
		,m_pd3dDevice
		,"Texture/hp.png",0xFFFFFFFF)))
		hr= -1;

	if(FAILED(GameTextureLoad(m_pGameTex[23].m_pTex
		,&m_pGameTex[23].m_pImg
		,m_pd3dDevice
		,"Texture/bg2_1.png",0x0)))
		hr= -1;

	if(FAILED(GameTextureLoad(m_pGameTex[24].m_pTex
		,&m_pGameTex[24].m_pImg
		,m_pd3dDevice
		,"Texture/bg2_2.png",0x0)))
		hr= -1;

	if(FAILED(GameTextureLoad(m_pGameTex[25].m_pTex
		,&m_pGameTex[25].m_pImg
		,m_pd3dDevice
		,"Texture/howto.png",0x0)))
		hr= -1;

	m_pTx = new CGameTexture;
	if(FAILED(m_pTx->Create(m_pd3dDevice,"Texture/boom2.png", 0x00FFFFFF)))
	{
		delete m_pTx;
		hr= -1;
	}
	return hr;
}

INT CMain::SoundLoad()
{
	HRESULT hr=0;

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
	iIndex = m_pSound.Load("Sound/menubg.mp3");		//���۸޴� bgm
	iIndex = m_pSound.Load("Sound/missile.wav");	//�̻��� �߻�
	iIndex = m_pSound.Load("Sound/misslfly.wav");	//�̻����� ��������
	iIndex = m_pSound.Load("Sound/playbg.mp3");		//���� bgm
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

void CMain::HowtoFrameMove()
{
	//m_pMp[1]->Reset();//m_pSndGameMenu->Reset();
	//m_pMp[1]->Stop();//m_pSndGameMenu->Stop();

	if(0 == m_KeyOld[VK_RETURN] && m_KeyCur[VK_RETURN] )
	{
		m_nGamePhase = 0;
		ImgArrayInit();
		//m_pSndExplo->Stop();
		//m_pSndTie->Reset();
		//m_pSndTie->Stop();
		//m_pSndGameover->Reset();
		//m_pSndGameover->Stop();
	}
}

void CMain::HowtoRender()
{
	m_pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_pd3dSprite->Draw(m_pGameTex[25].m_pTex,&rc,NULL,&vcPos,D3DXCOLOR(1,1,1,1.f));
	m_pd3dSprite->End();
}
