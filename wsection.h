#pragma once
#include "WType.h"

class WSection
{
public:
  WSection(int nSize = 1024);
  ~WSection();
  void Reset();
  bool Append(char* pData, int nSize);
  int GetSectionData(char* pData ,int nSize);
  int SetSectionData(const char* pData, int nSize);
  //todo other
private:
  char m_Section[1024]; //Section缓存
  int m_nSize; //数据大小
};
