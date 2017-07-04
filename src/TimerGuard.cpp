#include "DBConnectionWrapper.h"
#include "TimerGuard.h"

namespace DBUtils {
CEXETimeOutTask::CEXETimeOutTask(CConnectionWrapper &conRef)
:m_conRef(conRef)
{

}

void CEXETimeOutTask::OnRunTask()
{
	m_conRef.Break();
}

CEXETimeOutTask::~CEXETimeOutTask()
{

}

CTimerGuard::CTimerGuard(CConnectionWrapper &conRef, long lTimeOut)
{
	m_TimeOutTaskPtr.reset(new CEXETimeOutTask(conRef));
	CTimerTaskMgr::Instance().PushData(m_TimeOutTaskPtr, lTimeOut);
}


CTimerGuard::~CTimerGuard()
{
}


}
