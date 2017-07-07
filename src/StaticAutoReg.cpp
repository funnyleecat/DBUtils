#include "OracleConnectionWrapper.h"
#include "OraPool.h"
namespace DBUtils
{
	Utils::CIOCCreaterWithParam<DBType, CConnectionWrapper, const CreateConnParam &> CConnectionWrapper::sm_PoolCreater;
	COracleConnectionWrapper::AutoReg COracleConnectionWrapper::sm_AutoReg(CConnectionWrapper::sm_PoolCreater);

	Utils::CIOCCreater<DBType, CDBPoolItf> CDBPoolItf::sm_PoolCreater;
	CORAPool::AutoReg CORAPool::sm_AutoReg(CDBPoolItf::sm_PoolCreater);
}
