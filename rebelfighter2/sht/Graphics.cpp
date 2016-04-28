#include "Graphics.h"
#include <d3dx9.h>

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600


//==============================================================
// 생성자
CGraphics::CGraphics(HWND hwnd, bool zbuffer)
:	HWnd(hwnd), 
	WinModeWidth(DEFAULT_WIDTH), WinModeHeight(DEFAULT_HEIGHT),
	FullModeWidth(DEFAULT_WIDTH), FullModeHeight(DEFAULT_HEIGHT),
	FullMode(false), ZBuffer(zbuffer), 
	Device(NULL), Restored(false)
{
	// 커맨드 라인 옵션 해석
	for (int i=1; i<__argc; i++) {
		char* s=__argv[i];
		if (strcmp("-w", s)==0 && ++i<__argc) {
			WinModeWidth=FullModeWidth=atoi(__argv[i]);
		} else
		if (strcmp("-h", s)==0 && ++i<__argc) {
			WinModeHeight=FullModeHeight=atoi(__argv[i]);
		} else
		if (strcmp("-ww", s)==0 && ++i<__argc) {
			WinModeWidth=atoi(__argv[i]);
		} else
		if (strcmp("-wh", s)==0 && ++i<__argc) {
			WinModeHeight=atoi(__argv[i]);
		} else
		if (strcmp("-fw", s)==0 && ++i<__argc) {
			FullModeWidth=atoi(__argv[i]);
		} else
		if (strcmp("-fh", s)==0 && ++i<__argc) {
			FullModeHeight=atoi(__argv[i]);
		} else
		if (strcmp("-f", s)==0) {
			FullMode=true;
		}
	}

	// 사이즈와 위치 조절
	RECT dr, wr, cr;
	GetWindowRect(GetDesktopWindow(), &dr);
	GetWindowRect(HWnd, &wr);
	GetClientRect(HWnd, &cr);
	FrameWidth=(wr.right-wr.left)-(cr.right-cr.left);
	FrameHeight=(wr.bottom-wr.top)-(cr.bottom-cr.top);
	int w=FrameWidth+WinModeWidth, h=FrameHeight+WinModeHeight;
	SetWindowPos(HWnd, HWND_NOTOPMOST, 
		(dr.right-dr.left-w)/2, (dr.bottom-dr.top-h)/2, 
		w, h, SWP_SHOWWINDOW);
	WindowStyle=GetWindowLong(HWnd, GWL_STYLE);

	// Direct3D 인스턴스 생성
	if (!(D3D=Direct3DCreate9(D3D_SDK_VERSION))) return;

	// 디바이스 생성
	if (!RestoreDevice()) return;
}


//==============================================================
// 디바이스 초기화
bool CGraphics::RestoreDevice() {

	// 디바이스 파라메터 설정
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.hDeviceWindow=HWnd;
	d3dpp.Windowed=!FullMode;
	d3dpp.BackBufferWidth=FullMode?FullModeWidth:WinModeWidth;
	d3dpp.BackBufferHeight=FullMode?FullModeHeight:WinModeHeight;
	d3dpp.SwapEffect=D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat=D3DFMT_A8R8G8B8;
	d3dpp.PresentationInterval=D3DPRESENT_INTERVAL_DEFAULT;

	// Z버퍼 작성
	if (ZBuffer) {
		d3dpp.EnableAutoDepthStencil=TRUE;
		d3dpp.AutoDepthStencilFormat=D3DFMT_D16;
	}

	// 디바이스 작성 혹은 리셋
	if (!Device) {
		if (FAILED(D3D->CreateDevice(
			D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, HWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING,
			&d3dpp, &Device))
		) 
		if (FAILED(D3D->CreateDevice(
			D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, HWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &Device))
		)
		if (FAILED(D3D->CreateDevice(
			D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, HWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &Device))
		) return false;
	} else {
		if (FAILED(Device->Reset(&d3dpp))) return false;
	}

	Restored=true;
	return true;
}


//==============================================================
// 파괴자
CGraphics::~CGraphics() {
	if (Device) Device->Release();
	if (D3D) D3D->Release();
}


//==============================================================
// 윈도우 모드와 풀 스크린 모드 전환
void CGraphics::SetFullMode(bool full_mode) {
	if (FullMode!=full_mode) {
		FullMode=full_mode;
		RestoreDevice();
		if (full_mode) {

			// 풀 스크린 모드:
			// 캡션이나 시스템 메뉴를 제거하기
			SetWindowLong(HWnd, GWL_STYLE, WS_VISIBLE);

		} else {

			// 윈도우 모드:
			// 윈도우 스타일과 사이즈를 원래로 되돌림
			SetWindowLong(HWnd, GWL_STYLE, WindowStyle);
			RECT dr;
			GetWindowRect(GetDesktopWindow(), &dr);
			int w=FrameWidth+WinModeWidth, h=FrameHeight+WinModeHeight;
			SetWindowPos(HWnd, HWND_NOTOPMOST, 
				(dr.right-dr.left-w)/2, (dr.bottom-dr.top-h)/2, 
				w, h, SWP_SHOWWINDOW);

		}
	}
}


//==============================================================
// 화면(백 버퍼)의 제거
void CGraphics::Clear(D3DCOLOR color) {
	Device->Clear(0, NULL, 
		D3DCLEAR_TARGET | (ZBuffer?D3DCLEAR_ZBUFFER:0), 
		color, 1, 0 );
}


//==============================================================
// 화면의 갱신
void CGraphics::Present() {

	// 디바이스가 소실되었다면 다시 생성함
	switch (Device->TestCooperativeLevel()) {
		case D3DERR_DEVICELOST: 
			return;
		case D3DERR_DEVICENOTRESET:
			if (!RestoreDevice()) return;
			break;
	}

	// 화면을 갱신하기
	Device->Present(0, 0, 0, 0);
}


//==============================================================
// 광원의 초기화
void CGraphics::SetLight(
	int id, D3DLIGHTTYPE type, 
	float x, float y, float z,
	float range
) {
	// 광원의 작성
	D3DLIGHT9 light;
	D3DXVECTOR3 vec_lightdir(x, y, z);
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type=type;
	light.Diffuse.r=1.0f;
	light.Diffuse.g=1.0f;
	light.Diffuse.b=1.0f;
	D3DXVec3Normalize(
		(D3DXVECTOR3*)&light.Direction, &vec_lightdir);
	light.Position.x=x;
	light.Position.y=y;
	light.Position.z=z;
	light.Range=range;

	// 광원의 설정
	Device->SetLight(id, &light);
	Device->LightEnable(id, TRUE);
	Device->SetRenderState(D3DRS_LIGHTING, TRUE);
}


