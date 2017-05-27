#pragma once
#include <stdint.h>

class WSection
{
public:
  WSection(int nSize = 1024);
  ~WSection();
  void Reset();
  int SetHead(const char* pData, int nSize);
  int Append(const char* pData, int nSize);
  bool IsFinish();
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
  char* m_pSection; //Section����
  int m_nSize; //���ݴ�С
  int m_nTotalSectionSize;
  int m_nMaxSize;//���ֵ
};
