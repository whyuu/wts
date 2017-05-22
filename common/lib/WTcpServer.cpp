////////////////////////////////////////////////////////////
//Copyright (C), 2010-2010, 
//�ļ��� : WTcpServer.cpp
//ͷ�ļ� : WTcpServer.h
//��  ע : ע������
//         ����δ��ά����Ա�Ľ���
//		   
//��  ʷ : V1.0.0.0  2011-3-7 ����
//
/////////////////////////////////////////////////////////////////
#include "WTcpServer.h"
#include "WnScopedLock.h"
using namespace std;
CWTcpServer::CWTcpServer() //:m_bFlag(false)
{
}

CWTcpServer::~CWTcpServer()
{
}

//��  �� : ����
//��  �� : 
bool  CWTcpServer::Start(u_short uPort,
	int nMaxConnectNum,
	u_long uIp,
	int nBackLogin)
{
	if (nMaxConnectNum > 65535)
	{
		return false;
	}
	m_sessionflag.CreateQueue(nMaxConnectNum);
	m_arrSession.resize(nMaxConnectNum);
	for (int i = 0; i < nMaxConnectNum; i++)
	{
		m_arrSession[i] = CreateSession();
		m_arrSession[i]->m_Obj.nId = (UINT16)i;
		m_sessionflag.Enqueue(i);
	}
	//�����˿�
	bool  bRes = m_TcpServerPort.Create(uPort, this, uIp, nBackLogin);
	if (!bRes)
	{
		return false;
	}
	return true;
}

//��  �� : ֹͣ
//��  �� : 
void CWTcpServer::Stop()
{
	m_TcpServerPort.Destroy();
	for (int i = 0; i < m_sessionflag.GetMaxNum(); i++)
	{
		CWSvrTcpSession* pTmp = m_arrSession[i];
		pTmp->DisConnect();
		DestroySession(pTmp);
	}
}

//��  �� : �׽��ֽ���
//��  �� : s: IN  �׽���
//         uIp: IN IP��ַ
//         uPort: IN �˿�
//��  �� : 
void CWTcpServer::OnAccept(SOCKET s, u_long uIp, u_short uPort)
{
	CWSvrTcpSession* pS = GetSession();
	if (pS != NULL)
	{
		if (!pS->Attach(s, uIp, uPort))
		{
			ReleaseSession(pS);
		}
	}
	else
	{
		CWSocket obj;
		obj.Attach(s);
  obj.SetLinger(0);
		obj.Destroy();
	}
}

//��  �� :��ȡ�Ự
//��  �� :
//��  �� : 
CWSvrTcpSession* CWTcpServer::GetSession()
{
	UINT16 nPos = 0xffff;
	CWnScopedLock Lock(m_sessionLock);
	m_sessionflag.Dequeue(nPos);
	if (nPos != 0xffff)
	{
		return m_arrSession[nPos]; 
	}
	return NULL;
}
//��  �� :�ͷŻỰ
//��  �� :
//��  �� : 
void CWTcpServer::ReleaseSession(CWSvrTcpSession* pS)
{
	pS->m_Obj.nSId++;
	pS->DisConnect();
	CWnScopedLock Lock(m_sessionLock);
	m_sessionflag.Enqueue(pS->m_Obj.nId);
}

CWSvrTcpSession* CWTcpServer::GetSession(int nSessionId)
{
  TcpSessionObj sObj;
  sObj.uSessionId = nSessionId;
  if (sObj.nId >= (UINT16)m_sessionflag.GetMaxNum())
  {
    return NULL;
  }
  return m_arrSession[sObj.nId];
}
