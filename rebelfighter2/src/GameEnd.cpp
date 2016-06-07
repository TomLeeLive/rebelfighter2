#include "_StdAfx.h"

//extern  CDrawText		GMAIN->m_text;

INT CGameEnd::Init() { return 0; };
void CGameEnd::Frame()
{
	if (0 == GMAIN->m_KeyOld[VK_RETURN] && GMAIN->m_KeyCur[VK_RETURN])
	{
		if (GMAIN->m_nGameBeforePhase == ST_MULTI) {
			GGAMEMULTI->Init();
		}
		else {
			GGAME->Init();
		}

		GMAIN->m_nGameBeforePhase = GMAIN->m_nGamePhase;
		GMAIN->m_nGamePhase = ST_START;

		GMAIN->m_pSound.Stop(SND_THE_FORCE);
		GMAIN->m_pSound.Play(SND_MENUBG, true);
	}
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

	if (GMAIN->m_nGameBeforePhase == ST_MULTI) {
		sprintf(scoreBuf, "%d", GGAMEMULTI->score);//멀티 게임 점수
	}
	else {
		sprintf(scoreBuf, "%d", GGAME->score);//싱글 게임 점수.
	}
	

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
