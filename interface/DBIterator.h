/******************************************************************************
ģ����  �� DBUtils
�ļ���  �� DBIterator.h
����ļ���
�ļ�ʵ�ֹ��ܣ���������Դ
����    �� lifeng
�汾    ��1.0.0.0

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
			��ȡ�����������е�����
			���أ��е�����
		*/
		virtual unsigned int GetColumnCount() = 0;
		
		/*
			��ȡ��ͷ�ĵ�����
			��Σ�pos ������λ�ò����� pos �� 1��ʼ
			���أ���������
		*/
		virtual ColumnInfo GetColumnInfo(unsigned int unPos) = 0;
	
		/*
			��ȡ��ǰ����ĳһ�е����� 
			��Σ�index ������λ�ã�index�� �� 1��ʼ
			���أ���������
		*/
		virtual const char * At(unsigned int unIndex) = 0;	

		/*
			��ȡ���������һ��λ��
			���أ������Ƿ���Ч
		*/
		virtual bool Next(void) = 0;
		/*
			��ȡ�������ǰһ��λ��
			���أ������Ƿ���Ч
		*/
		virtual bool Prev(void) = 0;
	};
}
