#pragma once
#include "WType.h"
class WTsPk
{
public:
  void Set(const char data[188]);
  void Get(char data[188]);
  WORD GetPid();
  void SetPid(WORD wPid);
  BYTE* GetPayLoad();
  int GetPayLoadSize();
  void SetPayLoad(BYTE* pData, int nSize);
private:
  BYTE m_ts[188];
};