#pragma once
#include <d3d9.h>
#include <string>
using namespace std;


//==============================================================
// 매크로
inline int ARGB(int a, int r, int g, int b) {
	return ((a&255)<<24 | (r&255)<<16 |
		(g&255)<<8 | (b&255));
}
inline DWORD ARGB_A(DWORD rgb) { return (rgb)>>24&255; }
inline DWORD ARGB_R(DWORD rgb) { return (rgb)>>16&255; }
inline DWORD ARGB_G(DWORD rgb) { return (rgb)>>8&255; }
inline DWORD ARGB_B(DWORD rgb) { return rgb&255; }


//==============================================================
// 정점 형식

// 독자적인 정점 형식
struct CUSTOMVERTEX
{
	// 변환된 정점
	FLOAT X, Y, Z, RHW;

	// 디퓨즈 색
	D3DCOLOR Color;

	// 텍스쳐 좌표
	FLOAT TU, TV;
};

// 독자적인 정점 형식을 나타내는 FVF
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)


//==============================================================
// 이미지
class CImage {
protected:

	// Direct3D 디바이스，정점 버퍼
	static LPDIRECT3DDEVICE9 Device;
	static LPDIRECT3DVERTEXBUFFER9 VertexBuffer;

	// 텍스쳐
	LPDIRECT3DTEXTURE9* Textures;

	// 이미지의 폭，높이，텍스쳐 수(X방향, Y방향)
	int Width, Height, XCount, YCount;

public:

	// 초기화，디바이스 상태 설정，종료
	static void Init(LPDIRECT3DDEVICE9 device);
	static void SetDeviceState();
	static void Uninit();

	// 생성자，파괴자
	CImage(int width, int height);
	~CImage();

	int GetWidth() { return Width; }
	int GetHeight() { return Height; }

	// 이미지 파일 로드
	void LoadFromFile(wstring file_name);

	// 표시(화면 왼쪽 위에 이미지 전체):
	// color는 투명색(컬러 키)
	void Draw(D3DCOLOR color=0xffffffff) {
		Draw(0, 0, 0, 0, Width, Height, color);
	}

	// 표시(지정된 위치에 이미지 전체)
	void Draw(int dx, int dy, D3DCOLOR color=0xffffffff) {
		Draw(dx, dy, 0, 0, Width, Height, color);
	}

	// 표시(지정된 위치에 이미지 내의 지정부분)
	void Draw(int dx, int dy, int sx, int sy, int w, int h, 
		D3DCOLOR color=0xffffffff);

	void Clear(D3DCOLOR color=D3DCOLOR_ARGB(0, 0, 0, 0));
};


//==============================================================
// 텍스트 정렬
typedef enum {
	ALIGN_CENTER, 
	ALIGN_LEFT, ALIGN_RIGHT,
	ALIGN_TOP, ALIGN_BOTTOM	
} ALIGN_TYPE;


//==============================================================
// 텍스트
class CText : public CImage {
protected:

	// 텍스트 표시용 서피스
	LPDIRECT3DSURFACE9 TextSurface;

public:

	// 생성자，파괴자
	CText(int width, int height);
	~CText();

	// 텍스트 표시
	void DrawText(
		wstring text, int x, int y, 
		D3DCOLOR color, HFONT font,
		ALIGN_TYPE halign=ALIGN_LEFT, 
		ALIGN_TYPE valign=ALIGN_TOP,
		SIZE* psize=NULL
	);

};


