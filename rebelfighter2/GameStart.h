#pragma once
class CGameStart : public CGameScene
{
public:
	INT Init();
	void Frame();
	void Render(LPDIRECT3DDEVICE9& dxdevice, LPD3DXSPRITE& dxsprite);
	void Destroy();


	int		StartFrameMove();				// ���� ����
	void	StartRender();					// ���� ������
	void	StartRndBck();					// ���� Back buffer ������

	CGameStart();
	virtual ~CGameStart();
};

