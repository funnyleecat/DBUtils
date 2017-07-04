/******************************************************************************
模块名  ： DBUtils
文件名  ： DBExpection.h
相关文件：
文件实现功能：异常相关资源
作者    ：lifeng
版本    ：1.0.0.0
******************************************************************************/
#pragma once
#include <string>
#include <exception>
#include "Export.h"
const long ORA_CONNECTION_ERROR = 1;
const long MYSQL_CONNECTION_ERROR = 2;
const long ORA_EXECUTE_ERROR = 3;
const long MYSQL_EXECUTE_ERROR = 4;
const long DB_RW_ERROR = 5;
const long DB_EMPTY_CMD = 6;
const long DB_ILLEGAL_SCHEMA = 7;
const long DB_NULL_STATEMENT = 8;
const long DB_CFG_INITERROR = 9;
const long DB_ENV_INITFAILED = 10;
const long DB_VALUE_IS_NOT_EXSIT = 11;
const long DB_NULL_PTR = 12;
const long DB_BIND_UNKNOWN = 13;
const long DB_TIMEOUT_UNKNOWN = 14;

namespace DBUtils
{
	class DBUTILT_API CDBExpection
	{
	public:
		/*
			获取错误码
			返回：错误码值
		*/
		virtual long GetErrorCode() = 0;

		/*
			获取错误码
			返回：错误的类型
		*/
		virtual const char * GetErrorName() = 0;
	};
}
