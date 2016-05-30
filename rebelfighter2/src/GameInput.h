#pragma once

class CGameInput
{
protected:
	HWND			m_hWnd; 
	BYTE			m_KeyCur[256];		//���� �������� Ű���� ��//BYTE ==> unsigned char 
	BYTE			m_KeyOld[256];		//���� �������� Ű���� ��
	BYTE			m_MouseCur[16];	//���� �������� ���콺 ��
	BYTE			m_MouseOld[16];	//���� �������� ���콺 ��
	D3DXVECTOR3	m_MousePos;
	FLOAT			m_fWheel;

public:
	CGameInput();
	virtual ~CGameInput();
	
	virtual INT Create(HWND hWnd);
	virtual INT FrameMove();
	virtual LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual INT KeyDown(INT Key);
	virtual INT KeyUp(INT Key);
	virtual INT KeyPress(INT Key);
	virtual INT KeyState(INT Key); 
	
	virtual INT ButtonDown(INT Key);
	virtual INT ButtonUp(INT Key);
	virtual INT ButtonPress(INT Key);
	virtual INT ButtonState(INT Key);

	HRESULT MenuItemClickCheck(void);
};
