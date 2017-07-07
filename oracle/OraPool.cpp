//#include "ZKProxy.h"
#include "OraPool.h"
#include "ocilib.hpp"
#include "DBSvrCfg.h"
#include "ExpectionFacade.h"
#include "UtilsCommon.h"
//#include "StaticAutoReg.h"

namespace DBUtils {

using namespace std;
using namespace ocilib;


static bool ORA_ENVIRONMENT_INIT = false;
static bool ORA_ENVIRONMENT_FININIT = false;

CORAPool::AutoReg::AutoReg(Utils::CIOCCreater<DBType, CDBPoolItf> &m_PoolCreater)
{
	DBType type = DB_ORA;
	m_PoolCreater.RegisterType<CORAPool>(type);
}


CORAPool::CORAPool()
{

}

bool CORAPool::Init(const DBInstanceNode &dbIn)
{
	//只能初始化一次，否则会出问题
	if (!ORA_ENVIRONMENT_INIT)
	{
		InitEnv();
		ORA_ENVIRONMENT_INIT = true;
	}
	
	m_lConnectionCfgNum = atoi(dbIn.connectionNum.c_str());
	for (long i = 0; i < m_lConnectionCfgNum; ++i)
	{
		DBType type = Convert(dbIn.dbType);
		std::ostringstream os;
		os << dbIn.hostName << ":" << dbIn.portNum << "/" << dbIn.dbName;
		boost::shared_ptr<CConnectionWrapper> ptr = CConnectionWrapper::Create(os.str(), dbIn.usrName, dbIn.password, type);
		ptr->Init();
		m_connectionWrapVec.push_back(ptr);
		m_connectionQueue.push(ptr.get());
	}
	//启动心跳连接池检查
	InitHeartBeat(atoi(dbIn.heartBeatCycle.c_str()));
	m_eDBType = ConvertRWType(dbIn.rwType);
	return true;
}

void CORAPool::Finit()
{
	if (!ORA_ENVIRONMENT_FININIT)
	{
		ReleaseEnv();
		ORA_ENVIRONMENT_FININIT = true;
	}
	
	DisablePool();
	return;
}

CORAPool::~CORAPool() throw()
{

}

void CORAPool::InitEnv()
{
	try
	{
		string strOciPath;
		//CheckEnvVariable(strOciPath);
		Environment::Initialize(Environment::Default | Environment::Threaded);
		Environment::EnableWarnings(true);
	}
	catch (...)
	{
		CExpectionFacade error("Init ORA Envrionment Failed", DB_ENV_INITFAILED);
		throw error;
	}
}

void CORAPool::ReleaseEnv()
{
	Environment::Cleanup();
}

/*
void CORAPool::CheckEnvVariable(string &strOciPath)
{
	std::string strEnvPath = GetEnvVariable(DT_DB_NLS_LANG_NAME);
	if (0 != strcmp(strEnvPath.c_str(), DT_DB_NLS_LANG_VALUE))
	{
		throw runtime_error("NLS_LANG 为空或有误！");
	}

	strEnvPath = GetEnvVariable(DT_OCI_PATH_ENV_VARI);
	if (strEnvPath.empty())
	{
		throw runtime_error("OCI_PATH 为空!");
	}
	else
	{
		strOciPath = strEnvPath;
	}
}*/

//Utils::CIOCCreater<DBType, CDBPoolItf> CDBPoolItf::sm_PoolCreater;
//CORAPool::AutoReg CORAPool::sm_AutoReg(CDBPoolItf::sm_PoolCreater);

}

