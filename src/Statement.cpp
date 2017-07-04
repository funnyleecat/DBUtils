#include "Statement.h"
#include "DBStatementFacade.h"
#include <UtilsCommon.h>
#include <map>
#include <vector>
#include "DBIterator.h"
#include "DBConnectionWrapper.h"
#include "ExpectionFacade.h"
#include "VarVisitor.h"
#include "TimerGuard.h"

namespace DBUtils {

	class CStatementImpl : public CDBStatementFacade
	{
		

		public:
			CStatementImpl(CConnectionWrapper *pConnection, const std::string  &strDbInstance, EXEMode eMode, long lSlotIndex)
				:m_pConnection(pConnection)
				, m_dbInstance(strDbInstance)
				, m_mode(eMode)
				, m_slotIndex(lSlotIndex)
			{

			}

			~CStatementImpl() throw()
			{
			}
			
		
			virtual bool Excute(long lTimeOut)
			{
				IF_DO(lTimeOut > 300, throw CExpectionFacade("TimeOut is Out Range", DB_TIMEOUT_UNKNOWN))
				CTimerGuard guard(*m_pConnection,lTimeOut);
				m_pRstIterator = m_pConnection->Execute(m_cmd, m_inStrVariantType, m_inOutStrVariantType, m_dbIteratorPtrMap);
				return true;
			}

			virtual EXEMode GetEXEMode()
			{
				return m_mode;
			}

			virtual void RollBack()
			{
				m_pConnection->RollBack();
			}

			virtual void Commit()
			{
				m_pConnection->Commit();
			}
		
			virtual void BindCmd(const char *pSzCmd)
			{
				m_cmd = std::string(pSzCmd);
			}


			virtual void BindInt(const char *pSzName, int nValue, BindDirecation bd, bool bBindNull)
			{
				Bind_i<int>(pSzName, nValue, bd, bBindNull);
			}

			virtual void BindDouble(const char *pSzName, double dValue, BindDirecation bd, bool bBindNull)
			{
				Bind_i<double>(pSzName, dValue, bd, bBindNull);
			}

			virtual void BindString(const char *pSzName, const char *pSzValue, BindDirecation bd, bool bBindNull)
			{
				std::string strVal(pSzValue);
				Bind_i<std::string>(pSzName, strVal, bd, bBindNull);
			}

			virtual long GetSlotIndex()
			{
				return m_slotIndex;
			}

			virtual void BindOutCursor(const char *pSzName)
			{
				CDBIteratorSPtr pTmp;
				std::string strKeyName(pSzName);
				CDBIteratorSPtrMap::iterator iter = m_dbIteratorPtrMap.find(strKeyName);
				CHECK_COND_ACT(iter != m_dbIteratorPtrMap.end(), return, m_dbIteratorPtrMap.insert(make_pair(strKeyName, pTmp)))
			}
				
			virtual CConnectionWrapper* GetConnection()
			{
				return m_pConnection;
			}

			virtual CDBIterator* GetCursorRst(const char *pSzName)
			{
				IF_DO(pSzName == NULL, return m_pRstIterator.get())
				std::string strKey(pSzName);
				if (strKey.empty())
				{
					return m_pRstIterator.get();
				}
				CDBIteratorSPtrMap::iterator iter = m_dbIteratorPtrMap.find(strKey);
				IF_DO(iter != m_dbIteratorPtrMap.end(), return (iter->second).get())
				return NULL;
			}

			virtual const char *GetBindOutResult(const char *pSzName, DataType eType)
			{
				m_strRst = GetBindOutVal(pSzName);
				return m_strRst.c_str();
			}
		protected:

			template<typename valT>
			void Bind_i(const char *pSzName, valT &value, BindDirecation bd, bool bBindNull)
			{
				if (bd == D_IN)
				{
					BindInType<valT> intBind(value, bBindNull);
					VariantBindInType valBindIn = intBind;
					BindIn(pSzName, valBindIn);
				}
				else if (bd == D_OUT || bd == D_INOUT)
				{
					BindType<valT>(pSzName, value, bd);
				}
				else
				{
					throw CExpectionFacade("StatementImpl::unknown bind direction", DB_BIND_UNKNOWN);
				}
			}

			template<class T>
			void BindType(const char *pSzName, T &defaultVal, BindDirecation bd)
			{
				std::string strName(pSzName);
				typename std::map<std::string, VariantBindInOutType >::iterator iter;
				iter = m_inOutStrVariantType.find(strName);
				
				BindInOutType<T> typeVal;
				typeVal.value = defaultVal;
				typeVal.bindDirect = bd;
				if (iter != m_inOutStrVariantType.end())
				{
					iter->second = typeVal;
				}
				else
				{
					m_inOutStrVariantType.insert(make_pair(strName, typeVal));
				}
			}

			void BindIn(const char *pSzName, VariantBindInType &variantBind)
			{
				std::string strKeyName(pSzName);
				std::map<std::string, VariantBindInType>::iterator iter = m_inStrVariantType.find(pSzName);
				if (iter != m_inStrVariantType.end())
				{
					boost::apply_visitor(VarInVisistor(), variantBind, iter->second);

				}
				else
				{
					m_inStrVariantType.insert(make_pair(strKeyName, variantBind));
				}
			}

			std::string GetBindOutVal(const char *pSzName)
			{
				std::string strKeyName(pSzName);
				StrVariantBindInOutTypeMap::iterator iter = m_inOutStrVariantType.find(strKeyName);
				if (iter != m_inOutStrVariantType.end())
				{
					VariantBindInOutType &var_t = iter->second;
					VarOutVisitor vp;
					return boost::apply_visitor(vp, var_t);
				}
				else
				{
					throw CExpectionFacade("StatementImpl::getBindOutVal", DB_VALUE_IS_NOT_EXSIT);
				}
			}

			virtual std::string GetInstanceName()
			{
				return m_dbInstance;
			}

			virtual void Reset()
			{
				m_cmd.clear();
				m_inStrVariantType.clear();
				m_inOutStrVariantType.clear();
				m_dbIteratorPtrMap.clear();
			}
		protected:
			std::string m_cmd;
			//泛型类型设计绑定入参容器
			StrVariantBindInTypeMap m_inStrVariantType;	
			//泛型类型设计绑定出入参容器
			StrVariantBindInOutTypeMap m_inOutStrVariantType;
			//绑定出参类型的游标
			CDBIteratorSPtrMap m_dbIteratorPtrMap;
			CConnectionWrapper *m_pConnection;
			CDBIteratorSPtr m_pRstIterator;
			std::string m_dbInstance;
			EXEMode m_mode;
			long m_slotIndex;
			std::string m_strRst;

	};

	CDBStatementFacade::~CDBStatementFacade()throw()
	{

	}

	CDBStatement::~CDBStatement()throw()
	{

	}

	CDBStatement* CDBStatementFacade::Create(CConnectionWrapper *pConnection, const std::string  &strDbInstance, EXEMode eMode, long lSlotIndex)
	{
		return new CStatementImpl(pConnection, strDbInstance, eMode, lSlotIndex);
	}
}
