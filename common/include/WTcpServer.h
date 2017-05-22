////////////////////////////////////////////////////////////
//Copyright (C), 2011-2011, ����������Ѷ�Ƽ��ɷ����޹�˾
//�ļ��� : WTcpServer.h
//��  �� : ����TCP����
//��  ע : 
//��  ʷ : V1.0.������ 2011-3-7 ����
//		   V1.1 ������ 2014-09-10 �޸Ľӿ�
/////////////////////////////////////////////////////////////////
#pragma once
#include <boost/thread.hpp>
#include "WTcpSession.h"
#include "WTcpSvrSocket.h"
#include "WnEvent.h"
#include "WnLock.h"
#include "WQueue.h"
#include <vector>
class CWTcpServer;

union TcpSessionObj
{
  struct
  {
    UINT16 nSId;
    UINT16 nId;
  };
  UINT32 uSessionId;
};

class CWSvrTcpSession : public virtual CWTcpSession
{
public:
  CWSvrTcpSession(){m_Obj.uSessionId = 0;}
  inline UINT32 GetId(){return m_Obj.uSessionId;}
protected:
  TcpSessionObj m_Obj;
  friend class CWTcpServer;
};
class CWTcpServer : public CWTcpSocketDeal
{
public:
  CWTcpServer();
  ~CWTcpServer();

  //��  �� : ����
  //��  �� : 
  bool  Start(u_short nPort,
    int nMaxConnectNum = 80,
    u_long uIp = 0,
    int nBackLogin = 10);
  //��  �� : ֹͣ
  //��  �� : 
  void Stop();
  //��  �� :�ͷŻỰ
  //��  �� :
  //��  �� : 
  void ReleaseSession(CWSvrTcpSession* pS);

  CWSvrTcpSession* GetSession(int nSessionId);
protected:
  //�����Ự
  virtual CWSvrTcpSession* CreateSession() = 0;
  virtual void DestroySession(CWSvrTcpSession* pS)
  {
    delete pS;
  }
  //CWSvrTcpSession* GetSession(UINT32 uSessionId);

  //ɾ���Ự
  std::vector<CWSvrTcpSession*> m_arrSession;

  //��  �� :��ȡ�Ự
  //��  �� :
  //��  �� : 
  CWSvrTcpSession* GetSession();

  CWnLock m_sessionLock;
  WQueue<UINT16> m_sessionflag;

private:
  //��  �� : �׽��ֽ���
  //��  �� : s: IN  �׽���
  //         uIp: IN IP��ַ
  //         uPort: IN �˿�
  //��  �� : 
  void OnAccept(SOCKET s, u_long uIp, u_short uPort);

  //���ӽ��ն���
  CWTcpSvrSocket m_TcpServerPort;
};
