#include "_StdAfx.h"
//#include "Main.h"
//#include "CharacterData.h"

CCharacterData::~CCharacterData(void)
{
}

CCharacterData::CCharacterData(int _num, int _pos, float _xval, float _yval,
							   int _laserhit, UINT _laserability,
							   int _hp, int _k,
							   int _dead)
{
	if(_num==1)
	{
		name="Tie";position=_pos;xval=_xval;yval=_yval;
		laserhit=_laserhit;laserability=_laserability;
		hp=_hp*7;k=_k;dead=_dead;speed=9;flybysound=FALSE;

		laserable=TRUE;			//레이저 가능? TRUE: 가능 FALSE: 불가능
		laserability=2;			//레이저 발사발수
		ionable=FALSE;				//이온 가능? TRUE: 가능 FALSE: 불가능
		ionability=0;				//이온 발사발수
		missileable=FALSE;		//미사일 가능? TRUE: 가능 FALSE: 불가능
		missileability=0;			//미사일 발사발수
		point=10;
	}
	else if(_num==2)
	{
		name="Tie2";position=_pos;xval=_xval;yval=_yval;
		laserhit=_laserhit;laserability=_laserability;
		hp=_hp*7;k=_k;dead=_dead;speed=8;flybysound=FALSE;

		laserable=TRUE;			//레이저 가능? TRUE: 가능 FALSE: 불가능
		laserability=3;			//레이저 발사발수
		ionable=TRUE;				//이온 가능? TRUE: 가능 FALSE: 불가능
		ionability=1;				//이온 발사발수
		missileable=FALSE;		//미사일 가능? TRUE: 가능 FALSE: 불가능
		missileability=0;			//미사일 발사발수
		point=10;
	}
	else if(_num==3)
	{
		name="Bomber";position=_pos;xval=_xval;yval=_yval;
		laserhit=_laserhit;laserability=_laserability;
		hp=50+_hp*7;k=_k;dead=_dead;speed=7;flybysound=FALSE;

		laserable=TRUE;			//레이저 가능? TRUE: 가능 FALSE: 불가능
		laserability=1;			//레이저 발사발수
		ionable=FALSE;				//이온 가능? TRUE: 가능 FALSE: 불가능
		ionability=0;				//이온 발사발수
		missileable=TRUE;			//미사일 가능? TRUE: 가능 FALSE: 불가능
		missileability=1;			//미사일 발사발수
		point=10;
	}
	else if(_num==4)
	{
		name="Bomber2";position=_pos;xval=_xval;yval=_yval;
		laserhit=_laserhit;laserability=_laserability;
		hp=50+_hp*7;k=_k;dead=_dead;speed=6;flybysound=FALSE;

		laserable=TRUE;			//레이저 가능? TRUE: 가능 FALSE: 불가능
		laserability=2;			//레이저 발사발수
		ionable=TRUE;				//이온 가능? TRUE: 가능 FALSE: 불가능
		ionability=1;				//이온 발사발수
		missileable=TRUE;			//미사일 가능? TRUE: 가능 FALSE: 불가능
		missileability=1;			//미사일 발사발수
		point=10;
	}
	else if(_num==5)
	{
		name="BomberAdv";position=_pos;xval=_xval;yval=_yval;
		laserhit=_laserhit;laserability=_laserability;
		hp=100+_hp*7;k=_k;dead=_dead;speed=5;flybysound=FALSE;

		laserable=TRUE;			//레이저 가능? TRUE: 가능 FALSE: 불가능
		laserability=2;			//레이저 발사발수
		ionable=TRUE;				//이온 가능? TRUE: 가능 FALSE: 불가능
		ionability=1;				//이온 발사발수
		missileable=TRUE;			//미사일 가능? TRUE: 가능 FALSE: 불가능
		missileability=2;			//미사일 발사발수
		point=10;
	}
	else if(_num==6)
	{
		name="BomberAdv2";position=_pos;xval=_xval;yval=_yval;
		laserhit=_laserhit;laserability=_laserability;
		hp=100+_hp*7;k=_k;dead=_dead;speed=4;flybysound=FALSE;

		laserable=TRUE;			//레이저 가능? TRUE: 가능 FALSE: 불가능
		laserability=3;			//레이저 발사발수
		ionable=TRUE;				//이온 가능? TRUE: 가능 FALSE: 불가능
		ionability=1;				//이온 발사발수
		missileable=TRUE;			//미사일 가능? TRUE: 가능 FALSE: 불가능
		missileability=2;			//미사일 발사발수
		point=10;
	}
	else if(_num==7)
	{
		name="Interceptor";position=_pos;xval=_xval;yval=_yval;
		laserhit=_laserhit;laserability=_laserability;hp=50+_hp*7;
		k=_k;dead=_dead;speed=3;flybysound=FALSE;

		laserable=TRUE;			//레이저 가능? TRUE: 가능 FALSE: 불가능
		laserability=2;			//레이저 발사발수
		ionable=TRUE;				//이온 가능? TRUE: 가능 FALSE: 불가능
		ionability=2;				//이온 발사발수
		missileable=FALSE;		//미사일 가능? TRUE: 가능 FALSE: 불가능
		missileability=0;			//미사일 발사발수
		point=10;
	}
	else if(_num==8)
	{
		name="Advanced";position=_pos;xval=_xval;yval=_yval;
		laserhit=_laserhit;laserability=_laserability;
		hp=100+_hp*7;k=_k;dead=_dead;speed=2;flybysound=FALSE;

		laserable=TRUE;			//레이저 가능? TRUE: 가능 FALSE: 불가능
		laserability=2;			//레이저 발사발수
		ionable=TRUE;				//이온 가능? TRUE: 가능 FALSE: 불가능
		ionability=2;				//이온 발사발수
		missileable=TRUE;			//미사일 가능? TRUE: 가능 FALSE: 불가능
		missileability=1;			//미사일 발사발수
		point=10;
	}
	else if(_num==9)
	{
		name="Boss";position=_pos;xval=_xval;yval=_yval;
		laserhit=_laserhit;laserability=_laserability;
		hp=300+_hp*7;k=_k;dead=_dead;speed=1;flybysound=FALSE;

		laserable=TRUE;			//레이저 가능? TRUE: 가능 FALSE: 불가능
		laserability=4;			//레이저 발사발수
		ionable=TRUE;				//이온 가능? TRUE: 가능 FALSE: 불가능
		ionability=3;				//이온 발사발수
		missileable=TRUE;			//미사일 가능? TRUE: 가능 FALSE: 불가능
		missileability=2;			//미사일 발사발수
		point=10;
	}
	else if(_num==10)
	{
		name="Life";position=0;xval=_xval;yval=_yval;
		laserhit=_laserhit;laserability=_laserability;
		hp=_hp;k=_k;dead=_dead;speed=2;flybysound=FALSE;

		laserable=FALSE;laserability=0;ionable=FALSE;				
		ionability=0;	missileable=FALSE;missileability=0;	point=0;
	}
	else if(_num==11)
	{
		name="Laser";position=0;xval=_xval;yval=_yval;
		laserhit=_laserhit;laserability=_laserability;
		hp=_hp;k=_k;dead=_dead;speed=2;flybysound=FALSE;

		laserable=FALSE;laserability=0;ionable=FALSE;				
		ionability=0;	missileable=FALSE;missileability=0;	point=0;
	}
	else if(_num==12)
	{
		name="Speed";position=0;xval=_xval;yval=_yval;
		laserhit=_laserhit;laserability=_laserability;
		hp=_hp;k=_k;dead=_dead;speed=2;flybysound=FALSE;

		laserable=FALSE;laserability=0;ionable=FALSE;				
		ionability=0;	missileable=FALSE;missileability=0;	point=0;
	}
	else if(_num==13)
	{
		name="Power";position=0;xval=_xval;yval=_yval;
		laserhit=_laserhit;laserability=_laserability;
		hp=_hp;k=_k;dead=_dead;speed=2;flybysound=FALSE;

		laserable=FALSE;laserability=0;ionable=FALSE;				
		ionability=0;	missileable=FALSE;missileability=0;	point=0;
	}
	else if(_num==14)
	{
		name="Hp";position=0;xval=_xval;yval=_yval;
		laserhit=_laserhit;laserability=_laserability;
		hp=_hp;k=_k;dead=_dead;speed=2;flybysound=FALSE;

		laserable=FALSE;laserability=0;ionable=FALSE;				
		ionability=0;	missileable=FALSE;missileability=0;	point=0;
	}
	else
	{
		name="Tie";position=0;xval=_xval;yval=_yval;
		laserhit=_laserhit;laserability=_laserability;
		hp=_hp*7;k=_k;dead=_dead;speed=9;flybysound=FALSE;

		laserable=TRUE;			//레이저 가능? TRUE: 가능 FALSE: 불가능
		laserability=1;			//레이저 발사발수
		ionable=FALSE;				//이온 가능? TRUE: 가능 FALSE: 불가능
		ionability=0;				//이온 발사발수
		missileable=FALSE;		//미사일 가능? TRUE: 가능 FALSE: 불가능
		missileability=0;			//미사일 발사발수
		point=10;
	}

}

void CCharacterData::CharacterDraw(char* name,RECT rc,std::vector<CCharacterData*>::iterator _FT)
{
//struct T
//{
//	char * name;
//	int nIdx;
//} t[] ={ 
//		{"Tie", 3},
//		{"Tie2", 8},
//		{"Bomber",9},
//		{"Bomber2",10},
//		{"BomberAdv",11},
//		{"BomberAdv2",12},
//		{"Interceptor",13},
//		{"Advanced",14},
//		{NULL, 0},
//		};
//
//int nCnt = 0;
//
//while( !t[nCnt].name)
//{
//	if(0== _stricmp(t[nCnt].name, name))
//	{
//		GMAIN->m_pd3dSprite->Draw(GMAIN->m_pGameTex[ t[nCnt].nIdx ].m_pTex,&rc,NULL,&(*_FT)->vcPosC,D3DXCOLOR(1,1,1,1.f));
//		break;
//	}
//	nCnt++;
//}

	if(name=="Tie")
	{
		GMAIN->m_pd3dSprite->Draw(GMAIN->m_pGameTex[3].m_pTex,&rc,NULL,&(*_FT)->vcPosC,D3DXCOLOR(1,1,1,1.f));
	}

	else if(name=="Tie2")
	{
		GMAIN->m_pd3dSprite->Draw(GMAIN->m_pGameTex[8].m_pTex,&rc,NULL,&(*_FT)->vcPosC,D3DXCOLOR(1,1,1,1.f));
	}
	
	else if(name=="Bomber")
	{
		GMAIN->m_pd3dSprite->Draw(GMAIN->m_pGameTex[9].m_pTex,&rc,NULL,&(*_FT)->vcPosC,D3DXCOLOR(1,1,1,1.f));
	}
	
	else if(name=="Bomber2")
	{
		GMAIN->m_pd3dSprite->Draw(GMAIN->m_pGameTex[10].m_pTex,&rc,NULL,&(*_FT)->vcPosC,D3DXCOLOR(1,1,1,1.f));
	}
	
	else if(name=="BomberAdv")
	{
		GMAIN->m_pd3dSprite->Draw(GMAIN->m_pGameTex[11].m_pTex,&rc,NULL,&(*_FT)->vcPosC,D3DXCOLOR(1,1,1,1.f));
	}
	
	else if(name=="BomberAdv2")
	{
		GMAIN->m_pd3dSprite->Draw(GMAIN->m_pGameTex[12].m_pTex,&rc,NULL,&(*_FT)->vcPosC,D3DXCOLOR(1,1,1,1.f));
	}
	
	 else if(name=="Interceptor")
	{
		GMAIN->m_pd3dSprite->Draw(GMAIN->m_pGameTex[13].m_pTex,&rc,NULL,&(*_FT)->vcPosC,D3DXCOLOR(1,1,1,1.f));
	}
	
	 else if(name=="Advanced")
	{
		GMAIN->m_pd3dSprite->Draw(GMAIN->m_pGameTex[14].m_pTex,&rc,NULL,&(*_FT)->vcPosC,D3DXCOLOR(1,1,1,1.f));
	}
	 else if(name=="Boss")
	 {
		 GMAIN->m_pd3dSprite->Draw(GMAIN->m_pGameTex[15].m_pTex,&rc,NULL,&(*_FT)->vcPosC,D3DXCOLOR(1,1,1,1.f));
	 }
	 else if(name=="Life")
	 {
		 GMAIN->m_pd3dSprite->Draw(GMAIN->m_pGameTex[18].m_pTex,&rc,NULL,&(*_FT)->vcPosC,D3DXCOLOR(1,1,1,1.f));
	 }
	 else if(name=="Laser")
	 {
		 GMAIN->m_pd3dSprite->Draw(GMAIN->m_pGameTex[19].m_pTex,&rc,NULL,&(*_FT)->vcPosC,D3DXCOLOR(1,1,1,1.f));
	 }
	 else if(name=="Speed")
	 {
		 GMAIN->m_pd3dSprite->Draw(GMAIN->m_pGameTex[20].m_pTex,&rc,NULL,&(*_FT)->vcPosC,D3DXCOLOR(1,1,1,1.f));
	 }
	 else if(name=="Power")
	 {
		 GMAIN->m_pd3dSprite->Draw(GMAIN->m_pGameTex[21].m_pTex,&rc,NULL,&(*_FT)->vcPosC,D3DXCOLOR(1,1,1,1.f));
	 }
	 else if(name=="Hp")
	 {
		 GMAIN->m_pd3dSprite->Draw(GMAIN->m_pGameTex[22].m_pTex,&rc,NULL,&(*_FT)->vcPosC,D3DXCOLOR(1,1,1,1.f));
	 }
	 else{}
}