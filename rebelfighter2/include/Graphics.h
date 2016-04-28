#pragma once
#include <d3d9.h>


//==============================================================
// 그래픽스 표시 기능을 관리하는 클래스
class CGraphics {
protected:

	// Direct3D 인터페이스，디바이스，정점 버퍼
	LPDIRECT3D9 D3D;
	LPDIRECT3DDEVICE9 Device;

	// 윈도우 핸들
	HWND HWnd;
	
	// 폭과 높이(윈도우 모드，풀 스크린 모드)
	int WinModeWidth, WinModeHeight;
	int FullModeWidth, FullModeHeight;
	
	// 윈도우 둘레의 폭과 높이，윈도우 스타일
	int FrameWidth, FrameHeight;
	DWORD WindowStyle;

	// 풀 스크린 모드，Z버퍼 사용 여부
	bool FullMode, ZBuffer;

	// 디바이스 복구
	bool RestoreDevice();
	bool Restored;

public:

	// 생성자，파괴자
	CGraphics(HWND hwnd, bool zbuffer);
	~CGraphics();

	// 장면 표시의 시작, 종료
	bool BeginScene() { return SUCCEEDED(Device->BeginScene()); }
	void EndScene() { Device->EndScene(); }

	// 화면 제거, 화면 플립
	void Clear(D3DCOLOR color=D3DCOLOR_XRGB(0,0,0));
	void Present();

	// 디바이스 복구
	bool CheckRestored() {
		bool ret=Restored;
		Restored=false;
		return ret;
	}

	// 화면의 폭과 높이
	int GetWidth() { return FullMode?FullModeWidth:WinModeWidth; }
	int GetHeight() { return FullMode?FullModeHeight:WinModeHeight; }

	// 디바이스 취득
	LPDIRECT3DDEVICE9 GetDevice() { return Device; }

	void SetDevice(LPDIRECT3DDEVICE9* dxdevice) { Device = *dxdevice; }

	// 풀 스크린 모드(설정，현재 값，토글)
	void SetFullMode(bool full_mode);
	bool IsFullMode() { return FullMode; }

	// 광원(빛)의 설정
	void SetLight(
		int id, D3DLIGHTTYPE type, 
		float x, float y, float z,
		float range=1000
	);
};





