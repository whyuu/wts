#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include "wts.h"
#include "WQueue.h"

struct UdpStream
{
  int nId;
  std::string strIp;
  uint16_t uPort;
};

class TsOverUdpStream;
//����udp���յ�ts�б�
class CWTsOverUdpMgr : public WTsEvent
{
public:
  //������
  virtual bool AddTs(UdpStream ts);
  void DelTs(int nId);
  void GetTsList(std::vector<TsOverUdpStream>& arrStream);

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
  void OnPsiSiFinish(uint64_t lparam)
  {
    //TDOO
  }

  std::map<int, TsOverUdpStream*> m_arrStream;//map�ֶ��е�һ��Ϊsocket���
};