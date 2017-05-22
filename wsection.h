#pragma once
#include <stdint.h>

class WSection
{
public:
  WSection();
  ~WSection();
  void Reset();

  virtual bool Append(char* pData, int nSize);
  uint8_t GetSectionSyntaxIndicator();
  uint8_t GetSectionLength();
  uint8_t GetVersionNumber();
  uint8_t GetCurrentNextIndicator();
  uint8_t GetSectionNumber();
  uint8_t GetLastSectionNumber();
  uint32_t GetCrc();
  const char* GetSectionData();
  int GetSectionSize();
  int SetSectionData(const char* pData, int nSize);

private:
  char m_section[1024]; //Section缓存
  char* m_pSelfSection; //EIT表时使用这个字段
  int m_nSize; //数据大小
};
