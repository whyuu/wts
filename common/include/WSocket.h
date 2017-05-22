////////////////////////////////////////////////////////////
//Copyright (C), 2011-2011, 
//�ļ��� : WSocket.h
//��  �� : ����Socket�࣬����⵽Socket�ɶ�ʱ�����쳣ʱ��ִ��Sokcet���������غ���
//��  ʷ : V1.0  2011-3-8 ����
//		   V1.1  2014-10-19 ����linux
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
	
	//��  �� : ����֪�׽��ֳ�ʼ������
	//��  �� : s: �׽��ֱ�ʶ
	//��  �� : 
	void Attach(SOCKET s){m_hSocket = s;}

	//��  �� : ���ٶ���
	//��  �� ��
	//��  �� : 
	void Destroy();

	//��  �� : ��������״̬��soket
	//��  �� : time ���ӹرպ�ȴ�ʱ�䣬time = 0ʱ�ڵ���closesocket��ǿ�ƹرգ�������TIME_WAIT�Ĺ���
	//��  �� : TRUE:�ɹ�
	//		   FALSE:ʧ��
	bool SetLinger(int time);

	//��  �� : ���õ�ַ����
	//��  �� : bFlag: IN  ���ñ�־
	//��  �� : TRUE��������
	//		   FALSE����������
	bool SetReuse(int bFlag);

	//��  �� : ���ö˿ڱ�������
	//��  �� : bFlag: IN  FALSE �رգ�TRUE ��
	//��  �� : TRUE:�ɹ�
	//		   FALSE:ʧ��
	bool KeepAlive(int bFlag, int nOutTime = 5000, int nInt = 1000, int nFreq = 3);

	//��  �� : ���öಥ���շ���־
	//��  �� : bFlag: IN  �ಥ�Է����ձ�־
	//��  �� : TRUE���ɹ�
	//		   FALSE��ʧ��
	bool SetMultiLoop(int bFlag);

	//��  �� : ���ý��ջ�����
	//��  �� : nSize: IN ��������С
	//��  �� : TRUE:�ɹ�
	//		   FALSEʧ��
	bool SetReceiveBuf(int nSize);

	//��  �� : ��ȡ���ջ�������С
	//��  �� : nSize: OUT ��������С
	//��  �� : TRUE���ɹ�
	//		   FALSE��ʧ��
	bool GetReceiveBuf(int& nSize);

	//��  �� : ���÷��ͻ�����
	//��  �� : nSize: IN ��������С
	//��  �� : TRUE���ɹ�
	//		   FALSE��ʧ��
	bool SetSendBuf(int nSize);

	//��  �� : ��ȡ���ͻ�������С
	//��  �� : nSize: IN ��������С
	//��  �� : TRUE���ɹ�
	//		   FALSE��ʧ��
	bool GetSendBuf(int& nSize);

	//��  �� : ����Socket������
	//��  �� : bFlag: IN  ��������־�� TURE����������FALSE:����
	//��  �� : TRUE���ɹ�
	//		   FALSE��ʧ�� 
	bool SetNoBlock(bool bFlag);

	//��  �� : ���ý��ճ�ʱ(linuxʹ��)
	//��  �� : tv: ��ʱʱ����
	//��  �� : TRUE���ɹ�
	//		   FALSE��ʧ�� 
	bool SetRecvTimeOut(timeval& tv);

	//��  �� : ���÷��ͳ�ʱ(linuxʹ��)
	//��  �� : tv: ��ʱʱ����
	//��  �� : TRUE���ɹ�
	//		   FALSE��ʧ�� 
	bool SetSendTimeOut(timeval& tv);

	//��  �� : ���ý��ճ�ʱ
	//��  �� : tv: ��ʱʱ����
	//��  �� : TRUE���ɹ�
	//		   FALSE��ʧ�� 
	bool SetRecvTimeOut(int tv);

	//��  �� : ��ȡ���ճ�ʱ
	//��  �� : tv: ��ʱʱ����
	//��  �� : TRUE���ɹ�
	//		   FALSE��ʧ�� 
	bool GetRecvTimeOut(int& tv);

	//��  �� : ���ó�ʱ��⣬��û�����ݷ���ʱ�������·�Ƿ��ж�
	//��  �� : tv: ��ʱʱ����
	//��  �� : TRUE���ɹ�
	//		   FALSE��ʧ�� 
	bool SetOutTimeMonitor(int nInterval, int KeepLiveTime);
protected:
	SOCKET m_hSocket;
};