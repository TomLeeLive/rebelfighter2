// LcTexture.cpp: implementation of the CGameTexture class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>


#include "GameTexture.h"


CGameTexture::CGameTexture()
{
	m_pDev	= NULL;
	m_pTex	= NULL;
	memset(&m_pImg, 0, sizeof m_pImg);
}

CGameTexture::~CGameTexture()
{
	Destroy();
}


void CGameTexture::Destroy()
{
	if(m_pTex)
	{
		m_pTex->Release();
		m_pTex= NULL;
	}
}


INT CGameTexture::Create(LPDIRECT3DDEVICE9 pDev, char* sFile, DWORD	dRsc)
{
	m_pDev	= pDev;

	DWORD	dColorKey	= 0x00FFFFFF;

	if(sFile && strlen(sFile)>6)
	{
		dColorKey	= dRsc;
		if(FAILED(D3DXCreateTextureFromFileEx(
			m_pDev
			, sFile
			, D3DX_DEFAULT
			, D3DX_DEFAULT
			, 1
			, 0
			, D3DFMT_UNKNOWN
			, D3DPOOL_MANAGED
			, D3DX_FILTER_NONE
			, D3DX_FILTER_NONE
			, dColorKey
			, &m_pImg
			, NULL
			, &m_pTex
			)) )
		{
			m_pTex = NULL;
			MessageBox( GetActiveWindow()
				, "Create Texture Failed"
				, "Err"
				, MB_ICONEXCLAMATION
				);
			return -1;
		}
	}

	else if(dRsc)
	{
		HINSTANCE	hInst	= GetModuleHandle(NULL);
		HRSRC		hrsc	= FindResource(hInst, MAKEINTRESOURCE( dRsc ), "png");
		HGLOBAL		hglobal = LoadResource(hInst, hrsc);
		DWORD		dwSize	= SizeofResource(hInst,hrsc);
		void*		pMemory = LockResource(hglobal);

		if(FAILED(D3DXCreateTextureFromFileInMemoryEx(
			m_pDev
			, pMemory
			, dwSize
			, D3DX_DEFAULT
			, D3DX_DEFAULT
			, 1
			, 0
			, D3DFMT_UNKNOWN
			, D3DPOOL_MANAGED
			, D3DX_FILTER_NONE
			, D3DX_FILTER_NONE
			, dColorKey
			, &m_pImg
			, NULL
			, &m_pTex
			)) )
		{
			m_pTex = NULL;
			MessageBox( GetActiveWindow()
				, "Create Texture Failed"
				, "Err"
				, MB_ICONEXCLAMATION
				);
			return -1;
		}

		UnlockResource(hglobal);
		FreeResource(hglobal);
	}


	return 0;
}



INT CGameTexture::GetImageWidth()
{
	return m_pImg.Width;
}


INT CGameTexture::GetImageHeight()
{
	return m_pImg.Height;
}

void CGameTexture::GetImageRect(RECT* pRc)
{
	pRc->left	= 0;
	pRc->top	= 0;
	pRc->right	= m_pImg.Width;
	pRc->bottom	= m_pImg.Height;
}

LPDIRECT3DTEXTURE9 CGameTexture::GetTexture()
{
	return m_pTex;
}