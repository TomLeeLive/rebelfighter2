#pragma once
#include "KSound.h"
class CMain : public CD3DApplication
{
public:
	CGameTexture m_pGameTex[26];
	CGameInput* m_pInput;

public:

	std::vector<CLaserData*>		pvLaser0;			//주인공 총알 벡터 컨테이너
	std::vector<CLaserData*>		pvLaser1;			//적 총알 벡터 컨테이너
	std::vector<CCharacterData*> pvTie0;			//적 케릭터 벡터 컨테이너

	CGameTexture*		m_pTx;
	RECT				m_ImgRc2;						// RECT 애니 이미지

	DWORD				m_dTimeBegin;					// 시작 타임
	DWORD				m_dTimeEnd;					// 끝 타임

	KSound				m_pSound;



	char phpBuf[80], scoreBuf[80], stageBuf[80], lifeBuf[80]
	, playerabilBuf[80], vectorsize[80], killcount[80], timeBuf[80]
	,ioncntBuf[80], lasercntBuf[80], missilecntBuf[80]
	,curioncntBuf[80], curlasercntBuf[80], curmissilecntBuf[80],enemyBuf[80]
	,speedBuf[80],powerBuf[80],enemylevelBuf[80];

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

	char	m_KeyOld[256];		// Old Key
	char	m_KeyCur[256];		// Current Key

	RECT	rc;

	CPlayerData m_xwing;

	D3DXVECTOR3 vcPos;						//배경위치
	D3DXVECTOR3 vcPos1;						//배경위치
	D3DXVECTOR3 vcPos2;						//배경위치
	D3DXVECTOR3 vcBar;						//상태바위치
	D3DXVECTOR3 vcLpos0;					//주인공총알위치0
	D3DXVECTOR3 vcLpos1;					//적총알위치0
	//D3DXVECTOR3 boom;						//폭발위치
	
	CMain();
	virtual ~CMain(void);

	virtual INT		Init();					//게임 객체 생성, 초기화
	virtual void	Destroy();				//게임 객체 소멸
	virtual INT		GameFrameMove();		//게임 객체 갱신
	virtual INT		GameRender();			//게임 객체 렌더링

	INT ColCheck();		//충돌체크 함수1: 적총알이 주인공에 맞았는지 체크
	INT ColCheck2();	//충돌체크 함수2: 주인공총알이 적에 맞았는지 체크
	INT ColCheck3();	//충돌체크 함수3: 적과 주인공의 충돌 체크

////////////////////////////////////////////////////////////////////////////////
//메뉴별로 게임 메뉴, 게임 실행, 스코어 
	int		m_nGamePhase;					//게임 상태 변경(시작화면, 게임, 종료)
	
	int		StartFrameMove();				// 시작 갱신
	void	StartRender();					// 시작 렌더링
	void	StartRndBck();					// 시작 Back buffer 렌더링

	int		PlayFrameMove();				// 플레이 갱신
	void	PlayRender();					// 플레이 렌더링
	void	PlayRndBck();					// 플레이 Back buffer 렌더링

	int		ScoreFrameMove();				// 스코어 갱신
	void	ScoreRender();					// 스코어 렌더링
	void	ScoreRndBck();					// 스코어 Back buffer 렌더링

	int		EndFrameMove();					// 게임오버 갱신
	void	EndRender();					// 게임오버 렌더링
	void	EndRndBck();					// 게임오버 Back buffer 렌더링

	void	ScoreRead();					// 스코어 읽기
	void	ScoreWrite();					// 스코어 쓰기

	void	HowtoFrameMove();
	void	HowtoRender();

	int		ImgArrayInit();					// Image Array Init
	void	ImgArrayDestroy();				// Image Array Delete
	void	ImgArrayShuffle();				// Image Array Shuffle
////////////////////////////////////////////////////////////////////////////////
	
	void	InputMove();
	void	CharacterMove();

	int GameTextureLoad(LPDIRECT3DTEXTURE9& pTex, D3DXIMAGE_INFO* pImg				
		,LPDIRECT3DDEVICE9 pDevice, char* sfile, DWORD dColor);

	INT	TextureLoad();
	INT	SoundLoad();
////////////////////////////////////////////////////////////////////////////////
	virtual LRESULT MsgProc( HWND, UINT, WPARAM, LPARAM);

};

extern CMain*	g_pApp; //외부 연결 범위 선언

// _StdAfx.h에서 이 헤더를 인클루드 하였으므로
// SpBasic.cpp에서 선언된 g_pApp를 #define GMAIN g_pApp등으로 쓸 수 있다.