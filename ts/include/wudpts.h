#pragma once
#include <stdint.h>
#include <string>
#include "WSocketType.h"
#include "WUdpSocket.h"
#include "wts.h"
#define BUFSIZE 188 * 7

class WUdpTs : public WTs
{
public:
  virtual ~WUdpTs(){}
  uint64_t Create(const std::string& strIp, uint16_t uPort);
  void Destroy();
  void Read();
protected:
  void OnPes(uint16_t uPid, const WPes* wPes, uint16_t lparam);

  std::string m_strIp;
  uint16_t m_uPort;
  CWUdpSocket m_Socket;
  bool m_bPsisiFinish;
  char m_buf[BUFSIZE];
};