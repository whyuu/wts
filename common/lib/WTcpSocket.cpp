////////////////////////////////////////////////////////////
//Copyright (C), 2010-2010, 
//文件名 : WTcpSocket.cpp
//头文件 : WTcpSocket.h
//备  注 : 
//		   
//历  史 : V1.0.0.0 王辉永 2011-3-7 创建
//
/////////////////////////////////////////////////////////////////
#include "WTcpSocket.h"

CWTcpSocket::CWTcpSocket()
{
}

CWTcpSocket::~CWTcpSocket()
{
}

//描  述 : 创建对象
//返  回 : True,成功; False, 失败
bool  CWTcpSocket::Create()
{
  m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
  if (m_hSocket != INVALID_SOCKET)
  {
    return true;
  }
  return false;
}

 void CWTcpSocket::Destroy()
 {
   if (m_hSocket != INVALID_SOCKET)
   {
     shutdown(m_hSocket, SD_BOTH);
     CWSocket::Destroy();
   }
 }

//描  述 : 初始化连接
//参  数 : uHost: IN  要连接的主机
//         uPort: OUT 连接的端口
//         param3: IN/OUT 用途
//返  回 : 
bool  CWTcpSocket::Connect(u_long uHost, u_short uPort)
{
//   sockaddr_in sockAddr;
//   int nlen = (int)sizeof(sockAddr);
//   getsockname(m_hSocket, (sockaddr*)&sockAddr, &nlen);
//   int nPort = ntohs(sockAddr.sin_port);

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = uHost;
	server.sin_port = htons(uPort);
	if (SOCKET_ERROR == connect(m_hSocket, (struct sockaddr *)&server, sizeof(struct sockaddr)))
	{
		return false;
	}

//  getsockname(m_hSocket, (sockaddr*)&sockAddr, &nlen);
//  nPort = ntohs(sockAddr.sin_port);

	return true;
}

bool  CWTcpSocket::Connect(const char* pszIp, u_short uPort)
{
  return Connect(inet_addr(pszIp), uPort);
}

void  CWTcpSocket::DisConnect()
{
  shutdown(m_hSocket, SD_BOTH);
}
//描  述 : 接收数据
//参  数 : pBuffer: IN  接收缓存
//         nMaxSize: In 最大值
//返  回 : 
// int CWTcpSocket::Receive(char* pBuffer, 
// 			int nMaxSize)
// {
// 	return recv(m_hSocket, pBuffer, nMaxSize, 0);
// }
// 
// 	//描  述 : 发送数据
// 	//参  数 : pBuffer: IN  发送数据
// 	//         nBufSize: In 数据长度
// 	//返  回 : 
// int CWTcpSocket::Send(const char* pBuffer, 
// 		 int nBufSize)
// {
// 	return send(m_hSocket, pBuffer, nBufSize, 0);
// }
