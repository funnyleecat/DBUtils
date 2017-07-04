#pragma once
#include <string>
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>
#include "export.h"
#include "DBSvrCfg.h"
#include "ExportDef.hpp"
#include "DBConnectionWrapper.h"

#include <boost/lockfree/queue.hpp> 
#include <boost/atomic.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/signals2.hpp>
#include <boost/function.hpp>
#include <queue>
#include "IoCContainer.h"

using namespace std;
using namespace boost;


class TimerService;

namespace DBUtils {
	typedef std::vector<CConnectionWrapper*> CConnectionWrapperRawPtrVec;
	//typedef  SignalFinit;
#ifdef WIN64
	#define DT_OCI_PATH_ENV_VARI    "OCI_PATH_X64"
#else
	#define DT_OCI_PATH_ENV_VARI    "OCI_PATH"
#endif
#define DT_DB_NLS_LANG_NAME     "NLS_LANG"
	#define DT_DB_NLS_LANG_VALUE    "SIMPLIFIED CHINESE_CHINA.ZHS16GBK"
	class CDBPoolItf;

	class CDBPoolItf
	{
	public:		
		virtual ~CDBPoolItf()throw();
		virtual bool Init(const DBInstanceNode &dbIn) = 0;
		virtual void Finit() = 0;
		DBRWType DBWRType();
		CConnectionWrapper* GetConnection(long lTimeOut);
		void PushConnection(CConnectionWrapper *pConn);
		virtual long GetConnectionNum();
		static boost::shared_ptr<CDBPoolItf> Create(DBType eDBType);	
	protected:
		CDBPoolItf();
		void DisablePool();
		void PopConnection(CConnectionWrapper *&pConn, long lTimeOut);
		void InitHeartBeat(int nHeartBeatCycle);
		void CheckConnection();
	protected:
		//这个容器存储所有连接的智能指针,自动调用析构函数
		CConnectionWrapperPtrVec m_connectionWrapVec;
		//外部用户获取连接的队列池
		CConnectionWrapperPtrQueue m_connectionQueue;
		//配合条件变量下使用
		boost::mutex m_mutex;
		//队列在阻塞情况下同步使用
		boost::condition_variable_any m_dataCond;
		//连接池的读写类型
		DBRWType m_eDBType;
		boost::shared_ptr<TimerService> m_timerSrv;
		//连接池中一共配置了多少个连接数量
		boost::atomic_long m_lConnectionCfgNum;
		//失效的链接
		CConnectionWrapperRawPtrVec m_lostConnVec;
		//创建数据库池的函数池
		static Utils::CIOCCreater<DBType, CDBPoolItf> sm_PoolCreater;
		//static CreatePoolFuncMap sm_createPoolFuncMap;
	};

	typedef boost::shared_ptr<CDBPoolItf> CDBPoolItfPtr;
	typedef std::map<std::string, CDBPoolItfPtr> CDBPoolItfPtrMap;

	
}

