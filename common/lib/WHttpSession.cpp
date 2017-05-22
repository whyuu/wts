#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/trim.hpp>
#include "MemStream.h"
#include "WHttpSession.h"
#include "WnScopedLock.h"
#include "WLog.h"
using namespace boost;
//#include <algorithm>
//#include "Log.h"
#define MAX_CMD_SIZE  (1024 * 1024)  //1M字节
#ifdef WIN32
#define strwcmp strnicmp
#else
#define strwcmp strncasecmp
#endif


#define _T(a) a
#define CRLF    _T("\x0d\x0a")
#define IDS_STATUS_OK       _T("200 OK")
#define IDS_STATUS_CREATED      _T("201 Created")
#define IDS_STATUS_ACCEPTED     _T("202 Accepted")
#define IDS_STATUS_NOCONTENT    _T("204 No Content")
#define IDS_STATUS_MOVEDPERM    _T("301 Moved Permanently")
#define IDS_STATUS_MOVEDTEMP    _T("302 Redirection")
#define IDS_STATUS_NOTMODIFIED  _T("304 Not Modified")
#define IDS_STATUS_BADREQUEST   _T("400 Bad Request")
#define IDS_STATUS_UNAUTH       _T("401 Unauthorized")
#define IDS_STATUS_FORBIDDEN    _T("403 Forbidden")
#define IDS_STATUS_NOTFOUND     _T("404 Not Found")
#define IDS_STATUS_SVRERROR     _T("500 Internal Server Error")
#define IDS_STATUS_NOTIMPL      _T("501 Not Implemented")
#define IDS_STATUS_BADGATEWAY   _T("502 Bad Gateway")
#define IDS_STATUS_SVCUNAVAIL   _T("503 Service Unavailable")

bool GetLine(CMemStream& bytes, string& strLine)
{
  int ndx = bytes.GetCurPos();
  int nLength = bytes.GetLength() - 1; //保留两位
  if (ndx >= nLength)
  {
    return false;
  }
  while (ndx < nLength)
  {
    if (bytes[ndx] == '\r' && bytes[ndx + 1] == '\n')
    {
      break;
    }
    ++ndx;
  }
  if (ndx < nLength)
  {
    strLine.assign((char*)bytes.GetCurrentPosData(), ndx - bytes.GetCurPos());
    bytes.SeekPos(ndx+2);
    return true;
  }
  return false;
}

string Decode( const string& str, bool bQuery = false )
{
  string::size_type ndx;
  string strDecoded = str;
  // special processing or query strings....
  if ( bQuery )
  {
    // change all '+' to ' '....
    while( (ndx=strDecoded.find('+')) != string::npos)
      strDecoded = strDecoded.substr(0, ndx) + ' ' + strDecoded.substr(ndx+1);
  }

  // first see if there are any %s to decode....
  if ( strDecoded.find( '%' ) != string::npos)
  {
    // iterate through the string, changing %dd to special char....
    for( ndx = 0; ndx < strDecoded.size(); ndx ++ )
    {
      char ch = strDecoded.at( ndx );
      if ( ch == '%' )
      {
        if ( strDecoded.at( ndx+1 ) == '%' )
        {
          // wanna keep one percent sign....
          strDecoded = strDecoded.substr(0, ndx) + strDecoded.substr(ndx+1);
        }
        else
        {
          // assume we have a hex value
          char ch1 = strDecoded.at(ndx+1);
          char ch2 = strDecoded.at(ndx+2);
          ch1 = ch1 >= 'A' ? (ch1&0xdf)-'A' : ch1-'0';
          ch2 = ch2 >= 'A' ? (ch2&0xdf)-'A' : ch2-'0';
          // replace the escape sequence with the char
          strDecoded = strDecoded.substr(0, ndx)
            + (char)(ch1*16 + ch2)
            + strDecoded.substr(ndx+3);
        }
      }
    }
  }
  return strDecoded;
}

// string GetHttpDate( )
// {
// 	COleDateTime timeHttp = COleDateTime::GetCurrentTime();
// 	return timeHttp.Format( _T("%A, %d %B %Y %H:%M:%S GMT"));
// }

CWHttpSession::CWHttpSession() : m_reqStatus(REQ_REQUEST)
{
}
CWHttpSession::~CWHttpSession()
{
}
//发送数据
int CWHttpSession::SendData(const string& strRes, bool bCompress)
{
  CWnScopedLock Lock(m_SndLock);
  Construct(m_res, strRes, bCompress);
  return CWTcpSession::SendData(m_res.c_str(), (int)m_res.size());
}

int CWHttpSession::OnRecvData(char* pBuffer, int nSize)
{
  CMemStream dat((UINT8*)pBuffer, nSize);
  bool bContinue = true;
  string strLine;
  while (bContinue)
  {
    switch ( m_reqStatus )
    {
    case REQ_REQUEST:
      while (GetLine(dat, strLine) == true)
      {
        if (ProcessRequest(strLine) == true)
        {
          m_req.Reset();
          m_reqStatus = REQ_HEADER;
          break;
        }
      }
      if (m_reqStatus != REQ_HEADER)
      {
        bContinue = false;
        break;
      }
    case REQ_HEADER:
      while ( true )
      {
        bContinue = GetLine( dat, strLine);
        if (!bContinue)
        {
          break;
        }
        if ( !strLine.empty() )
          ProcessHeader(strLine);
        else
        {
          if (m_req.m_nBodySize != 0)
          {
            if (m_req.m_nBodySize <= MAX_CMD_SIZE)
            {
              m_reqStatus = REQ_BODY;
            }
            else
            {
              m_reqStatus = REQ_REQUEST;
              m_req.m_arrHeaders.clear();
              bContinue = false;
              dat.SeekPos(dat.GetLength());
              break; //需要低于1M
            }
          }
          else
          {
            m_reqStatus = REQ_REQUEST;
          }
          break;
        }
      }
     if (m_reqStatus != REQ_BODY)
     {
       break;
     }
      // else fall through....
    case REQ_BODY:
      {
        int nLeft = dat.GetLeftDataSize();
        int nNeed = m_req.m_nBodySize - (int)m_req.m_strBody.size();
        if (nLeft >= nNeed)
        {
          m_reqStatus = REQ_DONE;
        }
        else
        {
          nNeed = nLeft;
        }
        m_req.m_strBody.append((char*)dat.GetCurrentPosData(), nNeed);
        dat.Move(nNeed);
      }
      if (m_reqStatus != REQ_DONE)
      {
        bContinue = false;
        break;
      }
    case REQ_DONE:
      {
        m_reqStatus = REQ_REQUEST;
        bool bCompress = (m_req.m_nEncoding != 0);
        if (0 == DealHttpRequest(m_req.m_strBody,
          m_req.m_strURL,
          bCompress,
          m_rcvSend)
          )
        {
          if (m_rcvSend.size() > 0)
          {
            CWHttpSession::SendData(m_rcvSend, bCompress);
//             Construct(m_res, m_rcvSend, bCompress);
//             CWTcpSession::SendData(m_res.c_str(), (int)m_res.size());
            m_rcvSend.clear();
          }
        }
//      else
//      {
//        printf("error\n");
//      }
//      printf("%d\n", m_req.m_strBody.size());
        m_req.m_arrHeaders.clear();
      }
      break;
    default: 
      ;
    }
  }
  return dat.GetCurPos();
}

bool CWHttpSession::ProcessRequest(const string& strLine)
{
  string::size_type ndx, ndx2;
  ndx = strLine.find("POST");
  if ((int)ndx != -1)
  {
    //m_req.m_strMethod = "POST";
    ndx += 4;
    if (ndx >= strLine.size() || strLine[ndx] != ' ')
    {
      return false;
    }
    ndx2 = strLine.find(' ', ndx + 1);
    if ( (int)ndx2 == -1 )
    {
      m_req.m_strURL = strLine.substr(ndx + 1);
    }
    else
    {
      m_req.m_strURL = strLine.substr(ndx + 1, ndx2 - ndx - 1);
      //m_req.m_strVersion = strLine.substr( ndx2 + 1 );
    }

    // check for execution arguments....
    ndx = m_req.m_strURL.find( '?' );
    if ((int)ndx != -1 )
    {
      // yup; save the args....
      m_req.m_strArgs = m_req.m_strURL.substr( ndx+1 );
      // strip from file name....
      m_req.m_strURL = m_req.m_strURL.substr(0, ndx );
      m_req.m_nExecute = HttpRequest::APP_EXECUTE;
    }
    // change any "%xx"s to the appropriate char....
    m_req.m_strURL = Decode( m_req.m_strURL );
    return true;
  }
  return false;
}
void CWHttpSession::ProcessHeader(string& strLine)
{
  if (strwcmp(strLine.c_str(), "content-length", 14) == 0)
  {
    m_req.m_nBodySize = atoi( strLine.c_str() + 15);
  }
  else if (strwcmp(strLine.c_str(), "content-encoding", 16) == 0)
  {
    m_req.m_nEncoding = 1;
  }
}

void CWHttpSession::Construct(string& dst, const string& body, bool bCompress)
{
  dst = "HTTP/1.1 200 OK";
  dst.append(CRLF,2);
  char strSize[16];
  sprintf(strSize, "%d",(int)body.size());
  dst.append("Content-Length:");
  dst.append(strSize);
  dst.append(CRLF,2);
  if (bCompress)
  {
    dst.append("Content-Encoding:gzip");
    dst.append(CRLF,2);
  }
  dst.append(CRLF,2);
  dst.append(body);
}
