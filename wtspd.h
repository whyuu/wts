#pragma once
class CTsPayloadDealer
{
public:
	CTsPayloadDealer(WORD uPID): m_wPID(uPID),m_memStream(int(65535)){}
	virtual ~CTsPayloadDealer(){}
	//�ɹ����һ�����ݵķ���������1�����򷵻�0 
	int DealPayloadStartData(BYTE*, int);
	//�������ֵʱ����ջ��沢����1�����򷵻�0 
	int AppendPayloadData(BYTE*, int);
	WORD GetPid() const {return m_wPID;}
protected:
	//�ɹ�ʱ����1��δ��ʱ����0
	virtual int DealPayload(CMemStream& dat) = 0;
private:
	WORD m_wPID;
	CMemStream m_memStream;
};
