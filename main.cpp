#include "wudptsmgr.h"

int main()
{
  CWUdpTsMgr obj;
  obj.AddTs("127.0.0.1", 1234);
  obj.AddTs("127.0.0.1", 1235);
  return 0;
}