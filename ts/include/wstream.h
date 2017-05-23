#pragma once
/*
      �߳�1 --->ParserHead
            --->Append
      �߳�2 --->Get
            --->����
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
  int Append(const char* pBody, int nSize); // ���ش�����Ϊ3������������
  const WPesInfo* Get(); // ���ش�����Ϊ3������������
  void Free(const WPesInfo* pPes) const;
  //todo other
private:
  WPes* m_parserPes;
  int m_nPesSize;
  //WQueue<WPesInfo* > m_Pes;
  //WQueue<WPesInfo* > m_freePes;
};