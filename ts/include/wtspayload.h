#pragma once
class WTsPayLoad
{
public:
  virtual bool ParserHead(const char*, int) = 0;
  virtual bool Append(const char*, int) = 0;
};