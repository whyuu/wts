#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include "WnScopedLock.h"
#include "wstream.h"
#include "wpsisi.h"
#include "wpes.h"
struct Program
{
  uint16_t m_wProgramNo;
  std::string m_strName;
  std::string m_strProvider;
};
class WTsPayLoad;
class WTs
{
public:
  WTs();
  //����ts��
  void Parser(const char tsData[188]);

  //����psisi
  void ParserPsiSI(uint16_t uPid);

  //����pes
  bool AddPesParser(uint16_t uPid, IPesDealer* pesParser);
  IPesDealer* DelPesParser(uint16_t uPid);

  //��ȡ��Ŀ
  void GetProgramList(std::vector<Program>& arrProgList);
  
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
  ����ͬ�� 0x0015
  */
  WPsiSi m_arrPsisi10[6];

  /*
  ��������0x001C
  ����0x001D
  DIT 0x001E
  SIT 0x001F
  */
  WPsiSi m_arrPsisi1c[4];

  /*pmt*/
  std::vector<WPsiSi> m_arrPmt;

  //������
  CWnLock m_lock;
  std::map<uint16_t, WTsPayLoad*> m_arrPayLoad;
};