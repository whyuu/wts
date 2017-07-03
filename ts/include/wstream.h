#pragma once
/*
      线程1 --->ParserHead
            --->Append
      线程2 --->Get
            --->处理
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
  int Append(const char* pBody, int nSize); // 返回错误码为3级报错，错误码
  const WPes* Get(); // 返回错误码为3级报错，错误码
  void Free(const WPes* pPes) const;
private:
  void FinishFrame();
  WBuf* m_pPes;
  int m_nPesSize;
  int m_nCacheFrames;
  WQueue<WBuf*> m_FrameList;
  WQueue<WBuf*> m_freePes;
};