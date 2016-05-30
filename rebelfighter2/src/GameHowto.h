#pragma once
class CGameHowto : public CGameScene
{
public:
	INT Init();
	void Frame();
	void Render(LPDIRECT3DDEVICE9& dxdevice, LPD3DXSPRITE& dxsprite);
	void Destroy();


	CGameHowto();
	virtual ~CGameHowto();
};

