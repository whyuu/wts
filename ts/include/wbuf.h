#pragma once
class WBuf
{
public:
  const char* Get() const;
  int GetSize() const;
  int Get(char* pDat, int nMaxSize) const;
  bool operator ==(const WBuf& cmpObj);
  void Set(const char* pDat, int nSize);
  bool Append(const char* pData, int nSize);
  void Clear() { m_nSize = 0; }
private:
  char* m_pData;
  int m_nSize;
  int m_nBufSize;
};