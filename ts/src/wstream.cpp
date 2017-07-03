/*
      线程1 --->ParserHead
            --->Append
      线程2 --->Get
            --->处理
            --->Free
*/

#include "wtype.h"
#include "wstream.h"
WStream::WStream(int nSize) : m_nCacheFrames(nSize)
{
  for (int i = 0; i < nSize; i++)
  {
    m_freePes.Enqueue(new WBuf());
  }
}
WStream::~WStream()
{
}

int WStream::ParserHead(const char* pData, int nSize)
{
  const WPesHead* pPesHead = (const WPesHead*)pData;
  //校验头
  if (pPesHead->m_StartCodePreFixHigh != 0
    || pPesHead->m_StartCodePreFixMiddle != 0
    || pPesHead->m_StartCodePreFixLow != 1)
  {
    return false;
  }
  if (m_nPesSize == 0 && m_pPes->GetSize() > 0)
  {
    FinishFrame();
  }
  //判断长度
  m_nPesSize = pPesHead->m_PesPacketLength;
  m_pPes->Set(pData, nSize);
  if (m_nPesSize + 5 == nSize)
  {
    FinishFrame();
  }
}

int WStream::Append(const char* pData, int nSize) // 返回错误码为3级报错，错误码
{
  if (m_pPes->GetSize() <= 0)
  {
    return 0;
  }
  m_pPes->Append(pData, nSize);
  if (m_nPesSize > 0 && m_nPesSize + 5 <= m_pPes->GetSize())
  {
    FinishFrame();
  }
  return 0;
}

void WStream::FinishFrame()
{
  m_FrameList.Enqueue(m_pPes);
  if (!m_freePes.Dequeue(m_pPes))
  {
    m_FrameList.Dequeue(m_pPes);
  }
  m_pPes->Clear();
}