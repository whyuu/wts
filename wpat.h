#pragma once
#include <stdint.h>
#include "wbuf.h"
#include "wtspayload.h"

class  WPat : public WTsPayload
{
public:
  WPat();
  ~WPat();
  //�������
  int ParserHead(const char* pData, int nSize);
  int Append(const char* pData, int nSize);

  //��ȡ��Ŀ�б�
  int GetProgList(vector<pair<uint16_t, uint16_t>>& arrProgList);

  //��ȡ�汾��
  uint8_t GetVersionNo();
  //��ȡ������id
  uint16_t GetTransportStreamId();


private:
  int m_nCurrentSectionNum;
  vector<WBuf> m_patSection;
};