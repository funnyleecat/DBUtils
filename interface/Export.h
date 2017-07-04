#pragma once


#ifdef WIN32
#   ifdef DBUTILS_EXPORTS
#       define DBUTILT_API __declspec(dllexport)
#   else
#       define DBUTILT_API __declspec(dllimport)
#   endif //UTILS_EXPORTS
#else
#   define DBUTILT_API
#endif //WIN32
