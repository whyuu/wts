#include "wudptsmgr.h"

int main()
{
  CWUdpTsMgr obj;
  UdpInfo uobj = {0,"127.0.0.1", 1234};
  obj.AddTs(uobj);
  uobj.uPort = 1235;
  uobj.nId = 2;
  obj.AddTs(uobj);
  return 0;
}