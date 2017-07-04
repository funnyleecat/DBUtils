#pragma once
#include "UtilsCommon.h"
namespace DBUtils
{
	//���ε�ģ�������
	class VarOutVisitor :public boost::static_visitor<std::string>
	{
	public:
		template<typename T>
		std::string operator()(T &tRef)
		{
			std::ostringstream os;
			os << tRef.value;
			return os.str();
		}
	};


	//��εĺϲ�ģ�������
	class VarInVisistor : public boost::static_visitor<void>
	{
	public:
		//�������Ҫ����ȥ��������Ȼ���벻����ģ���ػ���Բ�ͬ���ͱ�ȻҲ�����������
		template<typename T, typename U>
		void operator()(T &src, U &dst) const
		{
			return;
		}
		template<typename T>
		void operator()(T &src, T &dst) const
		{
			IF_DO(src.bindEmpty, dst.bindEmpty = true)
			size_t nSrcSize = src.valueVec.size();
			for (size_t index = 0; index < nSrcSize; ++index)
			{
				dst.valueVec.push_back(src.valueVec[index]);
			}
		}
	};


	class VisitorCom
	{
	public:
		void Setkey(const std::string *pStrKeyName)
		{
			m_pStrKeyName = pStrKeyName;
		}
		void SetStatement(Statement *pStatement)
		{
			m_pStatement = pStatement;
		}
	protected:
		const std::string *m_pStrKeyName;
		Statement *m_pStatement;
	};

	//�󶨳���ε�ģ�������
	class VarBindVistor : public boost::static_visitor<void>, public VisitorCom
	{
	public:
		template<typename T>
		void operator()(T &tRef)
		{
			if (tRef.bindDirect == D_OUT)
			{
				m_pStatement->Bind(":" + *m_pStrKeyName, tRef.value, BindInfo::Out);
			}
			else
			{
				m_pStatement->Bind(":" + *m_pStrKeyName, tRef.value, BindInfo::InOut);
			}
		}
		void operator()(BindInOutType<std::string> &tRef)
		{
			if (tRef.bindDirect == D_OUT)
			{
				m_pStatement->Bind(":" + *m_pStrKeyName, tRef.value, 4096, BindInfo::Out);
			}
			else
			{
				m_pStatement->Bind(":" + *m_pStrKeyName, tRef.value, 4096, BindInfo::InOut);
			}
		}

	};

	//����ε�ģ�������
	class VarBindInVistor : public boost::static_visitor<void>, public VisitorCom
	{
	public:
		template<typename T>
		void operator()(T &tRef)
		{
			if (tRef.valueVec.size() > 1)
			{
				m_pStatement->Bind(":" + *m_pStrKeyName, tRef.valueVec, BindInfo::In);
			}
			else
			{
				m_pStatement->Bind(":" + *m_pStrKeyName, tRef.valueVec[0], BindInfo::In);
			}
			if (tRef.bindEmpty)
			{
				size_t numVec = tRef.valueVec.size();
				for (size_t index = 1; index <= numVec; ++index)
				{
					m_pStatement->GetBind(":" + *m_pStrKeyName).SetDataNull(true, (unsigned int)index);
				}
			}
		}
		void operator()(BindInType<std::string> &tRef)
		{
			if (tRef.valueVec.size() > 1)
			{
				//�ַ�������ʱ����4096���ֽڴ���
				m_pStatement->Bind(":" + *m_pStrKeyName, tRef.valueVec, 4096, BindInfo::In);
			}
			else
			{
				//int strLen = (int)ref.valueVec[0].size();
				m_pStatement->Bind(":" + *m_pStrKeyName, tRef.valueVec[0], (int)tRef.valueVec[0].size(), BindInfo::In);
			}

			if (tRef.bindEmpty)
			{
				size_t numVec = tRef.valueVec.size();
				for (size_t index = 1; index <= numVec; ++index)
				{
					m_pStatement->GetBind(":" + *m_pStrKeyName).SetDataNull(true, (unsigned int)index);
				}
			}
		}
	};

	//��ȡ��ε�Vector����������������
	class VarSizeVisitor :public boost::static_visitor<size_t>
	{
	public:
		template<typename T>
		size_t operator()(const T &tRef)
		{
			return tRef.valueVec.size();
		}

	};
}
