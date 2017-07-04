#pragma once
#include "DBExpection.h"
class DBExpection;
namespace DBUtils
{
	class CExpectionFacade : public CDBExpection, public std::exception
	{
	public:
		CExpectionFacade(const std::string &strName, long lCode);
		~CExpectionFacade() throw(){}
		virtual const char* what() const throw();
		long GetErrorCode();
		const char * GetErrorName();
	protected:
		std::string m_strError;
		long m_lCode;

	};
}
