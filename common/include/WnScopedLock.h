// WnScopedLock.h
#pragma once

// Author: Huangzhidan | great3779@sina.com 
// copyright: @Wind Information Co., Ltd (Wind Info) ShangHai
// Create time: 2011-09-10
// ��װ��boost��mutex��scoped_lock���ܿ�ƽ̨ʹ�á������CWnLock�����������ڷ����쳣ʱ���Զ������������߳�������

#include "WnLock.h"

class CWnScopedLock : public CWnLock
{
public:
  CWnScopedLock(CWnLock& mutex);
  virtual ~CWnScopedLock(void);
public:
  virtual void lock() {return m_pMutex->lock();}
  virtual void unlock() {return m_pMutex->unlock();}
private:
  CWnLock* m_pMutex;
};
