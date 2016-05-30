#include "_StdAfx.h"
#include "Main.h"
#include "LaserData.h"

CLaserData::CLaserData(char* _name,float _laserxval, float _laseryval)
{
	if(_name=="Laser")
	{
		laserxval=_laserxval;								//�Ѿ� ��ġ
		laseryval=_laseryval;								//�Ѿ� ��ġ
		laserhit=FALSE;									//�Ѿ��� �¾Ҵ���?
		flybysound=FALSE;									//�Ѿ��� �������� �Ҹ������� FALSE:�Ҹ��� �ȳ� TRUE: �Ҹ��� ��
		damage=30;											//�Ѿ� �Ŀ�
		speed=1;											//�Ѿ� ���ǵ�
		name=_name;										//�Ѿ� �̸�
		vcPosC=D3DXVECTOR3(_laserxval,_laseryval,0);	//��ġ
	}
	else if(_name=="Ion")
	{
		laserxval=_laserxval;								//�Ѿ� ��ġ
		laseryval=_laseryval;								//�Ѿ� ��ġ
		laserhit=FALSE;									//�Ѿ��� �¾Ҵ���?
		flybysound=FALSE;									//�Ѿ��� �������� �Ҹ������� FALSE:�Ҹ��� �ȳ� TRUE: �Ҹ��� ��
		damage=50;											//�Ѿ� �Ŀ�
		speed=2;											//�Ѿ� ���ǵ�
		name=_name;										//�Ѿ� �̸�
		vcPosC=D3DXVECTOR3(_laserxval,_laseryval,0);	//��ġ	
	}
	else if(_name=="Missile")
	{
		laserxval=_laserxval;								//�Ѿ� ��ġ
		laseryval=_laseryval;								//�Ѿ� ��ġ
		laserhit=FALSE;									//�Ѿ��� �¾Ҵ���?
		flybysound=FALSE;									//�Ѿ��� �������� �Ҹ������� FALSE:�Ҹ��� �ȳ� TRUE: �Ҹ��� ��
		damage=100;										//�Ѿ� �Ŀ�
		speed=3;											//�Ѿ� ���ǵ�
		name=_name;										//�Ѿ� �̸�
		vcPosC=D3DXVECTOR3(_laserxval,_laseryval,0);	//��ġ
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