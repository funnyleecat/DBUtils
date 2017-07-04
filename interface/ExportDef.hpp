/******************************************************************************
ģ����  �� DBUtils
�ļ���  �� exportDef.hpp
����ļ���
�ļ�ʵ�ֹ��ܣ��ӿڱ�������
����    ��lifeng
�汾    ��1.0.0.0
******************************************************************************/
#pragma once
#include <vector>
#include <list>
#include <map>
namespace DBUtils {

	enum DBType
	{
		DB_ORA,		// ORACLE���ݿ�
		DB_MYSQL,   // MYSQL�������ݿ�
		DB_REDIS,   // REDIS�������ݿ�
		DB_Unknown  // δ֪�������ݿ�
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
