/*****************************************************
 *Copyright(c) 2016-2017 ����������Ѷ�Ƽ��ɷ����޹�˾
 *All right reserved
 *
 *�ļ����ƣ�ProcessComm.h
 *��Ҫ������
 *
 *�������ڣ�2016-10-24
 *���ߣ�������(why) 
 *˵�������̼�ͨ���࣬�䵱�ͻ��˵Ľ���������ָ�
 *
 *�޸ģ�
********************************************************/
#pragma once
#include <string>
#include <vector>
#include <boost/thread.hpp>
#include "WTcpServer.h"
#include "WnScopedLock.h"
#include "WnEvent.h"
#include "MemStream.h"
using namespace std;

class CProcessCmdDealer
{
public:
  /*************************************  
  *��������������������ݣ�DealRequest������Ҫ�죬�ӳٻ�Ӱ��ϵͳ�ٶȣ�
             ������Ҫ����ʱ���ָ����̵߳�������
  *����1: ���յ�����
  *����2: �Ƿ�ѹ��
  *����ֵ: 
  *�������ڣ� 2016-10-24
  *����:	   why
  *�޸ģ�
  ***************************************/
  virtual int OnRcvProcessCmd(unsigned int nId, const string& strData,
    bool bCompress) = 0;
};

class CProcessComm : CWTcpServer, public CWSvrTcpSession
{
public:
  CProcessComm();
  virtual ~CProcessComm();
  //��ʼ��
  //url ��ʶ��ͬ��������url�Ĳ�ͬ��
  //isReq ��������Ӧ
  int Init(int nPort, bool isReq, CProcessCmdDealer* pDealer);
  void Destroy();
  //��������
  int SendData(unsigned int nId, bool bCompress, const string& strRes);

private:
  int OnRecvData(char* pBuffer, int nSize);
  //������ :false  �˳��� true�����˳�
  bool OnMsg(int nErrorCode);
  CWSvrTcpSession* CreateSession(){return this;}
  void DestroySession(CWSvrTcpSession* pS){};
  void OnSend();
  void GetCmd(CMemStream& dat);
  CMemStream m_SendData;
  CWnLock m_lock;
  CWnEvent m_event;
  bool m_bFlag;

  bool m_isReq;
  CProcessCmdDealer* m_pDealer;
  boost::thread m_thread;
};