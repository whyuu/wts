#include<string.h>
#include "wtype.h"
#include "wsection.h"


WSection::WSection(int nSize)
{
  m_pSection = new char[nSize];
  m_nMaxSize = nSize;
  m_nSize = 0;
}
WSection::~WSection()
{

}
void WSection::Reset()
{
  m_nSize = 0;
  m_nTotalSectionSize = 0;
}

int WSection::SetHead(const char* pData, int nSize)
{
  if (nSize > m_nMaxSize)
  {
    return -1;
  }
  memcpy(m_pSection, pData, nSize);
  m_nSize = nSize;
  const WSectionHead* pS = (const WSectionHead*) pData;
  m_nTotalSectionSize = 3 + pS->GetSectionLength();
  return 0;
}

int WSection::Append(const char* pData, int nSize)
{
  if (m_nSize + nSize > m_nMaxSize)
  {
    return -1;
  }
  memcpy(m_pSection + m_nSize, pData, nSize);
  m_nSize += nSize;
  return 0;
}

bool WSection::IsFinish()
{
 if (m_nSize <= 0 ||m_nSize <  m_nTotalSectionSize)
 {
   return false;
 }
 return true;
}

uint8_t WSection::GetSectionSyntaxIndicator() const
{
  return 0;
}

uint8_t WSection::GetSectionLength() const
{
  return 0;
}

uint8_t WSection::GetVersionNumber() const
{
  return 0;
}

uint8_t WSection::GetCurrentNextIndicator() const
{
  return 0;
}

uint8_t WSection::GetSectionNumber() const
{
  return 0;
}

uint8_t WSection::GetLastSectionNumber() const
{
  return 0;
}

uint32_t WSection::GetCrc() const
{
  return 0;
}
const char* WSection::GetSectionData() const 
{
  return 0;
}
int WSection::GetSectionSize() const
{
  return 0;
}
int WSection::SetSectionData(const char* pData, int nSize)
{
  return 0;
}