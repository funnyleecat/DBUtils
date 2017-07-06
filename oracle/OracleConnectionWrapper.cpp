#include "EasyTimer.hpp"
#include "OracleDBIterator.hpp"
#include "OracleConnectionWrapper.h"
#include "DBExpection.h"
#include "ExpectionFacade.h"
#include <boost/atomic.hpp>
#include <boost/ref.hpp>
#include "VarVisitor.h"

namespace DBUtils {
	
	COracleConnectionWrapper::AutoReg::AutoReg(Utils::CIOCCreaterWithParam<DBType, CConnectionWrapper, const CreateConnParam &> &m_PoolCreater)
	{
		DBType type = DB_ORA;
		m_PoolCreater.RegisterType<COracleConnectionWrapper>(type);
	}

	COracleConnectionWrapper::COracleConnectionWrapper(const CreateConnParam &refParam)
		: m_strDBName(refParam.strDBName)
		, m_strUName(refParam.strUName)
		, m_strPWDWord(refParam.strPWDWord)
	{

	}

	COracleConnectionWrapper::~COracleConnectionWrapper() throw()
	{

	}

	bool COracleConnectionWrapper::Init()
	{
		bool ret = true;
		try
		{
			WriteLock lock(m_mutex);
			m_hOraConn.Open(m_strDBName, m_strUName, m_strPWDWord, Environment::SessionDefault);
		}
		catch (std::exception &ex)
		{
			CExpectionFacade error(ex.what(), ORA_CONNECTION_ERROR);
			throw error;
			ret = false;
		}
		return ret;
	}

	void COracleConnectionWrapper::RollBack()
	{
		m_hOraConn.Rollback();
	}

	void COracleConnectionWrapper::Commit()
	{
		m_hOraConn.Commit();
	}

	bool COracleConnectionWrapper::TestValidate()
	{
		try {
			WriteLock lock(m_mutex);
			ocilib::Connection &conn = m_hOraConn;
			return conn.PingServer();
		}
		catch (...)
		{
			return false;
		}
	}

	CDBIteratorSPtr COracleConnectionWrapper::Execute(const std::string &strCmd,
		StrVariantBindInTypeMap &inVariantBindTypeMap,
		StrVariantBindInOutTypeMap &inOutStrVariantTypeValMap,
		CDBIteratorSPtrMap &iteratorMap)
	{
		try {
			WriteLock lock(m_mutex);
			boost::shared_ptr<Statement> stPtr = boost::make_shared<Statement>(m_hOraConn);

			Statement &st = *stPtr;

			//一次批量读取 1000 条
			st.SetFetchSize(1000);
			st.Prepare(strCmd);
			st.SetBindMode(Statement::BindByName);
			st.SetFetchMode(Statement::FetchScrollable);

			//对于批量插入场景需要设定插入
			size_t maxSize = GetMaxSize(inVariantBindTypeMap);
			IF_DO(maxSize, st.SetBindArraySize((unsigned int)maxSize))

				IF_DO(!inVariantBindTypeMap.empty(), (Bind<VariantBindInType, VarBindInVistor>(inVariantBindTypeMap, st)))
				IF_DO(!inOutStrVariantTypeValMap.empty(), (Bind<VariantBindInOutType, VarBindVistor>(inOutStrVariantTypeValMap, st)))


			boost::reference_wrapper<COracleConnectionWrapper> cWrp(*this);
			boost::reference_wrapper<Statement> stWrp(*stPtr);
			std::for_each(iteratorMap.begin(), iteratorMap.end(), boost::bind(&COracleConnectionWrapper::BindCurisor, cWrp, _1, stWrp));

			st.ExecutePrepared();

			std::for_each(iteratorMap.begin(), iteratorMap.end(), boost::bind(&COracleConnectionWrapper::InitOutputCursor, cWrp, _1));
			boost::shared_ptr<COracleDBIterator> rPtr(new COracleDBIterator(stPtr));
			rPtr->InitResultSet();
			return rPtr;
		}
		catch (ocilib::Exception &ex)
		{
			//#define ORA_ERR_DISCONNECT    3134
			//#define ORA_ERR_DISCONNECT2   3135 
			//#define ORA_ERR_BREAK         1013
			m_hOraConn.Rollback();
			throw CExpectionFacade(ex.what(), ConvertErrorCode(ex.GetOracleErrorCode()));
		}
	}

	long COracleConnectionWrapper::ConvertErrorCode(int oraCode)
	{
		#define ORA_ERR_BREAK         1013
		#define ORA_ERR_DISCONNECT    3134
		#define ORA_ERR_DISCONNECT2   3135 
		if (oraCode == ORA_ERR_BREAK)
		{
			return ORA_EXECUTE_TIMEOUT;
		}
		else if (oraCode == ORA_ERR_DISCONNECT)
		{
			return ORA_EXECUTE_DISCONNECT;
		}
		else if (oraCode == ORA_ERR_DISCONNECT2)
		{
			return ORA_EXECUTE_DISCONNECT;
		}
		else
		{
			return ORA_EXECUTE_ERROR;
		}
	}

	void COracleConnectionWrapper::BindCurisor(CDBIteratorSPtrPair &iterPair, Statement &st)
	{
		boost::shared_ptr<Statement> ts = boost::make_shared<Statement>(m_hOraConn);
		CDBIteratorSPtr tmp(new COracleDBIterator(ts));
		iterPair.second = tmp;
		st.Bind(":" + iterPair.first, *ts, BindInfo::Out);
	}

	void COracleConnectionWrapper::InitOutputCursor(CDBIteratorSPtrPair &iterPair)
	{
		CDBIteratorSPtr cPtr = iterPair.second;
		if (cPtr.get())
		{
			reinterpret_cast<COracleDBIterator *>(cPtr.get())->InitResultSet();
		}
	}

	size_t COracleConnectionWrapper::GetMaxSize(const StrVariantBindInTypeMap &inVariantBindTypeMap)
	{
		size_t maxSize = 0;
		StrVariantBindInTypeMap::const_iterator iter = inVariantBindTypeMap.begin();
		while (iter != inVariantBindTypeMap.end())
		{
			const VariantBindInType &var_t = iter->second;
			VarSizeVisitor vp;
			size_t nSize = boost::apply_visitor(vp, var_t);
			if (nSize >= maxSize)
			{
				maxSize = nSize;
			}
			++iter;
		}
		return maxSize;
	}

	template<typename T, typename Visitor>
	void COracleConnectionWrapper::Bind(std::map<std::string, T> &map, Statement &st)
	{
		typename std::map<std::string, T>::iterator bIter = map.begin();
		typename std::map<std::string, T>::iterator eIter = map.end();
		Visitor visitor;
		visitor.SetStatement(&st);
		while (bIter != eIter)
		{
			T &ref = bIter->second;
			visitor.Setkey(&(bIter->first));
			boost::apply_visitor(visitor, ref);
			++bIter;
		}
	}

	void COracleConnectionWrapper::Break()
	{
		m_hOraConn.Break();
	}

	void COracleConnectionWrapper::Finit()
	{
		m_hOraConn.Close();
	}

	Utils::CIOCCreaterWithParam<DBType, CConnectionWrapper, const CreateConnParam &> CConnectionWrapper::sm_PoolCreater;
	COracleConnectionWrapper::AutoReg COracleConnectionWrapper::sm_AutoReg(CConnectionWrapper::sm_PoolCreater);
}
