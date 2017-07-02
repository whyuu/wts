#pragma once
class WTsPayLoad
{
public:
  virtual ~WTsPayLoad() {}
  virtual bool IsPes() = 0;
  virtual bool ParserHead(const char*, int) = 0;
  virtual bool Append(const char*, int) = 0;
};