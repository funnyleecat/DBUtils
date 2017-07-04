/******************************************************************************
模块名  ： DBUtils
文件名  ： exportDef.hpp
相关文件：
文件实现功能：接口变量定义
作者    ：lifeng
版本    ：1.0.0.0
******************************************************************************/
#pragma once
#include <vector>
#include <list>
#include <map>
namespace DBUtils {

	enum DBType
	{
		DB_ORA,		// ORACLE数据库
		DB_MYSQL,   // MYSQL类型数据库
		DB_REDIS,   // REDIS类型数据库
		DB_Unknown  // 未知类型数据库
	};

	enum DataType
	{
		DT_INT,
		DT_DOUBLE,
		DT_String,
        DT_Cursor,
		DT_Uknown
	};

	typedef struct ColumnInfo
	{
		DataType type;
		const char *colName;
		bool isAllowedNull;
		bool isNull;
	}ColumnInfo;


	enum BindDirecation
	{
		D_IN,
		D_OUT,
		D_INOUT,
		D_Unknown
	};

	enum EXEMode
	{
		R_Mode,
		W_Mode,
		UnKnown_Mode
	};

}
