#pragma once
#include <stdint.h>
#include "wbuf.h"
#include "wtspayload.h"

class  WPes : public WTsPayload
{
public:
  WPes();
  ~WPes();
  //填充数据
  int ParserHead(const char* pData, int nSize);
  int Append(const char* pData, int nSize);

  //解析头
  enum
  {
    PROGRAM_STREAM_MAP = 0xBC,
    PRIVATE_STREAM_1,
    PADDING_STREAM,
    PRIVATE_STREAM_2,
    AUDIO_STREAM,
    VIDEO_STREAM = 0xE0,
    ECM_STREAM = 0xF0,
    EMM_STREAM,
    DSMCC_STREAM,
    STREAM_13522,
    H2221_A,
    H2221_B,
    H2221_C,
    H2221_D,
    H2221_E,
    ANCILLARY_STREAM,
    PROGRAM_STREAM_DIRECTORY = 0xFF
  };
  uint8_t GetStreamId() const;
  uint16_t GetPesSize() const;
  uint16_t GetCurSize() const;

  //解析flag
  struct WPesFlag
  {
    BYTE m_OriginalOrCopy : 1;
    BYTE m_CopyRight : 1;
    BYTE m_DataAlignmentIndicator : 1;
    BYTE m_PesPriority : 1;
    BYTE m_PesSCramblingControl : 2;
    BYTE m_Sign : 2;
    BYTE m_PesExtensionFlag : 1;
    BYTE m_PesCRCFlag : 1;
    BYTE m_AdditionalCopyInfoFLag : 1;
    BYTE m_DsmTrickModeFlag : 1;
    BYTE m_EsRateFlag : 1;
    BYTE m_EsCRFlag : 1;
    enum
    {
      FLAG_PTS = 2,
      FLAG_PTS_ADN_DTS = 3
    };
    BYTE m_PtsDtsFlag : 2;
    BYTE m_PesHeaderDataLength;
  };
  const WPesFlag* GetFlag() const;

  uint64_t GetDts() const;
  uint64_t GetPts() const;

  //解析数据
  int GetData(char* pData ,int nSize) const;

//int SetData(const char* pData, int nSize);

private:
  WBuf m_pesBuf;
  int m_nPesSize;
};