////////////////////////////////////////////////////////////
//Copyright (C), 2011-2011, 
//文件名 : WSocket.h
//描  述 : 声明Socket类，当监测到Socket可读时或者异常时将执行Sokcet处理类的相关函数
//历  史 : V1.0  2011-3-8 创建
//		   V1.1  2014-10-19 兼容linux
/////////////////////////////////////////////////////////////////
#pragma once
#include "WSocketType.h"

class CWSocket
{
// Construction
public:
	CWSocket();
	CWSocket(SOCKET s){m_hSocket = s;}
	virtual ~CWSocket();
#if (defined WIN32) || (defined _WINDLL)  
	operator SOCKET() const{return m_hSocket;}	
#endif
	SOCKET GetSocket(){return m_hSocket;}
	
	//描  述 : 用已知套接字初始化对象
	//参  数 : s: 套接字标识
	//返  回 : 
	void Attach(SOCKET s){m_hSocket = s;}

	//描  述 : 销毁对象
	//参  数 ：
	//返  回 : 
	void Destroy();

	//描  述 : 设置连接状态的soket
	//参  数 : time 连接关闭后等待时间，time = 0时在调用closesocket后强制关闭，不经历TIME_WAIT的过程
	//返  回 : TRUE:成功
	//		   FALSE:失败
	bool SetLinger(int time);

	//描  述 : 设置地址重用
	//参  数 : bFlag: IN  重用标志
	//返  回 : TRUE：可重用
	//		   FALSE：不可重用
	bool SetReuse(int bFlag);

	//描  述 : 设置端口保持连接
	//参  数 : bFlag: IN  FALSE 关闭，TRUE 打开
	//返  回 : TRUE:成功
	//		   FALSE:失败
	bool KeepAlive(int bFlag, int nOutTime = 5000, int nInt = 1000, int nFreq = 3);

	//描  述 : 设置多播自收发标志
	//参  数 : bFlag: IN  多播自发自收标志
	//返  回 : TRUE：成功
	//		   FALSE：失败
	bool SetMultiLoop(int bFlag);

	//描  述 : 设置接收缓冲区
	//参  数 : nSize: IN 缓冲区大小
	//返  回 : TRUE:成功
	//		   FALSE失败
	bool SetReceiveBuf(int nSize);

	//描  述 : 获取接收缓冲区大小
	//参  数 : nSize: OUT 缓冲区大小
	//返  回 : TRUE：成功
	//		   FALSE：失败
	bool GetReceiveBuf(int& nSize);

	//描  述 : 设置发送缓冲区
	//参  数 : nSize: IN 缓冲区大小
	//返  回 : TRUE：成功
	//		   FALSE：失败
	bool SetSendBuf(int nSize);

	//描  述 : 获取发送缓冲区大小
	//参  数 : nSize: IN 缓冲区大小
	//返  回 : TRUE：成功
	//		   FALSE：失败
	bool GetSendBuf(int& nSize);

	//描  述 : 设置Socket非阻塞
	//参  数 : bFlag: IN  非阻塞标志， TURE：非阻塞；FALSE:阻塞
	//返  回 : TRUE：成功
	//		   FALSE：失败 
	bool SetNoBlock(bool bFlag);

	//描  述 : 设置接收超时(linux使用)
	//参  数 : tv: 超时时间间隔
	//返  回 : TRUE：成功
	//		   FALSE：失败 
	bool SetRecvTimeOut(timeval& tv);

	//描  述 : 设置发送超时(linux使用)
	//参  数 : tv: 超时时间间隔
	//返  回 : TRUE：成功
	//		   FALSE：失败 
	bool SetSendTimeOut(timeval& tv);

	//描  述 : 设置接收超时
	//参  数 : tv: 超时时间间隔
	//返  回 : TRUE：成功
	//		   FALSE：失败 
	bool SetRecvTimeOut(int tv);

	//描  述 : 获取接收超时
	//参  数 : tv: 超时时间间隔
	//返  回 : TRUE：成功
	//		   FALSE：失败 
	bool GetRecvTimeOut(int& tv);

	//描  述 : 设置超时监测，当没有数据发送时，监测链路是否中断
	//参  数 : tv: 超时时间间隔
	//返  回 : TRUE：成功
	//		   FALSE：失败 
	bool SetOutTimeMonitor(int nInterval, int KeepLiveTime);
protected:
	SOCKET m_hSocket;
};