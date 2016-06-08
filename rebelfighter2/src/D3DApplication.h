#include "Timer.h"

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

	DOUBLE					m_fFps			;	//FPS ���� ����
	DOUBLE					m_deltatime   ;	//�ð� ���� ����
	FLOAT					m_movingdist  ;	//�⺻�̵��ȼ�
	UINT					m_gamebegin	;	//���ӽ��۽ð�
	UINT					m_alphatime	;	//timegettime�Լ��� ���ϰ��� ���ӽ��۽ð����� �� ��, �� ���� ���α׷��� ���۵ǰ������� �ð�

	CTimer					m_timer;				// Ÿ�̸�

	CD3DApplication();
	virtual ~CD3DApplication();

	//Window+Device���� �Լ��� 
	INT		Create(HINSTANCE hInst);
	INT		Run();
	void	Cleanup();

	static LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	////////////////////////////////////////////////////////////////////////////////
	
	virtual LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ); 
	virtual INT	Init();				//���� ��ü ����, �ʱ�ȭ
	virtual void	Destroy();				//���� ��ü �Ҹ�
	virtual INT	GameFrameMove();		//���� ��ü ����
	virtual INT	GameRender();			//���� ��ü ������

	virtual void UpdateFrame();
};

extern float g_fSecondPerFrame;