#include "wts.h"
#include "wtspk.h"

WTs::WTs()
{
  m_arrPsisi10[2].SetSessionSize(4096);
}
//解析ts包
void WTs::Parser(const char tsData[188])
{
  const WTsPk* pS = (const WTsPk*)tsData;
  uint16_t uPid = pS->GetPid();
  if (uPid < 0x20)
  {
    if (uPid < 0x2)
    {
      m_arrPsisi[uPid].ParserHead(pS->GetPayLoad(), pS->GetPayLoadSize());
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
