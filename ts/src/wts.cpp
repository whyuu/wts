#include "wts.h"
#include "wtspk.h"

WTs::WTs()
{
  m_arrPsisi10[2].SetSessionSize(4096);
}

//����ts��
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

//����pes
void WTs::AddPesParser(uint16_t uPid)
{
   WPes* pPes= new WPes();
}

void WTs::DelPesParser(uint16_t uPid)
{
}

//��ȡ��Ŀ
void WTs::GetProgramList(std::vector<Program>& arrProgList)
{
}
