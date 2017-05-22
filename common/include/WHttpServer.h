/*****************************************************
 *Copyright(c) 2016-2017 
 *All right reserved
 *
 *文件名称：WHttpServer.h
 *简要描述：简易的http服务器，用于处理基于post方式http通信
 *创建日期：2016-8-3
 *作者：王辉永(why) 
 *说明：
 *		该http需要使用post方式包含数据内容，否则响应失败
 *修改：
********************************************************/
#pragma once
#include "WTcpServer.h"
#include "WHttpSession.h"
#include "WnLock.h"

class CWHttpServer;
class CWSvrHttpSession : public CWHttpSession, public CWSvrTcpSession
{
public:
	CWSvrHttpSession(CWHttpServer* pS);
 virtual ~CWSvrHttpSession();
 int SendData(UINT32 uSId, const string& strRes, bool bComress);
private:
  int DealHttpRequest(IN const string& strReq, 
    IN const string& url,
    IN OUT bool& bCompress,
    OUT string& strRes);
  bool OnMsg(int nErrorCode);
  int OnRecvData(char* pBuffer, int nSize){return CWHttpSession::OnRecvData(pBuffer, nSize);}
  CWHttpServer* m_pServer;
	CWnLock m_Lock;
};
class CWHttpServer : public CWTcpServer
{
public:
	CWHttpServer();
	~CWHttpServer(void);
	//描  述 : 启动
	//返  回 : 
	bool Start(UINT16 nPort,
		int nMaxConnectNum = 80,
		const string& strIp = "",
		int nBackLogin = 10);
	void Stop();
	//发送数据
	int SendCmd(int nSession, const string& strData, bool bCompress);
protected:
	virtual void DealCmd(const string& strUrl, const string& strCmd, string& res, bool& bComress, int nSessionID) = 0;
private:
	CWSvrHttpSession* CreateSession();
	friend class CWSvrHttpSession;
};