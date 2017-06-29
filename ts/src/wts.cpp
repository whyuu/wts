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
  std::map<uint16_t, WTsPayLoad*>::iterator it = m_arrPayLoad.find(uPid);
  if (it != m_arrPayLoad.end())
  {
    WTsPayLoad* pPayLoad = it->second;
    if (pTs->GetPayLoadUnitStart())
    {
      pPayLoad->ParserHead(pTs->GetPayLoad(), pTs->GetPayLoadSize());
    }
    else
    {
      pPayLoad->Append(pTs->GetPayLoad(), pTs->GetPayLoadSize());
    }
  }
}

void WTs::ParserPsiSI(uint64_t flag)
{
  
}

//解析pes
void WTs::AddPesParser(uint16_t uPid)
{
   WPes* pPes= new WPes();
}

void WTs::DelPesParser(uint16_t uPid)
{
}

//获取节目
void WTs::GetProgramList(std::vector<Program>& arrProgList)
{
}
