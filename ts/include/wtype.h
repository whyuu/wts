#pragma once
#include <stdint.h>

typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;

//Word类型定义
class CMyWORD
{
public:
	CMyWORD():
	  byHigh(0),
	  byLow(0)
	{}
	CMyWORD(WORD w)
	{
		byHigh = (BYTE)(w>>8);
		byLow = (BYTE)w;
	}
	operator WORD() const
	{
		return ((WORD)byHigh<<8)|byLow;
	}
	void operator = (WORD w)
	{
		byHigh = (BYTE)(w>>8);
		byLow = (BYTE)w;
	}
private:
	BYTE byHigh;
	BYTE byLow;
};

//DWORD类型定义
class CMyDWORD
{
public:
	CMyDWORD():
	  by1(0),
	  by2(0),
	  by3(0),
	  by4(0)
	{}
	CMyDWORD(DWORD dw)
	{
		by1 = (BYTE)(dw>>24);
		by2 = (BYTE)(dw>>16);
		by3 = (BYTE)(dw>>8);
		by4 = (BYTE)dw;	
	}
	operator DWORD() 
	{
		DWORD dw;
		dw = DWORD(by1<<24) | DWORD(by2<<16) | DWORD(by3<<8) | by4;
		return dw;
	};
	void operator = (DWORD dw)
	{
		by1 = (BYTE)(dw>>24);
		by2 = (BYTE)(dw>>16);
		by3 = (BYTE)(dw>>8);
		by4 = (BYTE)dw;	
	};
private:
	BYTE by1;
	BYTE by2;
	BYTE by3;
	BYTE by4;
};

struct WSectionHead
{
public:
  WORD GetSectionLength() const
  {
    return ( (WORD)(m_SectionLengthHigh & 0x3) << 8) | m_SectionLengthLow;
  };
  void SetSectionLength(WORD wLength)
  {
    m_SectionLengthHigh = (wLength >> 8) & 0x3;
    m_SectionLengthLow = (BYTE &)wLength;
  }
  BYTE m_TableID;
  BYTE m_SectionLengthHigh : 4;
  BYTE m_Reserve1 : 2;
  BYTE m_Zero : 1;
  BYTE m_SectionSyntaxIndicator : 1;
  BYTE m_SectionLengthLow;
  CMyWORD m_extensionValue; //pat, sdt为 steamID, pmt为节目号
  BYTE m_CurrentNextIndicator : 1;
  BYTE m_VersionNumber : 5;
  BYTE m_Reserve2 : 2;
  BYTE m_SectionNumber;
  BYTE m_LastSectionNumber;
};

struct WPesHead
{
  WPesHead() :m_StartCodePreFixHigh(0),
    m_StartCodePreFixMiddle(0),
    m_StartCodePreFixLow(1),
    m_StreamID(0),
    m_PesPacketLength((WORD)0) {}
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
  BYTE m_StartCodePreFixHigh;
  BYTE m_StartCodePreFixMiddle;
  BYTE m_StartCodePreFixLow;
  BYTE m_StreamID;
  CMyWORD m_PesPacketLength;
};