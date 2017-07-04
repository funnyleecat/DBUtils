/******************************************************************************
模块名  ： DBUtils
文件名  ： Statement.h
相关文件：
文件实现功能：数据库操作资源
作者    ：lifeng
版本    ：1.0.0.0
******************************************************************************/
#pragma once
#include <string>
#include "export.h"
#include "exportDef.hpp"
namespace DBUtils {

	class CConnectionWrapper;
	class CDBIterator;

	class DBUTILT_API CDBStatement
	{
		public:
			/*绑定接口*/
			virtual ~CDBStatement()throw();
			/*
				入参： （1）绑定cmd命令 注意传入的语句不带分号。
			*/
			virtual void BindCmd(const char *pSzCmd) = 0;

			/*
				入参：
				（1）name 入参名称
				（2）value 入参的值
				（3）bd 否绑参数类型，出参或者出入参类型
				（4）bindNull是否以NULL形式绑定,该参数只有在bd 设置为 D_IN 生效
			*/
			virtual void BindInt(const char *pSzName, int nValue, BindDirecation bd, bool bBindNull = false) = 0;

			/*
				入参：
				（1）name 入参名称
				（2）value 入参的值
				（3）bd 否绑参数类型，出参或者出入参类型
				（4）bindNull是否以NULL形式绑定,该参数只有在bd 设置为 D_IN 生效
			*/
			virtual void BindDouble(const char *pSzName, double dValue, BindDirecation bd, bool bBindNull = false) = 0;
			
			/*
				入参：
				（1）name 入参名称
				（2）value 入参的值
				（3）bd 否绑参数类型，出参或者出入参类型
				（4）bindNull是否以NULL形式绑定,该参数只有在bd 设置为 D_IN 生效
			*/
			virtual void BindString(const char *pSzName, const char *pSzValue, BindDirecation bd, bool bindNull = false) = 0;
			

			/*
				绑定游标出参类型
				入参： （1）name 出参名称
			*/
			virtual void BindOutCursor(const char *pSzName) = 0;
			
			/*
				获取出参字符串的值
				入参：（1）参数名称
					  （2）数据类型
				出参： 返数据字符串
			*/
			virtual const char *GetBindOutResult(const char *pSzName, DataType eType) = 0;

			/*
				获取指定的返回游标迭代器
				入参：（1）name 迭代器的名称，如果传入为空串，则返回默认迭代器，在查询场景下，可以使用默认迭代器
				返回：迭代器指针
			*/
			virtual CDBIterator* GetCursorRst(const char *pSzName) = 0;

			/*
				清空所有绑定的参数			
			*/
			virtual void Reset() = 0;
	};


}
