/*
      �߳�1 --->ParserHead
            --->Append
      �߳�2 --->Get
            --->����
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
    //У��ͷ
    if (pPesHead->m_StartCodePreFixHigh != 0
      || pPesHead->m_StartCodePreFixMiddle != 0
      || pPesHead->m_StartCodePreFixLow != 1)
    {
      return false;
    }

    if (m_nPesSize == 0 && m_pesBuf.GetSize() > 0)
    {

    }
    //�жϳ���
    m_nPesSize = pPesHead->m_PesPacketLength;
    m_pesBuf.Set(pData, nSize);
  } while (0);
}
int WStream::Append(const char* pBody, int nSize) // ���ش�����Ϊ3������������
{
  return 0;
}
