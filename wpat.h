#pragma once
#include "wpsisi.h"
class  WPat : public WPsiSi
{
public:
  //��ȡ��Ŀ�б�
  int GetProgList(std::vector<std::pair<uint16_t, uint16_t>>& arrProgList);
  //��ȡ������id
  uint16_t GetTransportStreamId();
};