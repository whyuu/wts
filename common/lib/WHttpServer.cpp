#include "WHttpServer.h"
#include "Public.h"
#include "WnScopedLock.h"
#include "WLog.h"
#include "boost/format.hpp"
#define CHECK_JSON(a) if (a.isNull())return false
#define CHECK_JSON_BREAK(a) if (a.isNull()) {res["result"] = ERR_TASK_PARAM;break;}
#define CHECK_JSON_OBJECT(a) if (!a.isObject())return false

CWSvrHttpSession::CWSvrHttpSession(CWHttpServer* pS) : m_pServer(pS)
{
}
CWSvrHttpSession::~CWSvrHttpSession()
{
}

int CWSvrHttpSession::DealHttpRequest(IN const string& strReq, 
  IN const string& url,
  IN OUT bool& bCompress,
  OUT string& strRes)
{
//   if (strReq.size() <= 0)
//   {
//     strRes = "request is empty";
//     return 0;
//   }
  try
  {
    m_pServer->DealCmd(url, strReq, strRes, bCompress, GetId());
  }
  catch (...)
  {
    strRes = "Command error!";
  }
  return 0;
}
bool CWSvrHttpSession::OnMsg(int nErrorCode)
{
  CWnScopedLock Lock(m_Lock);
  m_pServer->ReleaseSession(this);
  return false;
}
int CWSvrHttpSession::SendData(UINT32 uSId, const string& strRes, bool bCompress)
{
  CWnScopedLock Lock(m_Lock);
  if (uSId != m_Obj.nSId)
  {
    return -1;
  }
  return CWHttpSession::SendData(strRes, bCompress);
}
CWHttpServer::CWHttpServer()
{
}
CWHttpServer::~CWHttpServer(void)
{
}
CWSvrHttpSession* CWHttpServer::CreateSession()
{
  CWSvrHttpSession* pS = new CWSvrHttpSession(this);
  return pS;
}
bool CWHttpServer::Start(UINT16 nPort,
  int nMaxConnectNum,
  const string& strIp,
  int nBackLogin)
{
  UINT32 uIp = 0;
  if (!strIp.empty())
  {
    uIp = inet_addr(strIp.c_str());
  }
  bool bRes = CWTcpServer::Start(nPort, nMaxConnectNum, uIp, nBackLogin);
  if (!bRes)
  {
    return bRes;
  }
  return bRes;
}
void CWHttpServer::Stop()
{
  CWTcpServer::Stop();
}
//发送数据
int CWHttpServer::SendCmd(int nSessionId, const string& strData, bool bCompress)
{//可以优化

  CWSvrHttpSession* pS = (CWSvrHttpSession*)GetSession(nSessionId);
  if (pS != NULL)
  {
    TcpSessionObj sObj;
    sObj.uSessionId = nSessionId;
    return pS->SendData(sObj.nSId, strData, bCompress);
  }
  return -1;
}
