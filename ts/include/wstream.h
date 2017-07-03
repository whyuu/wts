#pragma once
/*
      �߳�1 --->ParserHead
            --->Append
      �߳�2 --->Get
            --->����
            --->Free
*/

#include "wtspayload.h"
#include "wpes.h"
#include "wbuf.h"
#include "WQueue.h"

// class WPesInfo
// {
// private:
//   WPes m_pes;
//   uint64_t m_ats;
// };

class  WStream : public WTsPayLoad
{
public:
  WStream(int nSize);
  ~WStream();
  int ParserHead(const char* pHead, int nSize);
  int Append(const char* pBody, int nSize); // ���ش�����Ϊ3������������
  const WPes* Get(); // ���ش�����Ϊ3������������
  void Free(const WPes* pPes) const;
private:
  void FinishFrame();
  WBuf* m_pPes;
  int m_nPesSize;
  int m_nCacheFrames;
  WQueue<WBuf*> m_FrameList;
  WQueue<WBuf*> m_freePes;
};