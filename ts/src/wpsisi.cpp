#pragma once
#include "wpsisi.h"
WPsiSi::WPsiSi():m_nSessionSize(1024)
{
}
int WPsiSi::ParserHead(const char*, int)
{
  return 0;
}

int WPsiSi::Append(const char*, int)
{
  return 0;
}