#pragma once
class CGameStart : public CGameScene
{
public:
	INT Init();
	void Frame();
	void Render(LPDIRECT3DDEVICE9& dxdevice, LPD3DXSPRITE& dxsprite);
	void Destroy();


	int		StartFrameMove();				// 시작 갱신
	void	StartRender();					// 시작 렌더링
	void	StartRndBck();					// 시작 Back buffer 렌더링

	CGameStart();
	virtual ~CGameStart();
};

