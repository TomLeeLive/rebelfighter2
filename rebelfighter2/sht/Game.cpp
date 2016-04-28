#include "Game.h"
#include "Image.h"
#include <map>
using namespace std;

#define IDC_EXIT 0
#define IDC_TOGGLEFULLSCREEN 1
#define IDC_HELPURL 2


//==============================================================
// 파일명에서 경로를 추출하여 반환
static wstring ExtractFilePath(wstring s) {
	int pos=s.rfind(L"\\");
	if (pos==wstring::npos) return L"";
	return s.substr(0, pos+1);
}


//==============================================================
// HWND와 CGame 인스턴스를 연관시키는 맵
typedef map<HWND, CGame*, less<HWND> > THWndCGameMap;
static THWndCGameMap HWndCGameMap;


//==============================================================
// 빈 메세지 핸들러 :
// 그래픽 등의 초기화 전에 이것을 실행함.
static LRESULT WINAPI DummyWndProc(
	HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam
) {
	return DefWindowProc( hwnd, msg, wparam, lparam );
}


//==============================================================
// 메세지 핸들러
static LRESULT WINAPI WndProc(
	HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam
) {
	THWndCGameMap::iterator iter=HWndCGameMap.find(hwnd);
	if (iter==HWndCGameMap.end())
		return DefWindowProc( hwnd, msg, wparam, lparam );
	CGame* game=iter->second;
	return game->WndProc(msg, wparam, lparam);
}

LRESULT WINAPI CGame::WndProc(
	UINT msg, WPARAM wparam, LPARAM lparam
) {
	switch(msg) {

		// 그리기
		case WM_PAINT:
			Draw();
			break;

		// 각종 커맨드
		case WM_COMMAND:
			switch (LOWORD(wparam)) {

				// 종료
				case IDC_EXIT:
					DestroyWindow(HWnd);
					return 0;

				// Alt+Return으로 풀 스크린 모드로 전환하기
				case IDC_TOGGLEFULLSCREEN:
					Graphics->SetFullMode(!Graphics->IsFullMode());
					return 0;

				// 도움말
				case IDC_HELPURL:
					ShellExecute(HWnd, NULL, HelpURL.c_str(),
    					NULL, NULL, SW_SHOWNORMAL);					
					return 0;
			}
			break;

		// 시스템 커맨드
		case WM_SYSCOMMAND:
			switch (wparam) {

				// 최대화때에는 화면 모드를 전환
				case SC_MAXIMIZE:
					Graphics->SetFullMode(!Graphics->IsFullMode());
					return 0;					

				// 스크린 세이버와 모니터 전원 끄기를 방지하기
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
					return 0;
			}
			break;

		// 종료
		case WM_DESTROY:
			PostQuitMessage( 0 );
			return 0;
	}
	return DefWindowProc(HWnd, msg, wparam, lparam);
}


//==============================================================
// ＜CGame＞생성자
CGame::CGame(wstring app_name, bool zbuffer)
:	AppName(app_name)
{

	// 애플리케이션명, 실행파일의 경로, 도움말의 URL
	AppName=app_name;
	wchar_t buf[MAX_PATH+1];
	GetModuleFileName(NULL, buf, MAX_PATH);
	ExePath=ExtractFilePath(buf);
	HelpURL=L"";

	// 아이콘을 읽어들이기
	HINSTANCE hinst=GetModuleHandle(NULL);
	HICON icon=LoadIcon(hinst, L"IDI_GAME");

	// 윈도우 클래스 등록:
	// 여기서는 더미 메세지 핸들러를 등록함
	WNDCLASSEX wc={
		sizeof(WNDCLASSEX), CS_CLASSDC, DummyWndProc, 0L, 0L,
		hinst, icon, LoadCursor(NULL, IDC_ARROW), NULL, NULL,
		app_name.c_str(), NULL
	};
	RegisterClassEx(&wc);

	// 가속기 키 설정:
	// [ESC] : 종료
	// [Alt]+[Enter] : 화면 모드 전환
	// [F1] : 도움말
	ACCEL accel[]={
		{FVIRTKEY, VK_ESCAPE, IDC_EXIT},
		{FALT|FVIRTKEY, VK_RETURN, IDC_TOGGLEFULLSCREEN},
		{FVIRTKEY, VK_F1, IDC_HELPURL}
	};
	HAccel=CreateAcceleratorTable(
		accel, sizeof(accel)/sizeof(ACCEL));

	// 윈도우 생성
	HWnd=CreateWindow(
		app_name.c_str(), app_name.c_str(),
		WS_CAPTION|WS_VISIBLE|WS_SYSMENU|WS_MINIMIZEBOX|WS_MAXIMIZEBOX,
		0, 0, 0, 0, GetDesktopWindow(), NULL, wc.hInstance, NULL);
	HWndCGameMap.insert(
		THWndCGameMap::value_type(HWnd, this));

	// 그래픽스 생성
	Graphics=new CGraphics(HWnd, true);
	if (!Graphics) {
		MessageBox(HWnd, 
			L"이 프로그램을 실행하기 위해서는 DirectX 9.0이상이 필요합니다.\n"
			L"최신 버전의 DirectX와 드라이버가\n"
			L"설치되어있는지 확인하시기 바랍니다.\n"
			L"또한 실행시에 옵션으로 해상도를 변경해 보시기 바랍니다.\n\n"
			L"윈도우 800x600, 풀 스크린 1026x768의 경우:\n"
			L"-w 800 -h 600 -fw 1024 -fh 768",
			app_name.c_str(),
			MB_OK);
		exit(1);
	}

	// 이미지 초기화
	CImage::Init(Graphics->GetDevice());

	// 입력 초기화
	Input=new CInput(HWnd);
}


//==============================================================
// ＜CGame＞파괴자
CGame::~CGame() {
	delete Input;
	CImage::Uninit();
	delete Graphics;
}


//==============================================================
// ＜CGame＞게임 실행
void CGame::Run() {

	// 더미 메세지 핸들러를
	// 정식 메세지 핸들러로 바꿔 줌
	SetWindowLong(HWnd, GWL_WNDPROC, (long)::WndProc);

	// 메세지 루프
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message!=WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			if (!TranslateAccelerator(HWnd, HAccel, &msg)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		} else {
			if (HWnd==GetForegroundWindow()) {
				Move();
				Draw();
			}
		}
	}

	// 종료 처리
	UnregisterClass(AppName.c_str(), GetModuleHandle(NULL));
}

