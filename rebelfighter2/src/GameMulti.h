#pragma once

//#include "TDebugString.h"
//#include "TClient.h"
//#include "TGameUser.h"
//#include "TUdpSocket.h"

typedef struct _raknet {
	RakNet::RakNetStatistics *rss;
	// Pointers to the interfaces of our server and client.
	// Note we can easily have both in the same program
	RakNet::RakPeerInterface *client;
	//	client->InitializeSecurity(0,0,0,0);
	//RakNet::PacketLogger packetLogger;
	//client->AttachPlugin(&packetLogger);


	// Holds packets
	RakNet::Packet* p;

	// GetPacketIdentifier returns this
	unsigned char packetIdentifier;

	// Just so we can remember where the packet came from
	bool isServer;

	// Record the first client that connects to us so we can pass it to the ping function
	RakNet::SystemAddress clientID;

	// Crude interface

	// Holds user data
	char ip[64], serverPort[30], clientPort[30];
}st_RAKNET;

class CGameMulti : public CGameScene
{
public:
/////////////////////////////////////////////////////////////
// for Multi play

	//TUdpSocket			m_Udp;
	////��������
	////TGameUser				I_GameUser;
	//std::vector<TGameUser>	m_UserList;
	//TClient				m_Client;
	//int					m_iSerIndex;
	//bool				m_bLogin;

	HANDLE hThread;

	st_RAKNET m_raknet;

	int m_iMultiPlayer;//��Ƽ�÷��̾� ���� ��ȣ. 1: 1p, 2: 2p,  0: ���ӵ��� ����. 3: �����ڼ� 2���̻��̶� ������ ���� ����.


	void	UserMoveSend(int iUserNum, float fPosX, float fPosY, int iDirection);
/////////////////////////////////////////////////////////////
	INT Init();
	void Frame();
	void Render(LPDIRECT3DDEVICE9& dxdevice, LPD3DXSPRITE& dxsprite);
	void Destroy();




public:
	void MultiInit();						//��Ƽ�÷��̰��� init
	void NextStageCreate();						//������������ �������� ó��
	void EnemyBullet();							//�� �Ѿ� �߻� ó��
	void ProcessSideBar();						//���� ���¹� ���ó����

	std::vector<CLaserData*>		pvLaser0;	//���ΰ� �Ѿ� ���� �����̳�
	std::vector<CLaserData*>		pvLaser1;	//�� �Ѿ� ���� �����̳�
	std::vector<CCharacterData*> pvTie0;	    //�� �ɸ��� ���� �����̳�


	RECT				m_ImgRc2;				// RECT �ִ� �̹���

	DWORD				m_dTimeBegin;			// ���� Ÿ��
	DWORD				m_dTimeEnd;				// �� Ÿ��



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

	int		stage;			//ó�� ���۽� �������� 1
	int		curstage;
	int		score;			//����
	int		enemylevel;	//�� ���� ���� ����

						//�ð�����
	int		hh;	//��
	int		mm;	//��
	int		ss;	//��





	CPlayerData m_xwing;


	D3DXVECTOR3 vcPos1;						//�����ġ
	D3DXVECTOR3 vcPos2;						//�����ġ
	D3DXVECTOR3 vcBar;						//���¹���ġ
	D3DXVECTOR3 vcLpos0;					//���ΰ��Ѿ���ġ0
	D3DXVECTOR3 vcLpos1;					//���Ѿ���ġ0
	
	D3DXVECTOR3 boom;						//������ġ



	INT ColCheck();		//�浹üũ �Լ�1: ���Ѿ��� ���ΰ��� �¾Ҵ��� üũ
	INT ColCheck2();	//�浹üũ �Լ�2: ���ΰ��Ѿ��� ���� �¾Ҵ��� üũ
	INT ColCheck3();	//�浹üũ �Լ�3: ���� ���ΰ��� �浹 üũ

						////////////////////////////////////////////////////////////////////////////////
						//�޴����� ���� �޴�, ���� ����, ���ھ� 


	void	CharacterMovePath(std::vector<CCharacterData*>::iterator* _FT);
	void	CharacterMoveBasic(std::vector<CCharacterData*>::iterator* _FT);

	void	InputMove();
	void	CharacterMove();


public:
	CGameMulti();
	virtual ~CGameMulti();
};

extern CGameMulti*	g_pGameMultiplay; //�ܺ� ���� ���� ����