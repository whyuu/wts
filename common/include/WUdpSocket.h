#pragma once
#include "WSocket.h"

//////////////////////////////////////////////////////////
//类  名 : CWUdpSocket
//描  述 : 定义了Udp通信相关的接口
//备  注 : 
//历  史 : 王辉永 3-7-2011 创建
//
//////////////////////////////////////////////////////////
class CWUdpSocket : public CWSocket
{
public:
	CWUdpSocket();
	virtual ~CWUdpSocket();

	//描  述 : 创建对象
	//参	  数 ：
	//返  回 : TRUE:成功;
	//		   FALSE: 失败
	bool Create();

	//描  述 : 绑定端口
	//参  数 : nSocketPort: IN  Udp端口
	//         uIp: IN Ip地址
	//返  回 : TRUE:成功;
	//		   FALSE: 失败 
	bool Bind(u_short nSocketPort, u_long uIp = 0);

	//描  述 : 接收数据
	//参  数 : pBuffer: IN  接收缓存
	//         nMaxSize: In 最大值
	//		   Ip: OUT  对方Ip地址
	//         Port: OUT 对方发送端口
	//返  回 : SOCKET_ERROR错误码
	int Receive(char* pBuffer, 
				int nMaxSize, 
				u_long& uIp, 
				u_short& usPort);

 //描  述 : 接收数据
 //参  数 : pBuffer: IN  接收缓存
 //         nMaxSize: In 最大值
 //返  回 : SOCKET_ERROR错误码
 int Receive(char* pBuffer, 
   int nMaxSize);
	
	//描  述 : 发送数据
	//参  数 : pBuffer: IN  接收缓存
	//         nMaxSize: In 最大值
	//		   remoteIp: In	目的Ip地址
	//         remotePort: In 目的端口
	//返  回 : SOCKET_ERROR错误码
	int Send(const char* pBuffer, 
			 int nMaxSize, 
			 u_long uIp, 
			 u_short usPort);

	//描  述 : 设置多播发送地址
	//参  数 : uIp: IN 多播发送IP
	//         uSelectIp: IN 多播发送选择的网卡Ip
	//返  回 : SOCKET_ERROR错误码
	int SetMultiAddr(u_long uIp, u_long uSelectIp);
};