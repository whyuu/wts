#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include "wstream.h"
#include "wpsisi.h"

struct Program
{
  uint16_t m_wProgramNo;
  std::string m_strName;
  std::string m_strProvider;
};

class WTsEvent
{
public:
  virtual void OnPsiSiFinish(uint64_t lparam);
  virtual void OnPes(uint16_t uPid, const WPes* wPes, uint16_t lparam);
};

class WTs
{
public:
  void Init(WTsEvent* pD, uint64_t lparam);

  //����ts��
  void Parser(const char tsData[188]);

  //����psisi
  enum
  {
    PSI_PAT_PMT_SDT = 0x1, //Ĭ�ϲ�����
  };
  void ParserPsiSI(uint64_t flag);

  //����pes
  void AddPesParser(uint16_t uPid);
  void DelPesParser(uint16_t uPid);

  //��ȡ��Ŀ
  void GetProgramList(std::vector<Program>& arrProgList);

private:
  uint8_t* GetPayLoad();
  int GetPayLoadSize();
  void SetPayLoad(uint8_t* pData, int nSize);

private:
  uint64_t lparam;
  /*
  PAT 0x0000
  CAT 0x0001
  TSDT 0x0002
  */
  WPsiSi m_arrPsisi[16];

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
  WPsiSi m_arrPsisi1c[6];

  //pes Dealer
  struct PesDealer
  {
    WPes pes;
    PesDealer* pObj;
    uint64_t lparam;
  };
  std::map<uint16_t, PesDealer*> m_arrPes;
};