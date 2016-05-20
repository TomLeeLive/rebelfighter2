#include "_StdAfx.h"
#include "CharacterData.h"



CSprite::CSprite()
{
	m_pHero = NULL;
	m_boom_pos.x = 0;
	m_boom_pos.y = 0;
	Init();
}

CSprite::CSprite(int x, int y)
{
	m_boom_pos.x = x;
	m_boom_pos.y = y;
	Init();
}

INT		CSprite::Init()
{
	SetRect(&m_imgRc, 0, 0, 0, 70);
	m_pTex = (LPDIRECT3DTEXTURE9)(GMAIN->m_pTx->GetTexture());
}
void	CSprite::Frame()
{
}

void	CSprite::Frame(int anitime)
{
	if(m_pHero == NULL){
		(*m_FT)->m_dTimeEnd = timeGetTime();

		if (((*m_FT)->m_dTimeEnd - (*m_FT)->m_dTimeBegin)>anitime)
		{
			m_imgRc.left += 70;

			if (m_imgRc.left + 70 >= 1120)
			{
				m_imgRc.left = 0;
				(*m_FT)->laserhit = FALSE;
			}

			m_imgRc.right = m_imgRc.left + 70;
			(*m_FT)->m_dTimeBegin = (*m_FT)->m_dTimeEnd;

		}
	}
	else {
		m_pHero->m_dTimeEnd = timeGetTime();

		if ((m_pHero->m_dTimeEnd - m_pHero->m_dTimeBegin)>anitime)
		{
			m_imgRc.left += 70;

			if (m_imgRc.left + 70 >= 1120)
			{
				m_imgRc.left = 0;
				m_pHero->laserhit = FALSE;
			}

			m_imgRc.right = m_imgRc.left + 70;
			m_pHero->m_dTimeBegin = m_pHero->m_dTimeEnd;

		}
	}
}

void	CSprite::Destroy()
{
}

void CSprite::HitVariableInputHero(CCharacterData* hero)
{
	m_pHero = hero;
}

void CSprite::HitVariableInput(std::vector<CCharacterData*>::iterator* _FT)
{
	m_FT = (*_FT);
}
void CSprite::Render(LPDIRECT3DDEVICE9& dxdevice, LPD3DXSPRITE& dxsprite)
{
	dxsprite->Draw(m_pTex, &m_imgRc, NULL, &m_boom_pos, D3DXCOLOR(1, 1, 1, 1.F));
}


CSprite::~CSprite()
{
}
