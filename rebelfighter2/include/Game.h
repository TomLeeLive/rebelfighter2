#pragma once
#include "Graphics.h"
#include "Image.h"
#include "Input.h"
#include <string>
using namespace std;


// 게임 본체의 클래스
class CGame {
protected:

	// 가속 키
	HACCEL HAccel;

	// 윈도우 핸들
	HWND HWnd;

	// 그래픽스
	CGraphics* Graphics;

	// 입력
	CInput* Input;

	// 실행 파일의 경로
	wstring ExePath;

	// 애플리케이션 명
	wstring AppName;

	// 도움말 URL
	wstring HelpURL;

public:

	// 생성자，파괴자
	CGame(wstring app_name, bool zbuffer);
	~CGame();

	// 메세지 핸들러
	LRESULT WINAPI WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

	// 게임 실행
	void Run();

	// 이동과 표시:
	// 서브 클래스에서 오버라이드 함
	virtual void Move() {}
	virtual void Draw() {}

};

