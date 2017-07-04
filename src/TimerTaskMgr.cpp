#include "TimerTaskMgr.h"


namespace DBUtils {

	CTimeSlot::CTimeSlot()
	{

	}
	void CTimeSlot::Swap(CTimerTaskWPtrVec &tmpVec)
	{
		WriteLock lock(m_mutex);
		tmpVec.swap(m_slotInfo);
	}

	void CTimeSlot::Push_back(CTimerTaskWPtr pData)
	{
		WriteLock lock(m_mutex);
		m_slotInfo.push_back(pData);
	}


	CTimerTaskMgr & CTimerTaskMgr::Instance()
	{
		static CTimerTaskMgr s_Instance;
		return s_Instance;
	}

	void CTimerTaskMgr::Init(int nMaxUnit)
	{
		m_nMaxUnit = nMaxUnit;
		m_queueVecWheel.reserve(m_nMaxUnit);
		for (long i = 0; i < m_nMaxUnit; ++i)
		{
			TimeSlotPtr ptr = boost::make_shared<CTimeSlot>();
			m_queueVecWheel.push_back(ptr);
		}
		m_bStop = false;
		m_Pthread = thread_ptr(new boost::thread(boost::bind(&CTimerTaskMgr::Loop, this)));
	}

	CTimerTaskMgr::~CTimerTaskMgr() throw()
	{

	}

	void CTimerTaskMgr::Finit()
	{
		m_bStop = true;
		m_Pthread->interrupt();
		m_Pthread->join();
	}

	bool CTimerTaskMgr::PushData(CTimerTaskWPtr pData, int nDelaySecs)
	{
		if (nDelaySecs >= m_nMaxUnit)
		{
			return false;
		}
		else
		{
			int nPos = (m_UIndex + nDelaySecs) % m_nMaxUnit;
			TimeSlotPtr &ref = m_queueVecWheel[nPos];
			ref->Push_back(pData);
			return true;
		}
	}


	void CTimerTaskMgr::Loop()
	{
		try
		{
			do
			{
				boost::this_thread::sleep(boost::posix_time::seconds(1));
				PushWheel();
			} while (!m_bStop);
		}
		catch (std::exception&)
		{
			//���̱߳�interrupt֮�󣬽�������쳣
			//����ֻ�������쳣�������κδ���ֻ��Ϊ������ֹͣ�߳�
		}
	}

	void CTimerTaskMgr::PushWheel()
	{
		if (m_UIndex == m_nMaxUnit - 1)
		{
			m_UIndex = 0;
		}
		else
		{
			++m_UIndex;
		}

		CTimerTaskWPtrVec tempVec;
		m_queueVecWheel[m_UIndex]->Swap(tempVec);
		CTimerTaskWPtrVec::iterator iter = tempVec.begin();
		while (iter != tempVec.end())
		{	
			CTimerTaskSPtr sPtr = (*iter).lock();
			//����Ҫ�ٴμ������ָ���Ƿ��ͷţ�����
			if (sPtr.get())
			{
				sPtr->OnRunTask();
			}
			++iter;
		}
	}


	CTimerTaskMgr::CTimerTaskMgr()
		: m_nMaxUnit(60)
		, m_bStop(true)
	{

	}
}
