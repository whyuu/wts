////////////////////////////////////////////////////////////
//Copyright (C), 2011-2011, ����������Ѷ�Ƽ��ɷ����޹�˾
//�ļ��� : WTcpSocket.h
//��  �� : ����Tcpͨ�ŵĽӿ�
//��  ע : 
//��  ʷ : V1.0.0.0 ������ 2011-3-7 ����
//		   
/////////////////////////////////////////////////////////////////
#pragma once
#include "WSocket.h"
//////////////////////////////////////////////////////////
//��  �� : CWTcpSocket
//��  �� : ����Tcpͨ�ŵĽӿ�
//��  ע : 
//��  ʷ : ������ 3-7-2011 ����
//
//////////////////////////////////////////////////////////
class CWTcpSocket : public CWSocket
{
public:
	CWTcpSocket();
 CWTcpSocket(SOCKET s){m_hSocket = s;}
	virtual ~CWTcpSocket();
	//��  �� : ��������
	//��  �� : True,�ɹ�; False, ʧ��
	bool Create();
	void Destroy();

	void DisConnect();
	//��  �� : ��ʼ������
	//��  �� : uHost: IN  Ҫ���ӵ�����
	//         nPort: OUT ���ӵĶ˿�
	//         param3: IN/OUT ��;
	//��  �� : 
	bool  Connect(u_long uHost, u_short uPort);
	bool  Connect(const char* pszIp, u_short uPort);

	//��  �� : ��������
	//��  �� : pBuffer: IN  ���ջ���
	//         nMaxSize: In ���ֵ
	//��  �� : 
	int Receive(char* pBuffer, 
		int nMaxSize)
	{
		return recv(m_hSocket, pBuffer, nMaxSize, 0);
	}
	
	//��  �� : ��������
	//��  �� : pBuffer: IN  ��������
	//         nBufSize: In ���ݳ���
	//��  �� : 
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