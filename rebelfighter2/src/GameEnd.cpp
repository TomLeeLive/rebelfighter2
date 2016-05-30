#include "_StdAfx.h"

//extern  CDrawText		GMAIN->m_text;

INT CGameEnd::Init() { return 0; };
void CGameEnd::Frame()
{
	if (0 == GMAIN->m_KeyOld[VK_RETURN] && GMAIN->m_KeyCur[VK_RETURN])
	{
		GMAIN->m_nGamePhase = ST_START;
		GGAME->Init();

		GMAIN->m_pSound.Stop(SND_THE_FORCE);
		GMAIN->m_pSound.Play(SND_MENUBG, true);

		//ImgArrayInit();

		//m_pSound.Stop(22);
		//m_pSound.Play(10, true);

		//m_pSndExplo->Stop();
		//m_pSndTie->Reset();
		//m_pSndTie->Stop();
		//m_pSndGameover->Reset();
		//m_pSndGameover->Stop();
		//return 0;
	}

	//return 0;
}
void CGameEnd::Render(LPDIRECT3DDEVICE9& dxdevice, LPD3DXSPRITE& dxsprite)
{
	dxsprite->Begin(D3DXSPRITE_ALPHABLEND);
	dxsprite->Draw(GMAIN->m_pGameTex[6].m_pTex, &(GMAIN->rc), NULL, &(GMAIN->vcPos), D3DXCOLOR(1, 1, 1, 1.f));
	////////////////////////////////////////////////////////////////////////////////
	dxsprite->End();

	GMAIN->m_text.Begin();
	char	scoreBuf[80];
	TCHAR	fpsBuf[128];

	sprintf(scoreBuf, "%d", GGAME->score);

	GMAIN->m_text.Draw("Game Over", 355, 250, D3DXCOLOR(0, 0, 0, 1));

	GMAIN->m_text.Draw("Total Score", 355, 300, D3DXCOLOR(0, 0, 0, 1));
	GMAIN->m_text.Draw(scoreBuf, 355, 320, D3DXCOLOR(0, 0, 0, 1));

	GMAIN->m_text.Draw("Press Enter for going back to Menu", 250, 400, D3DXCOLOR(0, 0, 0, 1));

	////////////////////////////////////////////////////////////////////////////////
	//FPS 화면에 출력
	sprintf(fpsBuf, "FPS: %4.1f", GMAIN->m_fFps);
	GMAIN->m_text.Draw(fpsBuf, 700, 300);

	GMAIN->m_text.End();
}
void CGameEnd::Destroy() {

};


CGameEnd::CGameEnd()
{
}


CGameEnd::~CGameEnd()
{
}
