/*
      线程1 --->ParserHead
            --->Append
      线程2 --->Get
            --->处理
            --->Free
*/

#include "wstream.h"

int WStream::ParserHead(const char* pHead, int nSize)
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
  } while (0);
}
int WStream::Append(const char* pBody, int nSize) // 返回错误码为3级报错，错误码
{
  return 0;
}
