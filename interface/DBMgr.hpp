/******************************************************************************
ģ����  �� DBUtils
�ļ���  �� CDBMgr.hpp
����ļ���
�ļ�ʵ�ֹ��ܣ����ݿ����ӿ�
����    ��lifeng
�汾    ��1.0.0.0
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
			���� dbInstance �������ݿ��������ƣ�DBSvrCfg.xml �е�UniqueId�ֶ�
				 mode ִ��ģʽ����������У���д����
				 timeOut ���س�ʱʱ��
			���� ����DBStatement�ĻỰ
		*/
		virtual CDBStatement* AllocStatement(const std::string  &strDbInstance, EXEMode eMode, long lTimeOut = 3) = 0;
		
		/*
			��Σ��ͷ�ָ�� p
		*/
		virtual void Release(CDBStatement *&pStatement) = 0;
		
		/*
			������ DBStatement *p ����ָ��
				   timeOut �������ݿⳬʱʱ��(С��300��)
			���أ� ִ�н��
				   �����׳��쳣

			ע�����
				   ִ�гɹ�����ҪCommit
				   ʧ�ܻ����쳣�׳������û��������RollBack
		*/
		virtual bool Execute(CDBStatement *pStatement, long lTimeOut = 3) = 0;
		
		
		/*
			�ύһ������
			��Σ� ����DBStatement�ĻỰ
			       �ύһ���Ự
		*/
		virtual void Commit(CDBStatement *pStatement) = 0;


		/*
			�ع�һ������
			��Σ� ����DBStatement�ĻỰ
		*/
		virtual void RollBack(CDBStatement *pStatement) = 0;

		/*
			��Σ�cfgPath �����ʼ���������ļ�
			���أ�ִ�г�ʼ�����
		*/
		virtual bool Init(const std::string &strCfgPath) = 0;

		/*
			ִ���������ݿ��м������
		*/
		virtual void Finit() = 0;

		static CDBMgr& Instance();

	};

}
