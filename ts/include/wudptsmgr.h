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
//管理udp接收的ts列表
class CWUdpTsMgr
{
public:
  //流管理
  virtual int AddTs(const std::string& strIp, uint16_t uPort);
  void DelTs(int nId);

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
  std::map<int, WUdpTs*> m_arrTs;//map字段中第一个为socket句柄
};