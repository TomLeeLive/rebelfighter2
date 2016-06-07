#include "_StdAfx.h"

//extern  CDrawText		GMAIN->m_text;

INT CGameStart::Init()
{
	return 0;
}
void CGameStart::Frame()
{
	GMAIN->m_pInput->FrameMove();

	if (0 == GMAIN->m_KeyOld[VK_RETURN] && GMAIN->m_KeyCur[VK_RETURN])
	{
		GMAIN->m_nGameBeforePhase = GMAIN->m_nGamePhase;
		GMAIN->m_nGamePhase = ST_SINGLEGAME;
		GMAIN->m_pSound.Stop(SND_MENUBG);
		GMAIN->m_pSound.Play(SND_PLAYBG, true);
		GMAIN->m_pSound.Play(SND_XWENGLP, false);
		GMAIN->m_pSound.Volume(SND_XWENGLP,0.5f, false);
	}

	//09-08-28 Ãß°¡
		GMAIN->m_pInput->MenuItemClickCheck();

	//return 0;
}
void CGameStart::Render(LPDIRECT3DDEVICE9& dxdevice, LPD3DXSPRITE& dxsprite)
{
	dxsprite->Begin(D3DXSPRITE_ALPHABLEND);
	dxsprite->Draw(GMAIN->m_pGameTex[5].m_pTex, &(GMAIN->rc), NULL, &(GMAIN->vcPos), D3DXCOLOR(1, 1, 1, 1.f));
	dxsprite->End();
}
void CGameStart::Destroy()
{
}


CGameStart::CGameStart()
{
}


CGameStart::~CGameStart()
{
}
