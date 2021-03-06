#include "wtype.h"
#include "wpes.h"


WPes::WPes(IPesDealer* pDealer) : m_pesBuf(pDealer)
{
}
//填充数据
bool WPes::ParserHead(const char* pData, int nSize)
{
  do
  {
    const WPesHead* pPesHead = (const WPesHead*)pData;
    if (!pPesHead)
    {
      break; 
    }
    //校验头
    if (pPesHead->m_StartCodePreFixHigh != 0
      || pPesHead->m_StartCodePreFixMiddle != 0
      || pPesHead->m_StartCodePreFixLow != 1)
    {
      return false;
    }

    if (m_nPesSize == 0 && m_pesBuf.GetSize() > 0)
    {
      
    }
    //判断长度
    m_nPesSize = pPesHead->m_PesPacketLength;
    m_pesBuf.Set(pData, nSize);
    
    if (nLength > (uint32_t))
    {
      break;
    }
    else if (nLength == 0)
    {
      nLength = memStream.GetLeftDataSize();
    }

    switch (pPesHead->m_StreamID)
    {
    case WPesHead::PROGRAM_STREAM_MAP:
    case WPesHead::PADDING_STREAM:
    case WPesHead::PRIVATE_STREAM_2:
    case WPesHead::ECM_STREAM:
    case WPesHead::EMM_STREAM:
    case WPesHead::PROGRAM_STREAM_DIRECTORY:
    case WPesHead::DSMCC_STREAM:
    case WPesHead::H2221_E:
      break;
    default:
      {
        //获取flag
        WPesFlag* pPesFlag = (WPesFlag*)memStream.Read(sizeof(WPesFlag));
        if (NULL == pPesFlag)
        {
          break;
        }
        nLength = nLength - pPesFlag->m_PesHeaderDataLength - 3;

        //获取扩展数据
        BYTE* pExtenData = memStream.Read(pPesFlag->m_PesHeaderDataLength);
        if (NULL == pExtenData)
        {
          break;
        }
        CMemStream ExtenData(pExtenData, pPesFlag->m_PesHeaderDataLength);

        //ptsflag
        if (pPesFlag->m_PtsDtsFlag & 2)
        {
          WTimeStamp * pTimS = (WTimeStamp*)ExtenData.Read(sizeof(WTimeStamp));
          if (NULL == pTimS)
          {
            break;
          }

          esDat.m_uPts = pTimS->GetValue();
          //TRACE("Dts: %I64u\n", pTimS->GetValue());
        }
        if (pPesFlag->m_PtsDtsFlag & 1)
        {
          WTimeStamp * pTimS = (WTimeStamp*)ExtenData.Read(sizeof(WTimeStamp));
          if (NULL == pTimS)
          {
            break;
          }
          esDat.m_uDts = pTimS->GetValue();
        }
        else
        {
          esDat.m_uDts = esDat.m_uPts;
        }
      }
      char* pEs = NULL;
      if (nLength <= 0)
      {
        nLength = memStream.GetLeftDataSize();
      }

      BYTE* pBuf = memStream.GetCurrentPosData(nLength);
      if (!pBuf)
      {
        break;
      }
      esDat.m_Dat.Attach(pBuf, nLength);
      m_esDistributer.DealEs(esDat);
    }

  } while (0);
  return 0;

  return true;
}

bool WPes::Append(const char* pData, int nSize)
{
  return false;
}

uint64_t WPes::GetPts()
{
  const WPesHead* pPesHead = (const WPesHead*)m_pesBuf;
  if (!pPesHead)
  {
  }
}
uint64_t WPes::GetDts()
{

}

uint8_t WPes::GetStreamId() const
{
  return 0;
}
uint16_t WPes::GetPesSize() const
{
  return 0;
}
uint16_t WPes::GetCurSize() const
{
  return 0;
}
const WPes::WFlag* WPes::GetFlag() const
{
  return 0;
}

uint64_t WPes::GetDts() const
{
  return 0;
}
uint64_t WPes::GetPts() const
{
  return 0;
}

//解析数据
const char* WPes::GetPayLoad() const
{
  return 0;
}

int WPes::GetSize() const
{
  return 0;
}