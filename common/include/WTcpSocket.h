////////////////////////////////////////////////////////////
//Copyright (C), 2011-2011, 北京数码视讯科技股份有限公司
//文件名 : WTcpSocket.h
//描  述 : 定义Tcp通信的接口
//备  注 : 
//历  史 : V1.0.0.0 王辉永 2011-3-7 创建
//		   
/////////////////////////////////////////////////////////////////
#pragma once
#include "WSocket.h"
//////////////////////////////////////////////////////////
//类  名 : CWTcpSocket
//描  述 : 定义Tcp通信的接口
//备  注 : 
//历  史 : 王辉永 3-7-2011 创建
//
//////////////////////////////////////////////////////////
class CWTcpSocket : public CWSocket
{
public:
	CWTcpSocket();
 CWTcpSocket(SOCKET s){m_hSocket = s;}
	virtual ~CWTcpSocket();
	//描  述 : 创建对象
	//返  回 : True,成功; False, 失败
	bool Create();
	void Destroy();

	void DisConnect();
	//描  述 : 初始化连接
	//参  数 : uHost: IN  要连接的主机
	//         nPort: OUT 连接的端口
	//         param3: IN/OUT 用途
	//返  回 : 
	bool  Connect(u_long uHost, u_short uPort);
	bool  Connect(const char* pszIp, u_short uPort);

	//描  述 : 接收数据
	//参  数 : pBuffer: IN  接收缓存
	//         nMaxSize: In 最大值
	//返  回 : 
	int Receive(char* pBuffer, 
		int nMaxSize)
	{
		return recv(m_hSocket, pBuffer, nMaxSize, 0);
	}
	
	//描  述 : 发送数据
	//参  数 : pBuffer: IN  发送数据
	//         nBufSize: In 数据长度
	//返  回 : 
	int Send(const char* pBuffer, 
		int nBufSize)
 {
#if (defined WIN32) || (defined _WINDLL)
   return send(m_hSocket, pBuffer, nBufSize, 0);
#else
   return send(m_hSocket, pBuffer, nBufSize, MSG_NOSIGNAL);
#endif
		
	}
};