#include "_StdAfx.h"

//#��Ƽ ���� �ڵ� ������ ���� ��ũ�� for debugging
#define MULTIIMPLE 1

CGameMulti::~CGameMulti()
{
}

CGameMulti::CGameMulti()
{
	m_bLogin = false;
	m_iSerIndex = 0;

	ioncnt = 0;
	lasercnt = 0;
	missilecnt = 0;
	curioncnt = 0;
	curlasercnt = 0;
	curmissilecnt = 0;


	stage = 1;			//ó�� ���۽� �������� 1
	curstage;
	score = 0;			//����


	vcPos1 = D3DXVECTOR3(-75, 0, 0);		//�����ġ
	vcPos2 = D3DXVECTOR3(-75, -600, 0);	//�����ġ

	boom = D3DXVECTOR3(0, 0, 0);

	SetRect(&m_ImgRc2, 0, 0, 0, 70);		// RECT �ִ� �̹���
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

	

	//this->MultiInit();


	return 0;
}

//#��Ƽ�÷��̰��� init
void CGameMulti::MultiInit() {

	
	if (MessageBox(GHWND, L"IP�Է�?", L"����", MB_OK) == IDOK) {
	// ToDo: IP�Է�ó��.
	}
	
	m_iSerIndex = 0;
	I_DebugStr.Init();
	m_bLogin = true;
	m_Client.Init();
	I_GameUser.Init();
	m_Udp.Init();
}


void CGameMulti::Frame()
{
	/////////////////////////////////////////////////////////////
	if (m_bLogin == false) return;

	
	m_Client.Frame();


	I_GameUser.Frame();
	for (int iObj = 0; iObj < m_UserList.size(); iObj++)
	{
		m_UserList[iObj].Frame();
	}

	/////////////////////////////////////////////////////////////

	//int j = 0;

	if (pvTie0.size() == 0)
	{
		srand((int)time(NULL));
	}

	if (curstage != stage)
	{
		//#�� stage ���۽� ���� ����
		this->NextStageCreate();
	}
	if (m_xwing.dead != TRUE)
	{

		//GMAIN->m_pSound.Volume(SND_XWENGLP, 1000,false);
		//m_pSndEngine->Play();
		//m_pSndEngine->SetVolume(-1000);
	}

	//#���ΰ� Ű���� �Է� ó�� & �̵�
	this->InputMove();

	GMAIN->m_alphatime = timeGetTime() - GMAIN->m_gamebegin;

	//#��ĳ���� �̵� ó��
	this->CharacterMove();

	//#�� �Ѿ� �߻� ó��
	this->EnemyBullet();
}

void CGameMulti::NextStageCreate() {
	if (pvTie0.size() == 0)
	{
		lasercnt = 0;
		ioncnt = 0;
		missilecnt = 0;

		pvLaser0.clear();
		curstage = stage;

		enemylevel = (50 + stage * 7) / 50;

		if (stage != 0 && stage % 10 == 0 && (50 + stage * 7) - 50 * m_xwing.m_power >0)
		{//10�������� ���� �Ŀ������� ����
			pvTie0.push_back(new CCharacterData(13, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
		}
		if (stage != 0 && stage % 5 == 0)
		{

			//pvTie0.push_back(new CCharacterData("Boss",int(rand()%360),305,float(rand()%50-300),FALSE,5,500,0,FALSE));
			pvTie0.push_back(new CCharacterData(9, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 5, stage, int(rand() % 2), FALSE));//���� ����
			pvTie0.push_back(new CCharacterData(int(rand() % 9), int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 2, stage, int(rand() % 2), FALSE));
			pvTie0.push_back(new CCharacterData(int(rand() % 9), int(rand() % 360), 305, float(rand() % 50 + 100 - 300), FALSE, 2, stage, int(rand() % 2), FALSE));
			pvTie0.push_back(new CCharacterData(int(rand() % 9), int(rand() % 360), 305, float(rand() % 50 + 150 - 300), FALSE, 2, stage, int(rand() % 2), FALSE));
			pvTie0.push_back(new CCharacterData(int(rand() % 9), int(rand() % 360), 305, float(rand() % 50 + 200 - 300), FALSE, 2, stage, int(rand() % 2), FALSE));
			pvTie0.push_back(new CCharacterData(int(rand() % 9), int(rand() % 360), 305, float(rand() % 50 + 250 - 300), FALSE, 2, stage, int(rand() % 2), FALSE));

			if (rand() % 6 == 3 && stage % 10 != 0)
			{
				switch (rand() % 7)
				{
				case 0://10. ������ ������ ����
					if (m_xwing.m_life <3)
						pvTie0.push_back(new CCharacterData(10, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
					break;

				case 1://11. ������ ������ ����
					if (m_xwing.m_laserable <6)
						pvTie0.push_back(new CCharacterData(11, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
					break;

				case 2://12. ���ǵ� ������ ����
					pvTie0.push_back(new CCharacterData(12, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
					break;

				case 3://13. �Ŀ� ������ ����
					if ((50 + stage * 7) - 50 * m_xwing.m_power >0)
						pvTie0.push_back(new CCharacterData(13, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
					break;

				case 4://14. HP ������ ����
					pvTie0.push_back(new CCharacterData(14, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
					break;

				case 5://12. ���ǵ� ������ ����
					pvTie0.push_back(new CCharacterData(12, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
					break;

				default://11. ������ ������ ����
					if (m_xwing.m_laserable <6)
						pvTie0.push_back(new CCharacterData(11, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
					break;
				}
			}
		}
		else
		{

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
			case 0://10. ������ ������ ����
				if (m_xwing.m_life <3)
					pvTie0.push_back(new CCharacterData(10, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
				break;

			case 1://11. ������ ������ ����
				if (m_xwing.m_laserable <6)
					pvTie0.push_back(new CCharacterData(11, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
				break;

			case 2://12. ���ǵ� ������ ����
				pvTie0.push_back(new CCharacterData(12, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
				break;

			case 3://13. �Ŀ� ������ ����
				if ((50 + stage * 7) - 50 * m_xwing.m_power >0)
					pvTie0.push_back(new CCharacterData(13, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
				break;

			case 4://14. HP ������ ����
				pvTie0.push_back(new CCharacterData(14, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
				break;

			case 5://12. ���ǵ� ������ ����
				pvTie0.push_back(new CCharacterData(12, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
				break;

			default://11. ������ ������ ����
				if (m_xwing.m_laserable <6)
					pvTie0.push_back(new CCharacterData(11, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
				break;
			}
			//}
		}

		//////////////////////////////////////////////////////////////////
		std::vector<CCharacterData*>::iterator _FC = pvTie0.begin(); //������ ���� ī��Ʈ�� �ݺ���
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
//�� �Ѿ� �߻� ó��
void CGameMulti::EnemyBullet(){
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

						//�� �Ѿ� ����
						if (attackname == "Laser")
						{
							switch (rand() % 1)
							{
							case 0:
								GMAIN->m_pSound.Play(SND_TIELSR1, true);
								break;
							case 1:
								GMAIN->m_pSound.Play(SND_TIELSR2, true);
								break;
							default:
								GMAIN->m_pSound.Play(SND_TIELSR1, true);
								break;
							}
						}
						else if (attackname == "Missile")
						{
							GMAIN->m_pSound.Play(SND_MISSILE, true);
						}
						else if (attackname == "Ion")
						{
							GMAIN->m_pSound.Play(SND_IONCANNON, true);
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
	if (m_bLogin == false) return;

	dxsprite->Begin(D3DXSPRITE_ALPHABLEND);

	vcBar = D3DXVECTOR3(650, 0, 0);							//���¹���ġ
	m_xwing.AssignVal(m_xwing.xval, m_xwing.yval);				//���ΰ���ġ

	dxsprite->Draw(GMAIN->m_pGameTex[23].m_pTex, (&GMAIN->rc), NULL, &vcPos1, D3DXCOLOR(1, 1, 1, 1.f)); //���
	dxsprite->Draw(GMAIN->m_pGameTex[24].m_pTex, (&GMAIN->rc), NULL, &vcPos2, D3DXCOLOR(1, 1, 1, 1.f)); //���
																										//D3DXCOLOR ������ �Ű������� ���İ��� �ǹ��ϰ� �� �Լ��� �� �Ű������� RGBA�����μ� ���� 0f~1f�� ���� ������.
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
		dxsprite->Draw(GMAIN->m_pGameTex[1].m_pTex, &(GMAIN->rc), NULL, &m_xwing.vcPosC, D3DXCOLOR(1, 1, 1, 1.f)); //���ΰ�ĳ����
	}

	/////////////////////////////////////////////////////////////////////////////
	//���ΰ� �Ѿ�

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
	//��ĳ����
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
	//�� �Ѿ�

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
	//���� : ���ΰ��� �� �Ѿ� ������ �浹 �Ǿ��°�?
	ColCheck2();  // ���ΰ� -> �� �浹üũ
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
							}
							else if ((*_FT)->name == "Laser")
							{
								m_xwing.m_laserable++;
								GMAIN->m_pSound.Play(SND_BONUS, true);
								GMAIN->m_pSound.Play(SND_WONTFAIL, true);
							}
							else if ((*_FT)->name == "Power")
							{
								m_xwing.m_power++;
								GMAIN->m_pSound.Play(SND_BONUS, true);
								GMAIN->m_pSound.Play(SND_USEFORCE, true);
							}
							else if ((*_FT)->name == "Hp")
							{
								m_xwing.hp = 100;
								GMAIN->m_pSound.Play(SND_BONUS, true);
								GMAIN->m_pSound.Play(SND_WITHYOU, true);
							}
							else if ((*_FT)->name == "Speed")
							{
								m_xwing.m_speed++;
								GMAIN->m_pSound.Play(SND_BONUS, true);
								GMAIN->m_pSound.Play(SND_GOODINU, true);
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
	//���� : ������ �� �Ѿ� ���ΰ����� �浹 �Ǿ��°�?
	if (m_xwing.dead != TRUE)
	{
		ColCheck();	// �� -> ���ΰ� �浹üũ
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
				GMAIN->m_nGameBeforePhase = GMAIN->m_nGamePhase;
				GMAIN->m_nGamePhase = ST_END;
				GMAIN->m_pSound.Play(SND_THE_FORCE, true);
				GMAIN->m_pSound.Stop(SND_PLAYBG);
				GMAIN->m_pSound.Stop(SND_XWENGLP);
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
	ColCheck3(); //�浹üũ �Լ�3: ���� ���ΰ��� �浹 üũ
				 ////////////////////////////////////////////////////////////////////////////////
	
	
	dxsprite->Draw(GMAIN->m_pGameTex[7].m_pTex, (&GMAIN->rc), NULL, &vcBar, D3DXCOLOR(1, 1, 1, 1.f)); //���¹�
	dxsprite->End();

	//#���� ���¹� ���ó����
	this->ProcessSideBar();
}

//���� ���¹� ���ó����
void CGameMulti::ProcessSideBar() {
	////////////////////////////////////////////////////////////////////////////////
	GMAIN->m_text.Begin();

	hh = GMAIN->m_alphatime / 3600000;
	mm = GMAIN->m_alphatime % 3600000;
	mm = mm / 60000;
	ss = GMAIN->m_alphatime % 3600000 % 60000 / 1000;


	_stprintf(scoreBuf, L" %d", score);
	_stprintf(killcount, L" %d", m_xwing.m_killcount);
	_stprintf(enemyBuf, L"%d : Enemy", pvTie0.size());
	_stprintf(timeBuf, L" %d : %d : %d", hh, mm, ss);
	_stprintf(ioncntBuf, L"%d : Ion", ioncnt);
	_stprintf(lasercntBuf, L"%d : Laser", lasercnt);
	_stprintf(missilecntBuf, L"%d : Missile", missilecnt);
	_stprintf(curioncntBuf, L"%d : Ion Cnt", curioncnt);
	_stprintf(curlasercntBuf, L"%d : Laser Cnt", curlasercnt);
	_stprintf(curmissilecntBuf, L"%d : Missile Cnt", curmissilecnt);
	_stprintf(vectorsize, L"%d : Attack Cnt", pvLaser1.size());
	_stprintf(stageBuf, L" %d", stage);
	_stprintf(enemylevelBuf, L" %d", enemylevel);
	_stprintf(lifeBuf, L" Life : %d", m_xwing.m_life);
	_stprintf(phpBuf, L" H.P : %d", m_xwing.hp);
	_stprintf(playerabilBuf, L" Laser : %d", m_xwing.m_laserable);
	_stprintf(powerBuf, L" Power : %d", m_xwing.m_power);
	_stprintf(speedBuf, L" Speed : %d", m_xwing.m_speed);

	GMAIN->m_text.Draw(L"Score", 660, 10);
	GMAIN->m_text.Draw(scoreBuf, 660, 30);

	GMAIN->m_text.Draw(L"KillCount", 660, 50);
	GMAIN->m_text.Draw(killcount, 660, 70);
	GMAIN->m_text.Draw(L"FlightTime", 660, 90);
	GMAIN->m_text.Draw(timeBuf, 660, 110);
	//GMAIN->m_text.Draw("------------------",660,150);
	GMAIN->m_text.Draw(enemyBuf, 660, 160);
	GMAIN->m_text.Draw(L"------------------", 660, 170);
	GMAIN->m_text.Draw(lasercntBuf, 660, 180);
	GMAIN->m_text.Draw(ioncntBuf, 660, 200);
	GMAIN->m_text.Draw(missilecntBuf, 660, 220);
	GMAIN->m_text.Draw(L"------------------", 660, 230);
	GMAIN->m_text.Draw(curlasercntBuf, 660, 240);
	GMAIN->m_text.Draw(curioncntBuf, 660, 260);
	GMAIN->m_text.Draw(curmissilecntBuf, 660, 280);
	GMAIN->m_text.Draw(L"------------------", 660, 290);
	GMAIN->m_text.Draw(vectorsize, 660, 300);
	//GMAIN->m_text.Draw("------------------",660,310);

	/////////////////////////////////////////////////////////////////////////////
	//FPS ȭ�鿡 ���
	_stprintf(fpsBuf, L"FPS: %4.1f", GMAIN->m_fFps);
	GMAIN->m_text.Draw(fpsBuf, 660, 340);
	/////////////////////////////////////////////////////////////////////////////

	GMAIN->m_text.Draw(L"Stage", 660, 370);
	GMAIN->m_text.Draw(stageBuf, 660, 390);
	GMAIN->m_text.Draw(L"Level", 660, 410);
	GMAIN->m_text.Draw(enemylevelBuf, 660, 430);

	GMAIN->m_text.Draw(L"Player", 660, 480);
	GMAIN->m_text.Draw(lifeBuf, 660, 500);
	GMAIN->m_text.Draw(phpBuf, 660, 520);
	GMAIN->m_text.Draw(playerabilBuf, 660, 540);
	GMAIN->m_text.Draw(powerBuf, 660, 560);
	GMAIN->m_text.Draw(speedBuf, 660, 580);

	GMAIN->m_text.End();
}



void CGameMulti::Destroy()
{
	//���ΰ� �Ѿ� �Ҹ�κ�
	std::vector<CLaserData*>::iterator _F = pvLaser0.begin();
	std::vector<CLaserData*>::iterator _L = pvLaser0.end();
	for (; _F != _L; ++_F) { if (*_F != 0) delete (*_F); }
	pvLaser0.clear();

	//�� �Ѿ� �Ҹ�κ�
	std::vector<CLaserData*>::iterator _F1 = pvLaser1.begin();
	std::vector<CLaserData*>::iterator _L1 = pvLaser1.end();
	for (; _F1 != _L1; ++_F1) { if (*_F1 != 0) delete (*_F1); }
	pvLaser1.clear();

	//�� �ɸ��� �Ҹ�κ�
	std::vector<CCharacterData*>::iterator _FT = pvTie0.begin();
	std::vector<CCharacterData*>::iterator _LT = pvTie0.end();
	for (; _FT != _LT; ++_FT) { if (*_FT != 0) delete (*_FT); }
	pvTie0.clear();

	I_DebugStr.Release();
	m_Client.m_bExit = true;
	I_GameUser.Release();
	m_Client.Release();
	m_Udp.Release();

}



//�浹üũ �Լ�1: ���Ѿ��� ���ΰ��� �¾Ҵ��� üũ
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

			//���� ����
			GMAIN->m_pSound.Play(SND_EXPLOSION, true);
			//���� ��


			m_xwing.hp = m_xwing.hp - (*_F)->damage; //���ΰ� ĳ���� ü�� - (*_F)->damage
			if (m_xwing.hp <= 0)
			{
				m_xwing.dead = TRUE;

				//���ΰ��� ������ ��� ���� ȭ�� ������� �̵�

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
			}
			else if ((*_F)->name == "Ion")
			{
				GMAIN->m_pSound.Play(SND_FIREBY, true);
			}
			else if ((*_F)->name == "Missile")
			{
				GMAIN->m_pSound.Play(SND_MISSLFLY, true);
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

//�浹üũ �Լ�2: ���ΰ��Ѿ��� ���� �¾Ҵ��� üũ
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

				//���� ����
				GMAIN->m_pSound.Play(SND_EXPLOSION, true);
				//���� ��

				(*_FT)->hp = (*_FT)->hp - 50 * m_xwing.m_power;		//���ɸ��� ü�� -50

				if ((*_FT)->hp <= 0)				//HP�� 0�ϰ�� ��ĳ�� ���
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

//�浹üũ �Լ�3: ���� ���ΰ��� �浹 üũ
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
		RECT rcCol3;	//���� �浹 üũ��

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
				//�� �������� ����

				GMAIN->m_pSound.Play(SND_TIE, true);
				//m_pSndTie->Play();
				//m_pSndTie->SetVolume(0);
				//���� ��
				if ((*_FT)->name == "Boss"&&rand() % 2 == 0)
				{

					switch (rand() % 7)
					{
					case 0:
						GMAIN->m_pSound.Play(SND_DARK_SIDE, true);
						break;
					case 1:
						GMAIN->m_pSound.Play(SND_DARK_SIDE2_Y, true);
						break;
					case 2:
						GMAIN->m_pSound.Play(SND_DONT_UNDERESTIMATE, true);
						break;
					case 3:
						GMAIN->m_pSound.Play(SND_DONTMAKE, true);
						break;
					case 4:
						GMAIN->m_pSound.Play(SND_EXPECTING_YOU, true);
						break;
					case 5:
						GMAIN->m_pSound.Play(SND_MUSTOBEY, true);
						break;
					case 6:
						GMAIN->m_pSound.Play(SND_YOUR_FATHER, true);
						break;
					default:
						GMAIN->m_pSound.Play(SND_DONT_UNDERESTIMATE, true);
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
	//SetWindowText(m_hWnd, "���ʹ�ư�� �������ϴ�");
	//}
	//if(m_pInput->ButtonPress(1))
	//{
	//SetWindowText(m_hWnd, "�����ʹ�ư�� �������ϴ�");
	//}
	////////////////////////////////////////////////////////////////////////////////


	if (m_xwing.dead != TRUE)
	{
		if (GMAIN->m_pInput->KeyPress(VK_RIGHT))
		{
#ifdef MULTIIMPLE
			TPACKET_USER_POSITION userdata;
			userdata.direction = VK_RIGHT;
			userdata.posX = m_xwing.xval + GMAIN->m_movingdist*m_xwing.m_speed;//I_GameUser.m_fPosX;
			userdata.posY = m_xwing.yval;//I_GameUser.m_fPosY;
			userdata.user_idx = m_iSerIndex;
			char buffer[256] = { 0, };
			int iSize = sizeof(userdata);
			memcpy(buffer, &userdata, iSize);
			m_Client.SendMsg(buffer, iSize,//(char*)&userdata, 
				PACKET_USER_POSITION);
#else
			m_xwing.xval = m_xwing.xval + GMAIN->m_movingdist*m_xwing.m_speed;

			if (m_xwing.xval>593)
			{
				m_xwing.xval = 593;
			}
#endif
		}
		if (GMAIN->m_pInput->KeyPress(VK_LEFT))
		{
#ifdef MULTIIMPLE
			TPACKET_USER_POSITION userdata;
			userdata.direction = VK_LEFT;
			userdata.posX = m_xwing.xval - GMAIN->m_movingdist*m_xwing.m_speed;//I_GameUser.m_fPosX;
			userdata.posY = m_xwing.yval;//I_GameUser.m_fPosY;
			userdata.user_idx = m_iSerIndex;
			char buffer[256] = { 0, };
			int iSize = sizeof(userdata);
			memcpy(buffer, &userdata, iSize);
			m_Client.SendMsg(buffer, iSize,//(char*)&userdata, 
				PACKET_USER_POSITION);
#else
			m_xwing.xval = m_xwing.xval - GMAIN->m_movingdist*m_xwing.m_speed;
			if (m_xwing.xval<0)
			{
				m_xwing.xval = 0;
			}
#endif
		}

		if (GMAIN->m_pInput->KeyPress(VK_UP))
		{
#ifdef MULTIIMPLE
			TPACKET_USER_POSITION userdata;
			userdata.direction = VK_UP;
			userdata.posX = m_xwing.xval;//I_GameUser.m_fPosX;
			userdata.posY = m_xwing.yval - GMAIN->m_movingdist*m_xwing.m_speed; //I_GameUser.m_fPosY;
			userdata.user_idx = m_iSerIndex;
			char buffer[256] = { 0, };
			int iSize = sizeof(userdata);
			memcpy(buffer, &userdata, iSize);
			m_Client.SendMsg(buffer, iSize,//(char*)&userdata, 
				PACKET_USER_POSITION);
#else
			m_xwing.yval = m_xwing.yval - GMAIN->m_movingdist*m_xwing.m_speed;

			if (m_xwing.yval<0)
			{
				m_xwing.yval = 0;
			}
#endif
		}
		if (GMAIN->m_pInput->KeyPress(VK_DOWN))
		{
#ifdef MULTIIMPLE
			TPACKET_USER_POSITION userdata;
			userdata.direction = VK_DOWN;
			userdata.posX = m_xwing.xval;//I_GameUser.m_fPosX;
			userdata.posY = m_xwing.yval + GMAIN->m_movingdist*m_xwing.m_speed; //I_GameUser.m_fPosY;
			userdata.user_idx = m_iSerIndex;
			char buffer[256] = { 0, };
			int iSize = sizeof(userdata);
			memcpy(buffer, &userdata, iSize);
			m_Client.SendMsg(buffer, iSize,//(char*)&userdata, 
				PACKET_USER_POSITION);
#else
			m_xwing.yval = m_xwing.yval + GMAIN->m_movingdist*m_xwing.m_speed;
			if (m_xwing.yval>550)
			{
				m_xwing.yval = 550;
			}
#endif
		}
		if (GMAIN->m_pInput->KeyDown(VK_SPACE))
		{
#ifdef MULTIIMPLE
#else
			if (pvLaser0.size() < m_xwing.m_laserable)
			{
				pvLaser0.push_back(new CLaserData(m_xwing.xval, m_xwing.yval, FALSE)); // ���ΰ� �Ѿ� �߻�

				GMAIN->m_pSound.Play(SND_XWLSR1, true);
			}
#endif
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
