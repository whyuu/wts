#pragma once
#include <stdint.h>
#include "wbuf.h"

class  WPes
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
    uint8_t m_OriginalOrCopy : 1;
    uint8_t m_CopyRight : 1;
    uint8_t m_DataAlignmentIndicator : 1;
    uint8_t m_PesPriority : 1;
    uint8_t m_PesSCramblingControl : 2;
    uint8_t m_Sign : 2;
    uint8_t m_PesExtensionFlag : 1;
    uint8_t m_PesCRCFlag : 1;
    uint8_t m_AdditionalCopyInfoFLag : 1;
    uint8_t m_DsmTrickModeFlag : 1;
    uint8_t m_EsRateFlag : 1;
    uint8_t m_EsCRFlag : 1;
    enum
    {
      FLAG_PTS = 2,
      FLAG_PTS_ADN_DTS = 3
    };
    uint8_t m_PtsDtsFlag : 2;
    uint8_t m_PesHeaderDataLength;
  };
  const WPesFlag* GetFlag() const;

  uint64_t GetDts() const;
  uint64_t GetPts() const;

  //解析数据
  const char* GetPayLoad() const;
  int GetSize() const;

//int SetData(const char* pData, int nSize);

private:
  WBuf m_pesBuf;
  int m_nPesSize;
};