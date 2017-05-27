#pragma once
#include <vector>
#include "wsection.h"
#include "wtspayload.h"

class WPsiSi : public WTsPayLoad
{
public:
  WPsiSi();
  bool ParserHead(const char*, int);
  bool Append(const char*, int);

  void SetSessionSize(int nSessionSize){m_nSessionSize = nSessionSize;}
  bool IsFinish();
protected:
  bool IsPes(){return false;}
  std::vector<WSection*> m_arrSection;
  int m_nSessionSize;
  int m_nLastSessionNumber;
  int m_nCurrentNum;
  bool m_bFinish;
};
