////////////////////////////////////////////////////////////
//Copyright (C), 2011-2011, 
//文件名 : WTcpClient.h
//描  述 : 定义WTcpClient类
//备  注 : 
//历  史 : V1.0.0.0 王辉永 2011-3-9 创建
//		   
/////////////////////////////////////////////////////////////////
#pragma once
#include <boost/thread.hpp>
#include "WTcpSocket.h"
#include "commType.h"

class CWTcpSession
{
public:
	CWTcpSession(int nReadBuf = 1024 * 1024);
	virtual ~CWTcpSession();

	//描  述 : 附加套接字
	//参  数 : s: IN  套接字
	//         uRemoteIp: IN 目的IP
	//         uPort: IN 目的端口
	//		 bStartRecvThread: 是否启动接收线程
	//返  回 : 
 virtual bool Attach(SOCKET s,
   u_long uRemoteIp,
   u_short uRemotePort,
   bool bStartRecvThread = true);
	//描  述 : 连接端口
	//参  数 : uRemoteIp: IN  主机IP
	//         uRemotePort: IN 端口
	//		   bRunConnect: 是否立即执行连接操作
	//		   bAutoReconnect: 断线是否自动重连
	//		   bStartRecvThread: 是否启动接收线程。
	//		   
	//返  回 :
	bool Connect(u_long uRemoteIp, 
		u_short uRemotePort,
		bool bRunConnect,			//是否立即连接
		bool bAutoReconnect = true,  //是否自动重连
		bool bStartRecvThread  = true); //是否开启接收线程

 //设置连接配置
 void SetConnectConfig(u_long uRemoteIp, 
   u_short uRemotePort);
	//描  述 : 发送数据
	//参  数 : pBuffer: IN  输入缓存
	//         nSize: IN 缓存大小
	//返  回 : 
	int SendData(const char* pBuffer, int nSize);

	//描  述 : 断开 
	//bStopThread : 是否立即停止线程
	//返  回 : 
	void DisConnect(bool bStopThread = true);

	//描  述 : 获取IP
	//返  回 : 返回IP
	u_long GetRemoteIp(){return m_uRemoteIp;}
	//描  述 : 获取端口
	//返  回 : 端口
	u_short GetRemotePort(){return m_uRemotePort;}
 
	//描  述 : 获取套接字句柄
	//返  回 : 
	SOCKET GetSocket(){return m_Socet.GetSocket();}

	bool IsConnect(){return m_bConnect;}

	void SetAutoReconnect(bool bAutoReconnect){m_bAutoReconnect = bAutoReconnect;}

	//描  述 : 接收数据, 在未启动接收线程时起作用，启动接收线程后，无效
	//参  数 : pBuffer: IN  输入缓存
	//         nSize: IN 缓存大小
	//返  回 : 
	int RecvData(char* pBuffer, int nSize);
	//是否接收线程退出
	bool GetFlag(){return m_bFlag;}
	
protected:
	virtual int OnRecvData(char* pBuffer, int nSize) = 0;//{return nSize;}
	//错误处理 :false  退出， true，不退出
	virtual bool OnMsg(int nErrorCode) = 0;//{return m_bAutoReconnect;}
	//设置线程池大小
	static void SetThreadPoolSize(int nSize);

	//连接套接字
	CWTcpSocket m_Socet;

private:
	//enum{ BUFFER_SIZE = 1024 * 1024};
	//连接
	bool Connect();
	//描  述 : 读socket线程
	//参  数 : param1:  
	//         param2:  
	//返  回 : 0:成功
	//         其他:错误码
	void OnRead();
	//主机IP
	u_long m_uRemoteIp;

	//主机端口
	u_short m_uRemotePort;
	
	//线程函数
	boost::thread m_thThread;
	bool volatile m_bFlag;

	//数据缓存
	char* m_pBuffer;

	//重连相关的函数
	bool m_bAutoReconnect; //是否自动重连
	bool m_bConnect; //是否连接
	int m_nReadBufSize; //缓存大小
};