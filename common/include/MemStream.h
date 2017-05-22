////////////////////////////////////////////////////////////
//Copyright (C), 2012-2012, ����������Ѷ�Ƽ��ɷ����޹�˾
//�ļ��� : MemStream.h
//��  �� :  ������еĻ������ݻ�����������д��ȶ��任��ַ��
//			���Բ�Ҫ���ô����Ļ������ݵ�ַ��
//		    �������̲߳���ȫ�ġ�
//��  ʷ : V1.0  why 2012-10-10 ����   
//		   V1.1  why 2012-12-20 �޸�
//		   V1.2	 why 2014-10-20 �޸�ʵ��
//////////////////////////////////////////////////////////

#pragma once
typedef unsigned char UINT8;
class CMemStream
{
public:
	CMemStream();
	CMemStream(int, bool bAutoAdd = true);
	CMemStream(UINT8*, int); //attach����
 CMemStream(char*, int); //attach����
	CMemStream(const CMemStream&);
	virtual ~CMemStream();
	CMemStream& operator=(const CMemStream& obj);
	CMemStream& operator+=(const CMemStream& obj);
	UINT8& operator[](int nIndex);
	//��������
	void Attach(UINT8*, int);
	//��ȡ����
	const UINT8* GetMemAddress() const{return m_pData;}
	//��ȡ���ݲ��ƶ�λ��
	UINT8* GetBuffer(int nStartPos = 0, int nSize = 0);
	const UINT8* GetConstBuffer(int nStartPos = 0, int nSize = 0) const;
	//��ȡ��ǰλ�õ�����
	UINT8* GetCurrentPosData(int nSize = 0){return GetBuffer(m_nCurPos, nSize);}
	//��ȡ���ݲ��ƶ�λ��
	int Read(UINT8*, int);
	//��ȡ���ݲ��ƶ�λ��
	UINT8* Read(int);
	//ѹ������
	bool Write(const UINT8*, int);
	bool Write(const char*, int);
	//ѹ������
	bool Write(UINT8, int nSize = 1);
	//��������
	void ResetPos(){ m_nCurPos = 0;}
	//����λ��
	int SeekPos(int nPos);	
	//��ǰ�ƶ�λ��
	int Move(int nBytes);
	//��������
	int SetCapaciy(int nSize);

	//�������
	void Empty(){m_nCurPos = 0; m_nSize = 0;}

	//��ȡ��С
	int GetLength() const {return m_nSize;}
	//���ô�С
	int SetLength(int nSize);
	//��ȡ��ǰ��С
	int GetCurPos() const {return m_nCurPos;}
	//��ȡʣ�����ݴ�С
	int GetLeftDataSize() const {return m_nSize - m_nCurPos;}
	//��ȡʣ��ռ��С
	int GetLeftCapacitySize() const {return m_nCapacity - m_nCurPos;}
private:
	UINT8* m_pData; //���������
	int m_nSize;	//���ݴ�С
	int m_nCurPos;  //��ǰλ��
	int m_nCapacity; //��������
	bool m_bAttach; //�Ƿ���attach����
	bool m_bAutoAdd; //�Ƿ��Զ�����
};
