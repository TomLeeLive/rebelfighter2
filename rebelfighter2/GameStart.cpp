#include "_StdAfx.h"

//extern  CDrawText		GMAIN->m_text;

INT CGameStart::Init()
{
	return 0;
}
void CGameStart::Frame()
{
	GMAIN->m_pInput->FrameMove();


		//m_pSndTie->Reset();
		//m_pSndTie->Stop();
		//m_pMp[0]->Reset();//m_pSndGameBg->Reset();
		//m_pMp[1]->Play();//m_pSndGameMenu->Play();
		//m_pMp[1]->SetVolume(9000L);//m_pSndGameMenu->SetVolume(1000);
		//m_pSound.Stop(29);
		//m_pSound.Play(10, true);

		if (0 == GMAIN->m_KeyOld[VK_RETURN] && GMAIN->m_KeyCur[VK_RETURN])
		{
			GMAIN->m_nGamePhase = ST_SINGLEGAME;
			GMAIN->m_pSound.Stop(SND_MENUBG);
			GMAIN->m_pSound.Play(SND_PLAYBG, true);
			//ImgArrayInit();

			//m_pSound.Stop(10);
			//m_pSound.Play(13, true);
			//return 0;
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
