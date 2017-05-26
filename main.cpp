#include "wudptsmgr.h"

int main()
{
#if defined(WIN32) || defined(_WINDLL)
  WSADATA wsaData;
  UINT16 wVersionRequested = MAKEWORD(2, 0);
  unsigned long  err = WSAStartup(wVersionRequested, &wsaData);
  if(err != 0)
  {
    return -1;
  }
#endif
  CWUdpTsMgr obj;
  obj.Init();
  obj.AddTs("127.0.0.1", 1234);
  while (true) Sleep(1000);
  return 0;
}