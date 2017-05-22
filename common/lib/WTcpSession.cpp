////////////////////////////////////////////////////////////
//Copyright (C), 2010-2010, 
//�ļ��� : WTcpClient.cpp
//ͷ�ļ� : WTcpClient.h
//��  ע : ע������
//         ����δ��ά����Ա�Ľ���
//		   
//��  ʷ : V1.0.0.0  2011-3-9 ����
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

//��  �� : �����׽���
//��  �� : pObj IN �������
//		   s: IN  �׽���
//         uRemoteIp: IN Ŀ��IP
//         uPort: IN Ŀ�Ķ˿�
//��  �� : 
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

//��  �� : 
//��  �� : hostIp: IN  ����IP
//         uPort: IN �˿�
//��  �� : 
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
//������������
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
// 	t.tv_sec = 2;	//����2��
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

//��  �� : �Ͽ�
//��  �� : 
void CWTcpSession::DisConnect(bool bStopThread)
{
	if (m_bFlag)
	{//�����߳����������
		if (bStopThread)
		{
			m_bAutoReconnect = false;
			m_bFlag = false;
		}
	}
	else
	{//û�����������߳������
		  m_bConnect = false; 
	}
 	m_Socet.Destroy();
}

//��  �� : ��Socket���ж�����
//��  �� : CWSocket*: IN  ������׽���
//��  �� : 
void CWTcpSession::OnRead()
{
	int nCurrentSize = 0;
	//CWLog::GetInstance().WriteLog(0, "thread %d(CWTcpSession::OnRead) start (%x:%d)", Public::GetCurrentThreadId(), m_uRemoteIp, m_uRemotePort);
	while (m_bFlag)
	{
		try{
			if (!m_bConnect)
			{
				//��������
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
			//���մ���
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
					//����̫���޷�����,����
					nCurrentSize = 0; 
				}
			}
		}catch(boost::thread_interrupted&)
		{
		}
	}
	//CWLog::GetInstance().WriteLog(0, "thread %d(CWTcpSession::OnRead ) exit (%x:%d)", Public::GetCurrentThreadId(), m_uRemoteIp, m_uRemotePort);
}

//��  �� : ��������
//��  �� : pBuffer: IN  ���뻺��
//         nSize: IN �����С
//��  �� : 
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

//��  �� : ��������
//��  �� : pBuffer: IN  ���뻺��
//         nSize: IN �����С
//��  �� : 
int CWTcpSession::RecvData(char* pBuffer, int nSize)
{
	if (!m_bFlag)
	{
		return m_Socet.Receive(pBuffer, nSize);
	}
	return 0;
}
//�����̳߳ش�С
void CWTcpSession::SetThreadPoolSize(int nSize)
{
// 	boost::threadpool::pool::
// 	g_tcpSessionPool.
}