#pragma once
class CGamePlay : public CGameScene
{
public:
	INT Init();
	void Frame();
	void Render(LPDIRECT3DDEVICE9& dxdevice, LPD3DXSPRITE& dxsprite);
	void Destroy();

	


public:

	std::vector<CLaserData*>		pvLaser0;	//주인공 총알 벡터 컨테이너
	std::vector<CLaserData*>		pvLaser1;	//적 총알 벡터 컨테이너
	std::vector<CCharacterData*> pvTie0;	    //적 케릭터 벡터 컨테이너


	RECT				m_ImgRc2;				// RECT 애니 이미지

	DWORD				m_dTimeBegin;			// 시작 타임
	DWORD				m_dTimeEnd;				// 끝 타임



	char phpBuf[80], scoreBuf[80], stageBuf[80], lifeBuf[80]
		, playerabilBuf[80], vectorsize[80], killcount[80], timeBuf[80]
		, ioncntBuf[80], lasercntBuf[80], missilecntBuf[80]
		, curioncntBuf[80], curlasercntBuf[80], curmissilecntBuf[80], enemyBuf[80]
		, speedBuf[80], powerBuf[80], enemylevelBuf[80];

	TCHAR	fpsBuf[128];

	int    ioncnt;
	int		lasercnt;
	int		missilecnt;
	int		curioncnt;
	int		curlasercnt;
	int		curmissilecnt;

	int		stage;			//처음 시작시 스테이지 1
	int		curstage;
	int		score;			//점수
	int		enemylevel;	//적 레벨 계산용 변수

						//시간계산용
	int		hh;	//시
	int		mm;	//분
	int		ss;	//초





	CPlayerData m_xwing;


	D3DXVECTOR3 vcPos1;						//배경위치
	D3DXVECTOR3 vcPos2;						//배경위치
	D3DXVECTOR3 vcBar;						//상태바위치
	D3DXVECTOR3 vcLpos0;					//주인공총알위치0
	D3DXVECTOR3 vcLpos1;					//적총알위치0
	
	D3DXVECTOR3 boom;						//폭발위치

	INT ColCheck();		//충돌체크 함수1: 적총알이 주인공에 맞았는지 체크
	INT ColCheck2();	//충돌체크 함수2: 주인공총알이 적에 맞았는지 체크
	INT ColCheck3();	//충돌체크 함수3: 적과 주인공의 충돌 체크

						////////////////////////////////////////////////////////////////////////////////
						//메뉴별로 게임 메뉴, 게임 실행, 스코어 


	void	CharacterMovePath(std::vector<CCharacterData*>::iterator* _FT);
	void	CharacterMoveBasic(std::vector<CCharacterData*>::iterator* _FT);

	void	InputMove();
	void	CharacterMove();


public:
	CGamePlay();
	virtual ~CGamePlay();
};

extern CGamePlay*	g_pGameplay; //외부 연결 범위 선언
