#pragma once
#include "DBPool.h"
#include "UtilsCommon.h"
#include "DBSvrCfg.h"

namespace DBUtils {
	class CPoolProfile;
	typedef boost::shared_ptr<CPoolProfile> CPoolProfilePtr;

	class CLogcalDBPool
	{
		public:
			~CLogcalDBPool() throw();
			 void Finit();
			 CConnectionWrapper * GetConnection(EXEMode eMode, long lTimeOut, long &lSlotIndex);
			 void PushConnection(EXEMode eMode, CConnectionWrapper *pConnection, long lSlotIndex);
			 static boost::shared_ptr<CLogcalDBPool> Create();
			 bool Add(const DBInstanceNode &dbIn);
			 CLogcalDBPool();
		private:		
			CPoolProfile& GetPoolProfile(EXEMode eMode);
	protected:
		CPoolProfilePtr m_rPoolPtr;
		CPoolProfilePtr m_wPoolPtr;
	};

	typedef boost::shared_ptr<CLogcalDBPool> CLogcalDBPoolPtr;
	typedef std::map<std::string, CLogcalDBPoolPtr> CLogcalDBPtrPoolMap;
}

