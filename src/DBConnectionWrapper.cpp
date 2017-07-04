//#include "ZKProxy.h"
#include "ocilib.hpp"
#include <iostream>
#include "DBConnectionWrapper.h"
#include "OracleConnectionWrapper.h"
#include <sstream>
//#include "CreateProxy.h"

using namespace std;
using namespace ocilib;

namespace DBUtils {



CConnectionWrapper::~CConnectionWrapper() throw()
{

}

boost::shared_ptr<CConnectionWrapper> CConnectionWrapper::Create(const std::string &strDBName, 
																const std::string &strUName,
																const std::string &strPWDWord,
																DBType type)
{
	CreateConnParam connParam(strDBName, strUName, strPWDWord);
	return boost::shared_ptr<CConnectionWrapper>(sm_PoolCreater.GetRegisterType(type,connParam));
}

}
