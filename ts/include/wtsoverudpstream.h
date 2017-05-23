#pragma once
#include <stdint.h>
#include <string>
#include "wts.h"

class TsOverUdpStream
{
public:
  virtual ~TsOverUdpStream();
  int nId;
  std::string strIp;
  uint16_t uPort;
  int nSocket;
  bool bPsisiFinish;
  WTs ts;
};