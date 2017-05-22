////////////////////////////////////////////////////////////
//Copyright (C), 2010-2010, 北京数码视讯科技股份有限公司
//文件名 : WTcpSvrSocket.cpp
//头文件 : WTcpSvrSocket.h
//备  注 : 
//		   
//历  史 : V1.0.0.0 王辉永 2011-3-7 创建
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

//描  述 : 创建对象
//返  回 : True,成功; False, 失败
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
	//添参数
	local.sin_addr.s_addr = uIp;
	local.sin_family = AF_INET;
	local.sin_port = htons(nSocketPort);

	if (!SetLinger(false))
	{
		Destroy();
		return false;
	}
	SetReuse(1);
	//绑定端口
	if(bind(m_hSocket, (struct sockaddr *)&local, sizeof(local)) == SOCKET_ERROR)
	{
		Destroy();
		return false;
	}
	
	m_pObj = pObj;

	//创建监听socket

	// 	timeval timeout = {0, 500000};
	// 	SetRecvTimeOut(timeout);

	//开始侦听
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

//销毁
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

//描  述 : 
//参  数 : param1: IN  用途
//         param2: OUT 用途
//         param3: IN/OUT 用途
//返  回 : 
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
//描  述 : 接收连接处理器
//参  数 : 监听线程
//返  回 : 
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