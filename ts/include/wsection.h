#pragma once
#include <stdint.h>

class WSection
{
public:
  WSection(int nSize = 1024);
  ~WSection();
  void Reset();

  virtual bool Append(char* pData, int nSize);
  uint8_t GetSectionSyntaxIndicator() const;
  uint8_t GetSectionLength() const;
  uint8_t GetVersionNumber() const;
  uint8_t GetCurrentNextIndicator() const;
  uint8_t GetSectionNumber() const;
  uint8_t GetLastSectionNumber() const;
  uint32_t GetCrc() const;
  const char* GetSectionData() const;
  int GetSectionSize() const;
  int SetSectionData(const char* pData, int nSize);

private:
  char* m_pSection; //Section缓存
  int m_nSize; //数据大小
};
