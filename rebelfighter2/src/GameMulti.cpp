#include "_StdAfx.h"

//#멀티 구현 코드 구분을 위한 매크로 for debugging
#define MULTIIMPLE 1


CRITICAL_SECTION g_cs_pvLaser0;
CRITICAL_SECTION g_cs_pvLaser1;
CRITICAL_SECTION g_cs_pvTie0;


// We copy this from Multiplayer.cpp to keep things all in one file for this example
unsigned char GetPacketIdentifier(RakNet::Packet *p);

template <typename T>
unsigned short CreateUniqueID(T a) {
	srand((int)time(NULL));

	unsigned short usTemp;

	bool bLoop = true;

	T::iterator _F = a.begin();
	T::iterator _L = a.end();

	while (bLoop) {
		usTemp = unsigned short(rand() % 100);

		if (a.size() == 0) {
			return usTemp;
		}
		else {
			for (; _F != _L;)
			{
				if (*_F == 0)
					continue;

				if ((*_F)->id == usTemp) {
					_F = a.begin();
					break;
				}

				++_F;

				if (_F == _L) {
					bLoop = false;
					break;
				}

			}
		}
		
	}
	return usTemp;

}



void Process_ID_USER_MOVE(RakNet::Packet *p) {
	long lTemp;

	TID_USER_MOVE_DATA temp;
	memcpy(&temp, &(p->data[1]), sizeof(TID_USER_MOVE_DATA));

	lTemp = (long)temp.posX;

	if (temp.direction == DIRECTION_RR) {
		GGAMEMULTI->m_xwing_1p.xval += ((float)lTemp) / 10.0f;

		if (GGAMEMULTI->m_xwing_1p.xval>593)
		{
			GGAMEMULTI->m_xwing_1p.xval = 593;
		}
	}
	else if (temp.direction == DIRECTION_LL) {
		GGAMEMULTI->m_xwing_1p.xval -= ((float)lTemp) / 10.0f;

		if (GGAMEMULTI->m_xwing_1p.xval<0)
		{
			GGAMEMULTI->m_xwing_1p.xval = 0;
		}
	}
	lTemp = (long)temp.posY;

	if (temp.direction == DIRECTION_UU) {
		GGAMEMULTI->m_xwing_1p.yval -= ((float)lTemp) / 10.0f;

		if (GGAMEMULTI->m_xwing_1p.yval<0)
		{
			GGAMEMULTI->m_xwing_1p.yval = 0;
		}
	}
	else if (temp.direction == DIRECTION_DD) {
		GGAMEMULTI->m_xwing_1p.yval += ((float)lTemp) / 10.0f;

		if (GGAMEMULTI->m_xwing_1p.yval>550)
		{
			GGAMEMULTI->m_xwing_1p.yval = 550;
		}
	}
}

void Process_ID_USER_LASER_FIRE(RakNet::Packet *p) {
	
	if (GGAMEMULTI->m_iMultiPlayer == 1)
		return;

	long lTempX, lTempY;

	TID_USER_LASER_FIRE_DATA temp;
	memcpy(&temp, &(p->data[1]), sizeof(TID_USER_LASER_FIRE_DATA));

	lTempX = (long)temp.posX;

	lTempY = (long)temp.posY;

	EnterCriticalSection(&g_cs_pvLaser0);
	if (GGAMEMULTI->pvLaser0.size() < GGAMEMULTI->m_xwing_1p.m_laserable)
	{
		GGAMEMULTI->pvLaser0.push_back(new CLaserData(GGAMEMULTI->m_xwing_1p.xval, GGAMEMULTI->m_xwing_1p.yval, FALSE, temp.id)); // 주인공 총알 발생

		GMAIN->m_pSound.Play(SND_XWLSR1, true);
	}
	LeaveCriticalSection(&g_cs_pvLaser0);
}

UINT WINAPI ThreadFunc(void *arg)
{
	st_RAKNET* raknet = (st_RAKNET*)arg;
	
	//RakNet::RakNetStatistics *rss;

	// Pointers to the interfaces of our server and client.
	// Note we can easily have both in the same program
	raknet->client = RakNet::RakPeerInterface::GetInstance();
	//	client->InitializeSecurity(0,0,0,0);
	//RakNet::PacketLogger packetLogger;
	//client->AttachPlugin(&packetLogger);


	// Holds packets
	//RakNet::Packet* p;

	// GetPacketIdentifier returns this
	//unsigned char packetIdentifier;

	// Just so we can remember where the packet came from
	//bool isServer;

	// Record the first client that connects to us so we can pass it to the ping function
	raknet->clientID = RakNet::UNASSIGNED_SYSTEM_ADDRESS;

	// Crude interface

	// Holds user data
	//char ip[64], serverPort[30], clientPort[30];
	

	// A client
	raknet->isServer = false;

	printf("This is a sample implementation of a text based chat client.\n");
	printf("Connect to the project 'Chat Example Server'.\n");
	printf("Difficulty: Beginner\n\n");

	// Get our input
	//puts("Enter the client port to listen on");
	//Gets(clientPort, sizeof(clientPort));
	//if (clientPort[0] == 0)
	strcpy(raknet->clientPort, "0");

	/*
	puts("Enter IP to connect to");
	Gets(ip, sizeof(ip));
	*/

	raknet->client->AllowConnectionResponseIPMigration(false);

	//if (ip[0] == 0)
	strcpy(raknet->ip, "127.0.0.1");
	// strcpy(ip, "natpunch.jenkinssoftware.com");


	//puts("Enter the port to connect to");
	//Gets(serverPort, sizeof(serverPort));
	//if (serverPort[0] == 0)
	strcpy(raknet->serverPort, "1234");

	// Connecting the client is very simple.  0 means we don't care about
	// a connectionValidationInteger, and false for low priority threads
	RakNet::SocketDescriptor socketDescriptor(atoi(raknet->clientPort), 0);
	socketDescriptor.socketFamily = AF_INET;
	raknet->client->Startup(8, &socketDescriptor, 1);
	raknet->client->SetOccasionalPing(true);


#if LIBCAT_SECURITY==1
	char public_key[cat::EasyHandshake::PUBLIC_KEY_BYTES];
	FILE *fp = fopen("publicKey.dat", "rb");
	fread(public_key, sizeof(public_key), 1, fp);
	fclose(fp);
#endif

#if LIBCAT_SECURITY==1
	RakNet::PublicKey pk;
	pk.remoteServerPublicKey = public_key;
	pk.publicKeyMode = RakNet::PKM_USE_KNOWN_PUBLIC_KEY;
	bool b = client->Connect(ip, atoi(serverPort), "Rumpelstiltskin", (int)strlen("Rumpelstiltskin"), &pk) == RakNet::CONNECTION_ATTEMPT_STARTED;
#else
	RakNet::ConnectionAttemptResult car = raknet->client->Connect(raknet->ip, atoi(raknet->serverPort), "Rumpelstiltskin", (int)strlen("Rumpelstiltskin"));
	RakAssert(car == RakNet::CONNECTION_ATTEMPT_STARTED);
#endif

	printf("\nMy IP addresses:\n");
	unsigned int i;
	for (i = 0; i <raknet->client->GetNumberOfAddresses(); i++)
	{
		printf("%i. %s\n", i + 1, raknet->client->GetLocalIP(i));
	}

	printf("My GUID is %s\n", raknet->client->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS).ToString());
	puts("'quit' to quit. 'stat' to show stats. 'ping' to ping.\n'disconnect' to disconnect. 'connect' to reconnnect. Type to talk.");



	char message[2048];

	// Loop for input
	while (1)
	{
		// This sleep keeps RakNet responsive
#ifdef _WIN32
		Sleep(30);
#else
		usleep(30 * 1000);
#endif


		if (kbhit())
		{
			// Notice what is not here: something to keep our network running.  It's
			// fine to block on Gets or anything we want
			// Because the network engine was painstakingly written using threads.
			Gets(message, sizeof(message));

			if (strcmp(message, "quit") == 0)
			{
				puts("Quitting.");
				break;
			}

			//if (strcmp(message, "stat") == 0)
			//{

			//	raknet->rss = raknet->client->GetStatistics(raknet->client->GetSystemAddressFromIndex(0));
			//	StatisticsToString(raknet->rss, message, 2);
			//	printf("%s", message);
			//	printf("Ping=%i\n", raknet->client->GetAveragePing(raknet->client->GetSystemAddressFromIndex(0)));

			//	continue;
			//}

			if (strcmp(message, "disconnect") == 0)
			{
				printf("Enter index to disconnect: ");
				char str[32];
				Gets(str, sizeof(str));
				if (str[0] == 0)
					strcpy(str, "0");
				int index = atoi(str);
				raknet->client->CloseConnection(raknet->client->GetSystemAddressFromIndex(index), false);
				printf("Disconnecting.\n");
				continue;
			}

			if (strcmp(message, "shutdown") == 0)
			{
				raknet->client->Shutdown(100);
				printf("Shutdown.\n");
				continue;
			}

			if (strcmp(message, "startup") == 0)
			{
				bool b = raknet->client->Startup(8, &socketDescriptor, 1) == RakNet::RAKNET_STARTED;
				if (b)
					printf("Started.\n");
				else
					printf("Startup failed.\n");
				continue;
			}


			if (strcmp(message, "connect") == 0)
			{
				printf("Enter server ip: ");
				Gets(raknet->ip, sizeof(raknet->ip));
				if (raknet->ip[0] == 0)
					strcpy(raknet->ip, "127.0.0.1");

				printf("Enter server port: ");
				Gets(raknet->serverPort, sizeof(raknet->serverPort));
				if (raknet->serverPort[0] == 0)
					strcpy(raknet->serverPort, "1234");

#if LIBCAT_SECURITY==1
				bool b = client->Connect(ip, atoi(serverPort), "Rumpelstiltskin", (int)strlen("Rumpelstiltskin"), &pk) == RakNet::CONNECTION_ATTEMPT_STARTED;
#else
				bool b = raknet->client->Connect(raknet->ip, atoi(raknet->serverPort), "Rumpelstiltskin", (int)strlen("Rumpelstiltskin")) == RakNet::CONNECTION_ATTEMPT_STARTED;
#endif

				if (b)
					puts("Attempting connection");
				else
				{
					puts("Bad connection attempt.  Terminating.");
					exit(1);
				}
				continue;
			}

			if (strcmp(message, "ping") == 0)
			{
				if (raknet->client->GetSystemAddressFromIndex(0) != RakNet::UNASSIGNED_SYSTEM_ADDRESS)
					raknet->client->Ping(raknet->client->GetSystemAddressFromIndex(0));

				continue;
			}

			if (strcmp(message, "getlastping") == 0)
			{
				if (raknet->client->GetSystemAddressFromIndex(0) != RakNet::UNASSIGNED_SYSTEM_ADDRESS)
					printf("Last ping is %i\n", raknet->client->GetLastPing(raknet->client->GetSystemAddressFromIndex(0)));

				continue;
			}

			// message is the data to send
			// strlen(message)+1 is to send the null terminator
			// HIGH_PRIORITY doesn't actually matter here because we don't use any other priority
			// RELIABLE_ORDERED means make sure the message arrives in the right order
			raknet->client->Send(message, (int)strlen(message) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		}

		// Get a packet from either the server or the client

		for (raknet->p = raknet->client->Receive(); raknet->p; raknet->client->DeallocatePacket(raknet->p), raknet->p = raknet->client->Receive())
		{
			// We got a packet, get the identifier with our handy function
			raknet->packetIdentifier = GetPacketIdentifier(raknet->p);

			// Check if this is a network message packet
			switch (raknet->packetIdentifier)
			{
			case ID_DISCONNECTION_NOTIFICATION:
				// Connection lost normally
				printf("ID_DISCONNECTION_NOTIFICATION\n");
				break;
			case ID_ALREADY_CONNECTED:
				// Connection lost normally
				printf("ID_ALREADY_CONNECTED with guid %" PRINTF_64_BIT_MODIFIER "u\n", raknet->p->guid);
				break;
			case ID_INCOMPATIBLE_PROTOCOL_VERSION:
				printf("ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
				break;
			case ID_REMOTE_DISCONNECTION_NOTIFICATION: // Server telling the clients of another client disconnecting gracefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				printf("ID_REMOTE_DISCONNECTION_NOTIFICATION\n");
				break;
			case ID_REMOTE_CONNECTION_LOST: // Server telling the clients of another client disconnecting forcefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				printf("ID_REMOTE_CONNECTION_LOST\n");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION: // Server telling the clients of another client connecting.  You can manually broadcast this in a peer to peer enviroment if you want.
				printf("ID_REMOTE_NEW_INCOMING_CONNECTION\n");
				break;
			case ID_CONNECTION_BANNED: // Banned from this server
				printf("We are banned from this server.\n");
				break;
			case ID_CONNECTION_ATTEMPT_FAILED:
				printf("Connection attempt failed\n");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				// Sorry, the server is full.  I don't do anything here but
				// A real app should tell the user
				printf("ID_NO_FREE_INCOMING_CONNECTIONS\n");
				break;

			case ID_INVALID_PASSWORD:
				printf("ID_INVALID_PASSWORD\n");
				break;

			case ID_CONNECTION_LOST:
				// Couldn't deliver a reliable packet - i.e. the other system was abnormally
				// terminated
				printf("ID_CONNECTION_LOST\n");
				break;

			case ID_CONNECTION_REQUEST_ACCEPTED:
				// This tells the client they have connected
				printf("ID_CONNECTION_REQUEST_ACCEPTED to %s with GUID %s\n", raknet->p->systemAddress.ToString(true), raknet->p->guid.ToString());
				printf("My external address is %s\n", raknet->client->GetExternalID(raknet->p->systemAddress).ToString(true));
				break;
			case ID_CONNECTED_PING:
			case ID_UNCONNECTED_PING:
				printf("Ping from %s\n", raknet->p->systemAddress.ToString(true));
				break;
			case ID_USER_MOVE:
				Process_ID_USER_MOVE(raknet->p);
				break;
			case ID_USER_LASER_FIRE:
				Process_ID_USER_LASER_FIRE(raknet->p);
			default:
				// It's a client, so just show the message
				printf("%s\n", raknet->p->data);
				break;
			}
		}
	}

	// Be nice and let the server know we quit.
	raknet->client->Shutdown(300);

	// We're done with the network
	RakNet::RakPeerInterface::DestroyInstance(raknet->client);
	return 0;
}




CGameMulti::~CGameMulti()
{
}

CGameMulti::CGameMulti()
{
	//m_bLogin = false;
	//m_iSerIndex = 0;

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

	InitializeCriticalSection(&g_cs_pvLaser0);
	InitializeCriticalSection(&g_cs_pvLaser1);
	InitializeCriticalSection(&g_cs_pvTie0);

	score = 0;
	curstage = 0;
	stage = 1;

	//1p init
	m_xwing_1p.xval = 225;
	m_xwing_1p.yval = 500;
	m_xwing_1p.hp = 100;
	m_xwing_1p.dead = FALSE;
	m_xwing_1p.laserhit = FALSE;
	m_xwing_1p.laserability = 3;
	m_xwing_1p.m_killcount = 0;
	m_xwing_1p.m_life = 2;
	m_xwing_1p.m_laserable = 3;
	m_xwing_1p.m_speed = 1;
	m_xwing_1p.m_power = 1;


	EnterCriticalSection(&g_cs_pvLaser0);
	pvLaser0.clear();
	LeaveCriticalSection(&g_cs_pvLaser0);

	EnterCriticalSection(&g_cs_pvLaser1);
	pvLaser1.clear();
	LeaveCriticalSection(&g_cs_pvLaser1);

	EnterCriticalSection(&g_cs_pvTie0);
	pvTie0.clear();
	LeaveCriticalSection(&g_cs_pvTie0);

	curmissilecnt = 0;
	curlasercnt = 0;
	curioncnt = 0;

	GMAIN->m_gamebegin = timeGetTime();

	


	//this->MultiInit();


	return 0;
}

//#멀티플레이관련 init
void CGameMulti::MultiInit() {

	
	//if (MessageBox(GHWND, "IP입력?", "질문", MB_OK) == IDOK) {
	//// ToDo: IP입력처리.
	//}

	m_iMultiPlayer = 1;

	//memset(&m_raknet, 0, sizeof(m_raknet));

	DWORD dwThreadID;

	hThread = (HANDLE)_beginthreadex(0, 0, ThreadFunc, (void*)&m_raknet, 0, (unsigned int*)&dwThreadID);


	//2p init
	m_xwing_2p.xval = 425;
	m_xwing_2p.yval = 500;
	m_xwing_2p.hp = 100;
	m_xwing_2p.dead = FALSE;
	m_xwing_2p.laserhit = FALSE;
	m_xwing_2p.laserability = 3;
	m_xwing_2p.m_killcount = 0;
	m_xwing_2p.m_life = 2;
	m_xwing_2p.m_laserable = 3;
	m_xwing_2p.m_speed = 1;
	m_xwing_2p.m_power = 1;
	
}



void CGameMulti::Frame()
{
	/////////////////////////////////////////////////////////////
	//if (m_bLogin == false) return;

	//
	//m_Client.Frame();


	//I_GameUser.Frame();
	//for (int iObj = 0; iObj < m_UserList.size(); iObj++)
	//{
	//	m_UserList[iObj].Frame();
	//}

	/////////////////////////////////////////////////////////////

	//int j = 0;

	if (pvTie0.size() == 0)
	{
		srand((int)time(NULL));
	}

	if (curstage != stage)
	{
		//#새 stage 시작시 게임 로직
		this->NextStageCreate();
	}
	if (m_xwing_1p.dead != TRUE)
	{

		//GMAIN->m_pSound.Volume(SND_XWENGLP, 1000,false);
		//m_pSndEngine->Play();
		//m_pSndEngine->SetVolume(-1000);
	}

	//#주인공 키보드 입력 처리 & 이동
	this->InputMove();

	GMAIN->m_alphatime = timeGetTime() - GMAIN->m_gamebegin;

	//#적캐릭터 이동 처리
	this->CharacterMove();

	//#적 총알 발사 처리
	this->EnemyBullet();
}

void CGameMulti::NextStageCreate() {
	if (pvTie0.size() == 0)
	{
		lasercnt = 0;
		ioncnt = 0;
		missilecnt = 0;

		EnterCriticalSection(&g_cs_pvLaser0);
		pvLaser0.clear();
		LeaveCriticalSection(&g_cs_pvLaser0);

		curstage = stage;

		enemylevel = (50 + stage * 7) / 50;

		if (stage != 0 && stage % 10 == 0 && (50 + stage * 7) - 50 * m_xwing_1p.m_power >0)
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
					if (m_xwing_1p.m_life <3)
						pvTie0.push_back(new CCharacterData(10, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
					break;

				case 1://11. 레이저 아이템 생성
					if (m_xwing_1p.m_laserable <6)
						pvTie0.push_back(new CCharacterData(11, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
					break;

				case 2://12. 스피드 아이템 생성
					pvTie0.push_back(new CCharacterData(12, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
					break;

				case 3://13. 파워 아이템 생성
					if ((50 + stage * 7) - 50 * m_xwing_1p.m_power >0)
						pvTie0.push_back(new CCharacterData(13, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
					break;

				case 4://14. HP 아이템 생성
					pvTie0.push_back(new CCharacterData(14, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
					break;

				case 5://12. 스피드 아이템 생성
					pvTie0.push_back(new CCharacterData(12, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
					break;

				default://11. 레이저 아이템 생성
					if (m_xwing_1p.m_laserable <6)
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
			case 0://10. 라이프 아이템 생성
				if (m_xwing_1p.m_life <3)
					pvTie0.push_back(new CCharacterData(10, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
				break;

			case 1://11. 레이저 아이템 생성
				if (m_xwing_1p.m_laserable <6)
					pvTie0.push_back(new CCharacterData(11, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
				break;

			case 2://12. 스피드 아이템 생성
				pvTie0.push_back(new CCharacterData(12, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
				break;

			case 3://13. 파워 아이템 생성
				if ((50 + stage * 7) - 50 * m_xwing_1p.m_power >0)
					pvTie0.push_back(new CCharacterData(13, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
				break;

			case 4://14. HP 아이템 생성
				pvTie0.push_back(new CCharacterData(14, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
				break;

			case 5://12. 스피드 아이템 생성
				pvTie0.push_back(new CCharacterData(12, int(rand() % 360), 305, float(rand() % 50 + 50 - 300), FALSE, 0, 50, int(rand() % 2), FALSE));
				break;

			default://11. 레이저 아이템 생성
				if (m_xwing_1p.m_laserable <6)
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
//적 총알 발사 처리
void CGameMulti::EnemyBullet(){
	////////////////////////////////////////////////////////////////////////////////
	std::vector<CCharacterData*>::iterator _FT = pvTie0.begin();
	std::vector<CCharacterData*>::iterator _LT = pvTie0.end();

	char* attackname = "";


	for (; _FT != _LT; ++_FT)
	{
		if (*_FT == 0)
			continue;

		if ((*_FT)->xval <= m_xwing_1p.xval + 0.3 && (*_FT)->xval >= m_xwing_1p.xval - 0.3 && (*_FT)->yval >= 0 && (*_FT)->yval <= 600)
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
	//if (m_bLogin == false) return;

	dxsprite->Begin(D3DXSPRITE_ALPHABLEND);

	vcBar = D3DXVECTOR3(650, 0, 0);							//상태바위치

	m_xwing_1p.AssignVal(m_xwing_1p.xval, m_xwing_1p.yval);				//1p주인공위치
	m_xwing_2p.AssignVal(m_xwing_2p.xval, m_xwing_2p.yval);				//2p주인공위치

	//D3DXCOLOR 마지막 매개변수는 알파값을 의미하고 이 함수의 각 매개변수는 RGBA값으로서 각각 0f~1f의 값을 가진다.
	dxsprite->Draw(GMAIN->m_pGameTex[23].m_pTex, (&GMAIN->rc), NULL, &vcPos1, D3DXCOLOR(1, 1, 1, 1.f)); //배경
	dxsprite->Draw(GMAIN->m_pGameTex[24].m_pTex, (&GMAIN->rc), NULL, &vcPos2, D3DXCOLOR(1, 1, 1, 1.f)); //배경

	if (vcPos1.y >= 600) { vcPos1.y = -600; }
	else { vcPos1.y += GMAIN->m_movingdist / 20; }
	if (vcPos2.y >= 600) { vcPos2.y = -600; }
	else { vcPos2.y += GMAIN->m_movingdist / 20; }


	if (m_xwing_1p.dead == TRUE)
	{
		pvLaser1.clear();
		curlasercnt = 0;
		curioncnt = 0;
		curmissilecnt = 0;
	}

	if (m_xwing_1p.dead != TRUE)
	{
		dxsprite->Draw(GMAIN->m_pGameTex[1].m_pTex, &(GMAIN->rc), NULL, &m_xwing_1p.vcPosC, D3DXCOLOR(1, 1, 1, 1.f)); //1p주인공캐릭터
	}

	if (m_xwing_2p.dead != TRUE)
	{
		dxsprite->Draw(GMAIN->m_pGameTex[27].m_pTex, &(GMAIN->rc), NULL, &m_xwing_2p.vcPosC, D3DXCOLOR(1, 1, 1, 1.f)); //2p주인공캐릭터
	}


	/////////////////////////////////////////////////////////////////////////////
	//주인공 총알

	EnterCriticalSection(&g_cs_pvLaser0);
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
	LeaveCriticalSection(&g_cs_pvLaser0);
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
								m_xwing_1p.m_life++;
								GMAIN->m_pSound.Play(SND_BONUS, true);
								GMAIN->m_pSound.Play(SND_R2_D2, true);
							}
							else if ((*_FT)->name == "Laser")
							{
								m_xwing_1p.m_laserable++;
								GMAIN->m_pSound.Play(SND_BONUS, true);
								GMAIN->m_pSound.Play(SND_WONTFAIL, true);
							}
							else if ((*_FT)->name == "Power")
							{
								m_xwing_1p.m_power++;
								GMAIN->m_pSound.Play(SND_BONUS, true);
								GMAIN->m_pSound.Play(SND_USEFORCE, true);
							}
							else if ((*_FT)->name == "Hp")
							{
								m_xwing_1p.hp = 100;
								GMAIN->m_pSound.Play(SND_BONUS, true);
								GMAIN->m_pSound.Play(SND_WITHYOU, true);
							}
							else if ((*_FT)->name == "Speed")
							{
								m_xwing_1p.m_speed++;
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
							m_xwing_1p.m_killcount++;
						}
						if (score != 0 && score % 1000 == 0)
						{
							m_xwing_1p.m_life++;
							GMAIN->m_pSound.Play(SND_BONUS, true);
							GMAIN->m_pSound.Play(SND_R2_D2, true);
						}

						if ((m_xwing_1p.m_killcount != 0) && (m_xwing_1p.m_killcount % 6 == 0))
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
	if (m_xwing_1p.dead != TRUE)
	{
		ColCheck();	// 적 -> 주인공 충돌체크
	}
	if (m_xwing_1p.laserhit == TRUE)
	{
		boom.x = m_xwing_1p.vcPosC.x - 8;
		boom.y = m_xwing_1p.vcPosC.y - 7;
		////////////////////////////////////////////////////////////////////////////////
		m_xwing_1p.m_dTimeEnd = timeGetTime();

		if ((m_xwing_1p.m_dTimeEnd - m_xwing_1p.m_dTimeBegin)>100)
		{
			m_ImgRc2.left += 70;

			if (m_ImgRc2.left + 70 >= 1120)
			{
				m_ImgRc2.left = 0;
				m_xwing_1p.laserhit = FALSE;
			}

			m_ImgRc2.right = m_ImgRc2.left + 70;
			m_xwing_1p.m_dTimeBegin = m_xwing_1p.m_dTimeEnd;

		}
		////////////////////////////////////////////////////////////////////////////////
		dxsprite->Draw(pTex, &m_ImgRc2, NULL, &boom, D3DXCOLOR(1, 1, 1, 1.F)); //&m_vcPosImg2
	}
	if (m_xwing_1p.dead == TRUE)
	{
		m_xwing_1p.m_dTimeEnd = timeGetTime();

		if ((m_xwing_1p.m_dTimeEnd - m_xwing_1p.m_dTimeBegin)>100)
		{
			m_ImgRc2.left += 70;

			if (m_ImgRc2.left + 70 >= 1120)
			{
				m_ImgRc2.left = 0;
				//m_nGamePhase=2;
			}
			/////////////////////////////////////////////////////////////////////////////
			if (m_xwing_1p.dead == TRUE && m_xwing_1p.m_life != 0)
			{
				m_xwing_1p.xval = 325;
				m_xwing_1p.yval = 500;
				m_xwing_1p.hp = 100;
				m_xwing_1p.dead = FALSE;
				m_xwing_1p.laserhit = FALSE;
				m_xwing_1p.laserability = 3;
				--m_xwing_1p.m_life;
				EnterCriticalSection(&g_cs_pvLaser0);
				pvLaser0.clear();
				LeaveCriticalSection(&g_cs_pvLaser0);
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
			m_xwing_1p.m_dTimeBegin = m_xwing_1p.m_dTimeEnd;
		}
		if (m_xwing_1p.dead != TRUE)
		{
			dxsprite->Draw(pTex, &m_ImgRc2, NULL, &boom, D3DXCOLOR(1, 1, 1, 1.F)); //&m_vcPosImg2
		}
	}
	ColCheck3(); //충돌체크 함수3: 적과 주인공의 충돌 체크
				 ////////////////////////////////////////////////////////////////////////////////
	
	
	dxsprite->Draw(GMAIN->m_pGameTex[7].m_pTex, (&GMAIN->rc), NULL, &vcBar, D3DXCOLOR(1, 1, 1, 1.f)); //상태바
	dxsprite->End();

	//#옆에 상태바 출력처리용
	this->ProcessSideBar();
}

//옆에 상태바 출력처리용
void CGameMulti::ProcessSideBar() {
	////////////////////////////////////////////////////////////////////////////////
	GMAIN->m_text.Begin();

	hh = GMAIN->m_alphatime / 3600000;
	mm = GMAIN->m_alphatime % 3600000;
	mm = mm / 60000;
	ss = GMAIN->m_alphatime % 3600000 % 60000 / 1000;


	sprintf(scoreBuf, " %d", score);
	sprintf(killcount, " %d", m_xwing_1p.m_killcount);
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
	sprintf(lifeBuf, " Life : %d", m_xwing_1p.m_life);
	sprintf(phpBuf, " H.P : %d", m_xwing_1p.hp);
	sprintf(playerabilBuf, " Laser : %d", m_xwing_1p.m_laserable);
	sprintf(powerBuf, " Power : %d", m_xwing_1p.m_power);
	sprintf(speedBuf, " Speed : %d", m_xwing_1p.m_speed);

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
	EnterCriticalSection(&g_cs_pvLaser0);
	//주인공 총알 소멸부분
	std::vector<CLaserData*>::iterator _F = pvLaser0.begin();
	std::vector<CLaserData*>::iterator _L = pvLaser0.end();
	for (; _F != _L; ++_F) { if (*_F != 0) delete (*_F); }
	pvLaser0.clear();
	LeaveCriticalSection(&g_cs_pvLaser0);

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

	//I_DebugStr.Release();
	//m_Client.m_bExit = true;
	//I_GameUser.Release();
	//m_Client.Release();
	//m_Udp.Release();

	DeleteCriticalSection(&g_cs_pvLaser0);
	DeleteCriticalSection(&g_cs_pvLaser1);
	DeleteCriticalSection(&g_cs_pvTie0);



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

			SetRect(&rcCol2, INT(m_xwing_1p.vcPosC.x + 22)
				, INT(m_xwing_1p.vcPosC.y)
				, INT(m_xwing_1p.vcPosC.x + 22) + 11	//m_pGameTex[1].GetImageWidth()
				, INT(m_xwing_1p.vcPosC.y) + 56	//m_pGameTex[1].GetImageHeight()
				);

			SetRect(&rcColSound, INT(m_xwing_1p.vcPosC.x - 112)
				, INT(m_xwing_1p.vcPosC.y)
				, INT(m_xwing_1p.vcPosC.x - 112) + 275	//m_pGameTex[1].GetImageWidth()
				, INT(m_xwing_1p.vcPosC.y + 56)			//+ m_pGameTex[1].GetImageHeight()
				);
		}
		else
		{
			SetRect(&rcCol1, INT(TempPos.x)
				, INT(TempPos.y)
				, INT(TempPos.x) + GMAIN->m_pGameTex[4].GetImageWidth()
				, INT(TempPos.y) + GMAIN->m_pGameTex[4].GetImageHeight()
				);

			SetRect(&rcCol2, INT(m_xwing_1p.vcPosC.x + 22)
				, INT(m_xwing_1p.vcPosC.y)
				, INT(m_xwing_1p.vcPosC.x + 22) + 11		//m_pGameTex[1].GetImageWidth()
				, INT(m_xwing_1p.vcPosC.y) + 56		//m_pGameTex[1].GetImageHeight()
				);
			SetRect(&rcColSound, INT(m_xwing_1p.vcPosC.x - 112)
				, INT(m_xwing_1p.vcPosC.y)
				, INT(m_xwing_1p.vcPosC.x - 112) + 275	//m_pGameTex[1].GetImageWidth()
				, INT(m_xwing_1p.vcPosC.y + 56)			//+ m_pGameTex[1].GetImageHeight()
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
			//사운드 끝


			m_xwing_1p.hp = m_xwing_1p.hp - (*_F)->damage; //주인공 캐릭터 체력 - (*_F)->damage
			if (m_xwing_1p.hp <= 0)
			{
				m_xwing_1p.dead = TRUE;

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
			m_xwing_1p.laserhit = TRUE;
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

//충돌체크 함수2: 주인공총알이 적에 맞았는지 체크
INT CGameMulti::ColCheck2()
{
	D3DXVECTOR3	TempPos;
	INT bColl = 0;
	EnterCriticalSection(&g_cs_pvLaser0);
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
				//사운드 끝

				(*_FT)->hp = (*_FT)->hp - 50 * m_xwing_1p.m_power;		//적케릭터 체력 -50

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
	LeaveCriticalSection(&g_cs_pvLaser0);

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

		SetRect(&rcCol2, INT(m_xwing_1p.vcPosC.x - 112), INT(m_xwing_1p.vcPosC.y - 112)
			, INT(m_xwing_1p.vcPosC.x - 112) + 275
			, INT(m_xwing_1p.vcPosC.y) + 112);

		SetRect(&rcCol3, INT(m_xwing_1p.vcPosC.x + 22)
			, INT(m_xwing_1p.vcPosC.y)
			, INT(m_xwing_1p.vcPosC.x + 22) + 11//m_pGameTex[1].GetImageWidth()
			, INT(m_xwing_1p.vcPosC.y) + 56//m_pGameTex[1].GetImageHeight()
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

void	CGameMulti::Send_ID_USER_MOVE(float fPosX, float fPosY, int iDirection )
{
	char message[2048];
	int ping = m_raknet.client->GetAveragePing(m_raknet.client->GetSystemAddressFromIndex(0));
	long lTemp;

	TID_USER_MOVE packet;
	packet.typeId = ID_USER_MOVE;

	lTemp = (long)(fPosX*10.0f);
	packet.data.posX = (unsigned short)lTemp;
	lTemp = (long)(fPosY*10.0f);
	packet.data.posY = (unsigned short)lTemp;
	packet.data.direction = iDirection;

	if (m_iMultiPlayer != 0) {
		packet.data.user_idx = m_iMultiPlayer;
	}

	memcpy(message, &packet, sizeof(packet));
	//strcpy(message, "right");
	m_raknet.client->Send(message, sizeof(packet), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	//m_raknet.client->Send(message, (int)strlen(message) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);

#ifdef _DEBUG
	char buf[80];
	sprintf(buf, "ID_USER_MOVE user: %d, dir:%d, posX:%hu, posY:%hu, ping:%d \n", packet.data.user_idx, iDirection, packet.data.posX, packet.data.posY, ping);
	OutputDebugString(buf);
#endif
}




void	CGameMulti::Send_ID_USER_LASER_FIRE(float fPosX, float fPosY)
{
	TID_USER_LASER_FIRE packet;

	char message[2048];
	int ping = m_raknet.client->GetAveragePing(m_raknet.client->GetSystemAddressFromIndex(0));
	long lTemp;

	if (m_iMultiPlayer == 1) { //1p

		EnterCriticalSection(&g_cs_pvLaser0);
		
 		unsigned short usTemp = CreateUniqueID(pvLaser0);

		if (pvLaser0.size() < m_xwing_1p.m_laserable)
		{
  			pvLaser0.push_back(new CLaserData(m_xwing_1p.xval, m_xwing_1p.yval, FALSE, usTemp)); // 주인공 총알 발생

			GMAIN->m_pSound.Play(SND_XWLSR1, true);
		}

		LeaveCriticalSection(&g_cs_pvLaser0);




		packet.typeId = ID_USER_LASER_FIRE;

		lTemp = (long)(fPosX*10.0f);
		packet.data.posX = (unsigned short)lTemp;
		lTemp = (long)(fPosY*10.0f);
		packet.data.posY = (unsigned short)lTemp;
		packet.data.direction = DIRECTION_UU;
		packet.data.id = usTemp;

		if (m_iMultiPlayer != 0) {
			packet.data.user_idx = m_iMultiPlayer;
		}

		memcpy(message, &packet, sizeof(packet));
		//strcpy(message, "right");
		m_raknet.client->Send(message, sizeof(packet), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		//m_raknet.client->Send(message, (int)strlen(message) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	}
	else { //2p
	}
	

#ifdef _DEBUG
	char buf[80];
	sprintf(buf, "ID_USER_LASER_FIRE user:%d, posX:%hu, posY:%hu, id:%hu, ping:%d \n", packet.data.user_idx, packet.data.posX, packet.data.posY, packet.data.id, ping);
	OutputDebugString(buf);
#endif
}


void	CGameMulti::Send_ID_USER_LASER_MOVE(unsigned short id, float fPosX, float fPosY)
{
	char message[2048];
	int ping = m_raknet.client->GetAveragePing(m_raknet.client->GetSystemAddressFromIndex(0));
	long lTemp;

	TID_USER_LASER_FIRE packet;
	packet.typeId = ID_USER_LASER_FIRE;

	lTemp = (long)(fPosX*10.0f);
	packet.data.posX = (unsigned short)lTemp;
	lTemp = (long)(fPosY*10.0f);
	packet.data.posY = (unsigned short)lTemp;
	packet.data.direction = DIRECTION_UU;

	if (m_iMultiPlayer != 0) {
		packet.data.user_idx = m_iMultiPlayer;
	}

	memcpy(message, &packet, sizeof(packet));
	//strcpy(message, "right");
	m_raknet.client->Send(message, sizeof(packet), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	//m_raknet.client->Send(message, (int)strlen(message) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);

#ifdef _DEBUG
	char buf[80];
	sprintf(buf, "ID_USER_LASER_FIRE user:%d, posX:%hu, posY:%hu, ping:%d \n", packet.data.user_idx, packet.data.posX, packet.data.posY, ping);
	OutputDebugString(buf);
#endif
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


	if (m_xwing_1p.dead != TRUE)
	{
		if (GMAIN->m_pInput->KeyPress(VK_RIGHT))
		{
#ifdef MULTIIMPLE
			Send_ID_USER_MOVE( GMAIN->m_movingdist*m_xwing_1p.m_speed, 0, DIRECTION_RR);
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
			Send_ID_USER_MOVE( GMAIN->m_movingdist*m_xwing_1p.m_speed, 0, DIRECTION_LL);
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
			Send_ID_USER_MOVE( 0, GMAIN->m_movingdist*m_xwing_1p.m_speed, DIRECTION_UU);
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
			Send_ID_USER_MOVE(0, GMAIN->m_movingdist*m_xwing_1p.m_speed, DIRECTION_DD);
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
			Send_ID_USER_LASER_FIRE(m_xwing_1p.xval, m_xwing_1p.yval);
#else
			if (pvLaser0.size() < m_xwing.m_laserable)
			{
				pvLaser0.push_back(new CLaserData(m_xwing.xval, m_xwing.yval, FALSE)); // 주인공 총알 발생

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



// Copied from Multiplayer.cpp
// If the first byte is ID_TIMESTAMP, then we want the 5th byte
// Otherwise we want the 1st byte
unsigned char GetPacketIdentifier(RakNet::Packet *p)
{
	if (p == 0)
		return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
	{
		RakAssert(p->length > sizeof(RakNet::MessageID) + sizeof(RakNet::Time));
		return (unsigned char)p->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
	}
	else
		return (unsigned char)p->data[0];
}

