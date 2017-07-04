#pragma once
#include <boost/atomic.hpp>
#include <boost/thread.hpp>
#include <UtilsCommon.h>

namespace DBUtils {
	
	

	//超时执行虚接口，所有定时任务从这个类继承
	class CTimerTask
	{
		public:
			virtual void OnRunTask() = 0;
			/*
			virtual void Cancel();
		protected:
			boost::atomic_bool m_bCancel;*/
	};

	typedef boost::weak_ptr<CTimerTask> CTimerTaskWPtr;
	typedef boost::shared_ptr<CTimerTask> CTimerTaskSPtr;

	typedef boost::shared_ptr<boost::thread>  thread_ptr;
	typedef std::vector<CTimerTaskWPtr > CTimerTaskWPtrVec;

	class CTimeSlot
	{
	public:
		CTimeSlot();
		void Swap(CTimerTaskWPtrVec &tmpVec);
		void Push_back(CTimerTaskWPtr pData);
	private:
		CTimerTaskWPtrVec m_slotInfo;
		RWMutex m_mutex;
	};

	typedef boost::shared_ptr<CTimeSlot> TimeSlotPtr;
	typedef std::vector<TimeSlotPtr> TimeSlotVec;

	//时间轮转管理
	class CTimerTaskMgr
	{
	public:
		static CTimerTaskMgr &Instance();
		void Init(int nMaxUnit);
		~CTimerTaskMgr() throw();
		void Finit();				
		bool PushData(CTimerTaskWPtr pData, int nDelaySecs);
	protected:
		CTimerTaskMgr();
		void Loop();
		void PushWheel();
	protected:
		bool m_bStop;
		thread_ptr m_Pthread;
		TimeSlotVec m_queueVecWheel;
		boost::atomic_ulong m_UIndex;
		int m_nMaxUnit;
	};
}
