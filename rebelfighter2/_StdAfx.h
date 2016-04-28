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
#define GGAME		g_pGameplay


enum GAME_STATE {
	ST_START = 0,
	ST_SINGLEGAME,
	ST_END,
	ST_HOWTO = 4,
	ST_MULTI
};


#include "D3DApplication.h"
#include "dsutil.h"
#include "GameInput.h"
#include "GameTexture.h"

#include "CharacterData.h"
#include "PlayerData.h"
#include "LaserData.h"

//09-08-28 추가
#include "MiMp3.h"

#include "KSound.h"

#include "Timer.h"

#include "CDrawText.h"

//////////////////////////////////////////
// 슈팅 알고리즘 라이브러리
#pragma comment(lib, "sht.lib") 
#include "Game.h"
#include "Graphics.h"
#include "Hit.h"
#include "Image.h"
#include "Input.h"
#include "ShtMain.h"
#include "Mover.h"
#include "Object3D.h"
//////////////////////////////////////////

#include "GameScene.h"
#include "GamePlay.h"
#include "GameStart.h"
#include "GameEnd.h"
#include "GameMulti.h"
#include "GameHowto.h"


#include "Main.h"


#define SND_ALARM 				0
#define SND_BEAM1				1
#define SND_BONUS				2
#define SND_CANNON				3
#define SND_ELECTRICITYLOOP2	4
#define SND_EXPLOSION			5
#define SND_FIREBY				6
#define SND_GUNGEXPL			7
#define SND_IONCANNON			8
#define SND_JAVJAB1				9
#define SND_MENUBG				10
#define SND_MISSILE				11
#define SND_MISSLFLY			12
#define SND_PLAYBG				13
#define SND_SCORE				14
#define SND_SHOKWAV2			15
#define SND_TIE					16
#define SND_TIE_B				17
#define SND_TIELSR1				18
#define SND_TIELSR2				19
#define SND_XWLSR1				20
#define SND_R2_D2				21
#define SND_THE_FORCE			22
#define SND_XWENGLP				23
#define SND_GOODINU				24
#define SND_USEFORCE			25
#define SND_WITHYOU				26
#define SND_WONTFAIL			27
#define SND_DARK_SIDE			28
#define SND_DARK_SIDE2_Y		29
#define SND_DONT_UNDERESTIMATE	30
#define SND_DONTMAKE			31
#define SND_EXPECTING_YOU		32
#define SND_MUSTOBEY			33
#define SND_YOUR_FATHER			34

#define MV_BASIC	0
#define MV_PATH		1

#endif

