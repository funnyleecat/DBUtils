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
		��ȡ�����������е�����
		*/
		virtual unsigned int GetColumnCount()
		{
			return m_pRs->GetColumnCount();
		}

		/*
		��ȡ��ͷ�ĵ�����
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

		//�Խ�������г�ʼ��
		virtual void InitResultSet()
		{
			m_pRs = boost::make_shared<Resultset>(m_pSt->GetResultset());

			if (!m_pRs->IsNull())
			{
				ParseResultColumn(*m_pRs, m_columVec);
			}		
		}


		/*
		��ȡ��ǰ����ĳһ�е�����
		*/
		virtual const char * At(unsigned int index)
		{
			m_strRst.clear();
			m_strRst = (*m_pRs).Get<std::string>(index);
			return m_strRst.c_str();
		}

		/*
		��ȡ���������һ��λ��
		*/
		virtual bool Next(void)
		{
			//return rs_->Next();
			return ((*m_pRs)++);
		}

		/*
		��ȡ�������ǰһ��λ��
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
