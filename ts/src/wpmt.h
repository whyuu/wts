#pragma once
#include "wpsisi.h"
class  WPmt : public WPsiSi
{
public:
  //��ȡ��Ŀ�б�
  int GetProgramInfoLength();
  //int GetProgramInfo();
  //��ȡ������id
  uint16_t GetPcrPid();
};