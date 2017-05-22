#pragma once
#include "wpsisi.h"
class  WPat : public WPsiSi
{
public:
  //获取节目列表
  int GetProgList(std::vector<std::pair<uint16_t, uint16_t>>& arrProgList);
  //获取传送流id
  uint16_t GetTransportStreamId();
};