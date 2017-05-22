////////////////////////////////////////////////////////////
//Copyright (C), 2010-2010, 
//文件名 : WSocket.cpp
//头文件 : WSocket.h   
//历  史 : V1.0.0.0 王辉永 2011-3-8 创建
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

//描  述 : 销毁对象
//参  数 ：
//返  回 : 
void CWSocket::Destroy()
{
	if (m_hSocket != INVALID_SOCKET)
	{
   //解决time_wait问题，注释掉shutdown.
   //如果短连接确实要发送完数据,再关闭，需要显示调用shutdown，
   //而不应该在此处调用,并在下一次使用socket前close旧的socket。
		//shutdown(m_hSocket, SD_BOTH); 
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}
}

//描  述 : 设置连接状态的soket在调用closesocket后强制关闭，不经历TIME_WAIT的过程
//参  数 : bFlag: IN  参数标记，FALSE 强制关闭，TRUE不强制关闭
//返  回 : TRUE:成功
//		   FALSE:失败
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

//描  述 : 设置地址重用
//参  数 : bFlag: IN  重用标志
//返  回 : TRUE：可重用
//		   FALSE：不可重用
bool CWSocket::SetReuse(int bFlag)
{
	int nRet = setsockopt(m_hSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&bFlag, sizeof(int));
	if (nRet != 0)
	{
		return false;
	}
	return true;
}

//描  述 : 设置端口保持连接
//参  数 : bFlag: IN  FALSE 关闭，TRUE 打开
//返  回 : TRUE:成功
//		   FALSE:失败
bool CWSocket::KeepAlive(int bFlag, int nOutTime, int nInt, int nFreq)
{
	int nRet = setsockopt(m_hSocket, SOL_SOCKET, SO_KEEPALIVE, (char *)&bFlag, sizeof(int));
	if (nRet != 0)
	{
		return false;
	}
	return true;
}

//描  述 : 设置多播自收发标志
//参  数 : bFlag: IN  多播自发自收标志
//返  回 : TRUE：成功
//		   FALSE：失败
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

//描  述 : 设置接收缓冲区
//参  数 : nSize: IN 缓冲区大小
//返  回 : TRUE:成功
//		   FALSE失败 
bool CWSocket::SetReceiveBuf(int nSize)
{
	int nRet = setsockopt(m_hSocket, SOL_SOCKET, SO_RCVBUF, (char *)&nSize, sizeof(int));
	if (nRet != 0)
	{
		return false;
	}
	return true;
}

//描  述 : 获取接收缓冲区大小
//参  数 : nSize: OUT 缓冲区大小
//返  回 : TRUE：成功
//		   FALSE：失败
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

//描  述 : 设置发送缓冲区
//参  数 : nSize: IN 缓冲区大小
//返  回 : TRUE：成功
//		   FALSE：失败
bool CWSocket::SetSendBuf(int nSize)
{
	int nRet = setsockopt(m_hSocket, SOL_SOCKET, SO_SNDBUF, (char *)&nSize, sizeof(int));
	if (nRet != 0)
	{
		return false;
	}
	return true;
}

//描  述 : 获取发送缓冲区大小
//参  数 : nSize: IN 缓冲区大小
//返  回 : TRUE：成功
//		   FALSE：失败
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

//描  述 : 设置Socket非阻塞
//参  数 : bFlag: IN  非阻塞标志， TURE：非阻塞；FALSE:阻塞
//返  回 : TRUE：成功
//		   FALSE：失败 
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

//描  述 : 设置接收超时(linux使用)
//参  数 : tv: 超时时间间隔
//返  回 : TRUE：成功
//		   FALSE：失败 
bool CWSocket::SetRecvTimeOut(timeval& tv)
{
	int nRes = setsockopt(m_hSocket,SOL_SOCKET,SO_RCVTIMEO,(char*)&tv, sizeof(timeval));
	if (nRes != 0)
	{
		return false;
	}
	return true;
}
//描  述 : 设置发送超时(linux使用)
//参  数 : tv: 超时时间间隔
//返  回 : TRUE：成功
//		   FALSE：失败 
bool CWSocket::SetSendTimeOut(timeval& tv)
{
	int nRes = setsockopt(m_hSocket,SOL_SOCKET,SO_SNDTIMEO,(char*)&tv, sizeof(timeval));
	if (nRes != 0)
	{
		return false;
	}
	return true;
}

//描  述 : 设置接收超时
//参  数 : tv: 超时时间间隔
//返  回 : TRUE：成功
//		   FALSE：失败 
bool CWSocket::SetRecvTimeOut(int tv)
{
	int nRes = setsockopt(m_hSocket,SOL_SOCKET,SO_RCVTIMEO,(char*)&tv, sizeof(int));
	if (nRes != 0)
	{
		return false;
	}
	return true;
}
//描  述 : 获取接收超时
//参  数 : tv: 超时时间间隔
//返  回 : TRUE：成功
//		   FALSE：失败 
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
	//设置超时监测
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
	/* 秒钟无数据，触发保活机制，发送保活包 */  
	idle = nOutTime/1000;  
	if (setsockopt (m_hSocket, SOL_TCP, TCP_KEEPIDLE, &idle, sizeof idle) != 0)  
	{  
		return false; 
	}
	/* 如果没有收到回应，则５秒钟后重发保活包 */  
	intv = nInt/1000;  
	if (setsockopt (m_hSocket, SOL_TCP, TCP_KEEPINTVL, &intv, sizeof intv) != 0)  
	{
		return false;  
	}
	/* 连续３次没收到保活包，视为连接失效 */  
	cnt = 3;  
	if (setsockopt (m_hSocket, SOL_TCP, TCP_KEEPCNT, &cnt, sizeof cnt) != 0)  
	{
		return false;  
	} 
#endif
	return true;
}
