#include "WUdpSocket.h"
#ifdef _WIN32
#include "Ws2tcpip.h"
#else
#include<string.h>
#endif


CWUdpSocket::CWUdpSocket()
{
}
CWUdpSocket::~CWUdpSocket()
{
}

//描  述 : 创建对象
//参	  数 ：
//返  回 : TRUE:成功;
//		   FALSE: 失败
bool CWUdpSocket::Create()
{
	m_hSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_hSocket != INVALID_SOCKET)
	{
		return true;
	}
	return false;
}

//描  述 : 接收数据
//参  数 : pBuffer: IN  接收缓存
//         nMaxSize: In 最大值
//		   Ip: OUT  对方Ip地址
//         Port: OUT 对方发送端口
//返  回 : SOCKET_ERROR错误码
int CWUdpSocket::Receive(char* pBuffer, 
	int nMaxSize, 
	u_long& uIp, 
	u_short& usPort)
{
	struct sockaddr_in sockAddr;	
	socklen_t nSockAddrLen = sizeof(sockAddr);
	int nResult = recvfrom(m_hSocket, (char*)pBuffer, nMaxSize, 0, (sockaddr*)&sockAddr, &nSockAddrLen);
	if(nResult != SOCKET_ERROR)
	{
		usPort = ntohs(sockAddr.sin_port);
		uIp =  sockAddr.sin_addr.s_addr;
	}
	return nResult;
}
int CWUdpSocket::Receive(char* pBuffer, 
  int nMaxSize)
{
  return recv(m_hSocket,pBuffer, nMaxSize, 0);
}
//描  述 : 发送数据
//参  数 : pBuffer: IN  接收缓存
//         nMaxSize: In 最大值
//		   remoteIp: In	目的Ip地址
//         remotePort: In 目的端口
//返  回 : SOCKET_ERROR错误码
int CWUdpSocket::Send(const char* pBuffer, 
	int nMaxSize, 
	u_long uIp, 
	u_short usPort)
{
	struct sockaddr_in SockAddr;
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = uIp;
	SockAddr.sin_port = htons(usPort);
	return sendto(m_hSocket, pBuffer, nMaxSize, 0, (struct sockaddr *)&SockAddr, sizeof(SockAddr));
}

//描  述 : 绑定端口
//参  数 : nSocketPort: IN  Udp端口
//         uIp: IN Ip地址
//返  回 : TRUE:成功;
//		   FALSE: 失败 	
bool CWUdpSocket::Bind(u_short nSocketPort, u_long uIp)
{	
	if (uIp == INADDR_NONE)
	{
		return false;
	}
	struct sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = uIp;
	sockAddr.sin_port = htons((u_short)nSocketPort);
#ifdef _WIN32
 if (sockAddr.sin_addr.s_net>= 224
   && sockAddr.sin_addr.s_net <= 239)
 {
   return false;
 }
#endif
	if (SOCKET_ERROR == bind(m_hSocket, (struct sockaddr *)&sockAddr, sizeof(sockAddr)))
	{
		return false;
	}
	return true;
}

//描  述 : 设置多播发送地址
//参  数 : uIp: IN 多播发送IP
//         uSelectIp: IN 多播发送选择的网卡Ip
//返  回 : SOCKET_ERROR错误码
int CWUdpSocket::SetMultiAddr(u_long uIp, u_long uSelectIp)
{
  struct ip_mreq mcast;
  mcast.imr_multiaddr.s_addr = uIp;
  mcast.imr_interface.s_addr = uSelectIp;
  return setsockopt(m_hSocket, IPPROTO_IP,IP_ADD_MEMBERSHIP, (char*)&mcast, sizeof(mcast));
}