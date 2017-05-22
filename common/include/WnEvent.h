#pragma once
// Author: Huangzhidan | great3779@sina.com 
// copyright: @Wind Information Co., Ltd (Wind Info) ShangHai
// Create time: 2011-09-10
// Modify: 
//   ��whyuu@163.com�� 2014/12/24  ��дwait��timed_wait�ӿڣ�ȥ��notfiy_one��notify_all�ӿڣ�����SetEvent��ResetEvent�ӿ�
//   ��װ��boost��condition_variable��ʹ��ʹ�÷����ܽӽ�Windows��Event�������������ܿ�ƽ̨ʹ�á�
#include "WnLock.h"
#include "WnScopedLock.h"
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

class CWnEvent
{
public:
	CWnEvent(bool bManual = false): m_bFlag(false),m_bAuto(!bManual){}
	void SetEvent();
	void ResetEvent(){m_bFlag = false;}
	void wait();
	bool timed_wait(int nMillSec);
	// 	{
	// 		CWnScopedLock scoped_lock(m_lock);
	// 		return m_condition.timed_wait(scoped_lock, boost::posix_time::millisec(nMillSec));
	// 	}
	//void notify_one() { m_condition.notify_one(); }
	//void notify_all() { m_condition.notify_all(); }

private:
	CWnLock m_lock;
	bool m_bFlag;
	bool m_bAuto;
	boost::condition_variable_any m_condition;
};
inline void CWnEvent::SetEvent()
{
	{
		CWnScopedLock scoped_lock(m_lock);
		if (m_bFlag)
		{
			return;
		}
		m_bFlag = true;
	}
	if (m_bAuto)
	{
		m_condition.notify_one();
	}
	else
	{
		m_condition.notify_all();
	}
}
inline void CWnEvent::wait()
{
	CWnScopedLock scoped_lock(m_lock);
	if (!m_bFlag)
	{
		m_condition.wait(scoped_lock);
	}
	if (m_bAuto)
	{
		m_bFlag = false;
	}
}
inline bool CWnEvent::timed_wait(int nMillSec)
{
	CWnScopedLock scoped_lock(m_lock);
	if (m_bFlag)
	{
		if (m_bAuto)
		{
			m_bFlag = false;
		}
		return true;
	}
	else 
	{
		bool bRes = m_condition.timed_wait(scoped_lock, boost::posix_time::millisec(nMillSec));
		if (bRes && m_bAuto)
		{
			m_bFlag = false;
		}
		return bRes;
	}
}