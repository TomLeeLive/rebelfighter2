#include "_StdAfx.h"

CSprite::CSprite()
{
	m_position.x = 0;
	m_position.y = 0;
	Init();
}

CSprite::CSprite(D3DXVECTOR3 position)
{
	m_position.x = position.x;
	m_position.y = position.y;
	Init();
}


void CSprite::SetTimeBegin(DWORD dTimeBegin)
{
	m_dTimeBegin = dTimeBegin;
}
void CSprite::SetTimeEnd(DWORD dTimeEnd)
{
	m_dTimeEnd = dTimeEnd;
}
void CSprite::SetAniTime(DWORD dAniTime)
{
	m_dAniTime = dAniTime;
}



INT		CSprite::Init()
{
	SetRect(&m_imgRc, 0, 0, 0, 70);
	m_pTex = (LPDIRECT3DTEXTURE9)(GMAIN->m_pTx->GetTexture());
	return 0;
}
void	CSprite::Frame()
{
}

void CSprite::SetPosX(int x) 
{
	m_position.x = x;
}
void CSprite::SetPosY(int y) 
{
	m_position.y = y;
}

BOOL CSprite::AniFrame(DWORD dAniTime)
{
	m_dTimeEnd = timeGetTime();

	if ((m_dTimeEnd - m_dTimeBegin) > m_dAniTime)
	{
		m_imgRc.left += 70;

		if (m_imgRc.left + 70 >= 1120)
		{
			m_imgRc.left = 0;
			return FALSE;
		}

		m_imgRc.right = m_imgRc.left + 70;
		m_dTimeBegin = m_dTimeEnd;

	}
	return TRUE;
}

BOOL CSprite::AniFrame(DWORD dAniTime, DWORD dTime)
{
	m_dTimeEnd = dTime;

	if ((m_dTimeEnd - m_dTimeBegin) > m_dAniTime)
	{
		m_imgRc.left += 70;

		if (m_imgRc.left + 70 >= 1120)
		{
			m_imgRc.left = 0;
			return FALSE;
		}

		m_imgRc.right = m_imgRc.left + 70;
		m_dTimeBegin = m_dTimeEnd;

	}
	return TRUE;
}

void	CSprite::Destroy()
{
}

//void CSprite::HitVariableInputHero(CCharacterData* hero)
//{
//	m_pHero = hero;
//}
//
//void CSprite::HitVariableInput(std::vector<CCharacterData*>::iterator* _FT)
//{
//	m_FT = (*_FT);
//}
void CSprite::Render(LPDIRECT3DDEVICE9& dxdevice, LPD3DXSPRITE& dxsprite)
{
	dxsprite->Draw(m_pTex, &m_imgRc, NULL, &m_position, D3DXCOLOR(1, 1, 1, 1.F));
}


CSprite::~CSprite()
{
}
