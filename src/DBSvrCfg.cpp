
#include "DBSvrCfg.h"
#include "UtilsCommon.h"
#include "ExpectionFacade.h"

namespace DBUtils {

	const char* GetXmlAttrStr(TiXmlElement &pEl, const char *pSzAttr)
	{
		const char* p = pEl.Attribute(pSzAttr);
		if (p)
		{
			return p;
		}
		else
		{
			std::ostringstream os;
			os << "GetXmlAttrStr Empty Attr:" << pSzAttr;
			throw CExpectionFacade(os.str(), DB_CFG_INITERROR);
		}
	}

	template<typename T>
	void SearchInitNode(TiXmlElement &node,const std::string &keyName,std::vector<T> &refVec)
	{
		for (TiXmlElement* pXmlChild = node.FirstChildElement(); pXmlChild; pXmlChild = pXmlChild->NextSiblingElement())
		{
			std::string strValue = pXmlChild->Value();
			if (strValue == keyName)
			{
				T obj(*pXmlChild);
				refVec.push_back(obj);
			}
			else
			{
				std::ostringstream os;
				os << "SearchInitNode invalidate Attr:" << strValue;
				throw CExpectionFacade(os.str(), DB_CFG_INITERROR);
			}
		}
	}

	DBInstanceNode::DBInstanceNode(TiXmlElement &node)
	{
		hostName = GetXmlAttrStr(node, "hostName");
		portNum = GetXmlAttrStr(node, "portNum");
		dbName = GetXmlAttrStr(node, "dbName");
		usrName = GetXmlAttrStr(node, "usrName");
		password = GetXmlAttrStr(node, "password");
		rwType = GetXmlAttrStr(node, "rwType");
		dbType = GetXmlAttrStr(node, "dbType");
		connectionNum = GetXmlAttrStr(node, "connectionNum");
		heartBeatCycle = GetXmlAttrStr(node, "heartBeatCycle");
	}


	DBLogicNode::DBLogicNode(TiXmlElement &node)
	{
		uniqueId = GetXmlAttrStr(node, "UniqueId");
		SearchInitNode(node, "DBInstanceNode", dbInstanceVec);
	}

	class CDBSvrCfgImpl : public CDBSvrCfg
	{
	public:
		CDBSvrCfgImpl()
		{

		}

		~CDBSvrCfgImpl() throw()
		{

		}
		virtual bool Init(const std::string &strInitPath)
		{
			TiXmlDocument xmlDoc(strInitPath.c_str());
			if (!xmlDoc.LoadFile(TIXML_ENCODING_UTF8))return false;
			for (TiXmlElement *pXmlRoot = xmlDoc.RootElement(); pXmlRoot; pXmlRoot = pXmlRoot->NextSiblingElement())
			{
				std::string strValue = pXmlRoot->Value();				
				if ("DBGV" == strValue)
				{
					SearchInitNode(*pXmlRoot, "DBLogicNode", m_dbsvrVec);
				}
			}
			return true;
		}

		virtual CDBSvrIterator& GetIterator()
		{
			return *this;
		}

		virtual const DBLogicNode& At(int nIndex)
		{
			return m_dbsvrVec[nIndex];
		}

		virtual int Count()
		{
			return (int)(m_dbsvrVec.size());
		}
	private:
		std::string m_strInitPath;
		DBLogicNodeVec m_dbsvrVec;
	};


	CDBSvrCfg::~CDBSvrCfg() throw()
	{

	}


	CDBSvrCfg& CDBSvrCfg::Instance()
	{
		static CDBSvrCfgImpl s_DbSvrCfg;
		return s_DbSvrCfg;
	}
}
