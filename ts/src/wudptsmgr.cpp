#include "wudptsmgr.h"
#include "wudpts.h"
using namespace std;
//������
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

//����Psisi����flag
void CWUdpTsMgr::SetPsisiFlag(int nFlag)
{
}

//ˢ��Psisi
void CWUdpTsMgr::RefreshPsisi(int nId)
{
}

//��ȡ��Ŀ�б�
//param1: ������ʶid
//arrProg����Ŀ�б�
void CWUdpTsMgr::GetProgram(int nId, std::vector<Program>& arrProg)
{
}