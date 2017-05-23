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
//管理udp接收的ts列表
class CWTsOverUdpMgr : public WTsEvent
{
public:
  //流管理
  virtual bool AddTs(UdpStream ts);
  void DelTs(int nId);
  void GetTsList(std::vector<TsOverUdpStream>& arrStream);

  //设置Psisi解析flag
  void SetPsisiFlag(int nFlag);
  //刷新Psisi
  void RefreshPsisi(int nId);

  //获取节目列表
  //param1: 参数标识id
  //arrProg，节目列表
  void GetProgram(int nId, std::vector<Program>& arrProg);

  //其他
protected:
  void OnPsiSiFinish(uint64_t lparam)
  {
    //TDOO
  }

  std::map<int, TsOverUdpStream*> m_arrStream;//map字段中第一个为socket句柄
};