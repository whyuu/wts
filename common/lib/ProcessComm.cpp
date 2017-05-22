#include <boost/format.hpp>
#include "ProcessComm.h"
static const unsigned int SYN_VALUE = 0x414D5553;
#define MAX_BUF_SIZE (1<<20)
using namespace std;
CProcessComm::CProcessComm() : m_SendData(1024 * 1024, false)
{
}

CProcessComm::~CProcessComm()
{
}

int CProcessComm::Init(int nPort, bool isReq, CProcessCmdDealer* pDealer)
{
  m_isReq = isReq;
  m_pDealer = pDealer;
  unsigned long ip = inet_addr("127.0.0.1");
  if (m_isReq)
  {
    CWTcpSession::Connect(ip, nPort, false, true, true);
  }
  else
  {
    CWTcpServer::Start(nPort, 1, ip, 0);
  }
  m_bFlag = true;
  boost::function0<void> f = boost::bind(&CProcessComm::OnSend, this);
  m_thread = boost::thread(f);
  return 0;
}

void CProcessComm::Destroy()
{
  if (m_isReq)
  {
    CWTcpSession::DisConnect(true);
  }
  else
  {
    CWTcpServer::Stop();
  }
  m_bFlag = false;
  m_event.SetEvent();
  m_thread.join();
}

void CProcessComm::OnSend()
{
  CMemStream dat;
  while (m_bFlag)
  {
    m_event.wait();
    GetCmd(dat);
    if (CWTcpSession::IsConnect())
    {
      CWTcpSession::SendData((char*)dat.GetBuffer(), dat.GetLength());
    }
  }
}

//发送数据
int CProcessComm::SendData(unsigned int nId, bool bCompress, const std::string& strRes)
{
  CWnScopedLock Lock(m_lock);
  m_SendData.Write((UINT8*)&SYN_VALUE, 4);
  m_SendData.Write(bCompress?0x1:0x0);
  m_SendData.Write((UINT8*)&nId, 4);
  UINT32 uSize = (UINT32)strRes.size();
  m_SendData.Write((UINT8*)&uSize, 4);
  m_SendData.Write((UINT8*)strRes.c_str(), (int)strRes.size());
  m_event.SetEvent();
  return 0;
}

void CProcessComm::GetCmd(CMemStream& dat)
{
  CWnScopedLock Lock(m_lock);
  dat = m_SendData;
  m_SendData.SetLength(0);
}

int CProcessComm::OnRecvData(char* pBuf, int nSize)
{
  int nPos = 0;
  int nTmpPos = 0;
  unsigned char tmpV = 0;
  while (nPos + 13 <= nSize)
  {
    UINT32* pSyn = (UINT32*)(pBuf + nPos);
    //判断同步字节
    if (*pSyn != SYN_VALUE)
    {
      nPos++;
      continue;
    }
    nTmpPos = nPos + 4;
    tmpV = (unsigned char)pBuf[nTmpPos++];
    if (tmpV > 1)
    {
      nPos++;
      continue;
    }
    bool bCompress = (tmpV != '\0');
    UINT32 uId = *(UINT32*)(pBuf + nTmpPos);
    nTmpPos += 4;
    UINT32 uSize = *(UINT32*)(pBuf + nTmpPos);
    nTmpPos += 4;
    if (uSize <= (UINT32)(nSize - nTmpPos))
    {
      m_pDealer->OnRcvProcessCmd(uId, string(pBuf + nTmpPos, uSize), bCompress);
      nPos = nTmpPos + uSize;
    }
    else
    {
      break;
    }
  }
  return nPos;
}
bool CProcessComm::OnMsg(int nErrorCode)
{
  if (nErrorCode)
  {
    if (m_isReq)
    {
      return true;
    }
    else
    {
      CWTcpServer::ReleaseSession(this);
      return false;
    }
  }
  return true;
}