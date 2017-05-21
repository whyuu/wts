#pragma once
class WTsPayload
{
public:
  virtual int ParserHead(const char*, int) = 0;
  virtual int ParserBody(const char*, int) = 0;
};
