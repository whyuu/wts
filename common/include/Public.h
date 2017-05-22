//////////////////////////////////////////////////////////
//Copyright (C), 2012-2012, ����������Ѷ�Ƽ��ɷ����޹�˾
//�ļ��� : Public.h
//��  �� : �ṩ�������õĺ����ӿ�
//��  ʷ : 
//////////////////////////////////////////////////////////
#pragma once
#include <time.h>
#include <string>
#include <vector>
#if (defined WIN32) || (defined _WINDLL)
#include <Windows.h>
#endif
#include "commType.h"
namespace Public
{
	using namespace std;
	//��  �� : ��ȡִ�г���Ĭ��·��
	//��  �� : 
	//��  �� : Ĭ��·���ַ������ַ�������Ѿ���ӡ�\��
	extern string getCurrentDirectory();
	//������ �ж�һ��ip��ַ�Ƿ���Ч���Ƿ����鲥
	//����:  bMultiple�� ��ip�Ƿ�ִ���Ƿ����鲥���жϣ�
	//���أ� true��ip��Ч������ж��Ƿ����鲥��Ϊip��Ч�����鲥��ַ����false:����
	extern bool CheckIpV4(const string& strIp, bool bMultiple = false);
	//������ �ж�һ��ip�Ƿ����鲥
	//���أ� true�����鲥
	//		 false�����鲥
	extern bool CheckIpV4Multiple(UINT32 uIp);
	//��  �� :�����ַ�����ȡIP��ַ�Ƿ���Ч
	//��  �� :
	//��  �� : 
	extern bool GetIpV4(const string& strIp, UINT32& uIp);
	extern bool GetIpV4(const char* szIp, UINT32& uIp);
	extern void GetStrIpV4(UINT32 uIp, string& strIp);
	extern string GetStrIpV4(UINT32 uIp);

	//�� �����ָ��ַ�
	//�� ����srcԭ�ַ���
	//		 streDelims�ָ��
	//		 strDest�ָ��������
	//�� �أ�
	extern int split(const string & strSrc, const string& strDelims, vector<UINT32>& strDest);


	//�� �����ַ�ת���ӿڣ�Ŀǰֻʵ����linux�ӿ�
	//�� ����
	//�� �أ�
	extern int code_convert(char *from_charset,char *to_charset,char *inbuf,size_t inlen,char *outbuf,size_t outlen);

	//�� ������ȡ�߳�id
	//�� ����
	//�� �أ�
	extern int GetCurrentThreadId();

  //�� ������ȡ����id
  //�� ����
  //�� �أ�
  extern int GetCurrentProcessId();

  //�� ������ȡ��ǰϵͳʱ��
  //�� ����
  //�� �أ�
  extern string getCurrentTime(const char* pfmt = "%Y-%m-%d %H:%M:%S");

  //��ȡ����cpu���ڴ漰�߳���
  extern bool GetCpuMem(int pid, float &cpu ,float &mem, int& nThreadNum, int tid = 0);

  //�������ƻ�ȡpid
  extern bool GetPidByName(int *pid, const char *task_name);

#if (!defined WIN32) && (!defined _WINDLL)
			static inline unsigned long long GetSysTime()
			{
				unsigned long long timehi = 0, timelo = 0;
				__asm__ __volatile__ ("rdtsc":"=d"((unsigned long long)timehi), "=a"((unsigned long long)timelo)::);
				return ((unsigned long long)timehi << 32) + (unsigned long long)timelo;
			}
#endif

// #ifdef __cplusplus
// 		}
// #endif

  static inline UINT32 GetTickCount()
  {
#if (defined WIN32) || (defined _WINDLL)
    return ::GetTickCount();
#else
    //return GetSysTime() / 1000000;
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
#endif
  }

  extern bool CreateDirectory(const std::string& strPath);
  extern bool RemoveFiles(const std::string& strPath);

  extern time_t FormatTime(const char * szTime, const char* fmt /*= "%4d%2d%2d%2d%2d%2d"*/);
  extern string FormatTime(time_t time, const char* fmt /*="%Y-%m-%d %H:%M:%S"*/);

  extern void StringReplace(string& strValue, char src, char dst);

  extern UINT64 GetDirectorySize(const string& strPath);

  extern string RunShellCmd(const string &cmd);

  extern void CloseAllFD();
}