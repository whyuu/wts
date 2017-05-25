#pragma once
#include <stdint.h>
#include <string>
#include "wts.h"

class WUdpTs : public WTs
{
public:
  virtual ~WUdpTs(){}
  int Create(const std::string& strIp, uint16_t uPort);

protected:
  void OnPes(uint16_t uPid, const WPes* wPes, uint16_t lparam);

  int nId;
  std::string strIp;
  uint16_t uPort;
  int nSocket;
  bool bPsisiFinish;
};