#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include "WSocketType.h"
#include "wts.h"
#include "WQueue.h"
#include "SocketMonitor.h"
#include "WnScopedLock.h"

struct UdpInfo
{
  int nId;
  std::string strIp;
  uint16_t uPort;
};

class WUdpTs;
//����udp���յ�ts�б�
class CWUdpTsMgr : public ISocket
{
public:
  int Init();

  //������
  //��ȷ����Ψһ��ʶ�����󣬷���0
  virtual uint64_t AddTs(const std::string& strIp, uint16_t uPort);
  void DelTs(uint64_t nId);

  //����Psisi����flag
  void SetPsisiFlag(uint64_t nFlag);
  //ˢ��Psisi
  void RefreshPsisi(uint64_t nId);

  //��ȡ��Ŀ�б�
  //param1: ������ʶid
  //arrProg����Ŀ�б�
  void GetProgram(uint64_t nId, std::vector<Program>& arrProg);

  //����
protected:
  void OnRead(int nModuleId, SOCKET nSocket);

  std::map<SOCKET, WUdpTs*> m_arrTs;//map�ֶ��е�һ��Ϊsocket���
  CSocketMonitor m_SocketMonitor;

  CWnLock m_Lock;
};