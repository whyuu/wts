#pragma once
#include <stdint.h>
class WTsPk
{
public:
  void Set(const char data[188]);
  void Get(char data[188]);
  uint16_t GetPid();
  void SetPid(uint16_t wPid);
  uint8_t* GetPayLoad();
  int GetPayLoadSize();
  void SetPayLoad(uint8_t* pData, int nSize);
private:
  uint8_t m_ts[188];
};