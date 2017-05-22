////////////////////////////////////////////////////////////
//Copyright (C), 2011-2011, 北京数码视讯科技股份有限公司
//文件名 : WTcpServer.h
//描  述 : 定义TCP服务
//备  注 : 
//历  史 : V1.0.王辉永 2011-3-7 创建
//		   V1.1 王辉永 2014-09-10 修改接口
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

  //描  述 : 启动
  //返  回 : 
  bool  Start(u_short nPort,
    int nMaxConnectNum = 80,
    u_long uIp = 0,
    int nBackLogin = 10);
  //描  述 : 停止
  //返  回 : 
  void Stop();
  //描  述 :释放会话
  //参  数 :
  //返  回 : 
  void ReleaseSession(CWSvrTcpSession* pS);

  CWSvrTcpSession* GetSession(int nSessionId);
protected:
  //创建会话
  virtual CWSvrTcpSession* CreateSession() = 0;
  virtual void DestroySession(CWSvrTcpSession* pS)
  {
    delete pS;
  }
  //CWSvrTcpSession* GetSession(UINT32 uSessionId);

  //删除会话
  std::vector<CWSvrTcpSession*> m_arrSession;

  //描  述 :获取会话
  //参  数 :
  //返  回 : 
  CWSvrTcpSession* GetSession();

  CWnLock m_sessionLock;
  WQueue<UINT16> m_sessionflag;

private:
  //描  述 : 套接字接收
  //参  数 : s: IN  套接字
  //         uIp: IN IP地址
  //         uPort: IN 端口
  //返  回 : 
  void OnAccept(SOCKET s, u_long uIp, u_short uPort);

  //连接接收对象
  CWTcpSvrSocket m_TcpServerPort;
};
