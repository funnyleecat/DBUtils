#include "EasyTimer.hpp"
#include "DBPool.h"
#include "OraPool.h"
#include "TimerTaskMgr.h"
//#include "CreateProxy.h"

namespace DBUtils {


CDBPoolItf::CDBPoolItf()
		: m_eDBType(WR_Unknown)
		, m_lConnectionCfgNum(0)
{

}

CDBPoolItf::~CDBPoolItf()throw()
{

}

CConnectionWrapper* CDBPoolItf::GetConnection(long lTimeOut)
{
	CConnectionWrapper *pConn = NULL;
	PopConnection(pConn, lTimeOut);
	return pConn;
}

void CDBPoolItf::PopConnection(CConnectionWrapper *&pConn, long lTimeOut)
{
	Scopedlock lock(m_mutex);
	
	if (m_connectionQueue.empty())
	{
		if (m_dataCond.timed_wait(lock,posix_time::seconds(lTimeOut)))
		{

		}
		else
		{
			return;
		}
	}
	pConn = m_connectionQueue.front();
	m_connectionQueue.pop();

}

void CDBPoolItf::InitHeartBeat(int nHeartBeatCycle)
{
	m_timerSrv = boost::make_shared<TimerService>();
	m_timerSrv->AddTimer(boost::posix_time::seconds(nHeartBeatCycle), boost::bind(&CDBPoolItf::CheckConnection, this));
}

void CDBPoolItf::CheckConnection()
{
	//检查现有的连接池的链接是否可用
	static long TIMEOUTSECS = 5;
	for (int index = 0; index < m_lConnectionCfgNum; ++index)
	{
		CConnectionWrapper *pConn = GetConnection(TIMEOUTSECS);
		bool rst = false;
		{
			//CTimerGuard guard(*pConn, TIMEOUTSECS);
			rst = pConn->TestValidate();
		}
		if (rst)
		{
			PushConnection(pConn);
		}
		else
		{
			m_lostConnVec.push_back(pConn);
		}
	}
	//对断链进行重连
	CConnectionWrapperRawPtrVec::iterator iter = m_lostConnVec.begin();
	while (iter != m_lostConnVec.end())
	{
		CConnectionWrapper *pConn = *iter;
		bool rst = pConn->Init();
		if (rst)
		{
			PushConnection(pConn);
			m_lostConnVec.erase(iter++);
		}
		else
		{
			++iter;
		}
	}
}

long CDBPoolItf::GetConnectionNum()
{
	return m_lConnectionCfgNum;
}

DBRWType CDBPoolItf::DBWRType()
{
	return m_eDBType;
}

void CDBPoolItf::PushConnection(CConnectionWrapper *pConn)
{
	Scopedlock lock(m_mutex);
	m_connectionQueue.push(pConn);
	m_dataCond.notify_one();
}

void CDBPoolItf::DisablePool()
{
	m_timerSrv->RemoveAllTimer();
	BaseExe<CConnectionWrapper> funcProxy;
	std::for_each(m_connectionWrapVec.begin(), 
				  m_connectionWrapVec.end(),
				  boost::bind(&BaseExe<CConnectionWrapper>::ExecuteSharePtr, &funcProxy, _1, &CConnectionWrapper::Finit));
}

boost::shared_ptr<CDBPoolItf> CDBPoolItf::Create(DBType eDBType)
{
	return boost::shared_ptr<CDBPoolItf>(sm_PoolCreater.GetRegisterType(eDBType));
}
 


}
