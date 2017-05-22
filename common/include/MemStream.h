////////////////////////////////////////////////////////////
//Copyright (C), 2012-2012, 北京数码视讯科技股份有限公司
//文件名 : MemStream.h
//描  述 :  这个类中的缓存数据会随着数据量写入等而变换地址，
//			绝对不要引用传出的缓存数据地址。
//		    该类是线程不安全的。
//历  史 : V1.0  why 2012-10-10 创建   
//		   V1.1  why 2012-12-20 修改
//		   V1.2	 why 2014-10-20 修改实现
//////////////////////////////////////////////////////////

#pragma once
typedef unsigned char UINT8;
class CMemStream
{
public:
	CMemStream();
	CMemStream(int, bool bAutoAdd = true);
	CMemStream(UINT8*, int); //attach数据
 CMemStream(char*, int); //attach数据
	CMemStream(const CMemStream&);
	virtual ~CMemStream();
	CMemStream& operator=(const CMemStream& obj);
	CMemStream& operator+=(const CMemStream& obj);
	UINT8& operator[](int nIndex);
	//加载数据
	void Attach(UINT8*, int);
	//获取数据
	const UINT8* GetMemAddress() const{return m_pData;}
	//获取数据不移动位置
	UINT8* GetBuffer(int nStartPos = 0, int nSize = 0);
	const UINT8* GetConstBuffer(int nStartPos = 0, int nSize = 0) const;
	//获取当前位置的数据
	UINT8* GetCurrentPosData(int nSize = 0){return GetBuffer(m_nCurPos, nSize);}
	//获取数据并移动位置
	int Read(UINT8*, int);
	//获取数据并移动位置
	UINT8* Read(int);
	//压入数据
	bool Write(const UINT8*, int);
	bool Write(const char*, int);
	//压入数据
	bool Write(UINT8, int nSize = 1);
	//重新设置
	void ResetPos(){ m_nCurPos = 0;}
	//设置位置
	int SeekPos(int nPos);	
	//向前移动位置
	int Move(int nBytes);
	//设置容量
	int SetCapaciy(int nSize);

	//清空数据
	void Empty(){m_nCurPos = 0; m_nSize = 0;}

	//获取大小
	int GetLength() const {return m_nSize;}
	//设置大小
	int SetLength(int nSize);
	//获取当前大小
	int GetCurPos() const {return m_nCurPos;}
	//获取剩余数据大小
	int GetLeftDataSize() const {return m_nSize - m_nCurPos;}
	//获取剩余空间大小
	int GetLeftCapacitySize() const {return m_nCapacity - m_nCurPos;}
private:
	UINT8* m_pData; //分配的数据
	int m_nSize;	//数据大小
	int m_nCurPos;  //当前位置
	int m_nCapacity; //缓存容量
	bool m_bAttach; //是否是attach数据
	bool m_bAutoAdd; //是否自动增加
};
