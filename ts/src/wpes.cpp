#include "wpes.h"

//填充数据
int WPes::ParserHead(const char* pData, int nSize)
{
  return 0;
}

int WPes::Append(const char* pData, int nSize)
{
  return 0;
}

uint8_t WPes::GetStreamId() const
{
  return 0;
}
uint16_t WPes::GetPesSize() const
{
  return 0;
}
uint16_t WPes::GetCurSize() const
{
  return 0;
}
const WPes::WFlag* WPes::GetFlag() const
{
  return 0;
}

uint64_t WPes::GetDts() const
{
  return 0;
}
uint64_t WPes::GetPts() const
{
  return 0;
}

//解析数据
const char* WPes::GetPayLoad() const
{
  return 0;
}

int WPes::GetSize() const
{
  return 0;
}