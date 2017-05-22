////////////////////////////////////////////////////////////
//Copyright (C), 2011-2011, 
//文件名 : WTcpSvrSocket.h
//描  述 : 声明Tcp监听socket类
//备  注 : 
//历  史 : V1.0.0.0  2011-3-7 创建
//		   
/////////////////////////////////////////////////////////////////

#pragma once
#include "WSocket.h"
#include <boost/thread.hpp>
class CWTcpSocketDeal
{
public:
	//描  述 : 套接字接收
	//参  数 : s: IN  套接字
	//         uIp: IN IP地址
	//         uPort: IN 端口
	//返  回 : 
	virtual void OnAccept(SOCKET s, u_long uIp, u_short uPort) = 0;
};
//////////////////////////////////////////////////////////
//类  名 : CWTcpSvrSocket
//描  述 : Tcp监听socket类
//备  注 : 
//历  史 :  3-7-2011 创建
//
//////////////////////////////////////////////////////////
class CWTcpSvrSocket : public CWSocket
{
public:
	CWTcpSvrSocket();
	virtual ~CWTcpSvrSocket();
	//描  述 : 创建对象
	//返  回 : True,成功; False, 失败
	bool  Create(u_short nSocketPort,
		CWTcpSocketDeal* pObj,
		u_long uIp,
		int nBackLogin = 5);

	//销毁
	bool  Destroy();

private:
	//描  述 : 
	//参  数 : s: OUT  用途
	//         uIp: OUT 用途
	//         uPort: OUT 用途
	//返  回 : True,成功; False, 失败
	bool  GetConnect(SOCKET& s, 
		u_long& uIp,
		u_short& uPort);

private:
	//描  述 : 接收连接处理器
	//参  数 : 监听线程
	//返  回 : 
	void ThServerMonitor();
	//Socket接收对象
	CWTcpSocketDeal* m_pObj;
	//tcp监听线程
	boost::thread m_thMonitor;
	bool volatile m_bFlag;
};