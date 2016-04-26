#pragma once
class CGameMulti : public CGameScene
{
public:
	INT Init();
	void Frame();
	void Render(LPDIRECT3DDEVICE9& dxdevice, LPD3DXSPRITE& dxsprite);
	void Destroy();


	CGameMulti();
	virtual ~CGameMulti();
};

