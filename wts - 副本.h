#pragma once
#include "WType.h"
struct WTsHead
{
	WTsHead():m_SyncByte(0x47),
		m_TransportPriority(0),
		m_PayloadUnitStartIndicator(1),
		m_TransportErrorIndicator(0),
		m_ContinutyCounter(0),
		m_AdaptionFieldControl(1),
		m_TransportScramblingControl(0){}
	WORD GetPID()
	{
		return ((WORD)m_PIDHigh << 8) | m_PIDLow;
	}
	void SetPID(WORD PID)
	{
		m_PIDHigh = (PID >> 8) & 0x1f;
		m_PIDLow = (BYTE)PID;
	}

	BYTE m_SyncByte;
	BYTE m_PIDHigh : 5;
	BYTE m_TransportPriority : 1; //传输优先级，1：相同PID高优先级
	BYTE m_PayloadUnitStartIndicator : 1;//PES，1：以PES首字节开始，0：不以PES开始； PSI， 1：有point_field域
	BYTE m_TransportErrorIndicator : 1; //1： 有错误， 0：无错误 	
	BYTE m_PIDLow;
	BYTE m_ContinutyCounter : 4;
	BYTE m_AdaptionFieldControl : 2;
	BYTE m_TransportScramblingControl : 2;	
};

struct WAdaptFields
{
	BYTE m_AdaptationFieldExtensionFlag : 1;
	BYTE m_TransportPrivateDataFlag : 1;
	BYTE m_SplicingPointerFlag : 1;
	BYTE m_OPCRFlag : 1;
	BYTE m_PCRFlag : 1;
	BYTE m_ElementaryStreamPriorityIndicator : 1;
	BYTE m_RandomAccessIndicator : 1;
	BYTE m_DiscontinuityIndicator : 1;
};

struct WSectionHead
{
public:
	WORD GetSectionLength()
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

struct WPatItem
{
public:
	WORD GetPID()
	{
		return ((WORD)m_PIDHigh << 8) + (WORD)m_PIDLow;
	}
	void SetPID(WORD PID)
	{
		m_PIDHigh = PID >> 8;
		m_PIDLow = (BYTE)PID;
	}
	CMyWORD m_ProgramNumber;
	BYTE m_PIDHigh : 5;
	BYTE m_Reserve : 3;
	BYTE m_PIDLow;
};

struct WPCR
{
	UINT64 GetPcrValue()
	{
		return GetPcrBase() * 300 + GetPcrExt();
	}
	void SetPcrValue(const UINT64& pcrVal)
	{
		UINT64 pcrBase = pcrVal / 300;
		WORD pcrEx =(WORD) (pcrVal % 300);
		m_b1 = (BYTE)(pcrBase >> 25);
		m_b2 = (BYTE)(pcrBase >> 17);
		m_b3 = (BYTE)(pcrBase >> 9);
		m_b4 = (BYTE)(pcrBase >> 1);
		m_b5 = (BYTE)(pcrBase & 1) << 7;
		m_b5 = m_b5 | (BYTE)((pcrEx >> 8) & 1);
		m_b6 = (BYTE)pcrEx;
	}
	UINT64 GetPcrBase()
	{
		UINT64 temp = ((UINT64)m_b1<<25)
			| ((UINT64)m_b2<<17)
			| ((UINT64)m_b3<<9)
			| ((UINT64)m_b4<<1)
			| (((UINT64)m_b5&0x80)>>7);	//pcr base;
		return temp;
	}
	WORD GetPcrExt()
	{
		return (((WORD)m_b5 & 1) << 8) | m_b6;
	}
	BYTE m_b1;
	BYTE m_b2;
	BYTE m_b3;
	BYTE m_b4;
	BYTE m_b5;
	BYTE m_b6;
};

struct WPmtTable
{
	WORD GetPCRPID()
	{
		return ((WORD)m_PCRPIDHigh << 8) | m_PCRPIDLow;
	};
	void SetPCRPID(WORD pcrPID)
	{
		m_PCRPIDHigh = (pcrPID >> 8) & 0x1f;
		m_PCRPIDLow = (BYTE)pcrPID;
	}
	WORD GetDescriptorLength()
	{
		return ((WORD)(m_ProgramInfoLenghHigh & 0x3) << 8) | m_ProgramInfoLengthLow;
	}

	void SetDescriptorLength(WORD wLength)
	{
		m_ProgramInfoLenghHigh = (wLength >> 8) & 0x3;
		m_ProgramInfoLengthLow = (BYTE &)wLength;
	}
	BYTE m_PCRPIDHigh : 5;
	BYTE m_Reserve3 : 3;
	BYTE m_PCRPIDLow : 8;
	BYTE m_ProgramInfoLenghHigh : 4;
	BYTE m_Reserve4 : 4;
	BYTE m_ProgramInfoLengthLow;
};

struct WPmtItem
{
public:
	WPmtItem():m_Reserve1(7), m_Reserve2(0xf){}
	enum STREAM_TYPE
	{
		INTERNATIONAL_RESERVER,
		VIDEO_11172_2,
		VIDEO_13818_2_OR_11172_2_LIM,
		AUDIO_11172_3,
		AUDIO_13818_3,
		PRIVATE_SECTION,
		PES_13818_1_WITH_PRIVATE_DATA,
		MHEG_13522,
		DSM_CC_A,
		REC_H_222_1,
		TYPE_A,
		TYPE_B,
		TYPE_C,
		TYPE_D,
		ASSISTANT_13818_1
	};
	WORD GetPID()
	{
		return ((WORD)m_ElementaryPIDHigh << 8) | m_ElementaryPIDLow;
	};
	void SetPID(WORD wPID)
	{
		m_ElementaryPIDHigh = (wPID >> 8) & 0x1f;
		m_ElementaryPIDLow = (BYTE)wPID;
	}

	WORD GetEsLength()
	{
		return ((WORD)m_ESInfoLengthHigh & 0x3) << 8 | m_ESInfoLengthLow; 
	}

	void SetEsLength(const WORD& wLength)
	{
		m_ESInfoLengthHigh = (wLength >> 8) & 0x3;
		m_ESInfoLengthLow = (BYTE &)wLength;
	}

	BYTE m_StreamType;
	BYTE m_ElementaryPIDHigh : 5;
	BYTE m_Reserve1 : 3;
	BYTE m_ElementaryPIDLow : 8;
	BYTE m_ESInfoLengthHigh : 4;
	BYTE m_Reserve2 : 4;
	BYTE m_ESInfoLengthLow;
};

struct WSdtTable
{
	CMyWORD m_OriginalNetWorkID;
	BYTE m_ReserveFutureUse;
};

struct WSdtItem
{
public:
	WORD GetDescriptorLength()
	{
		return ((WORD)m_DescLengthHigh << 8) | m_DescLengthLow;
	}
	CMyWORD m_ServiceID;
	BYTE m_EITPresentFollowingFlag : 1;
	BYTE m_EITScheduleFlag : 1;
	BYTE m_ReserveFutureUse : 6;
	BYTE m_DescLengthHigh : 4;
	BYTE m_FreeCAMode : 1;
	BYTE m_RunningStatus : 3;
	BYTE m_DescLengthLow;
};

struct WDescriptor
{
	BYTE m_byTag;
	BYTE m_byLength;
};

struct WPesHead
{
	WPesHead():
		m_StartCodePreFixHigh(0),
		m_StartCodePreFixMiddle(0),
		m_StartCodePreFixLow(1),
		m_StreamID(0),
		m_PesPacketLength((WORD)0)
		{
		}
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

struct WTimeStamp
{
public:
	UINT64 GetValue()
	{
		return ((UINT64)m_High3 << 30) | ((UINT64)m_MiddleH8 << 22)
			|((UINT64)m_MiddleL7 << 15) | ((UINT64)m_LowH8 << 7)
			| ((UINT64)m_LowL7);
	}
	void SetValue(const UINT64 Val)
	{
		m_High3 = (BYTE)(Val >> 30) & 0x7;
		m_MiddleH8 = (BYTE)(Val >> 22);
		m_MiddleL7 = (BYTE)(Val>> 15) & 0x7f;
		m_LowH8 = (BYTE)(Val >> 7);
		m_LowL7 = (BYTE)(Val) & 0x7f;
	}
	BYTE m_MarkerBit1 : 1;
	BYTE m_High3 : 3;
	BYTE m_Sign : 4;
	BYTE m_MiddleH8;
	BYTE m_MarkerBit2 : 1;
	BYTE m_MiddleL7 : 7;
	BYTE m_LowH8;
	BYTE m_MarkerBit3 : 1;
	BYTE m_LowL7 : 7;
};

static unsigned long CalculateCRC(const BYTE* data, int nSize)
{
	int bit_count = 0;
	int bit_in_byte = 0,tt=0;
	unsigned short int data_bit;
	unsigned short int shift_reg[32];
	unsigned short int g[] = { 1,1,1,0,1,1,0,1,1,0,1,1,1,0,0,
		0,1,0,0,0,0,0,1,1,0,0,1,0,0,0,
		0,0,1 };
	int i,nr_bits;
	unsigned long crc32;

	// Initialize shift register's to '1'
	for(i=0; i<32; i++)
		shift_reg[i] = 1;

	// Calculate nr of data bits
	nr_bits = nSize * 8;

	while (bit_count < nr_bits)	{
		// Fetch bit from bitstream 
		data_bit = (short int) ((*data)  & (0x80 >> bit_in_byte));
		tt = 7 - bit_in_byte;
		data_bit = (data_bit >> tt);
		bit_in_byte++; bit_count++;
		if (bit_in_byte == 8) {
			bit_in_byte = 0;
			data++;
		}
		// Perform the shift and modula 2 addition 
		data_bit ^= shift_reg[31];
		i = 31;
		while (i != 0) {
			if (g[i])
				shift_reg[i] = shift_reg[i-1] ^ data_bit;
			else
				shift_reg[i] = shift_reg[i-1];
			i--;
		}
		shift_reg[0] = data_bit;
	}
	// make CRC an unsigned long 
	crc32=0x00000000;
	for(i=0;i<32;i++)
		crc32 = crc32|(((unsigned char) shift_reg[31-i])<<(31-i));
	return crc32;
}
