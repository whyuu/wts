#include "wtspk.h"
struct WTsHead
{
  WTsHead():m_SyncByte(0x47),
    m_TransportPriority(0),
    m_PayloadUnitStartIndicator(1),
    m_TransportErrorIndicator(0),
    m_ContinutyCounter(0),
    m_AdaptionFieldControl(1),
    m_TransportScramblingControl(0){}
  uint16_t GetPID()
  {
    return ((uint16_t)m_PIDHigh << 8) | m_PIDLow;
  }
  void SetPID(uint16_t PID)
  {
    m_PIDHigh = (PID >> 8) & 0x1f;
    m_PIDLow = (uint8_t)PID;
  }

  uint8_t m_SyncByte;
  uint8_t m_PIDHigh : 5;
  uint8_t m_TransportPriority : 1; //传输优先级，1：相同PID高优先级
  uint8_t m_PayloadUnitStartIndicator : 1;//PES，1：以PES首字节开始，0：不以PES开始； PSI， 1：有point_field域
  uint8_t m_TransportErrorIndicator : 1; //1： 有错误， 0：无错误 	
  uint8_t m_PIDLow;
  uint8_t m_ContinutyCounter : 4;
  uint8_t m_AdaptionFieldControl : 2;
  uint8_t m_TransportScramblingControl : 2;	
};

void WTsPk::Set(const char data[188])
{
}

void WTsPk::Get(char data[188]) const
{
}

bool WTsPk::GetPayLoadUnitStart() const
{
  WTsHead* pTs = (WTsHead*)m_ts;
  return pTs->m_PayloadUnitStartIndicator;
}

uint16_t WTsPk::GetPid() const
{
  WTsHead* pTs = (WTsHead*)m_ts;
  return pTs->GetPID();
}

void WTsPk::SetPid(uint16_t wPid)
{
}

uint8_t WTsPk::GetAdaptionFieldControl() const
{
  WTsHead* pTs = (WTsHead*)m_ts;
  return pTs->m_AdaptionFieldControl;
}

WTsPk::WAdaptFields WTsPk::GetAdaptField() const
{
  return (WAdaptFields&)m_ts[5];
}

const char* WTsPk::GetPayLoad() const
{
  WTsHead* pTs = (WTsHead*)m_ts;
  if (pTs->m_PayloadUnitStartIndicator & 0x2)
  {
    return &m_ts[5 + m_ts[4]] ;
  }
  return &m_ts[4];
}

int WTsPk::GetPayLoadSize() const
{
  WTsHead* pTs = (WTsHead*)m_ts;
  if (pTs->m_PayloadUnitStartIndicator & 0x2)
  {
    return 184 - 1 - m_ts[5] ;
  }
  return 184;
}

void WTsPk::SetPayLoad(const char* pData, int nSize)
{
}