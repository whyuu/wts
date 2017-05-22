#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include "wts.h"
#include "WQueue.h"

struct Program
{
  uint16_t m_wProgramNo;
  std::string m_strName;
  std::string m_strProvider;
};

class CWTsMgr
{
public:
  //解析psisi
  enum
  {
    PSI_PAT_PMT_SDT = 0x1,
  };
  void ParserPsiSI(uint64_t flag);

  //解析ts
  void AddTs(std::string strIp, uint16_t uPort);
  void DelTs(std::string strIp, uint16_t uPort);

  //解析ts
  void CompareTs()

  //获取节目
  void GetProgramList(std::vector<Program>& arrProgList);

private:
  struct TsItem 
  {
    std::string strIp;
    uint16_t uPort;
    int nSocket;
    WTs mts;
    WQueue<WPes*> m_freeObj;
    WQueue<WPes*> m_currentObj;
  };

  uint64_t lparam;
  std::map<int, TsItem*> m_arrStream;
  void OnPsiSi(int nMsg, int nParam, uint64_t lparam);
  void OnPes(uint16_t uPid, const WPes* wPes, uint16_t lparam);
};