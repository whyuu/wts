#include "wsection.h"

WSection::WSection(int nSize)
{

}
WSection::~WSection()
{

}
void WSection::Reset()
{

}

bool WSection::Append(char* pData, int nSize)
{
  return 0;
}
uint8_t WSection::GetSectionSyntaxIndicator() const
{
  return 0;
}
uint8_t WSection::GetSectionLength() const
{
  return 0;
}
uint8_t WSection::GetVersionNumber() const
{
  return 0;
}
uint8_t WSection::GetCurrentNextIndicator() const
{
  return 0;
}
uint8_t WSection::GetSectionNumber() const
{
  return 0;
}
uint8_t WSection::GetLastSectionNumber() const
{
  return 0;
}
uint32_t WSection::GetCrc() const
{
  return 0;
}
const char* WSection::GetSectionData() const 
{
  return 0;
}
int WSection::GetSectionSize() const
{
  return 0;
}
int WSection::SetSectionData(const char* pData, int nSize)
{
  return 0;
}