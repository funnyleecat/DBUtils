#include "DBMgr.hpp"
#include "DBPool.h"
#include "DBSvrCfg.h"
#include "DBConnectionWrapper.h"
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>  
#include <boost/atomic.hpp>
#include "Statement.h"
#include "DBStatementFacade.h"
#include "ExpectionFacade.h"
#include "DBIterator.h"
#include "ExportDef.hpp"
#include "DBProfile.h"
#include <assert.h>

namespace DBUtils {


class  CDBMgrImpl : public CDBMgr
{
	public:
		CDBMgrImpl() :m_bInit(false)
		{

		}

		bool InitDBProfile(const DBLogicNode &ref, CLogcalDBPool &dbProfile)
		{
			const DBInstanceVec &dbInstanceVec = ref.dbInstanceVec;
			DBInstanceVec::const_iterator iter = dbInstanceVec.begin();
			while (iter != dbInstanceVec.end())
			{
				IF_DO(!dbProfile.Add(*iter), return false)
				++iter;
			}
			return true;
		}

		virtual bool Init(const std::string &strCfgPath)
		{
			IF_DO(m_bInit, throw CExpectionFacade("Environment is already init!", DB_ENV_INITFAILED))
			WriteLock lock(m_mutex);
			CDBSvrCfg &cfg = CDBSvrCfg::Instance();
			if (!cfg.Init(strCfgPath))
			{
				return false;
			}
			CDBSvrIterator &iter = CDBSvrCfg::Instance().GetIterator();
			int totalNum = iter.Count();
			int i = 0;
			while (i < totalNum)
			{
				const DBLogicNode &ref = iter.At(i);
				CLogcalDBPoolPtr ptr = CLogcalDBPool::Create();
				IF_DO(!InitDBProfile(ref, *ptr),return false)
				m_dbPools.insert(make_pair(ref.uniqueId, ptr));
				m_sigFinit.connect(bind(&CLogcalDBPool::Finit, *ptr));
				++i;
			}

			CTimerTaskMgr::Instance().Init(300);
			m_bInit = true;
			return true;
		}

		virtual void Finit()
		{
			m_bInit = false;
			m_sigFinit();
			CTimerTaskMgr::Instance().Finit();
		}

		virtual CDBStatement* AllocStatement(const std::string &strDbInstance, EXEMode eMode, long lTimeOut)
		{
			IF_DO(!m_bInit, throw CExpectionFacade("Environment is not Init!", DB_ENV_INITFAILED))
			ReadLock lock(m_mutex);
			CLogcalDBPtrPoolMap::iterator iter = m_dbPools.find(strDbInstance);

			IF_DO(iter == m_dbPools.end(),return NULL)

			CLogcalDBPoolPtr ptr = iter->second;
			long slotIndex = 0;
			CConnectionWrapper *fc = ptr->GetConnection(eMode, lTimeOut, slotIndex);
			IF_DO(fc == NULL, return NULL)
				CDBStatement *st = reinterpret_cast<CDBStatement *>(CDBStatementFacade::Create(fc, strDbInstance, eMode, slotIndex));
			return st;		
		}
		
		virtual void Release(CDBStatement *&pStatement)
		{
			IF_DO(!m_bInit, throw CExpectionFacade("Environment is not Init!", DB_ENV_INITFAILED))
				Check_Ptr(pStatement);
			ReadLock lock(m_mutex);
			IF_DO(NULL == pStatement, throw CExpectionFacade("NULL DBStatement", DB_NULL_STATEMENT))
			CDBStatementFacade *dbF = reinterpret_cast<CDBStatementFacade *>(pStatement);
			CLogcalDBPtrPoolMap::iterator iter = m_dbPools.find(dbF->GetInstanceName());

			IF_DO(iter == m_dbPools.end(),return)

			CLogcalDBPoolPtr ptr = iter->second;
			if (NULL != pStatement)
			{
				ptr->PushConnection(dbF->GetEXEMode(), dbF->GetConnection(), dbF->GetSlotIndex());
				delete pStatement;
				pStatement = NULL;
			}
		}
		
		virtual void Commit(CDBStatement *pStatement)
		{
			IF_DO(!m_bInit, throw CExpectionFacade("Environment is not Init!", DB_ENV_INITFAILED))
			Check_Ptr(pStatement);
			reinterpret_cast<CDBStatementFacade*>(pStatement)->Commit();
		}

		virtual void RollBack(CDBStatement *pStatement)
		{
			IF_DO(!m_bInit, throw CExpectionFacade("Environment is not Init!", DB_ENV_INITFAILED))
			Check_Ptr(pStatement);
			reinterpret_cast<CDBStatementFacade*>(pStatement)->RollBack();
		}

		virtual bool Execute(CDBStatement *pStatement, long lTimeOut)
		{
			IF_DO(!m_bInit, throw CExpectionFacade("Environment is not Init!", DB_ENV_INITFAILED))
			Check_Ptr(pStatement);
			return reinterpret_cast<CDBStatementFacade *>(pStatement)->Excute(lTimeOut);
		}

	protected:
		template<typename Ptr>
		void Check_Ptr(Ptr *p)
		{
			assert(p != NULL);
		}
		void CheckRWMode(EXEMode mode, DBRWType wrType)
		{
			if ((int)mode != (int)wrType)
			{
				throw CExpectionFacade("DB RW type error", DB_RW_ERROR);
			}
		}

	private:
		RWMutex m_mutex;
		boost::atomic_bool m_bInit;
		CLogcalDBPtrPoolMap m_dbPools;
		SigVoid m_sigFinit;
};

CDBMgr& CDBMgr::Instance()
{
	static CDBMgrImpl s_instance;
	return s_instance;
}

}
