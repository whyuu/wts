#pragma once
class CTsPayloadDealer
{
public:
	CTsPayloadDealer(WORD uPID): m_wPID(uPID),m_memStream(int(65535)){}
	virtual ~CTsPayloadDealer(){}
	//成功完成一段数据的分析，返回1，否则返回0 
	int DealPayloadStartData(BYTE*, int);
	//超出最大值时，清空缓存并返回1，否则返回0 
	int AppendPayloadData(BYTE*, int);
	WORD GetPid() const {return m_wPID;}
protected:
	//成功时返回1，未完时返回0
	virtual int DealPayload(CMemStream& dat) = 0;
private:
	WORD m_wPID;
	CMemStream m_memStream;
};
