#pragma once

class CPlayerData : public CCharacterData
{
public:
	int m_force;		//���� ��� ���� Ƚ��
	int m_shield;		//���� ��� ���� Ƚ��
	UINT m_power;		//������ �Ŀ� 50x(�Ŀ�)
	UINT m_laserable; //������ �߼� ���� Ƚ��
	int m_speed;		//�̵��ӵ�
	int m_life;		//����
	int m_killcount;	//����Ƚ��
	int m_shotcount;	//������ �� Ƚ��
	int m_misscount;	//������ ������ ���� Ƚ��
	
	CPlayerData(void);
	~CPlayerData(void);
};