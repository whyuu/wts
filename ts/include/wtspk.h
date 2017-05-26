#pragma once
#include <stdint.h>
class WTsPk
{
public:
  void Set(const char data[188]);
  void Get(char data[188]) const;
  uint8_t GetAdaptionFieldControl() const;
  uint16_t GetPid() const;
  void SetPid(uint16_t wPid);
  struct WAdaptFields
  {
    uint8_t m_AdaptationFieldExtensionFlag : 1;
    uint8_t m_TransportPrivateDataFlag : 1;
    uint8_t m_SplicingPointerFlag : 1;
    uint8_t m_OPCRFlag : 1;
    uint8_t m_PCRFlag : 1;
    uint8_t m_ElementaryStreamPriorityIndicator : 1;
    uint8_t m_RandomAccessIndicator : 1;
    uint8_t m_DiscontinuityIndicator : 1;
  };
  WAdaptFields GetAdaptField() const;

  const char* GetPayLoad() const;
  int GetPayLoadSize() const;
  void SetPayLoad(const char* pData, int nSize);
private:
  char m_ts[188];
};