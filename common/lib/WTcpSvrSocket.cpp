////////////////////////////////////////////////////////////
//Copyright (C), 2010-2010, ����������Ѷ�Ƽ��ɷ����޹�˾
//�ļ��� : WTcpSvrSocket.cpp
//ͷ�ļ� : WTcpSvrSocket.h
//��  ע : 
//		   
//��  ʷ : V1.0.0.0 ������ 2011-3-7 ����
//
/////////////////////////////////////////////////////////////////
#include "WTcpSvrSocket.h"
#include "WLog.h"
#include "Public.h"
CWTcpSvrSocket::CWTcpSvrSocket() : m_pObj(NULL), m_bFlag(false)
{
}
CWTcpSvrSocket::~CWTcpSvrSocket()
{
	Destroy();
}

//��  �� : ��������
//��  �� : True,�ɹ�; False, ʧ��
bool  CWTcpSvrSocket::Create(u_short nSocketPort,
	CWTcpSocketDeal* pObj,
	u_long uIp,
	int nBackLogin)
{
	m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if(m_hSocket == SOCKET_ERROR)
	{
		return false;
	}
	struct	sockaddr_in local;	
	//�����
	local.sin_addr.s_addr = uIp;
	local.sin_family = AF_INET;
	local.sin_port = htons(nSocketPort);

	if (!SetLinger(false))
	{
		Destroy();
		return false;
	}
	SetReuse(1);
	//�󶨶˿�
	if(bind(m_hSocket, (struct sockaddr *)&local, sizeof(local)) == SOCKET_ERROR)
	{
		Destroy();
		return false;
	}
	
	m_pObj = pObj;

	//��������socket

	// 	timeval timeout = {0, 500000};
	// 	SetRecvTimeOut(timeout);

	//��ʼ����
	if(listen(m_hSocket, nBackLogin) != 0)
	{
		Destroy();
		return false;
	}
	m_bFlag = true;
	boost::function0<void> f = boost::bind(&CWTcpSvrSocket::ThServerMonitor, this); 
	m_thMonitor = boost::thread(f);
	return true;
}

//����
bool  CWTcpSvrSocket::Destroy()
{
	if (m_bFlag)
	{
		m_bFlag = false;
  shutdown(m_hSocket, SD_BOTH);
  CWSocket::Destroy();
		m_thMonitor.join();
	}
	return true;
}

//��  �� : 
//��  �� : param1: IN  ��;
//         param2: OUT ��;
//         param3: IN/OUT ��;
//��  �� : 
bool  CWTcpSvrSocket::GetConnect(SOCKET& s, u_long& uIp, u_short& uPort)
{
	int		iAddrSize;
	struct	sockaddr_in addr;	
	iAddrSize = sizeof(addr);

	s = accept(m_hSocket, (struct sockaddr *)&addr, (socklen_t*)&iAddrSize);	
	if(s == SOCKET_ERROR)
	{
		return false;
	}

	uIp = addr.sin_addr.s_addr;
	uPort = ntohs(addr.sin_port);
	return true;
}
//��  �� : �������Ӵ�����
//��  �� : �����߳�
//��  �� : 
void CWTcpSvrSocket::ThServerMonitor()
{
//	CWLog::GetInstance().WriteLog(0, "thread %d(CWTcpSvrSocket::ThServerMonitor) start ", Public::GetCurrentThreadId());
	try{
		SOCKET s;
		u_long uIp;
		u_short uPort;
		while (m_bFlag)
		{
			if (GetConnect(s, uIp, uPort ))
			{
				m_pObj->OnAccept(s, uIp, uPort);
			}
		}
	}catch(boost::thread_interrupted&)
	{
	}
}