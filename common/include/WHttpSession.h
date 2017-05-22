#pragma once
#include <string.h>
#include "WTcpSession.h"
#include "HttpRequest.h"
#include "WnLock.h"

class CWHttpSession : public virtual CWTcpSession
{
public:
  CWHttpSession();
  virtual ~CWHttpSession();
  //·¢ËÍÊý¾Ý
  int SendData(const string& strRes, bool bCompress = false);

protected:
  virtual int DealHttpRequest(IN const string& strReq, 
    IN const string& url,
    IN OUT bool& bCompress,
    OUT string& strRes) = 0;
  virtual int OnRecvData(char* pBuffer, int nSize);

private:
  bool ProcessRequest(const string& strLine);
  void ProcessHeader(string& strLine);
  void Construct(string& dst, const string& body, bool bCompress);

  HttpRequest m_req;
  string m_rcvSend;
  string m_res;

  enum REQSTATUS
  {
    REQ_REQUEST=0, REQ_HEADER, REQ_BODY, REQ_SIMPLE, REQ_DONE
  };
  REQSTATUS m_reqStatus;
  CWnLock m_SndLock;
};