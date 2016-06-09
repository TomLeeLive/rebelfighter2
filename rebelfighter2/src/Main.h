#pragma once
//#include "_StdAfx.h"


class CMain : public CD3DApplication
{
public:
	CDrawText		m_text;

	D3DXVECTOR3 vcPos;						//�����ġ
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
	KSound			m_pSound;				// ����(fmod)
	GAME_STATE		m_nGamePhase;					//���� ���� ����(����ȭ��, ����, ����
	GAME_STATE		m_nGameBeforePhase;				//���� ���� ���� ����(for ���ӿ��� ȭ�鿡�� ���� ��¿�. ��Ƽ���� �̱�����)

	INT Init();

	char	m_KeyOld[256];		// Old Key
	char	m_KeyCur[256];		// Current Key

	void Destroy();
////////////////////////////////////////////////////////////////////////////////
	


	virtual int GameTextureLoad(LPDIRECT3DTEXTURE9& pTex, D3DXIMAGE_INFO* pImg
		, LPDIRECT3DDEVICE9 pDevice, char* sfile, DWORD dColor);

	INT	TextureLoad();
	INT	SoundLoad();

	virtual INT		GameFrameMove();		//���� ��ü ����
	virtual INT		GameRender();			//���� ��ü ������
////////////////////////////////////////////////////////////////////////////////
	virtual LRESULT MsgProc( HWND, UINT, WPARAM, LPARAM);


	~CMain();
	CMain(void);
};

extern CMain*	g_pApp; //�ܺ� ���� ���� ����


// _StdAfx.h���� �� ����� ��Ŭ��� �Ͽ����Ƿ�
// SpBasic.cpp���� ����� g_pApp�� #define GMAIN g_pApp������ �� �� �ִ�.