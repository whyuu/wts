#pragma once
#include "wpsisi.h"
class  WSdt : public WPsiSi
{
public:
  //获取节目信息列表
  //int GetServiceList(std::vector<std::pair<uint16_t, uint16_t>>& arrProgList);
  //获取原始网络id
  uint16_t GetOriginalNetworkId();
};