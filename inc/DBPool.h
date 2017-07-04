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
		//��������洢�������ӵ�����ָ��,�Զ�������������
		CConnectionWrapperPtrVec m_connectionWrapVec;
		//�ⲿ�û���ȡ���ӵĶ��г�
		CConnectionWrapperPtrQueue m_connectionQueue;
		//�������������ʹ��
		boost::mutex m_mutex;
		//���������������ͬ��ʹ��
		boost::condition_variable_any m_dataCond;
		//���ӳصĶ�д����
		DBRWType m_eDBType;
		boost::shared_ptr<TimerService> m_timerSrv;
		//���ӳ���һ�������˶��ٸ���������
		boost::atomic_long m_lConnectionCfgNum;
		//ʧЧ������
		CConnectionWrapperRawPtrVec m_lostConnVec;
		//�������ݿ�صĺ�����
		static Utils::CIOCCreater<DBType, CDBPoolItf> sm_PoolCreater;
		//static CreatePoolFuncMap sm_createPoolFuncMap;
	};

	typedef boost::shared_ptr<CDBPoolItf> CDBPoolItfPtr;
	typedef std::map<std::string, CDBPoolItfPtr> CDBPoolItfPtrMap;

	
}

