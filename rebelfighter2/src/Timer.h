#pragma once

class CTimer //: public TBase
{
public:
	int		 m_iFPS; // �ʴ� ������ ī����
	float	 m_fSecondPerFrame;// �ð��� ����ȭ
	float	 m_fAccumulation; // �������Ŀ� ����ð�.
public:
	TCHAR	m_csBuffer[256];//char	m_csBuffer[256];
	DWORD	 m_dwBeforeTick;
	float    m_fFrameTime;
	DWORD	 m_dwFrameCounter;
public:
	bool     Init(); // �ʱ�ȭ
	bool     Frame();// ���
	bool     Render();// ��ο�
	bool     Release();// ����, �Ҹ�
public:
	CTimer();
	virtual ~CTimer();
};

