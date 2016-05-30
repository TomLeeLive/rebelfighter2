#pragma once
#include "CharacterData.h"
class CSprite : public CGameScene
{
public:
	D3DXVECTOR3			m_boom_pos;
	RECT				m_imgRc;
	LPDIRECT3DTEXTURE9	m_pTex;
	DWORD				m_dTimeBegin;		// �ִϸ��̼� ���� Ÿ��
	DWORD				m_dTimeEnd;			// �ִϸ��̼� �� Ÿ��
	CCharacterData*		m_pHero;

	std::vector<CCharacterData*>::iterator m_FT;

	INT Init();
	void Frame();
	void Render(LPDIRECT3DDEVICE9& dxdevice, LPD3DXSPRITE& dxsprite);
	void Destroy();

	void	Frame(int anitime);
	void CSprite::HitVariableInputHero(CCharacterData* hero);
	void HitVariableInput(std::vector<CCharacterData*>::iterator* _FT);

	CSprite();
	CSprite::CSprite(int x, int y);
	
	virtual ~CSprite();
};

