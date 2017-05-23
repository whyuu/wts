#pragma once
#include "wpsisi.h"
class  WPmt : public WPsiSi
{
public:
  //获取节目列表
  int GetProgramInfoLength();
  //int GetProgramInfo();
  //获取传送流id
  uint16_t GetPcrPid();
};