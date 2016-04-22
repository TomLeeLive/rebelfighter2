

class CD3DApplication
{
public:
	////////////////////////////////////////////////////////////////////////////////
	char				m_sCls[128];
	HINSTANCE			m_hInst		;
	HWND				m_hWnd		;
	DWORD				m_dWinStyle;
	DWORD				m_dScnX		;			// Screen Width
	DWORD				m_dScnY		;			// Screen Height
	BOOL				m_bShowCusor;			// Show Cusor

	LPDIRECT3D9       m_pD3D				;  // D3D
	LPDIRECT3DDEVICE9  m_pd3dDevice		; // Direct3D Device
	LPD3DXSPRITE        m_pd3dSprite		;// D3DX Sprite

public:

	DOUBLE					m_fFps			;	//FPS 계산용 변수
	DOUBLE					m_deltatime   ;	//시간 계산용 변수
	FLOAT					m_movingdist  ;	//기본이동픽셀
	UINT					m_gamebegin	;	//게임시작시간
	UINT					m_alphatime	;	//timegettime함수의 리턴값을 게임시작시간으로 뺀 값, 즉 게임 프로그램이 시작되고서부터의 시간

	CD3DApplication();
	virtual ~CD3DApplication();

	//Window+Device관련 함수들 
	INT		Create(HINSTANCE hInst);
	INT		Run();
	void	Cleanup();

	static LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	////////////////////////////////////////////////////////////////////////////////
	
	virtual LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ); 
	virtual INT	Init(){ return 0;};				//게임 객체 생성, 초기화
	virtual void	Destroy(){ return ;};				//게임 객체 소멸
	virtual INT	GameFrameMove(){ return 0;};		//게임 객체 갱신
	virtual INT	GameRender(){ return 0;};			//게임 객체 렌더링

protected:

	void	UpdateFrame();
};
