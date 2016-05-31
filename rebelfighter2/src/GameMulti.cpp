#include "_StdAfx.h"

CGameMulti::~CGameMulti()
{
}

CGameMulti::CGameMulti()
{
	ioncnt = 0;
	lasercnt = 0;
	missilecnt = 0;
	curioncnt = 0;
	curlasercnt = 0;
	curmissilecnt = 0;


	stage = 1;			//처음 시작시 스테이지 1
	curstage;
	score = 0;			//점수


	vcPos1 = D3DXVECTOR3(-75, 0, 0);		//배경위치
	vcPos2 = D3DXVECTOR3(-75, -600, 0);	//배경위치

	boom = D3DXVECTOR3(0, 0, 0);

	SetRect(&m_ImgRc2, 0, 0, 0, 70);		// RECT 애니 이미지
}

INT CGameMulti::Init()
{
	HRESULT hr;


	score = 0;
	curstage = 0;
	stage = 1;
	m_xwing.xval = 325;
	m_xwing.yval = 500;
	m_xwing.hp = 100;
	m_xwing.dead = FALSE;
	m_xwing.laserhit = FALSE;
	m_xwing.laserability = 3;
	m_xwing.m_killcount = 0;
	m_xwing.m_life = 2;
	m_xwing.m_laserable = 3;
	m_xwing.m_speed = 1;
	m_xwing.m_power = 1;

	pvLaser0.clear();
	pvLaser1.clear();
	pvTie0.clear();

	curmissilecnt = 0;
	curlasercnt = 0;
	curioncnt = 0;

	GMAIN->m_gamebegin = timeGetTime();

	
	m_Client.Init();
	I_GameUser.Init();
	m_Udp.Init();

	return 0;
}

void CGameMulti::Frame()
{
	int j = 0;

	if (pvTie0.size() == 0)
	{
		srand((int)time(NULL));
	}

	if (curstage != stage)
	{
		if (pvTie0.size() == 0)
		{
			lasercnt = 0;
			ioncnt = 0;
			missilecnt = 0;

			pvLaser0.clear();
			curstage = stage;

			enemylevel = (50 + stage * 7) / 50;

			if (stage != 0 && stage % 10 == 0 && (50 + stage * 7) - 50 * m_xwing.m_power >0)
			{//10스테이지 마다 파워아이템 나옴
				pvTie0.push_back(new CCharacterData(13, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
			}
			if (stage != 0 && stage % 5 == 0)
			{

				//pvTie0.push_back(new CCharacterData("Boss",int(rand()%360),305,float(rand()%50-300),FALSE,5,500,0,FALSE));
				pvTie0.push_back(new CCharacterData(9, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 5, stage, int(rand() % 2), FALSE));//보스 생성
				pvTie0.push_back(new CCharacterData(int(rand() % 9), int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 2, stage, int(rand() % 2), FALSE));
				pvTie0.push_back(new CCharacterData(int(rand() % 9), int(rand() % 360), 305, float(rand() % 50 + 100 - 300), FALSE, 2, stage, int(rand() % 2), FALSE));
				pvTie0.push_back(new CCharacterData(int(rand() % 9), int(rand() % 360), 305, float(rand() % 50 + 150 - 300), FALSE, 2, stage, int(rand() % 2), FALSE));
				pvTie0.push_back(new CCharacterData(int(rand() % 9), int(rand() % 360), 305, float(rand() % 50 + 200 - 300), FALSE, 2, stage, int(rand() % 2), FALSE));
				pvTie0.push_back(new CCharacterData(int(rand() % 9), int(rand() % 360), 305, float(rand() % 50 + 250 - 300), FALSE, 2, stage, int(rand() % 2), FALSE));

				if (rand() % 6 == 3 && stage % 10 != 0)
				{
					switch (rand() % 7)
					{
					case 0://10. 라이프 아이템 생성
						if (m_xwing.m_life <3)
							pvTie0.push_back(new CCharacterData(10, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
						break;

					case 1://11. 레이저 아이템 생성
						if (m_xwing.m_laserable <6)
							pvTie0.push_back(new CCharacterData(11, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
						break;

					case 2://12. 스피드 아이템 생성
						pvTie0.push_back(new CCharacterData(12, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
						break;

					case 3://13. 파워 아이템 생성
						if ((50 + stage * 7) - 50 * m_xwing.m_power >0)
							pvTie0.push_back(new CCharacterData(13, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
						break;

					case 4://14. HP 아이템 생성
						pvTie0.push_back(new CCharacterData(14, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
						break;

					case 5://12. 스피드 아이템 생성
						pvTie0.push_back(new CCharacterData(12, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
						break;

					default://11. 레이저 아이템 생성
						if (m_xwing.m_laserable <6)
							pvTie0.push_back(new CCharacterData(11, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
						break;
					}
				}
			}
			else
			{
				/*
				pvTie0.push_back(new CCharacterData(1, int(rand() % 360), 305, float(rand() % 50 - 300), FALSE, 2, stage, int(rand() % 2), FALSE));
				pvTie0.push_back(new CCharacterData(1, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 2, stage, int(rand() % 2), FALSE));
				pvTie0.push_back(new CCharacterData(1, int(rand() % 360), 305, float(rand() % 50 + 100 - 300), FALSE, 2, stage, int(rand() % 2), FALSE));
				pvTie0.push_back(new CCharacterData(1, int(rand() % 360), 305, float(rand() % 50 + 150 - 300), FALSE, 2, stage, int(rand() % 2), FALSE));
				pvTie0.push_back(new CCharacterData(1, int(rand() % 360), 305, float(rand() % 50 + 200 - 300), FALSE, 2, stage, int(rand() % 2), FALSE));
				pvTie0.push_back(new CCharacterData(1, int(rand() % 360), 305, float(rand() % 50 + 250 - 300), FALSE, 2, stage, int(rand() % 2), FALSE));
				*/
				pvTie0.push_back(new CCharacterData(int(rand() % 9), int(rand() % 360), 305, float(rand() % 50 - 300), FALSE, 2, stage, int(rand() % 2), FALSE));
				pvTie0.push_back(new CCharacterData(int(rand() % 9), int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 2, stage, int(rand() % 2), FALSE));
				pvTie0.push_back(new CCharacterData(int(rand() % 9), int(rand() % 360), 305, float(rand() % 50 + 100 - 300), FALSE, 2, stage, int(rand() % 2), FALSE));
				pvTie0.push_back(new CCharacterData(int(rand() % 9), int(rand() % 360), 305, float(rand() % 50 + 150 - 300), FALSE, 2, stage, int(rand() % 2), FALSE));
				pvTie0.push_back(new CCharacterData(int(rand() % 9), int(rand() % 360), 305, float(rand() % 50 + 200 - 300), FALSE, 2, stage, int(rand() % 2), FALSE));
				pvTie0.push_back(new CCharacterData(int(rand() % 9), int(rand() % 360), 305, float(rand() % 50 + 250 - 300), FALSE, 2, stage, int(rand() % 2), FALSE));

				//if (rand() % 6 == 3)
				//{
				switch (rand() % 7)
				{
				case 0://10. 라이프 아이템 생성
					if (m_xwing.m_life <3)
						pvTie0.push_back(new CCharacterData(10, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
					break;

				case 1://11. 레이저 아이템 생성
					if (m_xwing.m_laserable <6)
						pvTie0.push_back(new CCharacterData(11, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
					break;

				case 2://12. 스피드 아이템 생성
					pvTie0.push_back(new CCharacterData(12, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
					break;

				case 3://13. 파워 아이템 생성
					if ((50 + stage * 7) - 50 * m_xwing.m_power >0)
						pvTie0.push_back(new CCharacterData(13, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
					break;

				case 4://14. HP 아이템 생성
					pvTie0.push_back(new CCharacterData(14, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
					break;

				case 5://12. 스피드 아이템 생성
					pvTie0.push_back(new CCharacterData(12, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
					break;

				default://11. 레이저 아이템 생성
					if (m_xwing.m_laserable <6)
						pvTie0.push_back(new CCharacterData(11, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
					break;
				}
				//}
			}

			//////////////////////////////////////////////////////////////////
			std::vector<CCharacterData*>::iterator _FC = pvTie0.begin(); //레이저 종류 카운트용 반복자
			std::vector<CCharacterData*>::iterator _LC = pvTie0.end();
			for (; _FC != _LC; _FC++)
			{
				if ((*_FC)->laserable == TRUE)
				{
					lasercnt = lasercnt + (*_FC)->laserability;
				}
				if ((*_FC)->ionable == TRUE)
				{
					ioncnt = ioncnt + (*_FC)->ionability;
				}
				if ((*_FC)->missileable == TRUE)
				{
					missilecnt = missilecnt + (*_FC)->missileability;
				}
			}
			/////////////////////////////////////////////////////////////////////////			
		}
	}
	if (m_xwing.dead != TRUE)
	{

		//GMAIN->m_pSound.Volume(SND_XWENGLP, 1000,false);
		//m_pSndEngine->Play();
		//m_pSndEngine->SetVolume(-1000);
	}

	//m_pMp[1]->Reset();//m_pSndGameMenu->Reset();
	//m_pMp[1]->Stop();//m_pSndGameMenu->Stop();
	//m_pMp[0]->Play();//m_pSndGameBg->Play(0, DSBPLAY_LOOPING);
	//m_pSound.Stop(10);
	//m_pSound.Play(13, true);

	//this->BoomSprite();
	this->InputMove();

	GMAIN->m_alphatime = timeGetTime() - GMAIN->m_gamebegin;

	this->CharacterMove();


	////////////////////////////////////////////////////////////////////////////////
	std::vector<CCharacterData*>::iterator _FT = pvTie0.begin();
	std::vector<CCharacterData*>::iterator _LT = pvTie0.end();

	char* attackname = "";


	for (; _FT != _LT; ++_FT)
	{
		if (*_FT == 0)
			continue;

		if ((*_FT)->xval <= m_xwing.xval + 0.3 && (*_FT)->xval >= m_xwing.xval - 0.3 && (*_FT)->yval >= 0 && (*_FT)->yval <= 600)
		{
			if (pvLaser1.size() <= ((*_FT)->laserability + 1)*pvTie0.size())
			{
				m_dTimeEnd = timeGetTime();

				if ((m_dTimeEnd - m_dTimeBegin)>100)
				{
					if ((*_FT)->dead == FALSE)
					{
						////////////////////////////////////////////////////////////////////////////////////////
						if ((*_FT)->laserable == TRUE && (*_FT)->ionable == TRUE && (*_FT)->missileable == TRUE)
						{
							switch (rand() % 3)
							{
							case 0:
								if (lasercnt != curlasercnt)
								{
									attackname = "Laser";
									pvLaser1.push_back(new CLaserData(attackname, (*_FT)->xval + 15, (*_FT)->yval + 30));
									curlasercnt++;
								}
								break;

							case 1:
								if (ioncnt != curioncnt)
								{
									attackname = "Ion";
									pvLaser1.push_back(new CLaserData(attackname, (*_FT)->xval + 15, (*_FT)->yval + 30));
									curioncnt++;
								}
								break;

							case 2:
								if (missilecnt != curmissilecnt)
								{
									attackname = "Missile";
									pvLaser1.push_back(new CLaserData(attackname, (*_FT)->xval + 15, (*_FT)->yval + 30));
									curmissilecnt++;
								}
								break;

							default:
								attackname = "Missile";
								break;
							}
						}
						else if ((*_FT)->laserable == TRUE && (*_FT)->ionable == TRUE)
						{
							switch (rand() % 2)
							{
							case 0:
								if (lasercnt != curlasercnt)
								{
									attackname = "Laser";
									pvLaser1.push_back(new CLaserData(attackname, (*_FT)->xval + 15, (*_FT)->yval + 30));
									curlasercnt++;
								}
								break;

							case 1:
								if (ioncnt != curioncnt)
								{
									attackname = "Ion";
									pvLaser1.push_back(new CLaserData(attackname, (*_FT)->xval + 15, (*_FT)->yval + 30));
									curioncnt++;
								}
								break;
							}
						}
						else if ((*_FT)->laserable == TRUE && (*_FT)->missileable == TRUE)
						{
							switch (rand() % 2)
							{
							case 0:
								if (lasercnt != curlasercnt)
								{
									attackname = "Laser";
									pvLaser1.push_back(new CLaserData(attackname, (*_FT)->xval + 15, (*_FT)->yval + 30));
									curlasercnt++;
								}
								break;

							case 1:
								if (missilecnt != curmissilecnt)
								{
									attackname = "Missile";
									pvLaser1.push_back(new CLaserData(attackname, (*_FT)->xval + 15, (*_FT)->yval + 30));
									curmissilecnt++;
								}
								break;
							}
						}
						else if ((*_FT)->ionable == TRUE && (*_FT)->missileable == TRUE)
						{
							switch (rand() % 2)
							{
							case 0:
								if (ioncnt != curioncnt)
								{
									attackname = "Ion";
									pvLaser1.push_back(new CLaserData(attackname, (*_FT)->xval + 15, (*_FT)->yval + 30));
									curioncnt++;
								}
								break;

							case 1:
								if (missilecnt != curmissilecnt)
								{
									attackname = "Missile";
									pvLaser1.push_back(new CLaserData(attackname, (*_FT)->xval + 15, (*_FT)->yval + 30));
									curmissilecnt++;
								}
								break;
							}

						}
						else if ((*_FT)->missileable == TRUE)
						{
							if (missilecnt != curmissilecnt)
							{
								attackname = "Missile";
								pvLaser1.push_back(new CLaserData(attackname, (*_FT)->xval + 15, (*_FT)->yval + 30));
								curmissilecnt++;
							}
						}
						else if ((*_FT)->laserable == TRUE)
						{
							if (lasercnt != curlasercnt)
							{
								attackname = "Laser";
								pvLaser1.push_back(new CLaserData(attackname, (*_FT)->xval + 15, (*_FT)->yval + 30));
								curlasercnt++;
							}
						}
						else {}
						////////////////////////////////////////////////////////////////////////////////////////

						//적 총알 사운드
						if (attackname == "Laser")
						{
							switch (rand() % 1)
							{
							case 0:
								GMAIN->m_pSound.Play(SND_TIELSR1, true);
								//m_pSndTLsr1->Reset();
								//m_pSndTLsr1->Play();
								//m_pSndTLsr1->SetVolume(0);
								break;
							case 1:
								GMAIN->m_pSound.Play(SND_TIELSR2, true);
								//m_pSndTLsr2->Reset();
								//m_pSndTLsr2->Play();
								//m_pSndTLsr2->SetVolume(0);
								break;
							default:
								GMAIN->m_pSound.Play(SND_TIELSR1, true);
								//m_pSndTLsr1->Reset();
								//m_pSndTLsr1->Play();
								//m_pSndTLsr1->SetVolume(0);
								break;
							}
						}
						else if (attackname == "Missile")
						{
							GMAIN->m_pSound.Play(SND_MISSILE, true);
							//m_pSndMissl->Reset();
							//m_pSndMissl->Play();
							//m_pSndMissl->SetVolume(0);
						}
						else if (attackname == "Ion")
						{
							GMAIN->m_pSound.Play(SND_IONCANNON, true);
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
						//사운드 끝

						/////////////////////////////////////////////////////////////////////////

						std::vector<CLaserData*>::iterator _FL = pvLaser1.begin();
						std::vector<CLaserData*>::iterator _LL = pvLaser1.begin();

						for (; _FL != _LL; _FL++)
						{
							if ((*_FL)->name == "Laser")
							{
								curlasercnt++;
							}
							else if ((*_FL)->name == "Ion")
							{
								curioncnt++;
							}
							else if ((*_FL)->name == "Missile")
							{
								curmissilecnt++;
							}
							else
							{
							}
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
}
void CGameMulti::Render(LPDIRECT3DDEVICE9& dxdevice, LPD3DXSPRITE& dxsprite)
{
	dxsprite->Begin(D3DXSPRITE_ALPHABLEND);

	vcBar = D3DXVECTOR3(650, 0, 0);							//상태바위치
	m_xwing.AssignVal(m_xwing.xval, m_xwing.yval);				//주인공위치

	dxsprite->Draw(GMAIN->m_pGameTex[23].m_pTex, (&GMAIN->rc), NULL, &vcPos1, D3DXCOLOR(1, 1, 1, 1.f)); //배경
	dxsprite->Draw(GMAIN->m_pGameTex[24].m_pTex, (&GMAIN->rc), NULL, &vcPos2, D3DXCOLOR(1, 1, 1, 1.f)); //배경
																										//D3DXCOLOR 마지막 매개변수는 알파값을 의미하고 이 함수의 각 매개변수는 RGBA값으로서 각각 0f~1f의 값을 가진다.
	if (vcPos1.y >= 600) { vcPos1.y = -600; }
	else { vcPos1.y += GMAIN->m_movingdist / 20; }
	if (vcPos2.y >= 600) { vcPos2.y = -600; }
	else { vcPos2.y += GMAIN->m_movingdist / 20; }


	if (m_xwing.dead == TRUE)
	{
		pvLaser1.clear();
		curlasercnt = 0;
		curioncnt = 0;
		curmissilecnt = 0;
	}

	if (m_xwing.dead != TRUE)
	{
		dxsprite->Draw(GMAIN->m_pGameTex[1].m_pTex, &(GMAIN->rc), NULL, &m_xwing.vcPosC, D3DXCOLOR(1, 1, 1, 1.f)); //주인공캐릭터
	}

	/////////////////////////////////////////////////////////////////////////////
	//주인공 총알

	std::vector<CLaserData*>::iterator _F = pvLaser0.begin();
	std::vector<CLaserData*>::iterator _L = pvLaser0.end();

	for (; _F != _L; ++_F)
	{
		if (*_F == 0)
			continue;

		(*_F)->vcPosC = D3DXVECTOR3((*_F)->laserxval, (*_F)->laseryval, 0);
		dxsprite->Draw(GMAIN->m_pGameTex[2].m_pTex, &(GMAIN->rc), NULL, &((*_F)->vcPosC), D3DXCOLOR(1, 1, 1, 1.f));

		if ((*_F)->laseryval <= -30 || (*_F)->laserhit == TRUE)
		{
			if (*_F != 0)
			{
				delete (*_F);
				*_F = 0;
			}
		}

		else
		{
			(*_F)->laseryval = (*_F)->laseryval = (*_F)->laseryval - GMAIN->m_movingdist;
		}
	}

	_F = pvLaser0.begin();
	while (_F != pvLaser0.end())
	{
		if (*_F == 0)
			_F = pvLaser0.erase(_F);
		else
			_F++;
	}
	/////////////////////////////////////////////////////////////////////////////
	//적캐릭터
	std::vector<CCharacterData*>::iterator _FT = pvTie0.begin();
	std::vector<CCharacterData*>::iterator _LT = pvTie0.end();

	for (; _FT != _LT; ++_FT)
	{
		if (*_FT == 0)
			continue;

		(*_FT)->vcPosC = D3DXVECTOR3((*_FT)->xval, (*_FT)->yval, 0);
		if ((*_FT)->dead != TRUE)
		{
			(*_FT)->CharacterDraw((*_FT)->name, GMAIN->rc, _FT);
		}

	}

	_FT = pvTie0.begin();
	while (_FT != pvTie0.end())
	{
		if (*_FT == 0)
			_FT = pvTie0.erase(_FT);
		else
			_FT++;
	}
	/////////////////////////////////////////////////////////////////////////////
	//적 총알

	std::vector<CLaserData*>::iterator _F1 = pvLaser1.begin();
	std::vector<CLaserData*>::iterator _L1 = pvLaser1.end();

	for (; _F1 != _L1; ++_F1)
	{
		if (*_F1 == 0)
			continue;

		(*_F1)->vcPosC = D3DXVECTOR3((*_F1)->laserxval, (*_F1)->laseryval, 0);

		(*_F1)->LaserDraw((*_F1)->name, GMAIN->rc, _F1);

		if ((*_F1)->laseryval >= 630 || (*_F1)->laserhit == TRUE)
		{
			if (*_F1 != 0)
			{
				//////////////////////////////////////////////////////////////////
				if ((*_F1)->name == "Laser")
				{
					curlasercnt--;
				}
				else if ((*_F1)->name == "Ion")
				{
					curioncnt--;
				}
				else if ((*_F1)->name == "Missile")
				{
					curmissilecnt--;
				}
				else
				{
				}
				//////////////////////////////////////////////////////////////////
				delete (*_F1);
				*_F1 = 0;
			}
		}
		else
		{
			(*_F1)->laseryval = (*_F1)->laseryval = (*_F1)->laseryval + GMAIN->m_movingdist / (*_F1)->speed;
		}
	}

	_F1 = pvLaser1.begin();
	while (_F1 != pvLaser1.end())
	{
		if (*_F1 == 0)
			_F1 = pvLaser1.erase(_F1);
		else
			_F1++;
	}
	/////////////////////////////////////////////////////////////////////////////

	LPDIRECT3DTEXTURE9 pTex = (LPDIRECT3DTEXTURE9)(GMAIN->m_pTx->GetTexture());
	////////////////////////////////////////////////////////////////////////////////
	//폭발 : 주인공이 쏜 총알 적에게 충돌 되었는가?
	ColCheck2();  // 주인공 -> 적 충돌체크
	_FT = pvTie0.begin();
	_LT = pvTie0.end();

	for (; _FT != _LT; ++_FT)
	{
		if (*_FT == 0)
			continue;

		if ((*_FT)->laserhit == TRUE)
		{
			if ((*_FT)->name == "Boss")
			{
				boom.x = (*_FT)->vcPosC.x - 15 + 29;
				boom.y = (*_FT)->vcPosC.y - 13 + 61;
			}
			else
			{
				boom.x = (*_FT)->vcPosC.x - 15;
				boom.y = (*_FT)->vcPosC.y - 13;
			}
			//////////////////////////////////////////////////////////////////////
			(*_FT)->m_dTimeEnd = timeGetTime();

			if (((*_FT)->m_dTimeEnd - (*_FT)->m_dTimeBegin)>50)
			{
				m_ImgRc2.left += 70;

				if (m_ImgRc2.left + 70 >= 1120)
				{
					m_ImgRc2.left = 0;
					(*_FT)->laserhit = FALSE;
				}

				m_ImgRc2.right = m_ImgRc2.left + 70;
				(*_FT)->m_dTimeBegin = (*_FT)->m_dTimeEnd;

			}
			//////////////////////////////////////////////////////////////////////
			if (*_FT != 0)
			{
				dxsprite->Draw(pTex, &m_ImgRc2, NULL, &boom, D3DXCOLOR(1, 1, 1, 1.F)); //&m_vcPosImg2
			}
			//////////////////////////////////////////////////////////////////////
		}
		if ((*_FT)->dead == TRUE)
		{
			(*_FT)->m_dTimeEnd = timeGetTime();
			if (((*_FT)->m_dTimeEnd - (*_FT)->m_dTimeBegin)>50)
			{
				m_ImgRc2.left += 70;

				if (m_ImgRc2.left + 70 >= 1120)
				{
					m_ImgRc2.left = 0;
					if (*_FT != 0)
					{
						if ((*_FT)->laserable == TRUE) { lasercnt -= (*_FT)->laserability; }
						if ((*_FT)->ionable == TRUE) { ioncnt -= (*_FT)->ionability; }
						if ((*_FT)->missileable == TRUE) { missilecnt -= (*_FT)->missileability; }

						if ((*_FT)->name == "Life" || (*_FT)->name == "Power" || (*_FT)->name == "Speed" || (*_FT)->name == "Laser" || (*_FT)->name == "Hp")
						{
							if ((*_FT)->name == "Life")
							{
								m_xwing.m_life++;
								GMAIN->m_pSound.Play(SND_BONUS, true);
								GMAIN->m_pSound.Play(SND_R2_D2, true);
								//m_pSndBonus->Reset();
								//m_pSndBonus->Play();
								//m_pSndBonus->SetVolume(0);
								//m_pSndR2->Reset();
								//m_pSndR2->Play();
								//m_pSndR2->SetVolume(5000);
							}
							else if ((*_FT)->name == "Laser")
							{
								m_xwing.m_laserable++;
								GMAIN->m_pSound.Play(SND_BONUS, true);
								GMAIN->m_pSound.Play(SND_WONTFAIL, true);
								//m_pSndBonus->Reset();
								//m_pSndBonus->Play();
								//m_pSndBonus->SetVolume(0);
								//m_pSndLaserup->Reset();
								//m_pSndLaserup->Play();
								//m_pSndLaserup->SetVolume(5000);
							}
							else if ((*_FT)->name == "Power")
							{
								m_xwing.m_power++;
								GMAIN->m_pSound.Play(SND_BONUS, true);
								GMAIN->m_pSound.Play(SND_USEFORCE, true);
								//m_pSndBonus->Reset();
								//m_pSndBonus->Play();
								//m_pSndBonus->SetVolume(0);
								//m_pSndPowerup->Reset();
								//m_pSndPowerup->Play();
								//m_pSndPowerup->SetVolume(5000);
							}
							else if ((*_FT)->name == "Hp")
							{
								m_xwing.hp = 100;
								GMAIN->m_pSound.Play(SND_BONUS, true);
								GMAIN->m_pSound.Play(SND_WITHYOU, true);
								//m_pSndBonus->Reset();
								//m_pSndBonus->Play();
								//m_pSndBonus->SetVolume(0);
								//m_pSndHpup->Reset();
								//m_pSndHpup->Play();
								//m_pSndHpup->SetVolume(5000);
							}
							else if ((*_FT)->name == "Speed")
							{
								m_xwing.m_speed++;
								GMAIN->m_pSound.Play(SND_BONUS, true);
								GMAIN->m_pSound.Play(SND_GOODINU, true);
								//m_pSndBonus->Reset();
								//m_pSndBonus->Play();
								//m_pSndBonus->SetVolume(0);
								//m_pSndSpeedup->Reset();
								//m_pSndSpeedup->Play();
								//m_pSndSpeedup->SetVolume(5000);
							}
							else {}

							delete (*_FT);
							*_FT = 0;
						}
						else
						{
							delete (*_FT);
							*_FT = 0;
							score = score + 10;
							m_xwing.m_killcount++;
						}
						if (score != 0 && score % 1000 == 0)
						{
							m_xwing.m_life++;
							GMAIN->m_pSound.Play(SND_BONUS, true);
							GMAIN->m_pSound.Play(SND_R2_D2, true);
							//m_pSndBonus->Reset();
							//m_pSndBonus->Play();
							//m_pSndBonus->SetVolume(0);
							//m_pSndR2->Reset();
							//m_pSndR2->Play();
							//m_pSndR2->SetVolume(5000);
						}

						if ((m_xwing.m_killcount != 0) && (m_xwing.m_killcount % 6 == 0))
						{
							stage++;
							GMAIN->m_pSound.Play(SND_R2_D2, true);
						}
					}
				}
				m_ImgRc2.right = m_ImgRc2.left + 70;

				if (*_FT != 0)
				{
					(*_FT)->m_dTimeBegin = (*_FT)->m_dTimeEnd;
				}
			}
			//////////////////////////////////////////////////////////////////////
			if (*_FT != 0)
			{
				dxsprite->Draw(pTex, &m_ImgRc2, NULL, &boom, D3DXCOLOR(1, 1, 1, 1.F)); //&m_vcPosImg2
			}
			//////////////////////////////////////////////////////////////////////
		}
	}
	////////////////////////////////////////////////////////////////////////////////
	//폭발 : 적들이 쏜 총알 주인공에게 충돌 되었는가?
	if (m_xwing.dead != TRUE)
	{
		ColCheck();	// 적 -> 주인공 충돌체크
	}
	if (m_xwing.laserhit == TRUE)
	{
		boom.x = m_xwing.vcPosC.x - 8;
		boom.y = m_xwing.vcPosC.y - 7;
		////////////////////////////////////////////////////////////////////////////////
		m_xwing.m_dTimeEnd = timeGetTime();

		if ((m_xwing.m_dTimeEnd - m_xwing.m_dTimeBegin)>100)
		{
			m_ImgRc2.left += 70;

			if (m_ImgRc2.left + 70 >= 1120)
			{
				m_ImgRc2.left = 0;
				m_xwing.laserhit = FALSE;
			}

			m_ImgRc2.right = m_ImgRc2.left + 70;
			m_xwing.m_dTimeBegin = m_xwing.m_dTimeEnd;

		}
		////////////////////////////////////////////////////////////////////////////////
		dxsprite->Draw(pTex, &m_ImgRc2, NULL, &boom, D3DXCOLOR(1, 1, 1, 1.F)); //&m_vcPosImg2
	}
	if (m_xwing.dead == TRUE)
	{
		m_xwing.m_dTimeEnd = timeGetTime();

		if ((m_xwing.m_dTimeEnd - m_xwing.m_dTimeBegin)>100)
		{
			m_ImgRc2.left += 70;

			if (m_ImgRc2.left + 70 >= 1120)
			{
				m_ImgRc2.left = 0;
				//m_nGamePhase=2;
			}
			/////////////////////////////////////////////////////////////////////////////
			if (m_xwing.dead == TRUE && m_xwing.m_life != 0)
			{
				m_xwing.xval = 325;
				m_xwing.yval = 500;
				m_xwing.hp = 100;
				m_xwing.dead = FALSE;
				m_xwing.laserhit = FALSE;
				m_xwing.laserability = 3;
				--m_xwing.m_life;
				pvLaser0.clear();
				pvLaser1.clear();
				curmissilecnt = 0;
				curlasercnt = 0;
				curioncnt = 0;
			}
			else
			{
				//m_pSound.Stop(13);
				//m_pSound.Play(22, true);

				GMAIN->m_nGamePhase = ST_END;
				GMAIN->m_pSound.Play(SND_THE_FORCE, true);
				GMAIN->m_pSound.Stop(SND_PLAYBG);
				GMAIN->m_pSound.Stop(SND_XWENGLP);

				//m_pMp[0]->Reset();//m_pSndGameBg->Reset();
				//m_pMp[0]->Stop();//m_pSndGameBg->Stop();
				//m_pSndGameover->Reset();
				//m_pSndGameover->Play();
				//m_pSndGameover->SetVolume(0);
				//m_pSound.Stop(13);
				//m_pSound.Play(29, true);
			}
			////////////////////////////////////////////////////////////////////////////
			m_ImgRc2.right = m_ImgRc2.left + 70;
			m_xwing.m_dTimeBegin = m_xwing.m_dTimeEnd;
		}
		if (m_xwing.dead != TRUE)
		{
			dxsprite->Draw(pTex, &m_ImgRc2, NULL, &boom, D3DXCOLOR(1, 1, 1, 1.F)); //&m_vcPosImg2
		}
	}
	ColCheck3(); //충돌체크 함수3: 적과 주인공의 충돌 체크
				 ////////////////////////////////////////////////////////////////////////////////
	dxsprite->Draw(GMAIN->m_pGameTex[7].m_pTex, (&GMAIN->rc), NULL, &vcBar, D3DXCOLOR(1, 1, 1, 1.f)); //상태바
	dxsprite->End();
	////////////////////////////////////////////////////////////////////////////////
	GMAIN->m_text.Begin();

	hh = GMAIN->m_alphatime / 3600000;
	mm = GMAIN->m_alphatime % 3600000;
	mm = mm / 60000;
	ss = GMAIN->m_alphatime % 3600000 % 60000 / 1000;


	sprintf(scoreBuf, " %d", score);
	sprintf(killcount, " %d", m_xwing.m_killcount);
	sprintf(enemyBuf, "%d : Enemy", pvTie0.size());
	sprintf(timeBuf, " %d : %d : %d", hh, mm, ss);
	sprintf(ioncntBuf, "%d : Ion", ioncnt);
	sprintf(lasercntBuf, "%d : Laser", lasercnt);
	sprintf(missilecntBuf, "%d : Missile", missilecnt);
	sprintf(curioncntBuf, "%d : Ion Cnt", curioncnt);
	sprintf(curlasercntBuf, "%d : Laser Cnt", curlasercnt);
	sprintf(curmissilecntBuf, "%d : Missile Cnt", curmissilecnt);
	sprintf(vectorsize, "%d : Attack Cnt", pvLaser1.size());
	sprintf(stageBuf, " %d", stage);
	sprintf(enemylevelBuf, " %d", enemylevel);
	sprintf(lifeBuf, " Life : %d", m_xwing.m_life);
	sprintf(phpBuf, " H.P : %d", m_xwing.hp);
	sprintf(playerabilBuf, " Laser : %d", m_xwing.m_laserable);
	sprintf(powerBuf, " Power : %d", m_xwing.m_power);
	sprintf(speedBuf, " Speed : %d", m_xwing.m_speed);

	GMAIN->m_text.Draw("Score", 660, 10);
	GMAIN->m_text.Draw(scoreBuf, 660, 30);

	GMAIN->m_text.Draw("KillCount", 660, 50);
	GMAIN->m_text.Draw(killcount, 660, 70);
	GMAIN->m_text.Draw("FlightTime", 660, 90);
	GMAIN->m_text.Draw(timeBuf, 660, 110);
	//GMAIN->m_text.Draw("------------------",660,150);
	GMAIN->m_text.Draw(enemyBuf, 660, 160);
	GMAIN->m_text.Draw("------------------", 660, 170);
	GMAIN->m_text.Draw(lasercntBuf, 660, 180);
	GMAIN->m_text.Draw(ioncntBuf, 660, 200);
	GMAIN->m_text.Draw(missilecntBuf, 660, 220);
	GMAIN->m_text.Draw("------------------", 660, 230);
	GMAIN->m_text.Draw(curlasercntBuf, 660, 240);
	GMAIN->m_text.Draw(curioncntBuf, 660, 260);
	GMAIN->m_text.Draw(curmissilecntBuf, 660, 280);
	GMAIN->m_text.Draw("------------------", 660, 290);
	GMAIN->m_text.Draw(vectorsize, 660, 300);
	//GMAIN->m_text.Draw("------------------",660,310);

	/////////////////////////////////////////////////////////////////////////////
	//FPS 화면에 출력
	sprintf(fpsBuf, "FPS: %4.1f", GMAIN->m_fFps);
	GMAIN->m_text.Draw(fpsBuf, 660, 340);
	/////////////////////////////////////////////////////////////////////////////

	GMAIN->m_text.Draw("Stage", 660, 370);
	GMAIN->m_text.Draw(stageBuf, 660, 390);
	GMAIN->m_text.Draw("Level", 660, 410);
	GMAIN->m_text.Draw(enemylevelBuf, 660, 430);

	GMAIN->m_text.Draw("Player", 660, 480);
	GMAIN->m_text.Draw(lifeBuf, 660, 500);
	GMAIN->m_text.Draw(phpBuf, 660, 520);
	GMAIN->m_text.Draw(playerabilBuf, 660, 540);
	GMAIN->m_text.Draw(powerBuf, 660, 560);
	GMAIN->m_text.Draw(speedBuf, 660, 580);

	GMAIN->m_text.End();
}
void CGameMulti::Destroy()
{
	//주인공 총알 소멸부분
	std::vector<CLaserData*>::iterator _F = pvLaser0.begin();
	std::vector<CLaserData*>::iterator _L = pvLaser0.end();
	for (; _F != _L; ++_F) { if (*_F != 0) delete (*_F); }
	pvLaser0.clear();

	//적 총알 소멸부분
	std::vector<CLaserData*>::iterator _F1 = pvLaser1.begin();
	std::vector<CLaserData*>::iterator _L1 = pvLaser1.end();
	for (; _F1 != _L1; ++_F1) { if (*_F1 != 0) delete (*_F1); }
	pvLaser1.clear();

	//적 케릭터 소멸부분
	std::vector<CCharacterData*>::iterator _FT = pvTie0.begin();
	std::vector<CCharacterData*>::iterator _LT = pvTie0.end();
	for (; _FT != _LT; ++_FT) { if (*_FT != 0) delete (*_FT); }
	pvTie0.clear();


	I_GameUser.Release();
	m_Client.Release();
	m_Udp.Release();
}



//충돌체크 함수1: 적총알이 주인공에 맞았는지 체크
INT CGameMulti::ColCheck()
{
	D3DXVECTOR3	TempPos;
	INT bColl = 0;

	std::vector<CLaserData*>::iterator _F = pvLaser1.begin();
	std::vector<CLaserData*>::iterator _L = pvLaser1.end();

	for (; _F != _L; ++_F)
	{
		TempPos = D3DXVECTOR3((*_F)->laserxval, (*_F)->laseryval, 0);
		//TempPos.x=(*_F)->laserxval;
		//TempPos.y=(*_F)->laseryval;
		//TempPos.z=0;

		RECT rcCol1;
		RECT rcCol2;
		RECT rcColSound;

		if ((*_F)->name == "Missile")
		{
			SetRect(&rcCol1, INT(TempPos.x + 4)
				, INT(TempPos.y)
				, INT(TempPos.x + 4) + 5	//m_pGameTex[4].GetImageWidth()
				, INT(TempPos.y) + 30	//m_pGameTex[4].GetImageHeight()
				);

			SetRect(&rcCol2, INT(m_xwing.vcPosC.x + 22)
				, INT(m_xwing.vcPosC.y)
				, INT(m_xwing.vcPosC.x + 22) + 11	//m_pGameTex[1].GetImageWidth()
				, INT(m_xwing.vcPosC.y) + 56	//m_pGameTex[1].GetImageHeight()
				);

			SetRect(&rcColSound, INT(m_xwing.vcPosC.x - 112)
				, INT(m_xwing.vcPosC.y)
				, INT(m_xwing.vcPosC.x - 112) + 275	//m_pGameTex[1].GetImageWidth()
				, INT(m_xwing.vcPosC.y + 56)			//+ m_pGameTex[1].GetImageHeight()
				);
		}
		else
		{
			SetRect(&rcCol1, INT(TempPos.x)
				, INT(TempPos.y)
				, INT(TempPos.x) + GMAIN->m_pGameTex[4].GetImageWidth()
				, INT(TempPos.y) + GMAIN->m_pGameTex[4].GetImageHeight()
				);

			SetRect(&rcCol2, INT(m_xwing.vcPosC.x + 22)
				, INT(m_xwing.vcPosC.y)
				, INT(m_xwing.vcPosC.x + 22) + 11		//m_pGameTex[1].GetImageWidth()
				, INT(m_xwing.vcPosC.y) + 56		//m_pGameTex[1].GetImageHeight()
				);
			SetRect(&rcColSound, INT(m_xwing.vcPosC.x - 112)
				, INT(m_xwing.vcPosC.y)
				, INT(m_xwing.vcPosC.x - 112) + 275	//m_pGameTex[1].GetImageWidth()
				, INT(m_xwing.vcPosC.y + 56)			//+ m_pGameTex[1].GetImageHeight()
				);
		}
		///////////////////////////////////////////////////////////////////////

		if ((*_F)->laserhit == TRUE)
			continue;

		if (rcCol1.left <= rcCol2.right &&
			rcCol1.right >= rcCol2.left  &&

			rcCol1.top <= rcCol2.bottom &&
			rcCol1.bottom >= rcCol2.top)
		{
			bColl = 1;
			//SetWindowText(m_hWnd, "Collision");

			//폭발 사운드
			GMAIN->m_pSound.Play(SND_EXPLOSION, true);
			//m_pSndExplo->Reset();
			//m_pSndExplo->Play();
			//m_pSndExplo->SetVolume(0);
			//사운드 끝


			m_xwing.hp = m_xwing.hp - (*_F)->damage; //주인공 캐릭터 체력 - (*_F)->damage
			if (m_xwing.hp <= 0)
			{
				m_xwing.dead = TRUE;

				//주인공이 죽으면 모든 적을 화면 상단으로 이동

				std::vector<CCharacterData*>::iterator _FT = pvTie0.begin();
				std::vector<CCharacterData*>::iterator _LT = pvTie0.end();

				for (; _FT != _LT; ++_FT)
				{
					if (*_FT == 0)
						continue;

					(*_FT)->yval = (*_FT)->yval - 600;
				}
			}
			(*_F)->laserhit = TRUE;
			m_xwing.laserhit = TRUE;
		}

		else if (rcCol1.left <= rcColSound.right &&
			rcCol1.right >= rcColSound.left  &&

			rcCol1.top <= rcColSound.bottom &&
			rcCol1.bottom >= rcColSound.top && (*_F)->flybysound == FALSE)
		{
			//SetWindowText(m_hWnd, "Collision");

			if ((*_F)->name == "Laser")
			{
				GMAIN->m_pSound.Play(SND_FIREBY, true);
				//m_pSndLaserby->Reset();
				//m_pSndLaserby->Play();
				//m_pSndLaserby->SetVolume(0);
			}
			else if ((*_F)->name == "Ion")
			{
				GMAIN->m_pSound.Play(SND_FIREBY, true);
				//m_pSndLaserby->Reset();
				//m_pSndLaserby->Play();
				//m_pSndLaserby->SetVolume(0);
			}
			else if ((*_F)->name == "Missile")
			{
				GMAIN->m_pSound.Play(SND_MISSLFLY, true);
				//m_pSndMisslby->Reset();
				//m_pSndMisslby->Play();
				//m_pSndMisslby->SetVolume(0);
			}
			else {}
			(*_F)->flybysound = TRUE;
		}

		if (0 == bColl)
		{
			//SetWindowText(m_hWnd, "Press Left, Right, Up, Down Key!");
			(*_F)->vcPosC = TempPos;
		}
	}
	return bColl;
}

//충돌체크 함수2: 주인공총알이 적에 맞았는지 체크
INT CGameMulti::ColCheck2()
{
	D3DXVECTOR3	TempPos;
	INT bColl = 0;

	std::vector<CLaserData*>::iterator _F = pvLaser0.begin();
	std::vector<CLaserData*>::iterator _L = pvLaser0.end();

	for (; _F != _L; ++_F)
	{
		TempPos = D3DXVECTOR3((*_F)->laserxval, (*_F)->laseryval, 0);

		RECT rcCol1;
		RECT rcCol2;

		std::vector<CCharacterData*>::iterator _FT = pvTie0.begin();
		std::vector<CCharacterData*>::iterator _LT = pvTie0.end();

		for (; _FT != _LT; ++_FT)
		{
			if ((*_FT)->dead == TRUE)
				continue;

			if ((*_FT)->name == "Boss")
			{
				SetRect(&rcCol1, INT(TempPos.x)
					, INT(TempPos.y)
					, INT(TempPos.x) + GMAIN->m_pGameTex[2].GetImageWidth()
					, INT(TempPos.y) + GMAIN->m_pGameTex[2].GetImageHeight()
					);

				SetRect(&rcCol2, INT((*_FT)->vcPosC.x + 29)
					, INT((*_FT)->vcPosC.y + 61)
					, INT((*_FT)->vcPosC.x + 29) + 41	//m_pGameTex[3].GetImageWidth()
					, INT((*_FT)->vcPosC.y + 61) + 44	//m_pGameTex[3].GetImageHeight()
					);
			}
			else
			{
				SetRect(&rcCol1, INT(TempPos.x)
					, INT(TempPos.y)
					, INT(TempPos.x) + GMAIN->m_pGameTex[2].GetImageWidth()
					, INT(TempPos.y) + GMAIN->m_pGameTex[2].GetImageHeight()
					);

				SetRect(&rcCol2, INT((*_FT)->vcPosC.x)
					, INT((*_FT)->vcPosC.y)
					, INT((*_FT)->vcPosC.x) + GMAIN->m_pGameTex[3].GetImageWidth()
					, INT((*_FT)->vcPosC.y) + GMAIN->m_pGameTex[3].GetImageHeight()
					);
			}


			if (rcCol1.left <= rcCol2.right &&
				rcCol1.right >= rcCol2.left  &&

				rcCol1.top <= rcCol2.bottom &&
				rcCol1.bottom >= rcCol2.top)
			{
				bColl = 1;
				//SetWindowText(m_hWnd, "Collision");

				//폭발 사운드
				GMAIN->m_pSound.Play(SND_EXPLOSION, true);
				//m_pSndExplo->Reset();
				//m_pSndExplo->Play();
				//m_pSndExplo->SetVolume(0);
				//사운드 끝

				(*_FT)->hp = (*_FT)->hp - 50 * m_xwing.m_power;		//적케릭터 체력 -50

				if ((*_FT)->hp <= 0)				//HP가 0일경우 적캐릭 사망
				{
					(*_FT)->dead = TRUE;
				}
				else
				{
				}
				(*_F)->laserhit = TRUE;
				(*_FT)->laserhit = TRUE;

			}


			if (0 == bColl)
			{
				//SetWindowText(m_hWnd, "Press Left, Right, Up, Down Key!");
				(*_F)->vcPosC = TempPos;
			}
		}
	}

	return bColl;
}

//충돌체크 함수3: 적과 주인공의 충돌 체크
INT CGameMulti::ColCheck3()
{
	D3DXVECTOR3	TempPos;
	INT bColl = 0;

	std::vector<CCharacterData*>::iterator _FT = pvTie0.begin();
	std::vector<CCharacterData*>::iterator _LT = pvTie0.end();


	for (; _FT != _LT; ++_FT)
	{
		if (*_FT == 0)
			continue;

		TempPos = D3DXVECTOR3((*_FT)->xval, (*_FT)->yval, 0);

		RECT rcCol1;
		RECT rcCol2;
		RECT rcCol3;	//보스 충돌 체크용

		SetRect(&rcCol1, INT(TempPos.x), INT(TempPos.y)
			, INT(TempPos.x) + GMAIN->m_pGameTex[3].GetImageWidth()
			, INT(TempPos.y) + GMAIN->m_pGameTex[3].GetImageHeight());

		SetRect(&rcCol2, INT(m_xwing.vcPosC.x - 112), INT(m_xwing.vcPosC.y - 112)
			, INT(m_xwing.vcPosC.x - 112) + 275
			, INT(m_xwing.vcPosC.y) + 112);

		SetRect(&rcCol3, INT(m_xwing.vcPosC.x + 22)
			, INT(m_xwing.vcPosC.y)
			, INT(m_xwing.vcPosC.x + 22) + 11//m_pGameTex[1].GetImageWidth()
			, INT(m_xwing.vcPosC.y) + 56//m_pGameTex[1].GetImageHeight()
			);

		if (rcCol1.left <= rcCol2.right &&
			rcCol1.right >= rcCol2.left  &&

			rcCol1.top <= rcCol2.bottom &&
			rcCol1.bottom >= rcCol2.top && (*_FT)->flybysound == FALSE)
		{
			bColl = 1;
			//SetWindowText(m_hWnd, "Collision");
			if ((*_FT)->name != "Life" || (*_FT)->name != "Power" || (*_FT)->name != "Speed" || (*_FT)->name != "Laser" || (*_FT)->name != "Hp")
			{
				//적 지나가는 사운드

				GMAIN->m_pSound.Play(SND_TIE, true);
				//m_pSndTie->Play();
				//m_pSndTie->SetVolume(0);
				//사운드 끝
				if ((*_FT)->name == "Boss"&&rand() % 2 == 0)
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

					switch (rand() % 7)
					{
					case 0:
						GMAIN->m_pSound.Play(SND_DARK_SIDE, true);

						//m_pSndBossline1->Reset();
						//m_pSndBossline1->Play();
						//m_pSndBossline1->SetVolume(5000);
						break;
					case 1:
						GMAIN->m_pSound.Play(SND_DARK_SIDE2_Y, true);

						//m_pSndBossline2->Reset();
						//m_pSndBossline2->Play();
						//m_pSndBossline2->SetVolume(5000);
						break;
					case 2:
						GMAIN->m_pSound.Play(SND_DONT_UNDERESTIMATE, true);

						//m_pSndBossline3->Reset();
						//m_pSndBossline3->Play();
						//m_pSndBossline3->SetVolume(5000);
						break;
					case 3:
						GMAIN->m_pSound.Play(SND_DONTMAKE, true);

						//m_pSndBossline4->Reset();
						//m_pSndBossline4->Play();
						//m_pSndBossline4->SetVolume(5000);
						break;
					case 4:
						GMAIN->m_pSound.Play(SND_EXPECTING_YOU, true);

						//m_pSndBossline5->Reset();
						//m_pSndBossline5->Play();
						//m_pSndBossline5->SetVolume(5000);
						break;
					case 5:
						GMAIN->m_pSound.Play(SND_MUSTOBEY, true);

						//m_pSndBossline6->Reset();
						//m_pSndBossline6->Play();
						//m_pSndBossline6->SetVolume(5000);
						break;
					case 6:
						GMAIN->m_pSound.Play(SND_YOUR_FATHER, true);
						//m_pSndBossline7->Reset();
						//m_pSndBossline7->Play();
						//m_pSndBossline7->SetVolume(5000);
						break;
					default:
						GMAIN->m_pSound.Play(SND_DONT_UNDERESTIMATE, true);
						//m_pSndBossline1->Reset();
						//m_pSndBossline1->Play();
						//m_pSndBossline1->SetVolume(5000);
						break;
					}
				}
			}
			else {}

			(*_FT)->flybysound = TRUE;
		}

		if (0 == bColl)
		{
			//SetWindowText(m_hWnd, "Press Left, Right, Up, Down Key!");
			(*_FT)->vcPosC = TempPos;
		}

	}
	return bColl;
}

void	CGameMulti::InputMove()
{
	GMAIN->m_pInput->FrameMove();

	////////////////////////////////////////////////////////////////////////////////
	//SetWindowText(m_hWnd,"");
	//if(m_pInput->ButtonPress(0))
	//{
	//SetWindowText(m_hWnd, "왼쪽버튼을 눌렀습니다");
	//}
	//if(m_pInput->ButtonPress(1))
	//{
	//SetWindowText(m_hWnd, "오른쪽버튼을 눌렀습니다");
	//}
	////////////////////////////////////////////////////////////////////////////////
	if (m_xwing.dead != TRUE)
	{
		if (GMAIN->m_pInput->KeyPress(VK_RIGHT))
		{
			m_xwing.xval = m_xwing.xval + GMAIN->m_movingdist*m_xwing.m_speed;
			//++m_xwing.xval;
			if (m_xwing.xval>593)
			{
				m_xwing.xval = 593;
			}
		}
		if (GMAIN->m_pInput->KeyPress(VK_LEFT))
		{
			m_xwing.xval = m_xwing.xval - GMAIN->m_movingdist*m_xwing.m_speed;
			if (m_xwing.xval<0)
			{
				m_xwing.xval = 0;
			}
		}

		if (GMAIN->m_pInput->KeyPress(VK_UP))
		{
			m_xwing.yval = m_xwing.yval - GMAIN->m_movingdist*m_xwing.m_speed;
			//++m_xwing.xval;
			if (m_xwing.yval<0)
			{
				m_xwing.yval = 0;
			}
		}
		if (GMAIN->m_pInput->KeyPress(VK_DOWN))
		{
			m_xwing.yval = m_xwing.yval + GMAIN->m_movingdist*m_xwing.m_speed;
			if (m_xwing.yval>550)
			{
				m_xwing.yval = 550;
			}
		}
		if (GMAIN->m_pInput->KeyDown(VK_SPACE))
		{
			if (pvLaser0.size() < m_xwing.m_laserable)
			{
				pvLaser0.push_back(new CLaserData(m_xwing.xval, m_xwing.yval, FALSE)); // 주인공 총알 발생

				GMAIN->m_pSound.Play(SND_XWLSR1, true);
				//////////////////////////////////////////////////////////////////////
				//m_pSndXwLsr1->Reset();

				//m_pSndXwLsr1->Play();

				//m_pSndXwLsr1->SetVolume(0);
				//////////////////////////////////////////////////////////////////////
			}
		}
	}
}


void CGameMulti::CharacterMoveBasic(std::vector<CCharacterData*>::iterator* _FT) {



	(**_FT)->xval += sinf((GMAIN->m_alphatime + 290000) * 0.0015f + (**_FT)->position) * GMAIN->m_movingdist;

	////(*_FT)->yval=(*_FT)->yval+movingdist;
	////(*_FT)->yval=+cosf(timeGetTime()	 * 0.001f) * 300.f+300;

	if ((**_FT)->yval >= 600)
	{
		if ((**_FT)->name == "Boss")
		{
			(**_FT)->yval = (**_FT)->yval = -108;
			(**_FT)->flybysound = FALSE;
		}
		else
		{
			if ((**_FT)->name == "Life" || (**_FT)->name == "Power" || (**_FT)->name == "Speed" || (**_FT)->name == "Laser" || (**_FT)->name == "Hp")
			{
				delete (**_FT);
				**_FT = 0;
			}
			else
			{
				(**_FT)->yval = (**_FT)->yval = -44;
				(**_FT)->flybysound = FALSE;
			}
		}
	}
	else
	{
		(**_FT)->yval = (**_FT)->yval + GMAIN->m_movingdist / (**_FT)->speed;
		//(**_FT)->yval = (**_FT)->yval + GMAIN->m_movingdist / 10;
	}
}


void CGameMulti::CharacterMovePath(std::vector<CCharacterData*>::iterator* _FT) {
	//(**_FT)->xval;
	if ((**_FT)->yval >= 600)
	{
		(**_FT)->yval = (**_FT)->yval = -44;
		(**_FT)->flybysound = FALSE;
	}
	else {
		(**_FT)->yval = (**_FT)->yval + GMAIN->m_movingdist;
	}



}

void CGameMulti::CharacterMove()
{

	std::vector<CCharacterData*>::iterator _FT = pvTie0.begin();
	std::vector<CCharacterData*>::iterator _LT = pvTie0.end();

	for (; _FT != _LT; ++_FT)
	{
		if (*_FT == 0)
			continue;

		if ((*_FT)->dead == TRUE)
			continue;

		//if ((*_FT)->movepattern == MV_BASIC)
		//{
		CharacterMoveBasic(&_FT);
		//};
		//if ((*_FT)->movepattern == MV_PATH)
		//{
		//	CharacterMovePath(&_FT);
		//};

	}

}
