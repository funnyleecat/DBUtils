/******************************************************************************
ģ����  �� DBUtils
�ļ���  �� DBExpection.h
����ļ���
�ļ�ʵ�ֹ��ܣ��쳣�����Դ
����    ��lifeng
�汾    ��1.0.0.0
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
			��ȡ������
			���أ�������ֵ
		*/
		virtual long GetErrorCode() = 0;

		/*
			��ȡ������
			���أ����������
		*/
		virtual const char * GetErrorName() = 0;
	};
}
