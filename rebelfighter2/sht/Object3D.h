#pragma once

#include <d3dx9.h>
#include <string>
#include "Hit.h"
using namespace std;


//==============================================================
// 접촉 판정의 종류
typedef enum {
	HIT_ALL, HIT_CORE, NUM_HIT
} HIT_TYPE;


//==============================================================
// 뷰의 종류
typedef enum {
	VIEW_TOP, VIEW_SIDE
} VIEW_TYPE;


//==============================================================
// 3D 오브젝트의 표시를 수행하는 클래스
class CObject3D {
private:

	// 메쉬
	ID3DXMesh* Mesh;

	// Material의 수, Material
	DWORD NumMaterials;
	D3DMATERIAL9* Materials;

	// 텍스쳐
	LPDIRECT3DTEXTURE9* Textures;

	// 접촉 판정 영역
	CHit Hit[NUM_HIT];

	// 표시모드
	static VIEW_TYPE ViewType;

	// 디바이스
	static LPDIRECT3DDEVICE9 Device;

	// 디바이스 상태 설정
	static void SetDeviceState_Common();
	static void SetDeviceState_TopView();
	static void SetDeviceState_SideView();
	static float RotationRad;

	// 광원(빛) 초기화
	static void InitLight(
		D3DLIGHT9& light, D3DLIGHTTYPE type, 
		FLOAT x, FLOAT y, FLOAT z);

	// 백 버퍼의 사이즈 취득
	static SIZE GetBackBufferSize();

	// 탑 뷰인가?
	bool TopViewOnly;

public:

	// 생성자，파괴자
	CObject3D();
	~CObject3D();

	// 파일 읽기
	CObject3D* LoadFromFile(wstring file);

	// 표시
	void Draw(float alpha, bool alpha_addition);
	void Draw(float x, float y, float z);
	void DrawTopView(float x, float y, 
		float sx, float sy, 
		float yaw, float pitch, float roll,
		float alpha, bool alpha_addition);
	void DrawTopView(float x, float y, float z,
		float sx, float sy, 
		float yaw, float pitch, float roll,
		float alpha, bool alpha_addition);
	void Draw(
		D3DXMATRIX& mat,		
		float x, float y, float z, 
		float sx, float sy, float sz,
		float rx, float ry, float rz,
		float alpha, bool alpha_addition);

	// 접촉 판정
	CObject3D* SetHit(HIT_TYPE type, const CHit& hit);
	CHit GetHit(HIT_TYPE type);

	void SetTopViewOnly(bool tv_only) { TopViewOnly=tv_only; }

	// 초기화，종료
	static void Init(LPDIRECT3DDEVICE9 device);
	static void Uninit();

	// 표시모드，디바이스 상태 설정
	static void SetViewType(VIEW_TYPE view_type) { ViewType=view_type; }
	static void SetDeviceState();
	static void SetRotation(float angle);
};

