#pragma once
#include <string>
#include <TimerTaskMgr.h>
class CConnectionWrapper;
class CDBIterator;

namespace DBUtils {

	class CDBStatementFacade : public CDBStatement
	{
		public:
			virtual bool Excute(long timeOut) = 0;
			virtual ~CDBStatementFacade() throw();
			virtual void Commit() = 0;
			virtual void RollBack() = 0;
			virtual CConnectionWrapper* GetConnection() = 0;
			virtual std::string GetInstanceName() = 0;
			virtual EXEMode GetEXEMode() = 0;
			virtual long GetSlotIndex() = 0;
			static CDBStatement* Create(CConnectionWrapper *pConnection, const std::string  &strDbInstance, EXEMode eMode, long lSlotIndex);
	};
}
