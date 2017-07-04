#include "DBProfile.h"
#include "ExpectionFacade.h"
#include <boost/signals2.hpp>

namespace DBUtils {


typedef std::vector<CDBPoolItfPtr> CDBPoolItfPtrVec;

class CPoolProfile
{
public:
	CPoolProfile() 
		: m_index(0)
		, m_totalSize(0)
	{
	}

	CPoolProfile(CPoolProfile &ref)
	{

	}

	bool AddPool(const DBInstanceNode  &dbIn)
	{
		DBType type = Convert(dbIn.dbType);
		CDBPoolItfPtr ptr = CDBPoolItf::Create(type);
		ptr->Init(dbIn);
		m_poolVec.push_back(ptr);
		m_totalSize++;
		return true;
	}

    long ProfileSize()
	{
		return m_totalSize;
	}

	void FinitPool()
	{
		std::for_each(m_poolVec.begin(), m_poolVec.end(), boost::bind(&CDBPoolItf::Finit,_1));
	}

	CConnectionWrapper* GetConnection(long lTimeOut, long &lSlotIndex)
	{
		if (m_totalSize)
		{
			lSlotIndex = (m_index++) % m_totalSize;
			return m_poolVec[lSlotIndex]->GetConnection(lTimeOut);
		}
		else
		{
			throw CExpectionFacade("PoolProfile is empty!", DB_NULL_STATEMENT);
			return NULL;
		}
	}

	void PushConnection(CConnectionWrapper *pConnection, long lSlotIndex)
	{
		if (m_totalSize)
		{
			m_poolVec[lSlotIndex]->PushConnection(pConnection);
		}
	}
private:
	CDBPoolItfPtrVec m_poolVec;
	boost::atomic_long m_totalSize;
	boost::atomic_long m_index;
};

CLogcalDBPool::CLogcalDBPool()
{
	m_rPoolPtr = boost::make_shared<CPoolProfile>();
	m_wPoolPtr = boost::make_shared<CPoolProfile>();
}

bool CLogcalDBPool::Add(const DBInstanceNode &dbIn)
{
	DBRWType type = ConvertRWType(dbIn.rwType);
	switch (type)
	{
	case RO:
		return m_rPoolPtr->AddPool(dbIn);
	case WO:
		return m_wPoolPtr->AddPool(dbIn);
	default:
		return false;
	}
}

void CLogcalDBPool::Finit()
{
	IF_DO(m_rPoolPtr.get(), m_rPoolPtr->FinitPool())
	IF_DO(m_wPoolPtr.get(), m_wPoolPtr->FinitPool())
	return;
}

CConnectionWrapper* CLogcalDBPool::GetConnection(EXEMode eMode, long lTimeOut, long &lSlotIndex)
{
	CPoolProfile & refProfile = GetPoolProfile(eMode);
	return refProfile.GetConnection(lTimeOut, lSlotIndex);
}

void CLogcalDBPool::PushConnection(EXEMode eMode, CConnectionWrapper *pConnection, long lSlotIndex)
{
	CPoolProfile &refProfile = GetPoolProfile(eMode);
	return refProfile.PushConnection(pConnection, lSlotIndex);
}

CPoolProfile& CLogcalDBPool::GetPoolProfile(EXEMode eMode)
{
	if (R_Mode  == eMode)
	{
		CHECK_COND_ACT(NULL != m_rPoolPtr.get(), return *m_rPoolPtr, throw CExpectionFacade("Read Pool is empty!", DB_NULL_STATEMENT))
	}
	else if (W_Mode  == eMode)
	{
		CHECK_COND_ACT(NULL != m_wPoolPtr.get(), return *m_wPoolPtr, throw CExpectionFacade("Write Pool is empty!", DB_NULL_STATEMENT))
	}
	else
	{
		throw CExpectionFacade("Unknown EXEMode", DB_NULL_STATEMENT);
	}
}


CLogcalDBPool::~CLogcalDBPool() throw()
{

}

boost::shared_ptr<CLogcalDBPool> CLogcalDBPool::Create()
{
	return boost::make_shared<CLogcalDBPool>();
}

}
