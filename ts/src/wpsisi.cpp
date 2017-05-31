#pragma once
#include "wtype.h"
#include "wpsisi.h"
using namespace std;
WPsiSi::WPsiSi():m_nSessionSize(1024),
  m_nLastSessionNumber(-1),
  m_nCurrentNum(-1),
  m_bFinish(false)
{
}
bool WPsiSi::ParserHead(const char* pbuf, int nSize)
{
  int JumpSize = 1 + *pbuf;
  const char* pRealData = pbuf + JumpSize;
  const WSectionHead* pS = (const WSectionHead*) pRealData;

  while (pS->m_LastSectionNumber + 1 > m_arrSection.size())
  {
    m_arrSection.push_back(new WSection(m_nSessionSize));
  }
  if (m_nSessionSize == 0)
  {
    m_nSessionSize = pS->m_LastSectionNumber;
  }
  m_nCurrentNum = pS->m_SectionNumber;
  WSection* pSS = m_arrSection[m_nCurrentNum];
  if (!pSS->IsFinish())
  {
    pSS->SetHead(pRealData, nSize - JumpSize);
    return true;
  }
  return false;
}

bool WPsiSi::Append(const char* pbuf, int nSize)
{
  WSection* pSS = m_arrSection[m_nCurrentNum];
  if (!pSS->IsFinish())
  {
    pSS->Append(pbuf, nSize);
    return true;
  }
  return false;
}
bool WPsiSi::IsFinish()
{
  if (!m_bFinish)
  {
    for (int i = 0; i < m_nLastSessionNumber; i++)
    {
      if (!m_arrSection[i]->IsFinish())
      {
        return false;
      }
    }
    m_bFinish = true;
  }
  return true;
}