#pragma once

class CCharacterData
{
public:
	char*			name;					//�ɸ��� �̸�
	float			xval,yval;				//ĳ����(���ΰ�,��) ��ġ
	int				laserhit;				//�Ѿ��� �¾Ҵ���?

	bool			laserable;				//������ ����? TRUE: ���� FALSE: �Ұ���
	UINT			laserability;			//������ �߻�߼�

	bool			ionable;				//�̿� ����? TRUE: ���� FALSE: �Ұ���
	UINT			ionability;			//�̿� �߻�߼�

	bool			missileable;			//�̻��� ����? TRUE: ���� FALSE: �Ұ���
	UINT			missileability;		//�̻��� �߻�߼�
	
	int				hp;						//ü��
	int				dead;					//�׾�����? FALSE: ���� TRUE: ����
	D3DXVECTOR3	vcPosC;					//��ġ
	DWORD			m_dTimeBegin;			// ���� Ÿ��
	DWORD			m_dTimeEnd;			// �� Ÿ��
	int				k;						//�� �¿� �̵� ī��Ʈ 0:�¿��� ��� 1:�쿡�� �·�
	int				speed;					//���ǵ�
	int				position;				//����
	bool			flybysound;			//�������� �Ҹ������� FALSE:�Ҹ��� �ȳ� TRUE: �Ҹ��� ��

	int				point;					//���⸦ ���ָ� +�� ����Ʈ
				
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