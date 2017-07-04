#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <vector>
#include <ocilib.h>
#include <UtilsCommon.h>
#include <queue>
#include "Export.h"
#include <ExportDef.hpp>
#include "IoCContainer.h"

namespace DBUtils {

using namespace std;
//class ZKProxy;

#define DT_DATE_FORMAT     "YYYY-MM-DD HH24:MI:SS"

typedef struct CreateConnParam
{
	CreateConnParam(const std::string &strDBNameP,
	const std::string &strUNameP,
	const std::string &strPWDWordP)
	:strDBName(strDBNameP)
	, strUName(strUNameP)
	, strPWDWord(strPWDWordP)
	{

	}
	const std::string &strDBName;
	const std::string &strUName;
	const std::string &strPWDWord;
}CreateConnParam;

class CConnectionWrapper
{
	public:
		virtual ~CConnectionWrapper() throw();

		virtual bool Init() = 0;

		virtual void Finit() = 0;

		virtual CDBIteratorSPtr Execute(const std::string &strCmd,
			StrVariantBindInTypeMap &inVariantBindTypeMap,
			StrVariantBindInOutTypeMap &inOutStrVariantTypeValMap,
			CDBIteratorSPtrMap &iteratorMap) = 0;

		static boost::shared_ptr<CConnectionWrapper> Create(const std::string &strDBName, const std::string &strUName, const std::string &strPWDWord, DBType dbType);

		virtual bool TestValidate() = 0; 

		virtual void RollBack() = 0;

		virtual void Commit() = 0;

		virtual void Break() = 0;
	protected:
		RWMutex m_mutex;
		//创建数据库连接的函数池
		static Utils::CIOCCreaterWithParam<DBType, CConnectionWrapper,const CreateConnParam &> sm_PoolCreater;
};




typedef boost::shared_ptr<CConnectionWrapper> CConnectionWrapperPtr;
typedef std::vector<CConnectionWrapperPtr> CConnectionWrapperPtrVec;

typedef std::queue<CConnectionWrapper*> CConnectionWrapperPtrQueue;


}
