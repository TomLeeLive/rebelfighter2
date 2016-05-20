#pragma once
class CSprite
{
public:
	D3DXVECTOR3			m_position;			// 스프라이트가 보여지는 위치
	RECT				m_imgRc;			// 이미지 rect
	LPDIRECT3DTEXTURE9	m_pTex;				// 텍스쳐
	DWORD				m_dTimeBegin;		// 애니메이션 시작 타임을 위한 변수
	DWORD				m_dTimeEnd;			// 애니메이션 끝 타임을 위한 변수
	DWORD				m_dAniTime;			// 애니메이션 플레이 시간.

	
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

