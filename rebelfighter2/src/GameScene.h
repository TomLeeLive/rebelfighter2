#pragma once
class CGameScene
{
public:
	virtual INT Init() = 0;
	virtual void Frame() = 0;
	virtual void Render(LPDIRECT3DDEVICE9& dxdevice, LPD3DXSPRITE& dxsprite) = 0;
	virtual void Destroy() = 0;




	CGameScene();
	virtual ~CGameScene();
};

