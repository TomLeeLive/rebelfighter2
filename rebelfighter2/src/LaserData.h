#pragma once

class CLaserData
{
public:
	float			laserxval,laseryval;			//�Ѿ� ��ġ
	int				laserhit;						//�Ѿ��� �¾Ҵ���?
	bool			flybysound;						//�Ѿ��� �������� �Ҹ������� FALSE:�Ҹ��� �ȳ� TRUE: �Ҹ��� ��
	int				damage;							//�Ѿ� �Ŀ�
	int				speed;							//�Ѿ� ���ǵ�
	char*			name;							//�Ѿ� �̸�
	D3DXVECTOR3		vcPosC;							//��ġ
	unsigned short	id;								//��Ʈ��ũ �÷��̸� ���� id


	CLaserData(){};

	CLaserData(char* _name,float _laserxval, float _laseryval);

	CLaserData(float _laserxval, float _laseryval, int _laserhit, unsigned short _id = 0)
	{
		laserxval=_laserxval;
		laseryval=_laseryval;
		laserhit=_laserhit;
		vcPosC=D3DXVECTOR3(laserxval,laseryval,0);
		speed=1;
		name="Laser";
		damage=10;
		flybysound=FALSE;
		id = _id;
	}
	void LaserDraw(char* name,RECT rc,std::vector<CLaserData*>::iterator _F);
};