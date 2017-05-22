////////////////////////////////////////////////////////////
//Copyright (C), 2010-2010, 
//�ļ��� : WTcpSocket.cpp
//ͷ�ļ� : WTcpSocket.h
//��  ע : 
//		   
//��  ʷ : V1.0.0.0 ������ 2011-3-7 ����
//
/////////////////////////////////////////////////////////////////
#include "WTcpSocket.h"

CWTcpSocket::CWTcpSocket()
{
}

CWTcpSocket::~CWTcpSocket()
{
}

//��  �� : ��������
//��  �� : True,�ɹ�; False, ʧ��
bool  CWTcpSocket::Create()
{
  m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
  if (m_hSocket != INVALID_SOCKET)
  {
    return true;
  }
  return false;
}

 void CWTcpSocket::Destroy()
 {
   if (m_hSocket != INVALID_SOCKET)
   {
     shutdown(m_hSocket, SD_BOTH);
     CWSocket::Destroy();
   }
 }

//��  �� : ��ʼ������
//��  �� : uHost: IN  Ҫ���ӵ�����
//         uPort: OUT ���ӵĶ˿�
//         param3: IN/OUT ��;
//��  �� : 
bool  CWTcpSocket::Connect(u_long uHost, u_short uPort)
{
//   sockaddr_in sockAddr;
//   int nlen = (int)sizeof(sockAddr);
//   getsockname(m_hSocket, (sockaddr*)&sockAddr, &nlen);
//   int nPort = ntohs(sockAddr.sin_port);

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = uHost;
	server.sin_port = htons(uPort);
	if (SOCKET_ERROR == connect(m_hSocket, (struct sockaddr *)&server, sizeof(struct sockaddr)))
	{
		return false;
	}

//  getsockname(m_hSocket, (sockaddr*)&sockAddr, &nlen);
//  nPort = ntohs(sockAddr.sin_port);

	return true;
}

bool  CWTcpSocket::Connect(const char* pszIp, u_short uPort)
{
  return Connect(inet_addr(pszIp), uPort);
}

void  CWTcpSocket::DisConnect()
{
  shutdown(m_hSocket, SD_BOTH);
}
//��  �� : ��������
//��  �� : pBuffer: IN  ���ջ���
//         nMaxSize: In ���ֵ
//��  �� : 
// int CWTcpSocket::Receive(char* pBuffer, 
// 			int nMaxSize)
// {
// 	return recv(m_hSocket, pBuffer, nMaxSize, 0);
// }
// 
// 	//��  �� : ��������
// 	//��  �� : pBuffer: IN  ��������
// 	//         nBufSize: In ���ݳ���
// 	//��  �� : 
// int CWTcpSocket::Send(const char* pBuffer, 
// 		 int nBufSize)
// {
// 	return send(m_hSocket, pBuffer, nBufSize, 0);
// }
