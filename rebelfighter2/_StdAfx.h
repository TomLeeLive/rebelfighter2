// D3D Sprite Tutorial
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _STDAFX_H_ //인클루드 가드
#define	 _STDAFX_H_

#define SAFE_RELEASE(p){if(p){(p)->Release();(p) = NULL;}}
#define SAFE_DELETE(p){if(p){delete(p);(p) = NULL;}}
#define SAFE_DELETE_ARRAY(p){if(p){delete[](p);(p) = NULL;}}
#define _WIN32_WINNT 0x0400 //마우스 휠 사용을 위해

#pragma comment(linker, "/subsystem:windows")

//Direct3D Device
#pragma comment(lib, "d3d9.lib") //
#include <d3d9.h> //

//D3DXSprite
#pragma comment(lib, "d3dx9.lib") //
#include <d3dx9.h> //

//09-08-28추가
#pragma comment(lib, "DxErr.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dsound.lib")	// Direct Sound
#define WIN32_LEAN_AND_MEAN
#include <dmusicc.h>			// DirectMusic includes
//#include <dmusici.h>
#include <cguid.h>			// for GUID_NULL

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>


//09-08-28 추가
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <vector>
#include <math.h>
#include <time.h>

#define GMAIN			g_pApp
#define GHINST			g_pApp->m_hInst
#define GHWND			g_pApp->m_hWnd
#define GDEVICE		g_pApp->m_pd3dDevice

#include "D3DApplication.h"
#include "dsutil.h"
#include "GameInput.h"
#include "GameTexture.h"

#include "CharacterData.h"
#include "PlayerData.h"
#include "LaserData.h"

//09-08-28 추가
#include "MiMp3.h"

#include "Main.h"



#endif

