#pragma once
#include <string>
#include <vector>
#include "WType.h"
#include "wstream.h"
#include "wtspayload.h"
using namespace std;

struct Program
{
  WORD m_wProgramNo;
  std::string m_strName;
  std::string m_strProvider;
};

class WTs
{
public:
  void Parser(const char tsData[188]);
  void AddPesDealer(uint16_t uPid);
  void GetProgramList(std::vector<Program>& arrProgList);
  void GetEs(WORD wPid, Es);

private:
  WORD GetPid();
  void SetPid(WORD wPid);
  BYTE* GetPayLoad();
  int GetPayLoadSize();
  void SetPayLoad(BYTE* pData, int nSize);

private:
  BYTE m_ts[188];
  map<uint16_t, WTsPayload*> m_arrStream;
  
};