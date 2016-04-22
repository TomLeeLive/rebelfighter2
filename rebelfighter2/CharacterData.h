#pragma once

class CCharacterData
{
public:
	char*			name;					//케릭터 이름
	float			xval,yval;				//캐릭터(주인공,적) 위치
	int				laserhit;				//총알이 맞았는지?

	bool			laserable;				//레이저 가능? TRUE: 가능 FALSE: 불가능
	UINT			laserability;			//레이저 발사발수

	bool			ionable;				//이온 가능? TRUE: 가능 FALSE: 불가능
	UINT			ionability;			//이온 발사발수

	bool			missileable;			//미사일 가능? TRUE: 가능 FALSE: 불가능
	UINT			missileability;		//미사일 발사발수
	
	int				hp;						//체력
	int				dead;					//죽었는지? FALSE: 살음 TRUE: 죽음
	D3DXVECTOR3	vcPosC;					//위치
	DWORD			m_dTimeBegin;			// 시작 타임
	DWORD			m_dTimeEnd;			// 끝 타임
	int				k;						//적 좌우 이동 카운트 0:좌에서 우로 1:우에서 좌로
	int				speed;					//스피드
	int				position;				//위상
	bool			flybysound;			//지나가면 소리나도록 FALSE:소리가 안남 TRUE: 소리가 남

	int				point;					//적기를 없애면 +될 포인트
				
	CCharacterData(){};
	CCharacterData::~CCharacterData(void);

	CCharacterData(char* _name, int _pos, float _xval, float _yval,int _laserhit, UINT _laserability,int _hp, int _k,int _dead);
	
	CCharacterData(int _num, int _pos, 
		float _xval, float _yval,
		int _laserhit, UINT _laserability,
		int _hp, int _k,
		int _dead);
	void AssignVal(float _xval, float _yval)
	{
		vcPosC=D3DXVECTOR3(_xval,_yval,0);
	}
	void CharacterDraw(char* name,RECT rc,std::vector<CCharacterData*>::iterator _FT);

};