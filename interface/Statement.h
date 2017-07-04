/******************************************************************************
ģ����  �� DBUtils
�ļ���  �� Statement.h
����ļ���
�ļ�ʵ�ֹ��ܣ����ݿ������Դ
����    ��lifeng
�汾    ��1.0.0.0
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
			/*�󶨽ӿ�*/
			virtual ~CDBStatement()throw();
			/*
				��Σ� ��1����cmd���� ע�⴫�����䲻���ֺš�
			*/
			virtual void BindCmd(const char *pSzCmd) = 0;

			/*
				��Σ�
				��1��name �������
				��2��value ��ε�ֵ
				��3��bd ���������ͣ����λ��߳��������
				��4��bindNull�Ƿ���NULL��ʽ��,�ò���ֻ����bd ����Ϊ D_IN ��Ч
			*/
			virtual void BindInt(const char *pSzName, int nValue, BindDirecation bd, bool bBindNull = false) = 0;

			/*
				��Σ�
				��1��name �������
				��2��value ��ε�ֵ
				��3��bd ���������ͣ����λ��߳��������
				��4��bindNull�Ƿ���NULL��ʽ��,�ò���ֻ����bd ����Ϊ D_IN ��Ч
			*/
			virtual void BindDouble(const char *pSzName, double dValue, BindDirecation bd, bool bBindNull = false) = 0;
			
			/*
				��Σ�
				��1��name �������
				��2��value ��ε�ֵ
				��3��bd ���������ͣ����λ��߳��������
				��4��bindNull�Ƿ���NULL��ʽ��,�ò���ֻ����bd ����Ϊ D_IN ��Ч
			*/
			virtual void BindString(const char *pSzName, const char *pSzValue, BindDirecation bd, bool bindNull = false) = 0;
			

			/*
				���α��������
				��Σ� ��1��name ��������
			*/
			virtual void BindOutCursor(const char *pSzName) = 0;
			
			/*
				��ȡ�����ַ�����ֵ
				��Σ���1����������
					  ��2����������
				���Σ� �������ַ���
			*/
			virtual const char *GetBindOutResult(const char *pSzName, DataType eType) = 0;

			/*
				��ȡָ���ķ����α������
				��Σ���1��name �����������ƣ��������Ϊ�մ����򷵻�Ĭ�ϵ��������ڲ�ѯ�����£�����ʹ��Ĭ�ϵ�����
				���أ�������ָ��
			*/
			virtual CDBIterator* GetCursorRst(const char *pSzName) = 0;

			/*
				������а󶨵Ĳ���			
			*/
			virtual void Reset() = 0;
	};


}
