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
	//����
	void Start(int nId, ISocket* pSocket);
	//ֹͣ
	void Stop();
	//�Ƿ�����
	bool IsRun(){return m_bFlag;}
	//��ӽ���socket
	bool AddSocket(SOCKET nSocket);
	//ɾ������socket
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

  CWnEvent m_event; //�¼�֪ͨ�仯
};