//////////////////////////////////////////////////////////
//Copyright (C), 2012-2012, 北京数码视讯科技股份有限公司
//文件名 : Public.h
//描  述 : 提供公共调用的函数接口
//历  史 : 
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
	//描  述 : 获取执行程序默认路径
	//参  数 : 
	//返  回 : 默认路径字符串，字符串最后已经添加“\”
	extern string getCurrentDirectory();
	//描述： 判断一个ip地址是否有效，是否是组播
	//参数:  bMultiple： 对ip是否执行是否是组播的判断；
	//返回： true：ip有效（如果判断是否是组播则为ip有效且是组播地址）；false:其他
	extern bool CheckIpV4(const string& strIp, bool bMultiple = false);
	//描述： 判断一个ip是否是组播
	//返回： true：是组播
	//		 false：非组播
	extern bool CheckIpV4Multiple(UINT32 uIp);
	//描  述 :根据字符串获取IP地址是否有效
	//参  数 :
	//返  回 : 
	extern bool GetIpV4(const string& strIp, UINT32& uIp);
	extern bool GetIpV4(const char* szIp, UINT32& uIp);
	extern void GetStrIpV4(UINT32 uIp, string& strIp);
	extern string GetStrIpV4(UINT32 uIp);

	//描 述：分割字符
	//参 数：src原字符串
	//		 streDelims分割符
	//		 strDest分割后整形数
	//返 回：
	extern int split(const string & strSrc, const string& strDelims, vector<UINT32>& strDest);


	//描 述：字符转换接口，目前只实现了linux接口
	//参 数：
	//返 回：
	extern int code_convert(char *from_charset,char *to_charset,char *inbuf,size_t inlen,char *outbuf,size_t outlen);

	//描 述：获取线程id
	//参 数：
	//返 回：
	extern int GetCurrentThreadId();

  //描 述：获取进程id
  //参 数：
  //返 回：
  extern int GetCurrentProcessId();

  //描 述：获取当前系统时间
  //参 数：
  //返 回：
  extern string getCurrentTime(const char* pfmt = "%Y-%m-%d %H:%M:%S");

  //获取进程cpu，内存及线程数
  extern bool GetCpuMem(int pid, float &cpu ,float &mem, int& nThreadNum, int tid = 0);

  //根据名称获取pid
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