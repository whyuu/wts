#include "Ping.h"
#include "WSocket.h"
#include "WLog.h"
#include "Public.h"

#if (defined WIN32) || (defined _WINDLL)
#else
#define far
#endif

#define MAX_ICMP_PACKET_SIZE 1024 //Maximum icmp packet size
#define ON_LINE_PING_TIME  (15)
//generate an IP checksum based on a given data buffer
uint16_t GenerateIPChecksum(uint16_t* pBuffer, int nSize) 
{
	unsigned long cksum = 0;

	while (nSize > 1) 
	{
		cksum += *pBuffer++;
		nSize -= sizeof(uint16_t);
	}

	if (nSize) 
		cksum += *(uint8_t*)pBuffer;

	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >>16);
	return (uint16_t)(~cksum);
}

// ICMP header
typedef struct tagICMP_HEADER 
{
	uint8_t i_type;
	uint8_t i_code; /* type sub code */
	uint16_t i_cksum;
	uint16_t i_id;
	uint16_t i_seq;
	/* This is not the std header, but we reserve space for time */
	uint32_t timestamp;
} ICMP_HEADER;

typedef ICMP_HEADER far* LPICMP_HEADER;

CPing::CPing()
	: m_nOutTime(500),
    m_pICMP(NULL),
    m_nIcmpSize(8),
	m_pRecvBuf(NULL),
	m_socket(INVALID_SOCKET)
{
}

CPing::~CPing()
{
	Destroy();
}

bool CPing::Init(int nTimeOut,int nPkSize, uint8_t ttl)
{
	int nHdrSize = sizeof(ICMP_HEADER);
	//Allocate the ICMP packet
	m_nIcmpSize = nPkSize + 8/*nHdrSize*/;//标准协议中不包含ULONG timestamp变量，去除后头长度为8
	m_pICMP = new char[m_nIcmpSize];
	//allocate the recv buffer	
	LPICMP_HEADER pIcmp = (LPICMP_HEADER)m_pICMP;
	pIcmp->i_type    = 8; //ICMP_ECHO type
	pIcmp->i_code    = 0;
	pIcmp->i_id      = (uint16_t)Public::GetCurrentProcessId();
	pIcmp->i_seq     = 0;
	pIcmp->i_cksum   = 0;
	pIcmp->timestamp = Public::GetTickCount();
	//Set up the data which will be sent
 	char* pData = m_pICMP + nHdrSize;
 	memset(pData, 'E', m_nIcmpSize - nHdrSize);
	//Generate the checksum
	pIcmp->i_cksum = GenerateIPChecksum((uint16_t*)pIcmp, m_nIcmpSize);
	m_nOutTime = nTimeOut;
	m_pRecvBuf = new char[MAX_ICMP_PACKET_SIZE];

	//Create the raw socket
	m_socket = (uint32_t)socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);// WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_socket == (uint32_t)INVALID_SOCKET) 
	{
		return false;
	}
	CWSocket tmpObj(m_socket);
	tmpObj.SetReceiveBuf(MAX_ICMP_PACKET_SIZE * 1500);
	SetPingOutTime(m_nOutTime);
	return true;
}
void CPing::SetPingOutTime(int nOutTime)
{
	m_nOutTime = nOutTime;
	if (m_socket == (uint32_t)INVALID_SOCKET)
	{
		return;
	}
	CWSocket tmpObj(m_socket);
    bool bRes = true;
    #if (defined WIN32) || (defined _WINDLL)
        bRes = tmpObj.SetRecvTimeOut(m_nOutTime);
    #else
        struct timeval timeOut={0,m_nOutTime*1000}; 
        bRes = tmpObj.SetRecvTimeOut(timeOut);
    #endif
	if (!bRes)
	{
		CWLog::GetInstance().WriteLog(LOG_ERR, "CPing::SetPingOutTime SetOutTime Fail");
	}
}
void CPing::Destroy()
{
	if (m_pICMP)
	{
		delete [] m_pICMP;
		m_pICMP = NULL;
	}
	if (m_pRecvBuf)
	{
		delete [] m_pRecvBuf;
		m_pRecvBuf = NULL;
	}
	CWSocket tmpObj(m_socket);
	tmpObj.Destroy();
}

bool CPing::Ping(std::vector<std::string>& arrIps, std::vector<bool>& arrStatus)
{
	arrStatus.clear();
	arrStatus.resize(arrIps.size(), false);

	//Resolve the address of the host to connect to
	sockaddr_in dest;
	memset(&dest,0,sizeof(dest));
	dest.sin_family = AF_INET;
	for (unsigned int i = 0; i < arrIps.size(); i++)
	{
		dest.sin_addr.s_addr = inet_addr(arrIps[i].c_str());
		//Send of the packet
		int nWrote = sendto(m_socket, m_pICMP, m_nIcmpSize, 0, (sockaddr*)&dest, sizeof(dest));
		if (nWrote == SOCKET_ERROR)
		{
			arrStatus[i] = false;
		}
	}
	try
	{
		sockaddr_in sockAddr;
		memset(&sockAddr, 0, sizeof(sockAddr));	
		int nSockAddrLen = sizeof(sockaddr_in);
		while (1)
		{
			int nResult = recvfrom(m_socket, (char*)m_pRecvBuf, MAX_ICMP_PACKET_SIZE, 0, (sockaddr*)&sockAddr, (socklen_t *)&nSockAddrLen);
			if(nResult != SOCKET_ERROR)
			{
				LPICMP_HEADER pIcmp = (LPICMP_HEADER)(m_pRecvBuf + 20);
				if (pIcmp->i_type != 0)
				{
					continue;
				}
				std::string strIp =  inet_ntoa((in_addr)sockAddr.sin_addr);
				for (uint32_t i = 0; i < arrIps.size(); i++)
				{
					if (arrIps[i] == strIp)
					{
						arrStatus[i] = true;
						break;
					}
				}
			}
			else
			{
				break;
			}
		}
	}
	catch(...)
	{
		CWLog::GetInstance().WriteLog(LOG_ERR, "CPing::Ping");
	}
	return true;
}