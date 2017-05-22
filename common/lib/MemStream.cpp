// WCache.cpp: implementation of the CWCache class.
//
//////////////////////////////////////////////////////////////////////

#include "MemStream.h"
#include <stdlib.h>
#include <assert.h>
#include <memory>
#include <string.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMemStream::CMemStream()
: m_pData(NULL), m_nSize(0),m_nCurPos(0),m_nCapacity(0),m_bAttach(false),m_bAutoAdd(true)
{
}

CMemStream::CMemStream(int nCapacity, bool bAutoAdd)
:m_pData(NULL),m_nSize(0), m_nCurPos(0), m_nCapacity(nCapacity), m_bAttach(false), m_bAutoAdd(bAutoAdd)
{
	m_pData = (UINT8*)malloc(m_nCapacity);
}
CMemStream::CMemStream(UINT8* pData, int nSize): m_pData(NULL) //attach数据
{
	Attach(pData, nSize);
}
CMemStream::CMemStream(char* pData, int nSize)
{
  Attach((UINT8*)pData, nSize);
}
CMemStream::CMemStream(const CMemStream& Src)
:m_nSize(Src.m_nSize), m_nCurPos(0), m_nCapacity(Src.m_nSize), m_bAttach(false), m_bAutoAdd(Src.m_bAutoAdd) 
{
	assert(Src.m_nSize >= 0 && Src.m_nCapacity >= Src.m_nSize);

	if (m_nCapacity > 0)
	{
		m_pData = (UINT8*)malloc(m_nCapacity);
		memcpy(m_pData, Src.m_pData, m_nSize);
	}
	else
	{
		m_pData = NULL;
	}
}
CMemStream::~CMemStream()
{
	if (m_pData && !m_bAttach)
	{
		free(m_pData);
	}
}

CMemStream&  CMemStream::operator=(const CMemStream& obj)
{
	assert(obj.m_nSize >= 0 && obj.m_nCapacity >= obj.m_nSize);
	if (&obj == this)
	{
		return *this;
	}
	if (m_pData && !m_bAttach)
	{
		free(m_pData);
		m_pData = NULL;
	}
	m_nSize = m_nCapacity = obj.m_nSize;
	m_bAutoAdd = obj.m_bAutoAdd;
	m_bAttach = false;
	m_nCurPos = 0;
	if (m_nCapacity > 0)
	{
		m_pData = (UINT8 *)malloc(m_nCapacity);
		memcpy(m_pData, obj.m_pData, m_nSize);
	}	
	return *this;
}

CMemStream& CMemStream::operator+=(const CMemStream& obj)
{
	assert(obj.m_nSize >= 0 && obj.m_nCapacity >= obj.m_nSize);
	if (obj.m_nSize > 0)
	{
		Write(obj.m_pData, obj.m_nSize);
	}
	return *this;
}
UINT8& CMemStream::operator[](int nIndex)
{
	if (nIndex < m_nSize)
	{
		return m_pData[nIndex];
	}
	return  m_pData[m_nSize - 1];
}

//加载数据
void CMemStream::Attach(UINT8* pDat, int nSize)
{
	if (m_pData && !m_bAttach)
	{
		free(m_pData);
	}
	m_pData = pDat;
	m_nSize = nSize;
	m_nCapacity = nSize;
	m_nCurPos = 0;
	m_bAttach = true;
	m_bAutoAdd = false;
}

//获取数据不移动位置
UINT8* CMemStream::GetBuffer(int nStartPos, int nSize)
{
	if (nSize > m_nSize - nStartPos 
		|| nStartPos >= m_nSize)
		return NULL;
	return m_pData + nStartPos;
}
 const UINT8* CMemStream::GetConstBuffer(int nStartPos, int nSize) const
{
	if (nSize > m_nSize - nStartPos 
		|| nStartPos >= m_nSize)
		return NULL;
	return m_pData + nStartPos;
}
//获取数据
int CMemStream::Read(UINT8* pDat, int nSize)
{
	if (nSize > m_nSize - m_nCurPos)
	{
		nSize = m_nSize - m_nCurPos;
	}
	memcpy(pDat, m_pData + m_nCurPos, nSize);
	m_nCurPos += nSize;
	return nSize;
}

//获取数据并移动位置
UINT8* CMemStream::Read(int nSize)
{
	if (nSize > m_nSize - m_nCurPos)
	{
		return NULL;
	}
	UINT8* tmpDat = m_pData + m_nCurPos;
	m_nCurPos += nSize;
	return tmpDat;
}
bool CMemStream::Write(const char* pDat, int nSize)
{
  if (nSize > m_nCapacity - m_nSize)
  {
    if (m_bAutoAdd)
    {
      int nCapacity = 2 * (m_nCapacity + nSize);
      m_pData = (UINT8*)realloc(m_pData, nCapacity);
      m_nCapacity = nCapacity;
    }
    else
    {
      return false;
    }
  }
  memcpy(m_pData + m_nSize, pDat, nSize);
  m_nSize += nSize;
  return true;
}
//压入数据
bool CMemStream::Write(const UINT8* pDat, int nSize)
{
	if (nSize > m_nCapacity - m_nSize)
	{
		if (m_bAutoAdd)
		{
			int nCapacity = 2 * (m_nCapacity + nSize);
			m_pData = (UINT8*)realloc(m_pData, nCapacity);
			m_nCapacity = nCapacity;
		}
		else
		{
			return false;
		}
	}
	memcpy(m_pData + m_nSize, pDat, nSize);
	m_nSize += nSize;
	return true;
}
//压入数据
bool CMemStream::Write(UINT8 byDat, int nSize)
{
	if (nSize > m_nCapacity - m_nSize)
	{
		if (m_bAutoAdd)
		{
			int nCapacity = 2*(m_nCapacity + nSize);
			m_pData = (UINT8*)realloc(m_pData, nCapacity);
			m_nCapacity = nCapacity;
		}
		else
		{
			return false;
		}
	}
	memset(m_pData + m_nSize, byDat, nSize);
	m_nSize += nSize;
	return true;
}
//跳过字节
int CMemStream::Move(int nBytes)
{
	if (nBytes >= m_nSize - m_nCurPos)
	{
		m_nCurPos = m_nSize;
	}
	else
	{
		m_nCurPos += nBytes; 
	}
	return m_nCurPos;
}

//设置位置
int CMemStream::SeekPos(int nPos)
{
	if (nPos >= m_nSize)
	{
		nPos = m_nSize;
	}
	m_nCurPos = nPos;
	return nPos;
}
//设置大小
int CMemStream::SetLength(int nSize)
{
	if (nSize >= 0 && nSize <= m_nCapacity)
	{
		m_nSize = nSize;
		m_nCurPos = 0;
		return 0;
	}
	return -1;
}
//设置容量
int CMemStream::SetCapaciy(int nSize)
{
	m_nCapacity = nSize;
	m_pData = (UINT8*)realloc(m_pData, m_nCapacity);
	if (m_nSize > m_nCapacity)
	{
		m_nSize = m_nCapacity;
		if (m_nCurPos > m_nSize)
		{
			m_nCurPos = m_nSize;
		}
	}
	return m_nCapacity;
}