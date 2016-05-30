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
			// 하나의 소켓으로 RECV로 수신대기중인 상태에서
			// SEND로 데이터를 보내게 되면
			// RECV로 대기중이던 게 취소된다.
			if( keyValue !=0 &&
				GetLastError() != ERROR_OPERATION_ABORTED )
			{
				// 소켓을 삭제 유저 삭제.
				if(  bytesTransfer == 0)
				{
					if( rReturn == FALSE ) 
						I_DebugStr.DisplayText("%s%s\r\n", 
						pSession->m_Name.c_str(),"비동기 소켓 오류.");				
					// 클라이언트 접속 종료
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
