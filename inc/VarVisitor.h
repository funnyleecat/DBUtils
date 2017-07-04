#pragma once
#include "UtilsCommon.h"
namespace DBUtils
{
	//出参的模板访问器
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


	//入参的合并模板访问器
	class VarInVisistor : public boost::static_visitor<void>
	{
	public:
		//这个必须要加上去！！！不然编译不过，模板特化针对不同类型必然也存在这种情况
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

	//绑定出入参的模板访问器
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

	//绑定入参的模板访问器
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
				//字符串绑定暂时按照4096个字节处理
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

	//获取入参的Vector容器的容量访问器
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
