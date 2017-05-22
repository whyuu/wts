// Request.h : interface of the CRequest class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1997-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.
#include <string>
#include <vector>
using namespace std;
struct HttpRequest
{
  enum AppFlags {
    APP_NONE    = 0x0000,
    APP_EXECUTE = 0x0001,
    APP_CGI     = 0x0002,
    APP_ISAPI   = 0x0004 
  };
  // Attributes....
  string             m_strPathTranslated;
  string             m_strPathInfo;
  string             m_strHost; // host's address
  string             m_strMethod; // GET, HEAD or POST
  string             m_strURL;
  string             m_strVersion; // HTTP/1.0
  string             m_strArgs; // string after '?'
  time_t             m_timeReq; // time of request
  int                m_nExecute; // executable type
  int                m_nEncoding;
  int                m_nBodySize; //body大小
  string             m_strBody;
  vector<string>     m_arrHeaders;

  ///常用函数
  // access routines....
  string GetHeaderValue( string strName )
  {
    string strValue;
    for (uint32_t i = 0; i < m_arrHeaders.size(); i++)
    {
      if (m_arrHeaders[i].compare(0, strName.size(), strName) == 0)
      {
        strValue = m_arrHeaders[i].substr(strName.size() + 1);
        break;
      }
    }
    return strValue;
  }
  
  void Reset()
  {
    m_nEncoding = 0;
    m_nBodySize = 0;
    m_strBody.clear();
  }
};
