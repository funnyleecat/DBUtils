/******************************************************************************
模块名  ： DBUtils
文件名  ： DBIterator.h
相关文件：
文件实现功能：迭代器资源
作者    ： lifeng
版本    ：1.0.0.0

******************************************************************************/
#pragma once
#include <string>
#include "Export.h"
#include "ExportDef.hpp"
namespace DBUtils
{
	class DBUTILT_API CDBIterator
	{
	public:

		virtual ~CDBIterator()
		{

		}
		/*
			获取该行数据中列的数量
			返回：列的数量
		*/
		virtual unsigned int GetColumnCount() = 0;
		
		/*
			获取表头的的名称
			入参：pos 迭代器位置参数， pos 从 1开始
			返回：参数列名
		*/
		virtual ColumnInfo GetColumnInfo(unsigned int unPos) = 0;
	
		/*
			获取当前行中某一列的数据 
			入参：index 迭代器位置，index从 从 1开始
			返回：参数列名
		*/
		virtual const char * At(unsigned int unIndex) = 0;	

		/*
			获取结果集的下一个位置
			返回：迭代是否有效
		*/
		virtual bool Next(void) = 0;
		/*
			获取结果集的前一个位置
			返回：迭代是否有效
		*/
		virtual bool Prev(void) = 0;
	};
}
