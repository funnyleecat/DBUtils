#pragma once
#include "DBPool.h"
#include "DBConnectionWrapper.h"

class CConnectionWrapper;
class CDBPoolItf;

namespace DBUtils {


class CORAPool :public CDBPoolItf
{
	struct AutoReg
	{
		AutoReg(Utils::CIOCCreater<DBType, CDBPoolItf> &m_PoolCreater);
	};
public:
	bool Init(const DBInstanceNode &dbIn);
	void Finit();
	~CORAPool() throw();
	void InitEnv();
	void ReleaseEnv();
	void CheckEnvVariable(string &strOciPath);
	CORAPool();
protected:
	static AutoReg sm_AutoReg;
};

}
