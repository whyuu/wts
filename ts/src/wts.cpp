#include "wts.h"
#include "wtspk.h"
#include "wtspayload.h"
using namespace std;
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
//     if (uPid != )
//     {
//     }
    //continue;
    return;
  }
  else
  {
    CWnScopedLock Lock(m_lock);
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
}

void WTs::ParserPsiSI(uint16_t uPid)
{
  
}

//解析pes
bool WTs::AddPesParser(uint16_t uPid, IPesDealer* pesParser)
{
  CWnScopedLock Lock(m_lock);
  WPes* pPes= new WPes(pesParser);
   std::pair<map<uint16_t, WTsPayLoad*>::iterator, bool > ret 
     = m_arrPayLoad.insert(pair<uint16_t, WTsPayLoad*>(uPid, pPes));
   if (ret.second)
   {
     return true;
   }
   return false;
}

IPesDealer* WTs::DelPesParser(uint16_t uPid)
{
  IPesDealer* pObj = NULL;
  CWnScopedLock Lock(m_lock);
  map<uint16_t, WTsPayLoad*>::iterator it = m_arrPayLoad.find(uPid);
  if (it != m_arrPayLoad.end() && it->second->IsPes())
  {
    WPes* pPayLoad = (WPes*)it->second;
    m_arrPayLoad.erase(it);
    Lock.unlock();
    pObj = pPayLoad->GetDealer();

    //加入删除队列；

    return pObj;
  }
  return;
}

//获取节目
void WTs::GetProgramList(std::vector<Program>& arrProgList)
{
}
