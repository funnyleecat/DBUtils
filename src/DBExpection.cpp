#include "ExpectionFacade.h"
#include <string>
namespace DBUtils
{
	CExpectionFacade::CExpectionFacade(const std::string &strName, long lCode)
		:m_strError(strName)
		,m_lCode(lCode)
	{

	}
	long CExpectionFacade::GetErrorCode()
	{
		return m_lCode;
	}

	const char * CExpectionFacade::GetErrorName()
	{
		return m_strError.c_str();
	}

	const char* CExpectionFacade::what() const throw()
	{
		return m_strError.c_str();
	}
}
