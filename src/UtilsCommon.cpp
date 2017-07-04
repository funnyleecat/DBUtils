#include "UtilsCommon.h"

namespace DBUtils
{
	std::string GetEnvVariable(const std::string &strVariName)
	{
		//效果和GetEnvironmentVariable差不多
		char* pPath = NULL;
		std::string strRtn;
#ifdef WIN32
		size_t nSize = 0;
		int nRtn = _dupenv_s(&pPath, &nSize, strVariName.c_str());
		if (0 == nRtn)
		{
			strRtn = std::string(pPath, nSize);
			//需要手动释放
			free(pPath);
			pPath = NULL;
		}
#else
		pPath = getenv(strVariName.c_str());
		if (pPath)
		{
			strRtn = pPath;
		}
#endif
		return strRtn;
	}

	DBType Convert(const std::string &str)
	{
		if ("ORACLE" == str)
		{
			return DB_ORA;
		}
		else if ("MYSQL" == str)
		{
			return DB_MYSQL;
		}
		else if ("REDIS" == str)
		{
			return DB_REDIS;
		}
		else
		{
			return DB_Unknown;
		}
	}

	DBRWType ConvertRWType(const std::string &str)
	{
		if ( "RO" == str)
		{
			return RO;
		}
		else if ("WO" == str)
		{
			return WO;
		}
		else
		{
			return WR_Unknown;
		}
	}
}
