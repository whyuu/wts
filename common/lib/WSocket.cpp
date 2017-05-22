////////////////////////////////////////////////////////////
//Copyright (C), 2010-2010, 
//�ļ��� : WSocket.cpp
//ͷ�ļ� : WSocket.h   
//��  ʷ : V1.0.0.0 ������ 2011-3-8 ����
//
/////////////////////////////////////////////////////////////////

#include "WSocket.h"
#if (defined WIN32) || (defined _WINDLL) 
#include <MSTcpIP.h>
#endif
CWSocket::CWSocket():m_hSocket(INVALID_SOCKET)
{
}

CWSocket::~CWSocket()
{
}

//��  �� : ���ٶ���
//��  �� ��
//��  �� : 
void CWSocket::Destroy()
{
	if (m_hSocket != INVALID_SOCKET)
	{
   //���time_wait���⣬ע�͵�shutdown.
   //���������ȷʵҪ����������,�ٹرգ���Ҫ��ʾ����shutdown��
   //����Ӧ���ڴ˴�����,������һ��ʹ��socketǰclose�ɵ�socket��
		//shutdown(m_hSocket, SD_BOTH); 
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}
}

//��  �� : ��������״̬��soket�ڵ���closesocket��ǿ�ƹرգ�������TIME_WAIT�Ĺ���
//��  �� : bFlag: IN  ������ǣ�FALSE ǿ�ƹرգ�TRUE��ǿ�ƹر�
//��  �� : TRUE:�ɹ�
//		   FALSE:ʧ��
bool CWSocket::SetLinger(int time)
{
// #if (defined WIN32) || (defined _WINDLL)  
// 	if (SOCKET_ERROR == setsockopt(m_hSocket, SOL_SOCKET, SO_DONTLINGER, (char *)&time, sizeof(int)))
// 	{
// 		return false;
// 	}
// #else
	struct linger tmp;
	tmp.l_onoff = 1;
	tmp.l_linger = (u_short)time;
	if (SOCKET_ERROR == setsockopt(m_hSocket, SOL_SOCKET, SO_LINGER, (char*)&tmp, sizeof(tmp)))
	{
		return false;
	}
//#endif
	return true;
}

//��  �� : ���õ�ַ����
//��  �� : bFlag: IN  ���ñ�־
//��  �� : TRUE��������
//		   FALSE����������
bool CWSocket::SetReuse(int bFlag)
{
	int nRet = setsockopt(m_hSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&bFlag, sizeof(int));
	if (nRet != 0)
	{
		return false;
	}
	return true;
}

//��  �� : ���ö˿ڱ�������
//��  �� : bFlag: IN  FALSE �رգ�TRUE ��
//��  �� : TRUE:�ɹ�
//		   FALSE:ʧ��
bool CWSocket::KeepAlive(int bFlag, int nOutTime, int nInt, int nFreq)
{
	int nRet = setsockopt(m_hSocket, SOL_SOCKET, SO_KEEPALIVE, (char *)&bFlag, sizeof(int));
	if (nRet != 0)
	{
		return false;
	}
	return true;
}

//��  �� : ���öಥ���շ���־
//��  �� : bFlag: IN  �ಥ�Է����ձ�־
//��  �� : TRUE���ɹ�
//		   FALSE��ʧ��
bool CWSocket::SetMultiLoop(int bFlag)
{
#if (defined WIN32) || (defined _WINDLL)  
	int nRet = setsockopt(m_hSocket, SOL_SOCKET, SIO_MULTIPOINT_LOOPBACK, (char *)&bFlag, sizeof(int));
	if (nRet != 0)
	{
		return false;
	}
#endif
	return true;
}

//��  �� : ���ý��ջ�����
//��  �� : nSize: IN ��������С
//��  �� : TRUE:�ɹ�
//		   FALSEʧ�� 
bool CWSocket::SetReceiveBuf(int nSize)
{
	int nRet = setsockopt(m_hSocket, SOL_SOCKET, SO_RCVBUF, (char *)&nSize, sizeof(int));
	if (nRet != 0)
	{
		return false;
	}
	return true;
}

//��  �� : ��ȡ���ջ�������С
//��  �� : nSize: OUT ��������С
//��  �� : TRUE���ɹ�
//		   FALSE��ʧ��
bool CWSocket::GetReceiveBuf(int& nSize)
{
	int nTmp =  sizeof(nSize);
	int nRet = getsockopt(m_hSocket, SOL_SOCKET, SO_RCVBUF, (char *)&nSize, (socklen_t*)&nTmp);
	if (nRet != 0)
	{
		return false;
	}
	return true;
}

//��  �� : ���÷��ͻ�����
//��  �� : nSize: IN ��������С
//��  �� : TRUE���ɹ�
//		   FALSE��ʧ��
bool CWSocket::SetSendBuf(int nSize)
{
	int nRet = setsockopt(m_hSocket, SOL_SOCKET, SO_SNDBUF, (char *)&nSize, sizeof(int));
	if (nRet != 0)
	{
		return false;
	}
	return true;
}

//��  �� : ��ȡ���ͻ�������С
//��  �� : nSize: IN ��������С
//��  �� : TRUE���ɹ�
//		   FALSE��ʧ��
bool CWSocket::GetSendBuf(int& nSize)
{
	int nTmp = sizeof(int);
	int nRet = getsockopt(m_hSocket, SOL_SOCKET, SO_SNDBUF, (char *)&nSize, (socklen_t*)&nTmp);
	if (nRet != 0)
	{
		return false;
	}
	return true;
}

//��  �� : ����Socket������
//��  �� : bFlag: IN  ��������־�� TURE����������FALSE:����
//��  �� : TRUE���ɹ�
//		   FALSE��ʧ�� 
bool CWSocket::SetNoBlock(bool bFlag)
{
#if (defined WIN32) || (defined _WINDLL)  
	unsigned long nVal = bFlag;
	int nRes = ioctlsocket(m_hSocket, FIONBIO, &nVal);
	if (nRes != 0)
	{
		return false;
	}
#else
	if (bFlag)
	{
		int flags = fcntl(m_hSocket, F_GETFL, 0); 
		fcntl(m_hSocket, F_SETFL, flags | O_NONBLOCK);
	}
	else
	{
		int flags = fcntl(m_hSocket, F_GETFL, 0); 
		fcntl(m_hSocket, F_SETFL, flags & ~O_NONBLOCK);
	}
#endif
	return true;
}

//��  �� : ���ý��ճ�ʱ(linuxʹ��)
//��  �� : tv: ��ʱʱ����
//��  �� : TRUE���ɹ�
//		   FALSE��ʧ�� 
bool CWSocket::SetRecvTimeOut(timeval& tv)
{
	int nRes = setsockopt(m_hSocket,SOL_SOCKET,SO_RCVTIMEO,(char*)&tv, sizeof(timeval));
	if (nRes != 0)
	{
		return false;
	}
	return true;
}
//��  �� : ���÷��ͳ�ʱ(linuxʹ��)
//��  �� : tv: ��ʱʱ����
//��  �� : TRUE���ɹ�
//		   FALSE��ʧ�� 
bool CWSocket::SetSendTimeOut(timeval& tv)
{
	int nRes = setsockopt(m_hSocket,SOL_SOCKET,SO_SNDTIMEO,(char*)&tv, sizeof(timeval));
	if (nRes != 0)
	{
		return false;
	}
	return true;
}

//��  �� : ���ý��ճ�ʱ
//��  �� : tv: ��ʱʱ����
//��  �� : TRUE���ɹ�
//		   FALSE��ʧ�� 
bool CWSocket::SetRecvTimeOut(int tv)
{
	int nRes = setsockopt(m_hSocket,SOL_SOCKET,SO_RCVTIMEO,(char*)&tv, sizeof(int));
	if (nRes != 0)
	{
		return false;
	}
	return true;
}
//��  �� : ��ȡ���ճ�ʱ
//��  �� : tv: ��ʱʱ����
//��  �� : TRUE���ɹ�
//		   FALSE��ʧ�� 
bool CWSocket::GetRecvTimeOut(int& tv)
{
	int nTmp = sizeof(int);
	int nRet = getsockopt(m_hSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, (socklen_t*)&nTmp);
	if (nRet != 0)
	{
		return false;
	}
	return true;
}

bool CWSocket::SetOutTimeMonitor(int nInt, int nOutTime)
{
#if (defined WIN32) || (defined _WINDLL) 
	//���ó�ʱ���
	tcp_keepalive tkl = { 0 };
	tkl.onoff    = true;
	tkl.keepaliveinterval   = nInt;
	tkl.keepalivetime       = nOutTime;
	UINT32 dwReturn(0);
	UINT32 dwTransBytes(0);
	int nRet = WSAIoctl(m_hSocket, SIO_KEEPALIVE_VALS, &tkl, sizeof(tkl), &dwReturn, sizeof(dwReturn), (LPDWORD)&dwTransBytes, NULL, NULL);
#else
	int idle, cnt, alive, intv;  
	/* Set: use keepalive on fd */  
	alive = 1;  
	if (setsockopt  
		(m_hSocket, SOL_SOCKET, SO_KEEPALIVE, &alive,  
		sizeof alive) != 0)  
	{  
		return false;  
	}  
	/* ���������ݣ�����������ƣ����ͱ���� */  
	idle = nOutTime/1000;  
	if (setsockopt (m_hSocket, SOL_TCP, TCP_KEEPIDLE, &idle, sizeof idle) != 0)  
	{  
		return false; 
	}
	/* ���û���յ���Ӧ�������Ӻ��ط������ */  
	intv = nInt/1000;  
	if (setsockopt (m_hSocket, SOL_TCP, TCP_KEEPINTVL, &intv, sizeof intv) != 0)  
	{
		return false;  
	}
	/* ��������û�յ����������Ϊ����ʧЧ */  
	cnt = 3;  
	if (setsockopt (m_hSocket, SOL_TCP, TCP_KEEPCNT, &cnt, sizeof cnt) != 0)  
	{
		return false;  
	} 
#endif
	return true;
}
