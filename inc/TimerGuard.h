#pragma once
#include "TimerTaskMgr.h"
class CConnectionWrapper;
namespace DBUtils {

class CEXETimeOutTask :public CTimerTask
{
	public:
		CEXETimeOutTask(CConnectionWrapper &conRef);
		void OnRunTask();
		~CEXETimeOutTask();
	private:
		CConnectionWrapper &m_conRef;	
};

typedef boost::shared_ptr<CEXETimeOutTask> CEXETimeOutTaskSPtr;

class CTimerGuard
{
	public:
		CTimerGuard(CConnectionWrapper &conRef,long lTimeOut);
		~CTimerGuard();
	private:
		CEXETimeOutTaskSPtr m_TimeOutTaskPtr;
};


}
