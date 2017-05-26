#include "wudpts.h"
#include "Public.h"

uint64_t WUdpTs::Create(const std::string& strIp, uint16_t uPort)
{
  m_Socket.Create();
  u_long uIp = inet_addr(strIp.c_str());
  if (!m_Socket.Bind(uPort, 0))
  {
    m_Socket.Destroy();
    return -1;
  }
  if (Public::CheckIpV4Multiple(uIp))
  {
    if (m_Socket.SetMultiAddr(uIp, 0))
    {
      return (uint64_t)m_Socket;
    }
    else
    {
      m_Socket.Destroy();
      return -1;
    }
  }
  return m_Socket;
}
void WUdpTs::Destroy()
{
  m_Socket.Destroy();
}
void WUdpTs::Read()
{
  int nSize = m_Socket.Receive(m_buf, BUFSIZE);
  if (nSize < 0)
  {
    printf(" WUdpTs::Read error %d \n", WSAGetLastError());
  }
  int nPos = 0;
  while(nPos < nSize)
  {
    Parser(m_buf + nPos);
    nPos += 188;
  }
}
void WUdpTs::OnPes(uint16_t uPid, const WPes* wPes, uint16_t lparam)
{

}