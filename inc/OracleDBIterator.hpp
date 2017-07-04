#include "UtilsCommon.h"
#include "DBIterator.h"
#include "ocilib.hpp"
#include "exportDef.hpp"
#include <iostream>
namespace DBUtils
{
	using namespace ocilib;
	typedef struct ColumnData
	{
		DataType type;
		std::string colName;
		bool isAllowedNull;
		bool isNull;
	}ColumnData;
	typedef std::vector<ColumnData> ColumnDataVec;


	class COracleDBIterator : public CDBIterator
	{
		
	public:
		COracleDBIterator(boost::shared_ptr<Statement> &pSt)
			:m_pSt(pSt)
		{
		}

		COracleDBIterator(const COracleDBIterator &ref)
		{

		}

		virtual ~COracleDBIterator()
		{
		}

		/*
		获取该行数据中列的数量
		*/
		virtual unsigned int GetColumnCount()
		{
			return m_pRs->GetColumnCount();
		}

		/*
		获取表头的的名称
		*/
		virtual ColumnInfo GetColumnInfo(unsigned int nPos)
		{
			ColumnInfo info;
			info.type = m_columVec[nPos - 1].type;
			info.colName = m_columVec[nPos - 1].colName.c_str();
			info.isAllowedNull = m_columVec[nPos - 1].isAllowedNull;
			info.isNull = m_columVec[nPos - 1].isNull;
			return info;
		}

		//对结果集进行初始化
		virtual void InitResultSet()
		{
			m_pRs = boost::make_shared<Resultset>(m_pSt->GetResultset());

			if (!m_pRs->IsNull())
			{
				ParseResultColumn(*m_pRs, m_columVec);
			}		
		}


		/*
		获取当前行中某一列的数据
		*/
		virtual const char * At(unsigned int index)
		{
			m_strRst.clear();
			m_strRst = (*m_pRs).Get<std::string>(index);
			return m_strRst.c_str();
		}

		/*
		获取结果集的下一个位置
		*/
		virtual bool Next(void)
		{
			//return rs_->Next();
			return ((*m_pRs)++);
		}

		/*
		获取结果集的前一个位置
		*/
		virtual bool Prev(void)
		{
			//return rs_->Prev();
			return ((*m_pRs)--);
		}

		void ParseResultColumn(const Resultset &rs, ColumnDataVec &columnInfoVec) const
		{
			for (unsigned int i = 1; i <= rs.GetColumnCount(); i++)
			{
				Column col = rs.GetColumn(i);
				ColumnData info;
				if (col.GetType() == TypeLong)
				{
					info.type = DT_INT;
				}
				else if (col.GetType() == TypeNumeric)
				{
					info.type = DT_DOUBLE;
				}
				std::string colName = col.GetName();

				info.colName = col.GetName();
				info.isAllowedNull = col.IsNullable();
				info.isNull = col.IsNull();
				columnInfoVec.push_back(info);
			}
		}

	private:
		boost::shared_ptr<Statement> m_pSt;
		boost::shared_ptr<Resultset> m_pRs;
		ColumnDataVec m_columVec;
		std::string m_strRst;
	};

}
