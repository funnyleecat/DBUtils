#pragma once
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <vector>
#include <ocilib.h>
#include <UtilsCommon.h>
#include <queue>
#include <UtilsCommon.h>
#include "DBIterator.h"
#include "export.h"
#include <../interface/exportDef.hpp>
#include "DBConnectionWrapper.h"



namespace DBUtils
{
	
	class COracleConnectionWrapper : public CConnectionWrapper
	{
		struct AutoReg
		{
			AutoReg(Utils::CIOCCreaterWithParam<DBType, CConnectionWrapper, const CreateConnParam &> &m_PoolCreater);
		};
	public:
		COracleConnectionWrapper(const CreateConnParam &refParam);
		~COracleConnectionWrapper() throw();
		CDBIteratorSPtr Execute(const std::string &strCmd,
			StrVariantBindInTypeMap &inVariantBindTypeMap,
			StrVariantBindInOutTypeMap &inOutStrVariantTypeValMap,
			CDBIteratorSPtrMap &iteratorMap);
		bool Init();
		void Finit();
		void RollBack();
		void Commit();
		bool TestValidate();
		void Break();
	protected:	
		void BindCurisor(CDBIteratorSPtrPair &iterPair, Statement &st);
		void InitOutputCursor(CDBIteratorSPtrPair &iterPair);
		size_t GetMaxSize(const StrVariantBindInTypeMap &inVariantBindTypeMap);
		template<typename T, typename Visitor>
		void Bind(std::map<std::string, T> &map, Statement &st);	
		
	private:
		std::string const m_strDBName;
		std::string const m_strUName;
		std::string const m_strPWDWord;
		Connection m_hOraConn;
		static AutoReg sm_AutoReg;
	};
}
