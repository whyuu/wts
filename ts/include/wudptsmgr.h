#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include "wts.h"
#include "WQueue.h"

struct UdpInfo
{
  int nId;
  std::string strIp;
  uint16_t uPort;
};

class WUdpTs;
//����udp���յ�ts�б�
class CWUdpTsMgr
{
public:
  //������
  virtual int AddTs(const std::string& strIp, uint16_t uPort);
  void DelTs(int nId);

  //����Psisi����flag
  void SetPsisiFlag(int nFlag);
  //ˢ��Psisi
  void RefreshPsisi(int nId);

  //��ȡ��Ŀ�б�
  //param1: ������ʶid
  //arrProg����Ŀ�б�
  void GetProgram(int nId, std::vector<Program>& arrProg);

  //����
protected:
  std::map<int, WUdpTs*> m_arrTs;//map�ֶ��е�һ��Ϊsocket���
};