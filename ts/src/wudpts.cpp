#pragma once
#include "wudpts.h"
#include "WUdpSocket.h"

int WUdpTs::Create(const std::string& strIp, uint16_t uPort)
{
  CWUdpSocket tmp;
  tmp.Create();
  return tmp.GetSocket();
}

void WUdpTs::OnPes(uint16_t uPid, const WPes* wPes, uint16_t lparam)
{

}