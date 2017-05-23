#pragma once
#include "wsection.h"
#include <vector>
class WPsiSi
{
public:
  int ParserHead(const char*, int);
  int Append(const char*, int);
private:
  std::vector<WSection> m_arrSection;
  int m_nCurSectionNum;
};
