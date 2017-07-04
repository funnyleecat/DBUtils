#pragma once
#include <string>
#include <vector>
#include <Singleton.hpp>
#include <map>
#include "ExportDef.hpp"
#include "UtilsCommon.h"
#include "tinyxml.h"

namespace DBUtils {


//一个物理数据库节点的信息
struct DBInstanceNode
{
public:
	DBInstanceNode(TiXmlElement &node);
	std::string hostName;
	std::string portNum;
	std::string dbName;
	std::string usrName;
	std::string password;
	std::string dbType;
	std::string rwType;
	std::string connectionNum;
	std::string heartBeatCycle;
};

typedef std::vector<DBInstanceNode> DBInstanceVec;


//一个逻辑数据库节点的信息
struct DBLogicNode
{
public:
	DBLogicNode(TiXmlElement &node);
	std::string uniqueId;
	DBInstanceVec dbInstanceVec;
};

typedef std::vector<DBLogicNode> DBLogicNodeVec;

class CDBSvrIterator
{
	public:
		virtual const DBLogicNode& At(int nIndex) = 0;
		virtual int Count() = 0;	
};

class CDBSvrCfg : public CDBSvrIterator
{
	public:
		virtual bool Init(const std::string  &strInitPath) = 0;
		virtual CDBSvrIterator& GetIterator() = 0;
		virtual ~CDBSvrCfg() throw();
		static CDBSvrCfg& Instance();
};


}
