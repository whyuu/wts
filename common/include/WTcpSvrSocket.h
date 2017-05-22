////////////////////////////////////////////////////////////
//Copyright (C), 2011-2011, 
//�ļ��� : WTcpSvrSocket.h
//��  �� : ����Tcp����socket��
//��  ע : 
//��  ʷ : V1.0.0.0  2011-3-7 ����
//		   
/////////////////////////////////////////////////////////////////

#pragma once
#include "WSocket.h"
#include <boost/thread.hpp>
class CWTcpSocketDeal
{
public:
	//��  �� : �׽��ֽ���
	//��  �� : s: IN  �׽���
	//         uIp: IN IP��ַ
	//         uPort: IN �˿�
	//��  �� : 
	virtual void OnAccept(SOCKET s, u_long uIp, u_short uPort) = 0;
};
//////////////////////////////////////////////////////////
//��  �� : CWTcpSvrSocket
//��  �� : Tcp����socket��
//��  ע : 
//��  ʷ :  3-7-2011 ����
//
//////////////////////////////////////////////////////////
class CWTcpSvrSocket : public CWSocket
{
public:
	CWTcpSvrSocket();
	virtual ~CWTcpSvrSocket();
	//��  �� : ��������
	//��  �� : True,�ɹ�; False, ʧ��
	bool  Create(u_short nSocketPort,
		CWTcpSocketDeal* pObj,
		u_long uIp,
		int nBackLogin = 5);

	//����
	bool  Destroy();

private:
	//��  �� : 
	//��  �� : s: OUT  ��;
	//         uIp: OUT ��;
	//         uPort: OUT ��;
	//��  �� : True,�ɹ�; False, ʧ��
	bool  GetConnect(SOCKET& s, 
		u_long& uIp,
		u_short& uPort);

private:
	//��  �� : �������Ӵ�����
	//��  �� : �����߳�
	//��  �� : 
	void ThServerMonitor();
	//Socket���ն���
	CWTcpSocketDeal* m_pObj;
	//tcp�����߳�
	boost::thread m_thMonitor;
	bool volatile m_bFlag;
};