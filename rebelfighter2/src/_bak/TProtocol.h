
#ifndef SAMPLE_MUTEX
#define SAMPLE_MUTEX

#pragma  pack(push, 1 )

typedef struct {
	WORD	len;	// msg ����Ʈ ũ��
	WORD	type;	// � ��Ŷ�̴�.
}PACKET_HEADER;

typedef struct {
	PACKET_HEADER	ph;
	char			msg[1024];
}UPACKET, *P_UPACKET;

typedef struct {
	WORD	user_idx;
	WORD	posX;
	WORD	posY;
	WORD	direction; //0 ~7 8����
}TPACKET_USER_POSITION;

typedef struct {
	WORD	user_idx;
	WORD	posX;
	WORD	posY;
	WORD	direction; //0 ~7 8����
}TPACKET_USER_PARTNER_MAKE;

typedef struct {
	WORD	user_idx;
	WORD	posX;
	WORD	posY;
	WORD	direction; //0 ~7 8����
}TPACKET_USER_PARTNER_POSITION;

typedef struct {
	WORD	user_idx;
	WORD	posX;
	WORD	posY;
	WORD	direction; //0 ~7 8����
}TPACKET_USER_LOGIN;

typedef struct {
	WORD	user_idx;
	WORD	posX;
	WORD	posY;
	WORD	direction; //0 ~7 8����
}TPACKET_USER_MAKEROOM;

typedef struct {
	WORD	user_idx;
	WORD	posX;
	WORD	posY;
	WORD	direction; //0 ~7 8����
}TPACKET_USER_ENTERTHEROOM;

typedef struct {
	WORD	user_idx;
	WORD	posX;
	WORD	posY;
	WORD	direction; //0 ~7 8����
}TPACKET_USER_GAMESTART;

typedef struct {
	WORD	user_idx;
	WORD	posX;
	WORD	posY;
	WORD	direction; //0 ~7 8����
}TPACKET_USER_GETROOMLISTINFO;

typedef struct {
	WORD	user_idx;
	WORD	posX;
	WORD	posY;
	WORD	direction; //0 ~7 8����
}TPACKET_USER_GETTHEROOMINFO;

#define PACKET_USER_POSITION				3000
#define PACKET_USER_PARTNER_MAKE			3001
#define PACKET_USER_PARTNER_POSITION		3002
#define PACKET_USER_LOGIN					4000
#define PACKET_USER_MAKEROOM				5000
#define PACKET_USER_ENTERTHEROOM			6000
#define PACKET_USER_GAMESTART				7000
#define PACKET_USER_GETROOMLISTINFO			8000
#define PACKET_USER_GETTHEROOMINFO			9000








//typedef struct {
//	int		user_idx;
//	char	uid[21];
//	char	ups[15];
//}TPACKET_USER_LOGIN;
//#define PACKET_USER_LOGIN	2000

// ����
#define PACKET_CHAT_NAME_REQ 1000 // ��ȭ�� �Է��ض�.
// Ŭ���̾�Ʈ
#define PACKET_CHAT_NAME_ACK 1001 // ��ȭ�� �Է� ����.
// ���� ä�� �޼���
#define PACKET_CHAT_MSG 1002 // ��ȭ�� �Է� ����.

#pragma pack(pop)

#endif