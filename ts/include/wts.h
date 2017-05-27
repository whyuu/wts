#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include "wstream.h"
#include "wpsisi.h"
#include "wtspayload.h"

struct Program
{
  uint16_t m_wProgramNo;
  std::string m_strName;
  std::string m_strProvider;
};

class WTs
{
public:
  WTs();
  //解析ts包
  void Parser(const char tsData[188]);

  //解析psisi
  enum
  {
    PSI_PAT_PMT_SDT = 0x1, //默认不解析
  };
  void ParserPsiSI(uint64_t flag);

  //解析pes
  void AddPesParser(uint16_t uPid);
  void DelPesParser(uint16_t uPid);

  //获取节目
  void GetProgramList(std::vector<Program>& arrProgList);

protected:
  virtual void OnPes(uint16_t uPid, const WPes* wPes, uint16_t lparam) = 0;

private:
  /*
  PAT 0x0000
  CAT 0x0001
  TSDT 0x0002
  */
  WPsiSi m_arrPsisi[3];

  /*
  NIT, ST 0x0010
  SDT , BAT, ST 0x0011
  EIT, ST 0x0012
  RST, ST 0x0013
  TDT, TOT, ST 0x0014
  网络同步 0x0015
  */
  WPsiSi m_arrPsisi10[6];

  /*
  带内信令0x001C
  测量0x001D
  DIT 0x001E
  SIT 0x001F
  */
  WPsiSi m_arrPsisi1c[4];

  std::vector<WPsiSi> m_arrPmt;
  std::map<uint16_t, WTsPayLoad*> m_arrPayLoad;
};