////////////////////////////////////////////////////////////
//Copyright (C), 2010-2010, 
//文件名 : WTcpClient.cpp
//头文件 : WTcpClient.h
//备  注 : 注意事项
//         留给未来维护人员的建议
//		   
//历  史 : V1.0.0.0  2011-3-9 创建
//
/////////////////////////////////////////////////////////////////
#include "WTcpSession.h"
#include "commType.h"
#include "WLog.h"
#include "Public.h"
#include <boost/threadpool.hpp>
#define RECV_BUF 64 * 1024
//static boost::threadpool::pool g_tcpSessionPool(10);
CWTcpSession::CWTcpSession(int nReadBuf):
m_uRemoteIp(0),
m_uRemotePort(0),
m_bFlag(false),
	m_bAutoReconnect(false),
	m_bConnect(false),
	m_nReadBufSize(nReadBuf)
{
	m_pBuffer = new char[m_nReadBufSize];
}

CWTcpSession::~CWTcpSession()
{
	delete [] m_pBuffer;
}

//描  述 : 附加套接字
//参  数 : pObj IN 处理对象
//		   s: IN  套接字
//         uRemoteIp: IN 目的IP
//         uPort: IN 目的端口
//返  回 : 
bool CWTcpSession::Attach(SOCKET s,
	u_long uRemoteIp,
	u_short uRemotePort,
	bool bStartRecvThread)
{
	m_bAutoReconnect = false;
	m_bConnect  = true;
	m_uRemoteIp = uRemoteIp;
	m_uRemotePort = uRemotePort;
	m_Socet.Attach(s);
	bool  bRes = m_Socet.SetLinger(false);
	if (!bRes)
	{
		return false;
	}
// 	if (!SetSocketKeepLive())
// 	{
// 		return false;
// 	}
	if (bStartRecvThread)
	{
		assert(m_bFlag == false);
		m_bFlag = true;
		boost::function0<void> f = boost::bind(&CWTcpSession::OnRead, this);
		//g_tcpSessionPool.schedule(f);
 		m_thThread = boost::thread(f);
 		m_thThread.detach();
	}
	return true;
}

//描  述 : 
//参  数 : hostIp: IN  主机IP
//         uPort: IN 端口
//返  回 : 
bool CWTcpSession::Connect(u_long uRemoteIp, 
	u_short uRemotePort,
	bool bRunConnect,
	bool bAutoReconnect, 
	bool bStartRecvThread)
{
	m_uRemotePort = uRemotePort;
	m_uRemoteIp = uRemoteIp;
	if (bRunConnect)
	{
		m_bConnect = Connect();
		if (!m_bConnect)
		{
			return false;
		}
	}
	else
	{
		m_bConnect = false;
	}
	if (!m_bConnect && !bAutoReconnect)
	{
		return false;
	}
	m_bAutoReconnect = bAutoReconnect;
	if (bStartRecvThread)
	{
		assert(m_bFlag == false);
		m_bFlag = true;
		boost::function0<void> f = boost::bind(&CWTcpSession::OnRead, this); 
		m_thThread = boost::thread(f);
		m_thThread.detach();
	}
	return true;
}
//设置连接配置
void CWTcpSession::SetConnectConfig(u_long uRemoteIp, 
  u_short uRemotePort)
{
  m_uRemotePort = uRemotePort;
  m_uRemoteIp = uRemoteIp;
}
bool CWTcpSession::Connect()
{
  bool bRes = 0;
  if (m_Socet.GetSocket() == INVALID_SOCKET)
  {
    bRes = m_Socet.Create();
    if (!bRes)
    {
      return bRes;
    }
    bRes = m_Socet.SetReceiveBuf(RECV_BUF);
    if (!bRes)
    {
      return bRes;
    }
  }

// #if !((defined WIN32) || (defined _WINDLL))
// 	struct timeval t;
// 	t.tv_sec = 2;	//设置2秒
// 	t.tv_usec = 0;
// 	setsockopt(m_hSocket, SOL_SOCKET, SO_SNDTIMEO, &t, sizeof(t));
// #endif

	bRes = m_Socet.Connect(m_uRemoteIp, m_uRemotePort);
	if (!bRes)
	{
		return false;
	}
 bRes = m_Socet.SetLinger(false);
 if (!bRes)
 {
   return false;
 }
// 	bRes = m_Socet.SetOutTimeMonitor(3000, 5000);
// 	if (!bRes)
// 	{
// 		m_Socet.Destroy();
// 		return false;
// 	}
	return true;
}

//描  述 : 断开
//返  回 : 
void CWTcpSession::DisConnect(bool bStopThread)
{
	if (m_bFlag)
	{//接收线程启动情况下
		if (bStopThread)
		{
			m_bAutoReconnect = false;
			m_bFlag = false;
		}
	}
	else
	{//没有启动接收线程情况下
		  m_bConnect = false; 
	}
 	m_Socet.Destroy();
}

//描  述 : 对Socket进行读处理
//参  数 : CWSocket*: IN  处理的套接字
//返  回 : 
void CWTcpSession::OnRead()
{
	int nCurrentSize = 0;
	//CWLog::GetInstance().WriteLog(0, "thread %d(CWTcpSession::OnRead) start (%x:%d)", Public::GetCurrentThreadId(), m_uRemoteIp, m_uRemotePort);
	while (m_bFlag)
	{
		try{
			if (!m_bConnect)
			{
				//重连处理
				if (!m_bAutoReconnect)
				{
					break;
				}
				m_bConnect = Connect();
				if (m_bConnect)
				{
					nCurrentSize = 0;
					OnMsg(0);
					continue;
				}
				else if (m_bAutoReconnect)
				{
					Sleep(1000);
					continue;
				}
				break;
			}
			//接收处理
			int nRecSize = m_Socet.Receive(m_pBuffer + nCurrentSize, m_nReadBufSize - nCurrentSize);
			if (nRecSize <= 0)
			{
				m_Socet.Destroy();
				m_bConnect = false;
				nCurrentSize = 0;
				if (OnMsg(-1))
				{
					continue;
				}
				else
				{
					break;
				}
			}
			nCurrentSize += nRecSize;
			int nDealSize = nCurrentSize;
			try
			{
				nDealSize = OnRecvData(m_pBuffer, nCurrentSize);
			}
			catch (...)
			{
				nDealSize = nCurrentSize;
			}
			if (nDealSize > 0)
			{
				if (nCurrentSize > nDealSize)
				{
					int nSize = nCurrentSize - nDealSize;
					memmove(m_pBuffer, m_pBuffer + nDealSize, nSize);
					nCurrentSize = nSize;
				}
				else
				{
					nCurrentSize = 0;
				}
			}
			else
			{
				if (nCurrentSize >= m_nReadBufSize)
				{
					//命令太大，无法处理,丢弃
					nCurrentSize = 0; 
				}
			}
		}catch(boost::thread_interrupted&)
		{
		}
	}
	//CWLog::GetInstance().WriteLog(0, "thread %d(CWTcpSession::OnRead ) exit (%x:%d)", Public::GetCurrentThreadId(), m_uRemoteIp, m_uRemotePort);
}

//描  述 : 发送数据
//参  数 : pBuffer: IN  输入缓存
//         nSize: IN 缓存大小
//返  回 : 
int CWTcpSession::SendData(const char* pBuffer, int nSize)
{
  if (!m_bConnect)
  {
    return -1;
  }
	int nRes = m_Socet.Send(pBuffer, nSize);
	if (nRes < 0 || nRes == nSize)
	{
		return nRes;
	}
	while (nRes != nSize)
	{
		const char* pTmpB = pBuffer + nRes;
		int nTmp = m_Socet.Send(pTmpB, nSize - nRes);
		if (nTmp > 0)
		{
			nRes += nTmp;
		}
		else
		{
			break;
		}
	}
	return nRes;
}

//描  述 : 接收数据
//参  数 : pBuffer: IN  输入缓存
//         nSize: IN 缓存大小
//返  回 : 
int CWTcpSession::RecvData(char* pBuffer, int nSize)
{
	if (!m_bFlag)
	{
		return m_Socet.Receive(pBuffer, nSize);
	}
	return 0;
}
//设置线程池大小
void CWTcpSession::SetThreadPoolSize(int nSize)
{
// 	boost::threadpool::pool::
// 	g_tcpSessionPool.
}