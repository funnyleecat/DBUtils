// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once
#ifdef WIN32
#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib, "../../../../lib_libSelf/cross/DBUtils/x64/Debug/DBUtils.lib")
#else
#pragma comment(lib, "../../../../lib_libSelf/cross/DBUtils/x64/Release/DBUtils.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "../../../../lib_libSelf/cross/DBUtils/Win32/Debug/DBUtils.lib")
#else
#pragma comment(lib, "../../../../lib_libSelf/cross/DBUtils/Win32/Release/DBUtils.lib")
#endif
#endif

#endif
// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
