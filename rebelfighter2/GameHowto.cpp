#include "_StdAfx.h"

//extern  CDrawText		GMAIN->m_text;

INT CGameHowto::Init() { return 0; };
void CGameHowto::Frame() 
{
	//m_pMp[1]->Reset();//m_pSndGameMenu->Reset();
	//m_pMp[1]->Stop();//m_pSndGameMenu->Stop();

	if (0 == GMAIN->m_KeyOld[VK_RETURN] && GMAIN->m_KeyCur[VK_RETURN])
	{
		GMAIN->m_nGamePhase = ST_START;
		//ImgArrayInit();
		//m_pSndExplo->Stop();
		//m_pSndTie->Reset();
		//m_pSndTie->Stop();
		//m_pSndGameover->Reset();
		//m_pSndGameover->Stop();
	}
}
void CGameHowto::Render(LPDIRECT3DDEVICE9& dxdevice, LPD3DXSPRITE& dxsprite) 
{
	dxsprite->Begin(D3DXSPRITE_ALPHABLEND);
	dxsprite->Draw(GMAIN->m_pGameTex[25].m_pTex, &(GMAIN->rc), NULL, &(GMAIN->vcPos), D3DXCOLOR(1, 1, 1, 1.f));
	dxsprite->End();
}
void CGameHowto::Destroy() 
{
}


CGameHowto::CGameHowto()
{
}


CGameHowto::~CGameHowto()
{
}
