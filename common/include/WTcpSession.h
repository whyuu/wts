////////////////////////////////////////////////////////////
//Copyright (C), 2011-2011, 
//�ļ��� : WTcpClient.h
//��  �� : ����WTcpClient��
//��  ע : 
//��  ʷ : V1.0.0.0 ������ 2011-3-9 ����
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

	//��  �� : �����׽���
	//��  �� : s: IN  �׽���
	//         uRemoteIp: IN Ŀ��IP
	//         uPort: IN Ŀ�Ķ˿�
	//		 bStartRecvThread: �Ƿ����������߳�
	//��  �� : 
 virtual bool Attach(SOCKET s,
   u_long uRemoteIp,
   u_short uRemotePort,
   bool bStartRecvThread = true);
	//��  �� : ���Ӷ˿�
	//��  �� : uRemoteIp: IN  ����IP
	//         uRemotePort: IN �˿�
	//		   bRunConnect: �Ƿ�����ִ�����Ӳ���
	//		   bAutoReconnect: �����Ƿ��Զ�����
	//		   bStartRecvThread: �Ƿ����������̡߳�
	//		   
	//��  �� :
	bool Connect(u_long uRemoteIp, 
		u_short uRemotePort,
		bool bRunConnect,			//�Ƿ���������
		bool bAutoReconnect = true,  //�Ƿ��Զ�����
		bool bStartRecvThread  = true); //�Ƿ��������߳�

 //������������
 void SetConnectConfig(u_long uRemoteIp, 
   u_short uRemotePort);
	//��  �� : ��������
	//��  �� : pBuffer: IN  ���뻺��
	//         nSize: IN �����С
	//��  �� : 
	int SendData(const char* pBuffer, int nSize);

	//��  �� : �Ͽ� 
	//bStopThread : �Ƿ�����ֹͣ�߳�
	//��  �� : 
	void DisConnect(bool bStopThread = true);

	//��  �� : ��ȡIP
	//��  �� : ����IP
	u_long GetRemoteIp(){return m_uRemoteIp;}
	//��  �� : ��ȡ�˿�
	//��  �� : �˿�
	u_short GetRemotePort(){return m_uRemotePort;}
 
	//��  �� : ��ȡ�׽��־��
	//��  �� : 
	SOCKET GetSocket(){return m_Socet.GetSocket();}

	bool IsConnect(){return m_bConnect;}

	void SetAutoReconnect(bool bAutoReconnect){m_bAutoReconnect = bAutoReconnect;}

	//��  �� : ��������, ��δ���������߳�ʱ�����ã����������̺߳���Ч
	//��  �� : pBuffer: IN  ���뻺��
	//         nSize: IN �����С
	//��  �� : 
	int RecvData(char* pBuffer, int nSize);
	//�Ƿ�����߳��˳�
	bool GetFlag(){return m_bFlag;}
	
protected:
	virtual int OnRecvData(char* pBuffer, int nSize) = 0;//{return nSize;}
	//������ :false  �˳��� true�����˳�
	virtual bool OnMsg(int nErrorCode) = 0;//{return m_bAutoReconnect;}
	//�����̳߳ش�С
	static void SetThreadPoolSize(int nSize);

	//�����׽���
	CWTcpSocket m_Socet;

private:
	//enum{ BUFFER_SIZE = 1024 * 1024};
	//����
	bool Connect();
	//��  �� : ��socket�߳�
	//��  �� : param1:  
	//         param2:  
	//��  �� : 0:�ɹ�
	//         ����:������
	void OnRead();
	//����IP
	u_long m_uRemoteIp;

	//�����˿�
	u_short m_uRemotePort;
	
	//�̺߳���
	boost::thread m_thThread;
	bool volatile m_bFlag;

	//���ݻ���
	char* m_pBuffer;

	//������صĺ���
	bool m_bAutoReconnect; //�Ƿ��Զ�����
	bool m_bConnect; //�Ƿ�����
	int m_nReadBufSize; //�����С
};