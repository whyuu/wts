/*****************************************************
 *Copyright(c) 2016-2017 北京数码视讯科技股份有限公司
 *All right reserved
 *
 *文件名称：ProcessComm.h
 *简要描述：
 *
 *创建日期：2016-10-24
 *作者：王辉永(why) 
 *说明：进程间通信类，充当客户端的进程是请求指令方
 *
 *修改：
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
  *功能描述：处理接收数据，DealRequest处理函数要快，延迟会影响系统速度，
             对于需要处理时间的指令，开线程单独处理。
  *参数1: 接收的数据
  *参数2: 是否压缩
  *返回值: 
  *创建日期： 2016-10-24
  *作者:	   why
  *修改：
  ***************************************/
  virtual int OnRcvProcessCmd(unsigned int nId, const string& strData,
    bool bCompress) = 0;
};

class CProcessComm : CWTcpServer, public CWSvrTcpSession
{
public:
  CProcessComm();
  virtual ~CProcessComm();
  //初始化
  //url 标识不同进程依靠url的不同。
  //isReq 请求还是响应
  int Init(int nPort, bool isReq, CProcessCmdDealer* pDealer);
  void Destroy();
  //发送数据
  int SendData(unsigned int nId, bool bCompress, const string& strRes);

private:
  int OnRecvData(char* pBuffer, int nSize);
  //错误处理 :false  退出， true，不退出
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