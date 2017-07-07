// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
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
// TODO:  在此处引用程序需要的其他头文件
