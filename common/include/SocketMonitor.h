#pragma once
#include <boost/thread.hpp>
#include "WnEvent.h"
#include "WQueue.h"
#include "WnScopedLock.h"
class ISocket
{
public:
	virtual void OnRead(int nModuleId, SOCKET nSocket) = 0;
};

class CSocketMonitor
{
public:
	CSocketMonitor(void);
	~CSocketMonitor(void);
	//启动
	void Start(int nId, ISocket* pSocket);
	//停止
	void Stop();
	//是否启动
	bool IsRun(){return m_bFlag;}
	//添加接收socket
	bool AddSocket(SOCKET nSocket);
	//删除接收socket
	bool DelSocket(SOCKET nSocket);

private:
  void OnMonitor();
  CWnLock m_csLock;
  bool m_bFlag;
  int m_nChange;
  int m_nId;
  boost::thread m_thMonitor;
  ISocket* m_pSocket;

  struct MSocket
  {
    SOCKET socket;
    bool bAdd;
  };
  WQueue<MSocket> m_arrTmpSocket;
  int m_nCurSize;

  CWnEvent m_event; //事件通知变化
};