#include "wudptsmgr.h"
#include "wudpts.h"
using namespace std;
//流管理
int CWUdpTsMgr::AddTs(const std::string& strIp, uint16_t uPort)
{
  WUdpTs* pObj = new WUdpTs();
  int nSocket = pObj->Create(strIp, uPort);
  m_arrTs.insert(pair<int, WUdpTs*>(nSocket, pObj));
  return nSocket;
}

void CWUdpTsMgr::DelTs(int nId)
{
}

//设置Psisi解析flag
void CWUdpTsMgr::SetPsisiFlag(int nFlag)
{
}

//刷新Psisi
void CWUdpTsMgr::RefreshPsisi(int nId)
{
}

//获取节目列表
//param1: 参数标识id
//arrProg，节目列表
void CWUdpTsMgr::GetProgram(int nId, std::vector<Program>& arrProg)
{
}