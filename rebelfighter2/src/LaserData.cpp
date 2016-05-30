#include "_StdAfx.h"
#include "Main.h"
#include "LaserData.h"

CLaserData::CLaserData(char* _name,float _laserxval, float _laseryval)
{
	if(_name=="Laser")
	{
		laserxval=_laserxval;								//총알 위치
		laseryval=_laseryval;								//총알 위치
		laserhit=FALSE;									//총알이 맞았는지?
		flybysound=FALSE;									//총알이 지나가면 소리나도록 FALSE:소리가 안남 TRUE: 소리가 남
		damage=30;											//총알 파워
		speed=1;											//총알 스피드
		name=_name;										//총알 이름
		vcPosC=D3DXVECTOR3(_laserxval,_laseryval,0);	//위치
	}
	else if(_name=="Ion")
	{
		laserxval=_laserxval;								//총알 위치
		laseryval=_laseryval;								//총알 위치
		laserhit=FALSE;									//총알이 맞았는지?
		flybysound=FALSE;									//총알이 지나가면 소리나도록 FALSE:소리가 안남 TRUE: 소리가 남
		damage=50;											//총알 파워
		speed=2;											//총알 스피드
		name=_name;										//총알 이름
		vcPosC=D3DXVECTOR3(_laserxval,_laseryval,0);	//위치	
	}
	else if(_name=="Missile")
	{
		laserxval=_laserxval;								//총알 위치
		laseryval=_laseryval;								//총알 위치
		laserhit=FALSE;									//총알이 맞았는지?
		flybysound=FALSE;									//총알이 지나가면 소리나도록 FALSE:소리가 안남 TRUE: 소리가 남
		damage=100;										//총알 파워
		speed=3;											//총알 스피드
		name=_name;										//총알 이름
		vcPosC=D3DXVECTOR3(_laserxval,_laseryval,0);	//위치
	}
	else
	{
	}
}

void CLaserData::LaserDraw(char* name,RECT rc,std::vector<CLaserData*>::iterator _F)
{
	if(name=="Laser")
	{
		GMAIN->m_pd3dSprite->Draw(GMAIN->m_pGameTex[4].m_pTex,&rc,NULL,&(*_F)->vcPosC,D3DXCOLOR(1,1,1,1.f));
	}
	else if(name=="Ion")
	{
		GMAIN->m_pd3dSprite->Draw(GMAIN->m_pGameTex[16].m_pTex,&rc,NULL,&(*_F)->vcPosC,D3DXCOLOR(1,1,1,1.f));
	}
	else if(name=="Missile")
	{
		GMAIN->m_pd3dSprite->Draw(GMAIN->m_pGameTex[17].m_pTex,&rc,NULL,&(*_F)->vcPosC,D3DXCOLOR(1,1,1,1.f));
	}
	else
	{
	}
}