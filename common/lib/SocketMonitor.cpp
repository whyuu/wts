#include "WSocket.h"
#include "WLog.h"
#include "SocketMonitor.h"
#include "commType.h"
#define MAX_CONNECT_NUM 64
#define SYSTEM_LONG 64 //64位
#define SOCKET_LEN 6 //64位，是2的6次方

CSocketMonitor::CSocketMonitor(void):m_bFlag(false), m_nChange(0), m_nCurSize(0)
{
  m_arrTmpSocket.CreateQueue(MAX_CONNECT_NUM<<1);
}

CSocketMonitor::~CSocketMonitor(void)
{
}

//启动
void CSocketMonitor::Start(int nId, ISocket* pSocket)
{
  CWnScopedLock Lock(m_csLock);
  if (m_bFlag == false)
  {
    m_nId = nId;
    m_pSocket = pSocket;
    m_bFlag = true;
    boost::function0<void> f = boost::bind(&CSocketMonitor::OnMonitor, this);
    m_thMonitor = boost::thread(f);
  }
}

//停止
void CSocketMonitor::Stop()
{
  if (m_bFlag)
  {
    m_bFlag = false;
    m_thMonitor.join();
  }
}

//添加接收socket
bool CSocketMonitor::AddSocket(SOCKET nSocket)
{
  CWnScopedLock Lock(m_csLock);
  if (m_nCurSize >=  MAX_CONNECT_NUM)
  {
    CWLog::GetInstance().WriteLog(LOG_INFO, "Socket monitor connect reach max");
    return false;
  }
  MSocket tmp = {nSocket, true};
  if (!m_arrTmpSocket.Enqueue(tmp))
  {
    return false;
  }
  m_nChange++;
  m_event.SetEvent();
  return true;
}

//删除接收socket
bool CSocketMonitor::DelSocket(SOCKET nSocket)
{
  CWnScopedLock Lock(m_csLock);
  if (m_nCurSize <= 0)
  {
    return false;
  }
  MSocket tmp = {nSocket, false};
  m_arrTmpSocket.Enqueue(tmp);
  m_nChange++;
  m_event.SetEvent();
  return true;
}

void CSocketMonitor::OnMonitor()
{
  fd_set fdData;
  fd_set fdRead;
  int nChange = 0;
  int nFds = 0;
  struct timeval timeout;
  timeout.tv_sec = 0;
#ifdef WIN32
  timeout.tv_usec = 200;//window下单位是毫秒
#else
  timeout.tv_usec = 200000;
#endif
  FD_ZERO(&fdData);
  while (m_bFlag)
  {
    if (nChange != m_nChange)
    {
      CWnScopedLock Lock(m_csLock);
      nChange = m_nChange;
      MSocket tmp;
      while (m_arrTmpSocket.Dequeue(tmp))
      {
        if (0 == FD_ISSET(tmp.socket, &fdData))
        {
          if (tmp.bAdd)
          {
            FD_SET(tmp.socket, &fdData);
#if !(defined(WIN32) || defined(_WINDLL))
            if (tmp.socket >= nFds)
            {
              nFds = tmp.socket + 1;
            }
#endif
            m_nCurSize++;
          }
        }
        else
        {
          if (!tmp.bAdd)
          {
            FD_CLR(tmp.socket, &fdData);
            m_nCurSize--;

#if !(defined(WIN32) || defined(_WINDLL))
            if (m_nCurSize > 0)
            {
              if (tmp.socket + 1 == nFds)
              {
                int tmpFds = nFds;
                for (int i = (int)(tmp.socket >> SOCKET_LEN); i >= 0; i--)
                {
                  UINT64 uValue = fdData.fds_bits[i];
                  if (uValue == 0)
                  {
                    continue;
                  }
                  for (int j = SYSTEM_LONG - 1; j >= 0; j--)
                  {
                    if (((uValue >> j) & 0x1) == 0x1)
                    {
                      tmpFds = (i << SOCKET_LEN) + j + 1;
                      break;
                    }
                  }
                  if (tmpFds < nFds)
                  {
                    nFds = tmpFds;
                    break;
                  }
                }
              }
            }
#endif
          }
        }
      }
    }
    if (m_nCurSize == 0)
    {
      m_event.wait();
      continue;
    }
    fdRead = fdData;
    int nRes = select(nFds ,&fdRead,NULL,NULL, &timeout);
    if(nRes == 0)
    {
      continue;
    }
#if (defined(WIN32) || defined(_WINDLL))
    for(UINT32 i = 0;i < fdData.fd_count; i++)
    {
      if(FD_ISSET(fdData.fd_array[i],&fdRead))
      {
        m_pSocket->OnRead(m_nId, fdData.fd_array[i]);
      }
    }
#else
    for (int i = 0; i < (int)(FD_SETSIZE>>SOCKET_LEN); i++)
    {
      if (fdRead.fds_bits[i] == 0)
      {
        continue;
      }
      else
      {
        UINT64 uValue = fdRead.fds_bits[i];
        for (int j = 0; j < SYSTEM_LONG; j++)
        {
          int nValue = uValue >> j;
          if (nValue & 0x1)
          {
            m_pSocket->OnRead(m_nId, (i<<SOCKET_LEN) + j);
            if (nValue == 1)
            {
              break;
            }
          }
        }
      }
    }
#endif
  }
}