#if (defined WIN32) || (defined _WINDLL)
	#include <windows.h>
#define popen _popen
#define pclose _pclose
#else
	#include <unistd.h>  //包含了Linux C 中的函数getcwd()
	#include <dirent.h>
	#include <iconv.h>
	#include <string.h>
	#include <sys/syscall.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <arpa/inet.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <boost/filesystem.hpp>
#include "Public.h"
namespace Public
{
	//描  述 : 获取执行程序默认路径
	//参  数 : 
	//返  回 : 默认路径字符串
	string getCurrentDirectory()
	{
		string fullpath;
	#if (defined WIN32) || (defined _WINDLL)
		WCHAR path[MAX_PATH];
		char chpath[MAX_PATH];
		GetModuleFileName(NULL,path,sizeof(path));
		setlocale(LC_ALL,"");
		wcstombs(chpath,path,MAX_PATH);
		setlocale(LC_ALL,"C"); 
		*(strrchr(chpath , '\\')) = '\0';
		fullpath = chpath;
		fullpath = fullpath + "\\";
		//fullpath = boost::filesystem::initial_path<boost::filesystem::path>().string();
	#else
#define MAX_SIZE 256
		char current_absolute_path[MAX_SIZE];
		//获取当前程序绝对路径
		int cnt = readlink("/proc/self/exe", current_absolute_path, MAX_SIZE);
		if (cnt < 0 || cnt >= MAX_SIZE)
		{
			printf("***Error***\n");
			exit(-1);
		}
		//获取当前目录绝对路径，即去掉程序名
		int i;
		for (i = cnt; i >=0; --i)
		{
			if (current_absolute_path[i] == '/')
			{
				current_absolute_path[i+1] = '\0';
				break;
			}
		} 
		fullpath = current_absolute_path;
// 		char p_dir[1024] = {0};
// 		getcwd(p_dir, 1024);
// 		fullpath = p_dir;
// 		fullpath = fullpath + "/";
	#endif
		return fullpath;
	}

    string GetStrIpV4(UINT32 uIp)
    {
        struct sockaddr_in sTmpIp;
        sTmpIp.sin_addr.s_addr = uIp; 
        string strIp(inet_ntoa(sTmpIp.sin_addr));
        return strIp;
    }

    string getCurrentTime(const char* pfmt)
    {
        time_t t;  //秒时间  
        tm* local; //本地时间   
        char buf[32]= {0};  

        t = time(NULL); //获取目前秒时间  
        local = localtime(&t); //转为本地时间  
        strftime(buf, 32, pfmt, local);   
        return buf;
    }

	bool CheckIpV4(const string& strIp, bool bMultiple)
	{
		if (strIp.size() < 7 || strIp.size() > 15)
		{
			return false;
		}
		int a, b, c, d;
		if(4 == sscanf(strIp.c_str(),"%d.%d.%d.%d", &a, &b, &c, &d))
		{
			if (0<=a && a<=255
				&& 0<=b && b<=255
				&& 0<=c && c<=255
				&& 0<=d && d<=255)
			{
				if (bMultiple)
				{
					if (a < 224 || a >= 240)
					{
						return false;
					}
				}
				return true;
			}
		}
		return false;
	}
 bool CheckIpV4Multiple(UINT32 uIp)
 {
   in_addr tmp;
   tmp.S_un.S_addr = uIp;
   if (tmp.S_un.S_un_b.s_b1 >= 224
     && tmp.S_un.S_un_b.s_b1 <= 239)
   {
     return true;
   }
   return false;
 }
	int split(const string & strSrc, const string& strDelims, vector<UINT32>& strDest)
	{
		typedef std::string::size_type ST;  
		string delims = strDelims;  
		std::string STR;  
		if(delims.empty()) delims = "/n/r";  


		ST pos=0, LEN = strSrc.size();  
		while(pos < LEN )
		{  
			STR="";   
			while((delims.find(strSrc[pos]) != std::string::npos) && (pos < LEN) ) 
			{
				++pos;  
			}
			if(pos==LEN)
			{	
				return (int)strDest.size();  
			}
			while( (delims.find(strSrc[pos]) == std::string::npos) && (pos < LEN) ) 
			{	
				STR += strSrc[pos++];  
			}
			if( ! STR.empty() ) 
			{
				strDest.push_back(atoi(STR.c_str()));  
			}
		}  
		return (int)strDest.size();  
	}   

 
	int code_convert(char *from_charset,char *to_charset,char *inbuf,size_t inlen,char *outbuf,size_t outlen)
	{
#if (defined WIN32) || (defined _WINDLL)
		return 0;
#else
		iconv_t cd;
		char **pin = &inbuf;
		char **pout = &outbuf;

		cd = iconv_open(to_charset,from_charset);
		if (cd==0)
		{
			return -1;
		}
		memset(outbuf,0,outlen);
		if ((int)iconv(cd, pin, (size_t*)&inlen, pout, (size_t*)&outlen) == -1)
		{
			return -1;
		}
		iconv_close(cd);
#endif
		return 0;
	}
	int GetCurrentThreadId()
	{
#if (defined WIN32) || (defined _WINDLL)
		return ::GetCurrentThreadId();
#else
		return syscall(SYS_gettid);
#endif
	}

    int GetCurrentProcessId()
    {
#if (defined WIN32) || (defined _WINDLL)
        return ::GetCurrentProcessId();
#else
        return getpid();
#endif
    }


 /*************************************  
 *功能描述：   
 *参数1:
 *参数2:
 *返回值: 
 *创建日期： 2016-10-27
 *作者:	   why
 *修改：
 ***************************************/
 bool CreateDirectory(const std::string& strPath)
 {
//#if (defined WIN32) || (defined _WINDLL)
   //路径的可移植
   namespace fs = boost::filesystem;
   fs::path full_path( fs::initial_path() );
   full_path = fs::system_complete(fs::path(strPath));

   //判断各级子目录是否存在，不存在则需要创建
   if ( !fs::exists( full_path ) )
   {
     // 创建多层子目录full_path = {m_pathname="D:\Dep1\svn\cluster\cluster1.0.4rc5\switch\bin\SysLog\" }
     fs::create_directories(full_path);
     if (!fs::is_directory(full_path))
     {
       return false;
     }
   }
// #else
//    DIR *dir;
//    if( (dir=opendir(strPath.c_str())) == NULL)
//    {
//      //fprintf(stderr, "Can't open dir %s, reason = %s\n", strPath, strerror(errno));
//      int iRet = mkdir(strPath.c_str(), 0755);
//      if (iRet != 0)
//      {
//        fprintf(stderr, "create dir(%s) error, reason = %s\n" ,strPath.c_str(), strerror(errno));
//        return false;
//      }
//    }
//    closedir(dir);
// #endif
   return true;
 }
 bool RemoveFiles(const std::string& strPath)
 {
   namespace bf = boost::filesystem;//简单别名
   bf::path path(strPath);
   bf::remove_all(path);
   return !bf::exists(path);
 }

 time_t FormatTime(const char * szTime, const char* fmt)
 {
   struct tm tm1;
   time_t time1;
   sscanf(szTime, fmt,
     &tm1.tm_year,
     &tm1.tm_mon,
     &tm1.tm_mday,
     &tm1.tm_hour,
     &tm1.tm_min,
     &tm1.tm_sec);
   tm1.tm_year -= 1900;
   tm1.tm_mon --;
   tm1.tm_isdst=-1;
   time1 = mktime(&tm1);
   return time1;
 }

 string FormatTime(time_t t, const char* pfmt)
 {
   tm* local; //本地时间   
   char buf[64]= {0};  

   local = localtime(&t); //转为本地时间  
   strftime(buf, 64, pfmt, local);
   return buf;
 }
 void StringReplace(string& strValue, char src, char dst)
 {
   for (size_t i = 0; i < strValue.size(); i++)
   {
     if (strValue[i] == src)
     {
       strValue[i] = dst;
     }
   }
 }

 UINT64 GetDirectorySize(const string& strPath)
 {
   namespace fs = boost::filesystem;
   boost::uintmax_t uSize = 0;
   fs::path p(strPath);
   fs::recursive_directory_iterator end_iter;
   for(fs::recursive_directory_iterator iter(p); iter != end_iter; iter++)
   {
     if (fs::is_directory( *iter ) )
     {
     }
     else
     {
       uSize += fs::file_size(iter->path());
     }
   }
   return uSize;
 }

 string RunShellCmd(const string &cmd) {
   int32_t count(2048);
   char s[2048];
   string ret;

   FILE* stream = popen(cmd.c_str(), "r");

   if (stream != NULL) {
     // 每次从stream中读取指定大小的内容
     while (fgets(s, count, stream))
       ret += s;
     pclose(stream);
   }

   return ret;
 }

 void CloseAllFD()
 {
#if !(defined WIN32) || (defined _WINDLL)
//    struct rlimit lim;
//    unsigned int i;
// 
//    if (getrlimit(RLIMIT_NOFILE, &lim) < 0)
//      return;
//    if (lim.rlim_cur == RLIM_INFINITY)
//      lim.rlim_cur = 1024;
//    for (i = 0; i < lim.rlim_cur; i ++) {
// #ifdef MYPERF
//      if (i == 1)
//        continue;
// #endif
//      if (close(i) < 0 && errno != EBADF)
//        return;
//    }
   int fds = sysconf(_SC_OPEN_MAX);
   for   (int  i = 0;   i  < fds; i++)
   {
     if (i == 1)
       continue;
     close(i);
   }
#endif
 }
}

