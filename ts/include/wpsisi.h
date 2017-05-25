#pragma once
#include "wsection.h"
#include <vector>
class WPsiSi
{
public:
  WPsiSi(int nSessionSize = 1024);
  int ParserHead(const char*, int);
  int Append(const char*, int);

protected:
  std::vector<WSection*> m_arrSection;
  int m_nCurSectionNum;
  int m_nSessionSize;
};
