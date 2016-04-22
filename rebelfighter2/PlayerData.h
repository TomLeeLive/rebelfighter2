#pragma once

class CPlayerData : public CCharacterData
{
public:
	int m_force;		//포스 기술 가능 횟수
	int m_shield;		//쉴드 사용 가능 횟수
	UINT m_power;		//레이저 파워 50x(파워)
	UINT m_laserable; //레이저 발수 가능 횟수
	int m_speed;		//이동속도
	int m_life;		//생명
	int m_killcount;	//격추횟수
	int m_shotcount;	//레이저 쏜 횟수
	int m_misscount;	//레이저 맞추지 못한 횟수
	
	CPlayerData(void);
	~CPlayerData(void);
};