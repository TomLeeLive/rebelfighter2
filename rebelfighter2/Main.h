#pragma once
#include "KSound.h"
class CMain : public CD3DApplication
{
public:
	CGameTexture m_pGameTex[26];
	CGameInput* m_pInput;

public:

	std::vector<CLaserData*>		pvLaser0;			//���ΰ� �Ѿ� ���� �����̳�
	std::vector<CLaserData*>		pvLaser1;			//�� �Ѿ� ���� �����̳�
	std::vector<CCharacterData*> pvTie0;			//�� �ɸ��� ���� �����̳�

	CGameTexture*		m_pTx;
	RECT				m_ImgRc2;						// RECT �ִ� �̹���

	DWORD				m_dTimeBegin;					// ���� Ÿ��
	DWORD				m_dTimeEnd;					// �� Ÿ��

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

	int		stage;			//ó�� ���۽� �������� 1
	int		curstage;
	int		score;			//����
	int		enemylevel;	//�� ���� ���� ����

	//�ð�����
	int		hh;	//��
	int		mm;	//��
	int		ss;	//��

	char	m_KeyOld[256];		// Old Key
	char	m_KeyCur[256];		// Current Key

	RECT	rc;

	CPlayerData m_xwing;

	D3DXVECTOR3 vcPos;						//�����ġ
	D3DXVECTOR3 vcPos1;						//�����ġ
	D3DXVECTOR3 vcPos2;						//�����ġ
	D3DXVECTOR3 vcBar;						//���¹���ġ
	D3DXVECTOR3 vcLpos0;					//���ΰ��Ѿ���ġ0
	D3DXVECTOR3 vcLpos1;					//���Ѿ���ġ0
	//D3DXVECTOR3 boom;						//������ġ
	
	CMain();
	virtual ~CMain(void);

	virtual INT		Init();					//���� ��ü ����, �ʱ�ȭ
	virtual void	Destroy();				//���� ��ü �Ҹ�
	virtual INT		GameFrameMove();		//���� ��ü ����
	virtual INT		GameRender();			//���� ��ü ������

	INT ColCheck();		//�浹üũ �Լ�1: ���Ѿ��� ���ΰ��� �¾Ҵ��� üũ
	INT ColCheck2();	//�浹üũ �Լ�2: ���ΰ��Ѿ��� ���� �¾Ҵ��� üũ
	INT ColCheck3();	//�浹üũ �Լ�3: ���� ���ΰ��� �浹 üũ

////////////////////////////////////////////////////////////////////////////////
//�޴����� ���� �޴�, ���� ����, ���ھ� 
	int		m_nGamePhase;					//���� ���� ����(����ȭ��, ����, ����)
	
	int		StartFrameMove();				// ���� ����
	void	StartRender();					// ���� ������
	void	StartRndBck();					// ���� Back buffer ������

	int		PlayFrameMove();				// �÷��� ����
	void	PlayRender();					// �÷��� ������
	void	PlayRndBck();					// �÷��� Back buffer ������

	int		ScoreFrameMove();				// ���ھ� ����
	void	ScoreRender();					// ���ھ� ������
	void	ScoreRndBck();					// ���ھ� Back buffer ������

	int		EndFrameMove();					// ���ӿ��� ����
	void	EndRender();					// ���ӿ��� ������
	void	EndRndBck();					// ���ӿ��� Back buffer ������

	void	ScoreRead();					// ���ھ� �б�
	void	ScoreWrite();					// ���ھ� ����

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

extern CMain*	g_pApp; //�ܺ� ���� ���� ����

// _StdAfx.h���� �� ����� ��Ŭ��� �Ͽ����Ƿ�
// SpBasic.cpp���� ����� g_pApp�� #define GMAIN g_pApp������ �� �� �ִ�.