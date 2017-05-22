/*****************************************************
 *Copyright(c) 2016-2017 
 *All right reserved
 *
 *�ļ����ƣ�WHttpServer.h
 *��Ҫ���������׵�http�����������ڴ������post��ʽhttpͨ��
 *�������ڣ�2016-8-3
 *���ߣ�������(why) 
 *˵����
 *		��http��Ҫʹ��post��ʽ�����������ݣ�������Ӧʧ��
 *�޸ģ�
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
	//��  �� : ����
	//��  �� : 
	bool Start(UINT16 nPort,
		int nMaxConnectNum = 80,
		const string& strIp = "",
		int nBackLogin = 10);
	void Stop();
	//��������
	int SendCmd(int nSession, const string& strData, bool bCompress);
protected:
	virtual void DealCmd(const string& strUrl, const string& strCmd, string& res, bool& bComress, int nSessionID) = 0;
private:
	CWSvrHttpSession* CreateSession();
	friend class CWSvrHttpSession;
};