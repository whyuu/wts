#pragma once
#include <stdint.h>
#include "wbuf.h"
#include "wtspayload.h"

class  WPat : public WTsPayload
{
public:
  WPat();
  ~WPat();
  //填充数据
  int ParserHead(const char* pData, int nSize);
  int Append(const char* pData, int nSize);

  //获取节目列表
  int GetProgList(vector<pair<uint16_t, uint16_t>>& arrProgList);

  //获取版本号
  uint8_t GetVersionNo();
  //获取传送流id
  uint16_t GetTransportStreamId();


private:
  int m_nCurrentSectionNum;
  vector<WBuf> m_patSection;
};