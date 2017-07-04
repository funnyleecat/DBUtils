/******************************************************************************
模块名  ： DBUtils
文件名  ： CDBMgr.hpp
相关文件：
文件实现功能：数据库管理接口
作者    ：lifeng
版本    ：1.0.0.0
******************************************************************************/

#pragma once
#include <string>
#include <ExportDef.hpp>
#include "Export.h"
#include "DBIterator.h"
#include "Statement.h"
using namespace std;

namespace DBUtils {

	class CDBStatement;
	class CDBIterator;

	class DBUTILT_API CDBMgr
	{
	public:
		/*
			入数 dbInstance 配置数据库连接名称，DBSvrCfg.xml 中的UniqueId字段
				 mode 执行模式参数，用于校验读写分离
				 timeOut 返回超时时间
			返回 基于DBStatement的会话
		*/
		virtual CDBStatement* AllocStatement(const std::string  &strDbInstance, EXEMode eMode, long lTimeOut = 3) = 0;
		
		/*
			入参：释放指针 p
		*/
		virtual void Release(CDBStatement *&pStatement) = 0;
		
		/*
			入数： DBStatement *p 配置指针
				   timeOut 调用数据库超时时间(小于300秒)
			返回： 执行结果
				   可能抛出异常

			注意事项：
				   执行成功后需要Commit
				   失败或者异常抛出场景用户无需调用RollBack
		*/
		virtual bool Execute(CDBStatement *pStatement, long lTimeOut = 3) = 0;
		
		
		/*
			提交一个操作
			入参： 基于DBStatement的会话
			       提交一个会话
		*/
		virtual void Commit(CDBStatement *pStatement) = 0;


		/*
			回滚一个操作
			入参： 基于DBStatement的会话
		*/
		virtual void RollBack(CDBStatement *pStatement) = 0;

		/*
			入参：cfgPath 传入初始化的配置文件
			返回：执行初始化结果
		*/
		virtual bool Init(const std::string &strCfgPath) = 0;

		/*
			执行所有数据库中间件析构
		*/
		virtual void Finit() = 0;

		static CDBMgr& Instance();

	};

}
