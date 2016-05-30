#include "TWorkThread.h"
#include "TServerIOCP.h"
#include "TServer.h"
#include "TDebugString.h"
//#if defined(_WIN64)
bool TWorkThread::Run()
{
	DWORD			bytesTransfer;
	ULONG_PTR		keyValue;
	LPOVERLAPPED	overlapped;
	BOOL rReturn;
	while(m_bLoop)
	{
		rReturn = ::GetQueuedCompletionStatus(
					I_ServerIOCP.m_hIOCP,
					&bytesTransfer,
					&keyValue,
					&overlapped,
					INFINITE );
		
		TUser* pSession = (TUser*)keyValue;
		if( pSession == 0 ) continue;

		if( rReturn == TRUE &&
			bytesTransfer != 0 &&
			overlapped != 0 )
		{			
			pSession->Dispatch(bytesTransfer,
								overlapped );
		}else
		{
			// �ϳ��� �������� RECV�� ���Ŵ������ ���¿���
			// SEND�� �����͸� ������ �Ǹ�
			// RECV�� ������̴� �� ��ҵȴ�.
			if( keyValue !=0 &&
				GetLastError() != ERROR_OPERATION_ABORTED )
			{
				// ������ ���� ���� ����.
				if(  bytesTransfer == 0)
				{
					if( rReturn == FALSE ) 
						I_DebugStr.DisplayText("%s%s\r\n", 
						pSession->m_Name.c_str(),"�񵿱� ���� ����.");				
					// Ŭ���̾�Ʈ ���� ����
					I_Server.DelUser(pSession->m_iEvent);
					continue;			
				}
			}
		}
		I_Server.ProcessPacket();
	}
	return true;
}
TWorkThread::TWorkThread(void)
{
	m_bLoop = true;
}

TWorkThread::~TWorkThread(void)
{
	m_bLoop = false;
}
