#pragma once

class CLaserData
{
public:
	float			laserxval,laseryval;			//총알 위치
	int				laserhit;						//총알이 맞았는지?
	bool			flybysound;					//총알이 지나가면 소리나도록 FALSE:소리가 안남 TRUE: 소리가 남
	int				damage;							//총알 파워
	int				speed;							//총알 스피드
	char*			name;							//총알 이름
	D3DXVECTOR3	vcPosC;							//위치


	CLaserData(){};

	CLaserData(char* _name,float _laserxval, float _laseryval);

	CLaserData(float _laserxval, float _laseryval, int _laserhit)
	{
		laserxval=_laserxval;
		laseryval=_laseryval;
		laserhit=_laserhit;
		vcPosC=D3DXVECTOR3(laserxval,laseryval,0);
		speed=1;
		name="Laser";
		damage=10;
		flybysound=FALSE;
	}
	void LaserDraw(char* name,RECT rc,std::vector<CLaserData*>::iterator _F);
};