#include "Object3D.h"


typedef struct {
	float x, y, z, r, g, b, a;
} XYZRGBA;


float CObject3D::RotationRad=0;
void CObject3D::SetRotation(float angle) {
	RotationRad=D3DX_PI*2*angle;
}


//==============================================================
// 표시 모드
VIEW_TYPE CObject3D::ViewType=VIEW_TOP;


//==============================================================
// 디바이스
LPDIRECT3DDEVICE9 CObject3D::Device=NULL;


//==============================================================
// 초기화
void CObject3D::Init(LPDIRECT3DDEVICE9 device) {
	Device=device;
}


//==============================================================
// 종료
void CObject3D::Uninit() {
}


//==============================================================
// 접촉 판정 영역 설정
CObject3D* CObject3D::SetHit(
	HIT_TYPE type, const CHit& hit
) {
	if (type<0 || sizeof(HIT_TYPE)<=type) return this;
	Hit[type]=hit;
	return this;
}


//==============================================================
// 접촉 판정 영역 취득
CHit CObject3D::GetHit(HIT_TYPE type) {
	if (type<0 || sizeof(HIT_TYPE)<=type) 
		return CHit(0, 0, 0, 0);
	return Hit[type];
}


//==============================================================
// 생성자
CObject3D::CObject3D()
:	Mesh(NULL), NumMaterials(0), 
	Materials(NULL), Textures(NULL),
	TopViewOnly(true)
{}


//==============================================================
// 파괴자
CObject3D::~CObject3D() {
	if (Textures) {
		for (DWORD i=0; i<NumMaterials; i++) {
			if (Textures[i]) Textures[i]->Release();
		}
		delete[] Textures;
	}
	if (Materials) delete[] Materials;
	if (Mesh) Mesh->Release();
}


//==============================================================
// 파일 로드
CObject3D* CObject3D::LoadFromFile(wstring file) {

	// 파일을 읽어들이기
	LPD3DXBUFFER buf_materials=NULL;
	if (FAILED(D3DXLoadMeshFromX(
			file.c_str(), D3DXMESH_SYSTEMMEM, 
			Device, NULL, &buf_materials, NULL, &NumMaterials,
			&Mesh))
	) {
		return this;
	}
	
	// Material을 얻어냄
	D3DXMATERIAL* materials=
		(D3DXMATERIAL*)buf_materials->GetBufferPointer();
	Materials=new D3DMATERIAL9[NumMaterials];
	Textures=new LPDIRECT3DTEXTURE9[NumMaterials];

	for (DWORD i=0; i<NumMaterials; i++ ) {

		// Material을 복사
		Materials[i]=materials[i].MatD3D;

		// Ambient색을 설정하기
		Materials[i].Ambient=Materials[i].Diffuse;

		// 텍스쳐를 읽어들이기
		Textures[i]=NULL;
		if (materials[i].pTextureFilename) {
			wchar_t texture_file[MAX_PATH];
			MultiByteToWideChar(CP_ACP, 0, materials[i].pTextureFilename, -1, texture_file, MAX_PATH);
			D3DXCreateTextureFromFile(Device, texture_file, &Textures[i]);
		}
	}
	
	// 마무리
	buf_materials->Release();
	return this;
}


//==============================================================
// 표시(좌표를 지정)
void CObject3D::Draw(float x, float y, float z) {
	D3DXMATRIX mat;
	D3DXMatrixTranslation(&mat, x, y, z);
	Device->SetTransform(D3DTS_WORLD, &mat);
	Draw(1, false);
}


//==============================================================
// 광원(빛) 초기화
void CObject3D::InitLight(
	D3DLIGHT9& light, D3DLIGHTTYPE type, 
	FLOAT x, FLOAT y, FLOAT z
) {
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
	light.Range=1000.0f;
}


//==============================================================
// 디바이스 상태 설정(공통)
void CObject3D::SetDeviceState_Common() {
	RotationRad=0;

	// 렌더링 설정
	Device->SetRenderState(D3DRS_DITHERENABLE, FALSE);
	Device->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	Device->SetRenderState(D3DRS_LIGHTING, TRUE);
	Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	Device->SetRenderState(D3DRS_AMBIENT, 0x006F6F6F);

	// 텍스쳐 처리 방법 설정:
	Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	
	// 선형 보간을 사용하기 위한 설정
	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	// 월드 매트릭스
	D3DXMATRIX mat_world;
	D3DXMatrixIdentity(&mat_world);
	Device->SetTransform(D3DTS_WORLD, &mat_world);
}


//==============================================================
// 백 버퍼 사이즈 취득
SIZE CObject3D::GetBackBufferSize() {
	LPDIRECT3DSURFACE9 back_buffer;
	D3DSURFACE_DESC desc;
	SIZE size={0, 0};
	if (SUCCEEDED(Device->GetBackBuffer(
			0, 0, D3DBACKBUFFER_TYPE_MONO, &back_buffer))
	) {
		if (SUCCEEDED(back_buffer->GetDesc(&desc))) {
			size.cx=desc.Width; size.cy=desc.Height;	
		}
		back_buffer->Release();
	}
	return size;
}


//==============================================================
// 디바이스 상태 설정(탑 뷰)
void CObject3D::SetDeviceState_TopView() {

	// 공통 설정
	SetDeviceState_Common();

	// 시야 매트릭스
	D3DXMATRIX mat_view;
	D3DXVECTOR3 vec_from=D3DXVECTOR3(37.5f, 50.0f, 100);
	D3DXVECTOR3 vec_lookat=D3DXVECTOR3(37.5f, 50.0f, 0);
	D3DXVECTOR3 vec_up=D3DXVECTOR3(0, -1, 0);
	D3DXMatrixLookAtLH(&mat_view, &vec_from, &vec_lookat, &vec_up);
	Device->SetTransform(D3DTS_VIEW, &mat_view);
	
	// 투영 매트릭스
	D3DXMATRIX mat_proj;
	D3DXMatrixOrthoLH(&mat_proj, 75, 100, 1, 1000);
	Device->SetTransform(D3DTS_PROJECTION, &mat_proj);

	// 광원 설정
	D3DLIGHT9 light;
	InitLight(light, D3DLIGHT_DIRECTIONAL, -1.0f, -1.0f, -1.0f);
	Device->SetLight(0, &light);
	Device->LightEnable(0, TRUE);
	Device->SetRenderState(D3DRS_LIGHTING, TRUE);

	// 뷰 포트
	D3DVIEWPORT9 viewport;
	SIZE size=GetBackBufferSize();
	int w=size.cx, h=size.cy;
	viewport.X=(unsigned long)((w-h*0.75)/2);
	viewport.Y=0;
	viewport.Width=(unsigned long)(h*0.75);
	viewport.Height=h;
	viewport.MinZ=0;
	viewport.MaxZ=1;
	Device->SetViewport(&viewport);
}


//==============================================================
// 디바이스 상태 설정(사이드 뷰)
void CObject3D::SetDeviceState_SideView() {

	// 공통 설정
	SetDeviceState_Common();

	// 시야 매트릭스
	D3DXMATRIX mat_view;
	D3DXVECTOR3 vec_from=D3DXVECTOR3(50.0f, 37.5f, 100);
	D3DXVECTOR3 vec_lookat=D3DXVECTOR3(50.0f, 37.5f, 0);
	D3DXVECTOR3 vec_up=D3DXVECTOR3(0, -1, 0);
	D3DXMatrixLookAtLH(&mat_view, &vec_from, &vec_lookat, &vec_up);
	Device->SetTransform(D3DTS_VIEW, &mat_view);
	
	// 투영 매트릭스
	D3DXMATRIX mat_proj;
	D3DXMatrixOrthoLH(&mat_proj, 100, 75, 1, 1000);
	Device->SetTransform(D3DTS_PROJECTION, &mat_proj);

	// 광원 설정
	D3DLIGHT9 light;
	InitLight(light, D3DLIGHT_DIRECTIONAL, -1.0f, 1.0f, -1.0f);
	Device->SetLight(0, &light);
	Device->LightEnable(0, TRUE);
	Device->SetRenderState(D3DRS_LIGHTING, TRUE);

	// 뷰 포트
	D3DVIEWPORT9 viewport;
	SIZE size=GetBackBufferSize();
	int w=size.cx, h=size.cy;
	viewport.X=0;
	viewport.Y=(unsigned long)((h-w*0.75)/2);
	viewport.Width=w;
	viewport.Height=(unsigned long)(w*0.75);
	viewport.MinZ=0;
	viewport.MaxZ=1;
	Device->SetViewport(&viewport);
}


//==============================================================
// 디바이스 상태 설정
void CObject3D::SetDeviceState() {
	switch (ViewType) {
		case VIEW_TOP: SetDeviceState_TopView(); break;
		case VIEW_SIDE: SetDeviceState_SideView(); break;
	}
}


//==============================================================
// 표시(탑 뷰)
void CObject3D::DrawTopView(
	float x, float y, 
	float sx, float sy, 
	float yaw, float pitch, float roll,
	float alpha, bool alpha_addition
) {
	D3DXMATRIX mat, mat0;
	if (TopViewOnly || ViewType==VIEW_TOP) {
		D3DXMatrixRotationX(&mat, D3DX_PI/2);
		Draw(mat, x, y, 0, sx, sy, 1, 
			pitch, roll, yaw, alpha, alpha_addition);
	} else {
		D3DXMatrixRotationY(&mat, D3DX_PI/2);
		D3DXMatrixRotationX(&mat0, D3DX_PI);
		D3DXMatrixMultiply(&mat, &mat, &mat0);
		Draw(mat, x, y, 0, sx, sy, 1,
			roll, yaw, pitch, alpha, alpha_addition);
	}
}
void CObject3D::DrawTopView(
	float x, float y, float z,
	float sx, float sy, 
	float yaw, float pitch, float roll,
	float alpha, bool alpha_addition
) {
	D3DXMATRIX mat, mat0;
	if (TopViewOnly || ViewType==VIEW_TOP) {
		D3DXMatrixRotationX(&mat, D3DX_PI/2);
		Draw(mat, x, y, z, sx, sy, 1, 
			pitch, roll, yaw, alpha, alpha_addition);
	} else {
		D3DXMatrixRotationY(&mat, D3DX_PI/2);
		D3DXMatrixRotationX(&mat0, D3DX_PI);
		D3DXMatrixMultiply(&mat, &mat, &mat0);
		Draw(mat, x, y, z, sx, sy, 1,
			roll, yaw, pitch, alpha, alpha_addition);
	}
}


//==============================================================
// 표시(3D좌표，확대율，회전을 지정)
void CObject3D::Draw(
	D3DXMATRIX& mat,
	float x, float y, float z, 
	float sx, float sy, float sz,
	float rx, float ry, float rz,
	float alpha, bool alpha_addition
) {
	D3DXMATRIX mat0;
	
	// 확대 축소
	D3DXMatrixScaling(&mat0, sx, sy, sz);
	D3DXMatrixMultiply(&mat, &mat, &mat0);
	
	// 회전
	if (ViewType==VIEW_TOP) {
		if (rz!=0) {
			D3DXMatrixRotationZ(&mat0, D3DX_PI*2*rz);
			D3DXMatrixMultiply(&mat, &mat, &mat0);
		}
		if (rx!=0) {
			D3DXMatrixRotationX(&mat0, D3DX_PI*2*rx);
			D3DXMatrixMultiply(&mat, &mat, &mat0);
		}
		if (ry!=0) {
			D3DXMatrixRotationY(&mat0, D3DX_PI*2*ry);
			D3DXMatrixMultiply(&mat, &mat, &mat0);
		}
	} else {
		if (rx!=0) {
			D3DXMatrixRotationX(&mat0, D3DX_PI*2*rx);
			D3DXMatrixMultiply(&mat, &mat, &mat0);
		}
		if (ry!=0) {
			D3DXMatrixRotationY(&mat0, D3DX_PI*2*ry);
			D3DXMatrixMultiply(&mat, &mat, &mat0);
		}
		if (rz!=0) {
			D3DXMatrixRotationZ(&mat0, D3DX_PI*2*rz);
			D3DXMatrixMultiply(&mat, &mat, &mat0);
		}
	}

	// 좌표
	D3DXMatrixTranslation(&mat0, x, y, z);
	D3DXMatrixMultiply(&mat, &mat, &mat0);
	
	// 회전
	if (RotationRad!=0) {
		if (ViewType==VIEW_TOP) {
			D3DXMatrixTranslation(&mat0, -37.5f, -50, 0);
			D3DXMatrixMultiply(&mat, &mat, &mat0);
			D3DXMatrixRotationZ(&mat0, RotationRad);
			D3DXMatrixMultiply(&mat, &mat, &mat0);
			D3DXMatrixTranslation(&mat0, 37.5f, 50, 0);
			D3DXMatrixMultiply(&mat, &mat, &mat0);
		}
	}

	// 표시
	Device->SetTransform(D3DTS_WORLD, &mat);
	Draw(alpha, alpha_addition);
}


//==============================================================
// 실제로 메쉬를 표시하기
void CObject3D::Draw(float alpha, bool alpha_addition) {
	if (alpha<0) alpha=0; else if (alpha>1) alpha=1;
	D3DMATERIAL9 mat;
	for (DWORD i=0; i<NumMaterials; i++) {
		mat=Materials[i];
		if (alpha_addition) {
			Device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
			Device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			Device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE);
		} else {
			Device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
			Device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			Device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
		mat.Diffuse.a*=alpha;
		mat.Ambient.a*=alpha;
		mat.Specular.a*=alpha;
		mat.Emissive.a*=alpha;
		Device->SetMaterial(&mat);
		Device->SetTexture(0, Textures[i]);
		Mesh->DrawSubset(i);
	}
}


