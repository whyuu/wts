#pragma once
#include <vector>

template <class T>
class WQueue
{
public:
	WQueue(void);
	~WQueue(void);
	void CreateQueue(int maxsize);
	void Destroy();
	void Clear(){m_front = m_rear = 0;}
	bool Enqueue(T val);
	bool Dequeue(T &val);
	int GetValueNum();
	int GetMaxNum(){return m_nMaxDataSize;}
	void GetValueList(std::vector<T>& arrVal);
private:
	T *m_pBase;
	int m_front;    //指向队列第一个元素
	int m_rear;    //指向队列最后一个元素的下一个元素
	int m_maxsize; //循环队列的最大存储空间
	int m_nMaxDataSize;
};

template <class T>
WQueue<T>::WQueue(void):m_pBase(NULL),m_front(0),m_rear(0),m_maxsize(0)
{
}

template <class T>
WQueue<T>::~WQueue(void)
{
	if (m_pBase)
	{
		free(m_pBase);
	}
}

template <class T>
void WQueue<T>::CreateQueue(int maxsize)
{
	m_nMaxDataSize = maxsize;
	m_maxsize = maxsize + 1;
	m_pBase = (T *)malloc(sizeof(T) * m_maxsize);
	m_front = m_rear = 0;
}

template <class T>
void WQueue<T>::Destroy()
{  
	if (NULL == m_pBase)
	{
		free(m_pBase);
		m_pBase = NULL;
	}
	m_maxsize = 0;
	m_front = m_rear = 0;
}

template <class T>
bool WQueue<T>::Enqueue(T val)
{
	int nWrite = m_rear + 1;
	if (nWrite >= m_maxsize)
	{
		nWrite = 0;
	}
	if(nWrite == m_front)
		return false;
	else
	{
		m_pBase[m_rear] = val;
		m_rear = nWrite;
		return true;
	}
}

template <class T>
bool WQueue<T>::Dequeue(T &val)
{
	if(m_rear == m_front)
	{
		return false;
	}
	else
	{
		val = m_pBase[m_front];
		int nRead = m_front + 1;
		if (nRead >= m_maxsize)
		{
			m_front = 0;
		}
		else
		{
			m_front = nRead;
		}
		return true;
	}
}

template <class T>
int WQueue<T>::GetValueNum()
{
	int nNum = m_rear - m_front;
	if (nNum < 0)
	{
		nNum += m_maxsize;
	}
	return nNum;
}

template <class T>
void WQueue<T>::GetValueList(std::vector<T>& arrVal)
{
	int nTmp = m_front;
	while (nTmp != m_rear)
	{
		arrVal.push_back(nTmp);
		if (++nTmp >= m_maxsize)
		{
			nTmp = 0;
		}
	}
}