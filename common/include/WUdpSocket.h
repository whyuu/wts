#pragma once
#include "WSocket.h"

//////////////////////////////////////////////////////////
//��  �� : CWUdpSocket
//��  �� : ������Udpͨ����صĽӿ�
//��  ע : 
//��  ʷ : ������ 3-7-2011 ����
//
//////////////////////////////////////////////////////////
class CWUdpSocket : public CWSocket
{
public:
	CWUdpSocket();
	virtual ~CWUdpSocket();

	//��  �� : ��������
	//��	  �� ��
	//��  �� : TRUE:�ɹ�;
	//		   FALSE: ʧ��
	bool Create();

	//��  �� : �󶨶˿�
	//��  �� : nSocketPort: IN  Udp�˿�
	//         uIp: IN Ip��ַ
	//��  �� : TRUE:�ɹ�;
	//		   FALSE: ʧ�� 
	bool Bind(u_short nSocketPort, u_long uIp = 0);

	//��  �� : ��������
	//��  �� : pBuffer: IN  ���ջ���
	//         nMaxSize: In ���ֵ
	//		   Ip: OUT  �Է�Ip��ַ
	//         Port: OUT �Է����Ͷ˿�
	//��  �� : SOCKET_ERROR������
	int Receive(char* pBuffer, 
				int nMaxSize, 
				u_long& uIp, 
				u_short& usPort);

 //��  �� : ��������
 //��  �� : pBuffer: IN  ���ջ���
 //         nMaxSize: In ���ֵ
 //��  �� : SOCKET_ERROR������
 int Receive(char* pBuffer, 
   int nMaxSize);
	
	//��  �� : ��������
	//��  �� : pBuffer: IN  ���ջ���
	//         nMaxSize: In ���ֵ
	//		   remoteIp: In	Ŀ��Ip��ַ
	//         remotePort: In Ŀ�Ķ˿�
	//��  �� : SOCKET_ERROR������
	int Send(const char* pBuffer, 
			 int nMaxSize, 
			 u_long uIp, 
			 u_short usPort);

	//��  �� : ���öಥ���͵�ַ
	//��  �� : uIp: IN �ಥ����IP
	//         uSelectIp: IN �ಥ����ѡ�������Ip
	//��  �� : SOCKET_ERROR������
	int SetMultiAddr(u_long uIp, u_long uSelectIp);
};