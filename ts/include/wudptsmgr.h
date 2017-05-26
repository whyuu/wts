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
//管理udp接收的ts列表
class CWUdpTsMgr : public ISocket
{
public:
  int Init();

  //流管理
  //正确返回唯一标识，错误，返回0
  virtual uint64_t AddTs(const std::string& strIp, uint16_t uPort);
  void DelTs(uint64_t nId);

  //设置Psisi解析flag
  void SetPsisiFlag(uint64_t nFlag);
  //刷新Psisi
  void RefreshPsisi(uint64_t nId);

  //获取节目列表
  //param1: 参数标识id
  //arrProg，节目列表
  void GetProgram(uint64_t nId, std::vector<Program>& arrProg);

  //其他
protected:
  void OnRead(int nModuleId, SOCKET nSocket);

  std::map<SOCKET, WUdpTs*> m_arrTs;//map字段中第一个为socket句柄
  CSocketMonitor m_SocketMonitor;

  CWnLock m_Lock;
};