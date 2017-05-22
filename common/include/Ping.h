
#pragma once
#include <stdint.h>
#include <vector>
#include <string>

class CPing
{
public:
	CPing();
	~CPing();
	bool Init(int nTimeOut, int nPkSize, uint8_t ttl = 255);
	void Destroy();
	bool Ping(std::vector<std::string>& arrIps, std::vector<bool>& arrStatus);
	void SetPingOutTime(int nOutTime);
private:
	int m_nOutTime;
	char* m_pICMP;
	int m_nIcmpSize;
	char* m_pRecvBuf;
	uint32_t m_socket;
};
