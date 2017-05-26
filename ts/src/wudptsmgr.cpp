#include "wudptsmgr.h"
#include "wudpts.h"
using namespace std;

int CWUdpTsMgr::Init()
{
  m_SocketMonitor.Start(0, this);
  return 0;
}

//流管理
uint64_t CWUdpTsMgr::AddTs(const std::string& strIp, uint16_t uPort)
{
  CWnScopedLock Lock(m_Lock);
  WUdpTs* pObj = new WUdpTs();
  SOCKET nSocket = pObj->Create(strIp, uPort);
  if (nSocket == INVALID_SOCKET)
  {
    delete pObj;
    return 0;
  }
  m_arrTs.insert(pair<SOCKET, WUdpTs*>(nSocket, pObj));
  m_SocketMonitor.AddSocket(nSocket);
  return nSocket;
}

void CWUdpTsMgr::DelTs(uint64_t nId)
{
  CWnScopedLock Lock(m_Lock);
  map<SOCKET, WUdpTs*>::iterator it = m_arrTs.find((SOCKET)nId);
  if (it != m_arrTs.end())
  {
    it->second->Destroy();
    m_arrTs.erase(it);
  }
}

//设置Psisi解析flag
void CWUdpTsMgr::SetPsisiFlag(uint64_t nFlag)
{
}

//刷新Psisi
void CWUdpTsMgr::RefreshPsisi(uint64_t nId)
{
}

//获取节目列表
//param1: 参数标识id
//arrProg，节目列表
void CWUdpTsMgr::GetProgram(uint64_t nId, std::vector<Program>& arrProg)
{
}

void CWUdpTsMgr::OnRead(int nModuleId, SOCKET nSocket)
{
  CWnScopedLock Lock(m_Lock);
  map<SOCKET, WUdpTs*>::iterator it = m_arrTs.find((int)nSocket);
  if (it != m_arrTs.end())
  {
    it->second->Read();
  }
}