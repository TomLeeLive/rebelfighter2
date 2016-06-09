#pragma once
//#include "_StdAfx.h"


class CMain : public CD3DApplication
{
public:
	CDrawText		m_text;

	D3DXVECTOR3 vcPos;						//배경위치
	RECT	rc;

	CGameStart m_gamestart;
	CGameEnd   m_gameend;
	CGamePlay  m_gameplay;
	CGameHowto m_gamehowto;
	CGameMulti m_gamemulti;
	//CShtGame   m_shtgame;

	CGameInput* m_pInput;

	CGameTexture*		m_pTx;

	CGameTexture	m_pGameTex[28];
	KSound			m_pSound;				// 사운드(fmod)
	GAME_STATE		m_nGamePhase;					//게임 상태 변경(시작화면, 게임, 종료
	GAME_STATE		m_nGameBeforePhase;				//게임 이전 상태 저장(for 게임오버 화면에서 점수 출력용. 멀티인지 싱글인지)

	INT Init();

	char	m_KeyOld[256];		// Old Key
	char	m_KeyCur[256];		// Current Key

	void Destroy();
////////////////////////////////////////////////////////////////////////////////
	


	virtual int GameTextureLoad(LPDIRECT3DTEXTURE9& pTex, D3DXIMAGE_INFO* pImg
		, LPDIRECT3DDEVICE9 pDevice, char* sfile, DWORD dColor);

	INT	TextureLoad();
	INT	SoundLoad();

	virtual INT		GameFrameMove();		//게임 객체 갱신
	virtual INT		GameRender();			//게임 객체 렌더링
////////////////////////////////////////////////////////////////////////////////
	virtual LRESULT MsgProc( HWND, UINT, WPARAM, LPARAM);


	~CMain();
	CMain(void);
};

extern CMain*	g_pApp; //외부 연결 범위 선언


// _StdAfx.h에서 이 헤더를 인클루드 하였으므로
// SpBasic.cpp에서 선언된 g_pApp를 #define GMAIN g_pApp등으로 쓸 수 있다.