#pragma once
/*
      线程1 --->ParserHead
            --->Append
      线程2 --->Get
            --->处理
            --->Free
*/

#include "wpes.h"
#include "wbuf.h"

class WPesInfo
{
private:
  WPes m_pes;
  uint64_t m_ats;
};

class  WStream
{
public:
  WStream(int nSize);
  ~WStream();
  int ParserHead(const char* pHead, int nSize);
  int Append(const char* pBody, int nSize); // 返回错误码为3级报错，错误码
  const WPesInfo* Get(); // 返回错误码为3级报错，错误码
  void Free(const WPesInfo* pPes) const;
  //todo other
private:
  WPes* m_parserPes;
  int m_nPesSize;
  //WQueue<WPesInfo* > m_Pes;
  //WQueue<WPesInfo* > m_freePes;
};