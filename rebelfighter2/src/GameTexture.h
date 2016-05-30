#pragma once

class CGameTexture
{
public:
	LPDIRECT3DDEVICE9		m_pDev;
	LPDIRECT3DTEXTURE9	m_pTex; //Texture(Image)
	D3DXIMAGE_INFO			m_pImg; //Image Á¤º¸

public:
	CGameTexture();
	virtual ~CGameTexture();

	virtual INT		Create(LPDIRECT3DDEVICE9 pDev, char* sFile, DWORD	dRscID=0);
	virtual void	Destroy();

public:
	INT		GetImageWidth();
	INT		GetImageHeight();
	void	GetImageRect(RECT* pRc);

	LPDIRECT3DTEXTURE9	GetTexture();
};