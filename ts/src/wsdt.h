#pragma once
#include "wpsisi.h"
class  WSdt : public WPsiSi
{
public:
  //��ȡ��Ŀ��Ϣ�б�
  //int GetServiceList(std::vector<std::pair<uint16_t, uint16_t>>& arrProgList);
  //��ȡԭʼ����id
  uint16_t GetOriginalNetworkId();
};