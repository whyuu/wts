#include "wts.h"
#include "wtspk.h"

WTs::WTs()
{
  m_arrPsisi10[2].SetSessionSize(4096);
}

//解析ts包
void WTs::Parser(const char tsData[188])
{
  const WTsPk* pTs = (const WTsPk*)tsData;
  uint16_t uPid = pTs->GetPid();
  if (uPid < 0x20)
  {
    WPsiSi* pObj = NULL;
    if (uPid < 0x2)
    {
      pObj = &m_arrPsisi[uPid];
    }
    else if (uPid >= 0x10 && uPid <= 0x15)
    {
      pObj = &m_arrPsisi10[uPid - 0x10];
    }
    else if (uPid >= 0x1c)
    {
      pObj = &m_arrPsisi10[uPid - 0x1c];
    }
    else
    {
      printf("Unkonw Psisi\n");
    }
    if (pObj)
    {
      if (pObj->IsFinish())
      {
        printf("table %x finish\n", uPid);
        if (uPid == 0)
        {
        }
        return;
      }

      if (pTs->GetPayLoadUnitStart())
      {
        pObj->ParserHead(pTs->GetPayLoad(), pTs->GetPayLoadSize());
      }
      else
      {
        pObj->Append(pTs->GetPayLoad(), pTs->GetPayLoadSize());
      }
    }
  }
  else
  {
    //pes
  }
}

void WTs::ParserPsiSI(uint64_t flag)
{
}

//解析pes
void WTs::AddPesParser(uint16_t uPid)
{
}

void WTs::DelPesParser(uint16_t uPid)
{
}

//获取节目
void WTs::GetProgramList(std::vector<Program>& arrProgList)
{
}
