#pragma once
class CSprite
{
public:
	D3DXVECTOR3			m_position;			// ��������Ʈ�� �������� ��ġ
	RECT				m_imgRc;			// �̹��� rect
	LPDIRECT3DTEXTURE9	m_pTex;				// �ؽ���
	DWORD				m_dTimeBegin;		// �ִϸ��̼� ���� Ÿ���� ���� ����
	DWORD				m_dTimeEnd;			// �ִϸ��̼� �� Ÿ���� ���� ����
	DWORD				m_dAniTime;			// �ִϸ��̼� �÷��� �ð�.

	
	INT Init();
	void Frame();
	void Render(LPDIRECT3DDEVICE9& dxdevice, LPD3DXSPRITE& dxsprite);
	void Destroy();

	BOOL AniFrame(DWORD dAniTime);
	BOOL AniFrame(DWORD dAniTime, DWORD dTime);

	void SetPosX(int x);
	void SetPosY(int y);
	void SetTimeBegin(DWORD dTimeBegin);
	void SetTimeEnd(DWORD dTimeEnd);
	void SetAniTime(DWORD dAniTime);

	//std::vector<CCharacterData*>::iterator m_FT;
	//void CSprite::HitVariableInputHero(CCharacterData* hero);
	//void HitVariableInput(std::vector<CCharacterData*>::iterator* _FT);

	CSprite();
	CSprite(D3DXVECTOR3 position);
	
	virtual ~CSprite();
};

