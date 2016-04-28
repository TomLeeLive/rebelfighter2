#include "Input.h"

#define JOYAXIS_RANGE 1000
#define JOYAXIS_MARGIN 200


//==============================================================
// CInputState 클래스

// 생성자
CInputState::CInputState() {
	Clear();
}

// 상태 클리어
void CInputState::Clear() {
	Up=Down=Left=Right=false;
	Up2=Down2=Left2=Right2=false;
	for (int i=0; i<MAX_BUTTONS; i++) Button[i]=false;
	AnalogX=AnalogY=0;
	AnalogX2=AnalogY2=0;
}


//==============================================================
// 열거용 콜백함수
BOOL CALLBACK EnumJoysticksCallbackWrapper(
	const DIDEVICEINSTANCE* did, VOID* ref
) {
	CInput* input=(CInput*)ref;
	return input->EnumJoysticksCallback(did);
}

// 
BOOL CALLBACK CInput::EnumJoysticksCallback(
	const DIDEVICEINSTANCE* did
) {
	LPDIRECTINPUTDEVICE8 joystick=NULL;
	if (!FAILED(DI->CreateDevice(
		did->guidInstance, &joystick, NULL))) {
		JoySticks.push_back(joystick);
	}
	return DIENUM_CONTINUE;
}

// 축의 범위를 설정하기
BOOL CALLBACK EnumObjectsCallbackWrapper(
	const DIDEVICEOBJECTINSTANCE* dido, VOID* ref
) {
	LPDIRECTINPUTDEVICE8 joystick=(LPDIRECTINPUTDEVICE8)ref;
	if (dido->dwType & DIDFT_AXIS) {
		DIPROPRANGE dip;
		dip.diph.dwSize=sizeof(DIPROPRANGE); 
		dip.diph.dwHeaderSize=sizeof(DIPROPHEADER); 
		dip.diph.dwHow=DIPH_BYID; 
		dip.diph.dwObj=dido->dwType;
		dip.lMin=-JOYAXIS_RANGE; 
		dip.lMax=JOYAXIS_RANGE;
		joystick->SetProperty(DIPROP_RANGE, &dip.diph); 
	}	
	return DIENUM_CONTINUE;
}


//==============================================================
// 생성자
CInput::CInput(HWND window) {

	// DirectInput 인터페이스 생성
	DI=NULL;
	if (FAILED(DirectInput8Create(
			GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
			IID_IDirectInput8, (void**)&DI, NULL))
	) return;

	// 키보드 디바이스 생성과 초기화
	Keyboard=NULL;
	if (FAILED(DI->CreateDevice(
			GUID_SysKeyboard, &Keyboard, NULL)) ||
		FAILED(Keyboard->SetDataFormat(
			&c_dfDIKeyboard)) ||
		FAILED(Keyboard->SetCooperativeLevel(
			window, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))
	) { FreeResources(); return; }

	// 조이스틱 디바이스 생성과 초기화
	if (FAILED(DI->EnumDevices(DI8DEVCLASS_GAMECTRL, 
			EnumJoysticksCallbackWrapper, 
			this, DIEDFL_ATTACHEDONLY))
	) { FreeResources(); return; }
	for (int i=0, n=JoySticks.size(); i<n; i++) {
		if (FAILED(JoySticks[i]->SetDataFormat(
				&c_dfDIJoystick2)) ||
			FAILED(JoySticks[i]->SetCooperativeLevel(
				window, DISCL_FOREGROUND | DISCL_EXCLUSIVE)) ||
			FAILED(JoySticks[i]->EnumObjects(
				EnumObjectsCallbackWrapper, 
				JoySticks[i], DIDFT_ALL))
		) { FreeResources(); return; }
	}
}


//==============================================================
// 파괴자
CInput::~CInput() {
	FreeResources();
}


//==============================================================
// 자원의 해제
void CInput::FreeResources() {
	if (Keyboard) {
		Keyboard->Unacquire();
		Keyboard->Release();
	}
	for (int i=0, n=JoySticks.size(); i<n; i++) {
		JoySticks[i]->Unacquire();
		JoySticks[i]->Release();
	}
	if (DI) DI->Release();
}


//==============================================================
// 입력상태를 최신으로 업데이트
#define KEYDOWN(key) ((key_state[key]&0x80)!=0)
#define BTNDOWN(btn) ((joy_state.rgbButtons[btn]&0x80)!=0)

void CInput::UpdateState() {
	ClearState();
		
	// 키보드
	if (Keyboard) {
		char key_state[256];
		if (!FAILED(Keyboard->Acquire()) &&
			!FAILED(Keyboard->GetDeviceState(
				sizeof(key_state), key_state))
		) {
			CInputState& s=State[0];

			s.Up|=KEYDOWN(DIK_UP);
			s.Down|=KEYDOWN(DIK_DOWN);
			s.Left|=KEYDOWN(DIK_LEFT);
			s.Right|=KEYDOWN(DIK_RIGHT);
			s.Button[0]|=KEYDOWN(DIK_Z);
			s.Button[1]|=KEYDOWN(DIK_X);
			s.Button[2]|=KEYDOWN(DIK_C);
			s.Button[3]|=KEYDOWN(DIK_V);
			s.AnalogY+=(s.Up? -1 : (s.Down? 1 : 0));
			s.AnalogX+=(s.Left? -1 : (s.Right? 1 : 0));

			s.Up2|=KEYDOWN(DIK_E);
			s.Down2|=KEYDOWN(DIK_D);
			s.Left2|=KEYDOWN(DIK_S);
			s.Right2|=KEYDOWN(DIK_F);
			s.AnalogY2+=(s.Up2? -1 : (s.Down2? 1 : 0));
			s.AnalogX2+=(s.Left2? -1 : (s.Right2? 1 : 0));
		}
	}

	// 조이스틱
	for (int i=0, n=JoySticks.size(); i<n; i++) {
		DIJOYSTATE2 joy_state;
		JoySticks[i]->Poll();
		if (!FAILED(JoySticks[i]->Acquire()) &&
			!FAILED(JoySticks[i]->GetDeviceState(
				sizeof(joy_state), &joy_state))
		) {
			CInputState& s=State[0];

			s.Up|=(joy_state.lY<-JOYAXIS_MARGIN);
			s.Down|=(joy_state.lY>JOYAXIS_MARGIN);
			s.Left|=(joy_state.lX<-JOYAXIS_MARGIN);
			s.Right|=(joy_state.lX>JOYAXIS_MARGIN);
			s.Button[0]|=BTNDOWN(0);
			s.Button[1]|=BTNDOWN(1);
			s.Button[2]|=BTNDOWN(2);
			s.Button[3]|=BTNDOWN(3);

			// Y방향의 아날로그 입력
			int jy=joy_state.lY;
			if (jy<=-JOYAXIS_RANGE+JOYAXIS_MARGIN) s.AnalogY+=-1;
			else if (-JOYAXIS_MARGIN<=jy && jy<=JOYAXIS_MARGIN) s.AnalogY+=0;
			else if (JOYAXIS_RANGE-JOYAXIS_MARGIN<=jy) s.AnalogY+=1; 
			else if (jy<0) s.AnalogY+=(float)(jy+JOYAXIS_MARGIN)/(JOYAXIS_RANGE-JOYAXIS_MARGIN*2);
			else s.AnalogY+=(float)(jy-JOYAXIS_MARGIN)/(JOYAXIS_RANGE-JOYAXIS_MARGIN*2);

			// X방향의 아날로그 입력
			int jx=joy_state.lX;
			if (jx<=-JOYAXIS_RANGE+JOYAXIS_MARGIN) s.AnalogX+=-1; else 
			if (-JOYAXIS_MARGIN<=jx && jx<=JOYAXIS_MARGIN) s.AnalogX+=0; else
			if (JOYAXIS_RANGE-JOYAXIS_MARGIN<=jx) s.AnalogX+=1;
			else if (jx<0) s.AnalogX+=(float)(jx+JOYAXIS_MARGIN)/(JOYAXIS_RANGE-JOYAXIS_MARGIN*2);
			else s.AnalogX+=(float)(jx-JOYAXIS_MARGIN)/(JOYAXIS_RANGE-JOYAXIS_MARGIN*2);
		}
	}
}


//==============================================================
// 아무것도 입력되지 않은 상태로 만들기
void CInput::ClearState() {
	for (int i=0; i<MAX_PLAYERS; i++) {
		State[i].Clear();
	}
}


