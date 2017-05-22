#ifdef _WIN32
#include <process.h>
#include <io.h>
#else
#include <unistd.h>  //包含了Linux C 中的函数getcwd()
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "WUdpSocket.h"
#include "boost/format.hpp"
#include "WLog.h"
#include "tinyxml.h"
#include "Public.h"
#include <time.h>
#include "commType.h"
#include <sys/stat.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
using namespace std;
#ifdef _WIN32
int gettimeofday(struct timeval *tp, void *tzp)
{
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;
	GetLocalTime(&wtm);
	tm.tm_year     = wtm.wYear - 1900;
	tm.tm_mon     = wtm.wMonth - 1;
	tm.tm_mday     = wtm.wDay;
	tm.tm_hour     = wtm.wHour;
	tm.tm_min     = wtm.wMinute;
	tm.tm_sec     = wtm.wSecond;
	tm. tm_isdst    = -1;
	clock = mktime(&tm);
	tp->tv_sec = (long)clock;
	tp->tv_usec = wtm.wMilliseconds * 1000;
	return (0);
}
#else
#include <sys/time.h>
#endif

using namespace boost::gregorian;
using namespace boost::posix_time; 

typedef union WLogPrefix
{
	struct FLVL{
		UINT8 uLevel:3;
		UINT8 uFacility:5;
	} flvl;
	UINT8 uflvl;
} WLogPrefix;

//日志对象缓存
struct LogItem
{
	WLogPrefix flag;
	timeval time;
	string strLog;
};

struct LogItemEx
{
	WLogPrefix flag;
	tm tmDate;
	long uSecond;
	string strLog;
};

/************************************************************************/
/* 日志sink接口                                                         */
/************************************************************************/
class CWLogSink
{
public:
	virtual ~CWLogSink(){};
	virtual void Push(LogItem& item) = 0;
	virtual void WriteLog() = 0;
	string getType(){return m_strTypeName;}
	void Init(TiXmlElement *item)
	{
		m_flvl.uflvl = 0xFF;
		m_unfacility = 0xFF;
		const char* pChar = item->Attribute("level");
		if (pChar)
		{
			m_flvl.flvl.uLevel = atoi(pChar);
		}
		pChar = item->Attribute("facility");
		if (pChar)
		{
			m_flvl.flvl.uFacility = atoi(pChar);
		}
		pChar = item->Attribute("unfacility");
		if (pChar)
		{
			m_unfacility = atoi(pChar);
		}
	}
	bool CheckValid(WLogPrefix uVal)
	{
		if (uVal.flvl.uLevel > m_flvl.flvl.uLevel
			|| uVal.flvl.uFacility == m_unfacility)
		{
			return false;
		}
		if (m_flvl.flvl.uFacility == 0x1f
			||uVal.flvl.uFacility == m_flvl.flvl.uFacility)
		{
			return true;
		}
		return false;
	}
protected:
	UINT8 m_unfacility;
	WLogPrefix m_flvl;
	string m_strTypeName;
};
/************************************************************************/
/*文件日志																*/
/************************************************************************/
class CWFileLog : public CWLogSink
{
public:
	bool Init(TiXmlElement *item)
	{
		CWLogSink::Init(item);
		m_uCurrentSize = 0;
		m_uFileSize = 50;
		const char* pChar = item->Attribute("maxFileSize");
		if (pChar)
		{
			m_uFileSize = atoi(pChar);
		}
		m_uFileSize *= 1000000;
		m_nSayDays = 30;
		pChar = item->Attribute("saveDays");
		if (pChar)
		{
			m_nSayDays = atoi(pChar);
		}
		string strTmp = "LogFile";
		pChar = item->Attribute("path");
		if (pChar)
		{
			strTmp = pChar;
		}
		m_strPath = Public::getCurrentDirectory() + strTmp + "/";  //路径
#if (defined WIN32) || (defined _WINDLL)
		//路径的可移植
		namespace fs = boost::filesystem;
		fs::path full_path( fs::initial_path() );
		full_path = fs::system_complete(fs::path(m_strPath, fs::native));
		//判断各级子目录是否存在，不存在则需要创建
		if ( !fs::exists( full_path ) )
		{
			// 创建多层子目录full_path = {m_pathname="D:\Dep1\svn\cluster\cluster1.0.4rc5\switch\bin\SysLog\" }
			bool bRet = fs::create_directory(full_path);
			if (false == bRet)
			{
				return false;
			}
		}
#else
		DIR *dir;
		if( (dir=opendir(m_strPath.c_str())) == NULL)
		{
			//fprintf(stderr, "Can't open dir %s, reason = %s\n", strPath, strerror(errno));
			int iRet = mkdir(m_strPath.c_str(), 0755);
			if (iRet != 0)
			{
				fprintf(stderr, "create dir(%s) error, reason = %s\n" ,m_strPath.c_str(), strerror(errno));
				return false;
			}
		}
		closedir(dir);
#endif
		m_strTypeName = "file";  //类型
		return true;
	}
	void Push(LogItem& item)
	{
		LogItemEx tmpL;
		tmpL.strLog = item.strLog;
		tmpL.flag = item.flag;
		time_t tmp = item.time.tv_sec;
		tmpL.tmDate = *localtime(&tmp);
		tmpL.uSecond= item.time.tv_usec;
		if (tmpL.tmDate.tm_yday != m_yday)
		{
			WriteLog();
			CheckLogFile(m_strPath);
			char buf[] = "1970-01-01";
			(void)strftime(buf, sizeof(buf), "%Y-%m-%d", &tmpL.tmDate);
			sprintf(m_filePath, "%s%s.log", m_strPath.c_str(), buf);
			struct  stat file_stat;
			if ( 0 == stat(m_filePath, &file_stat))
			{
				m_uCurrentSize = (UINT32)file_stat.st_size;
			}
			else
			{
				m_uCurrentSize = 0;
			}
			m_yday = tmpL.tmDate.tm_yday;
		}
		m_arrLog.push_back(tmpL);
	}
	void WriteLog()
	{
		if (m_arrLog.size() > 0)
		{
			FILE *pf = fopen(m_filePath, "a+");
			if(pf == NULL)
			{
				return;
			}
			for (UINT32 i = 0; i < m_arrLog.size(); i++)
			{
				LogItemEx& tmpL = m_arrLog[i];
				UINT32 ufacility = tmpL.flag.flvl.uFacility;
				UINT32 ulevel = tmpL.flag.flvl.uLevel;
				char buf[] = "00:00:00.";
				UINT32 uS = tmpL.uSecond/1000;
				(void)strftime(buf, sizeof(buf), "%H:%M:%S.", &tmpL.tmDate);
				string strLog =  str(boost::format("[%d,%d] %s%03d: %s\n")
					%ufacility%ulevel%buf%uS
					%tmpL.strLog.c_str());
				if (fwrite(strLog.c_str(), strLog.size(), 1, pf))
				{
					m_uCurrentSize += strLog.size();
				}			
			}
			fclose(pf);
			m_arrLog.clear();
			if (m_uCurrentSize > m_uFileSize)
			{
				char tmpBuf[1024];
				int nSize = (int)strlen(m_filePath);
				memcpy(tmpBuf, m_filePath, nSize - 4);
				memcpy(&tmpBuf[nSize - 4], "b.log\0", 6);
				remove(tmpBuf);
				rename(m_filePath , tmpBuf );
				m_uCurrentSize = 0;
			}
		}
	}
	void CheckLogFile(string folderPath)
	{
#if (defined WIN32) || (defined _WINDLL)
		_finddata_t FileInfo;
		string strfind = folderPath + "\\*";
		long Handle = (long)_findfirst(strfind.c_str(), &FileInfo);

		if (Handle == -1L)
		{
			cerr << "can not match the folder path" << endl;
			return;
		}
		do{
			//判断是否有子目录
			if (!(FileInfo.attrib & _A_SUBDIR))
			{
				string filename = (folderPath + "\\" + FileInfo.name);
				struct  stat file_stat;
				stat(filename.c_str(), &file_stat);
				time_t tNow;
				time(&tNow);
				time_t tCreate = file_stat.st_ctime;
				int nSpenDay = (int)((tNow - tCreate) / (24 * 3600));
				//fprintf(stderr, "currfile= %s, nSpenDay = %d\n", currfile, nSpenDay);
				if(nSpenDay > m_nSayDays)
				{
					remove(filename.c_str());
				}
			}
		}while (_findnext(Handle, &FileInfo) == 0);
		_findclose(Handle);
#else
		DIR *dir;
		struct dirent *s_dir;
		struct  stat file_stat;
		const char* path = folderPath.c_str();
		char currfile[1024]={0};
		if( (dir=opendir(path)) == NULL)
		{
			fprintf(stderr, "Can't open dir %s, reason = %s\n", path, strerror(errno));
			return;
		}
		while((s_dir=readdir(dir))!=NULL)
		{
			if((strcmp(s_dir->d_name,".")==0)||(strcmp(s_dir->d_name,"..")==0))
				continue;
			sprintf(currfile,"%s%s",path,s_dir->d_name);
			stat(currfile,&file_stat);
			if(!S_ISDIR(file_stat.st_mode))
			{	//delete log file
				time_t tNow;
				time(&tNow);
				time_t tModify = file_stat.st_mtime;
				int nSpenDay = (tNow - tModify) / (24 * 3600);
				if(nSpenDay > m_nSayDays)
				{
					int nRes = remove(currfile);
					if(nRes != 0)
					{
						fprintf(stderr, "delete log file error, currfile= %s, nRes= %d, reason = %s\n", currfile, nRes, strerror(errno));
					}
				}
			}
		}
		closedir(dir);
#endif
	}
private:
	int m_yday;
	char m_filePath[1024];
	char m_logBuf[2048];
	string m_strPath;
	vector<LogItemEx> m_arrLog;
	UINT32 m_uFileSize;
	UINT32 m_uCurrentSize;
	int m_nSayDays;
};

/************************************************************************/
/* syslod日志，通过udp发送                                              */
/************************************************************************/
class CWUdpLog : public CWLogSink
{
public:
	bool Init(TiXmlElement *item)
	{
		m_strTypeName = "udp";
		CWLogSink::Init(item);
		const char* pChar = item->Attribute("dIp");
		if (pChar)
		{
			m_uDstIp = inet_addr(pChar);
		}
		else
		{
			return false;
		}
		pChar = item->Attribute("dPort");
		if (pChar)
		{
			m_uDstPort = (UINT16)atoi(pChar);
		}
		else
		{
			return false;
		}
		string strTmp = "unknown";
		pChar = item->Attribute("modelName");
		if (pChar)
		{
			m_strModelName = pChar;
		}
		strTmp = "unknown";
		pChar = item->Attribute("hostIp");
		if (pChar)
		{
			m_strHostIp = pChar;
		}
		m_udpSocket.Create();
		m_uPid = getpid();
		return true;
	}
	void Push(LogItem& item)
	{
		UINT32 uVal = item.flag.uflvl;
		time_t tmp = item.time.tv_sec;
		struct tm myTm = *localtime(&tmp);
		(void)strftime(buf, sizeof(buf), "%Y%m%d %H:%M:%S.", &myTm);
		UINT32 uS = item.time.tv_usec/1000;
		if (item.strLog.size() > 1200) //udp可能会超过最大包
		{
			item.strLog.resize(1200);
		}
		string strLog = str(boost::format("<%d>%s%03d %s %s[%d]:%s")
			%uVal%buf%uS
			%m_strHostIp.c_str()
			%m_strModelName.c_str()%m_uPid
			%item.strLog.c_str());
		m_udpSocket.Send(strLog.c_str(), (int)strLog.size(), m_uDstIp, m_uDstPort);
	}
	void WriteLog(){}
	void SetModelName(string strName){m_strModelName = strName;}
	void SetPid(UINT32 uPid){m_uPid = uPid;}
	void SetHostIp(string strHostIp){m_strHostIp = strHostIp;}
private:
	string m_strModelName;
	UINT32 m_uDstIp;
	string m_strHostIp;
	UINT32 m_uPid;
	UINT16 m_uDstPort;
	CWUdpSocket m_udpSocket;
	char buf[sizeof("19700101 00:00:00.") + 1];
};

/************************************************************************/
/* 统一日志管理                                                         */
/************************************************************************/
CWLog::CWLog()
{
	string strPath = Public::getCurrentDirectory();
	string strCfg = strPath + "log.xml";
	const char* pPath = strCfg.c_str();
	//读配置文件
	TiXmlDocument doc;
	if (access(pPath, 0) != 0) 
	{
		TiXmlElement root("log");
		TiXmlElement itemF("item");

		itemF.SetAttribute("type", "file");
		root.InsertEndChild(itemF);

		TiXmlElement itemU("item");
		itemU.SetAttribute("type", "udp");
		root.InsertEndChild(itemU);

		doc.InsertEndChild(root);
		doc.SaveFile(pPath);
	}

	if (!doc.LoadFile(pPath, TIXML_DEFAULT_ENCODING))
	{
		//加载文件失败
		fprintf(stderr, "load error\n");
		return;
	}
	if (!doc.Parse(pPath))
	{
		fprintf(stderr, "parse error\n");
		return;
	}

	TiXmlElement *root = doc.RootElement();
	TiXmlElement *item = root->FirstChildElement("item");
	while(item)
	{
		string strType;
		//fprintf(stderr, "id = %s, ip = %s\n", elementDev->Attribute("id"), elementDev->Attribute("ip"));
		do 
		{
			const char* pChar = item->Attribute("type");
			if (!pChar)
			{
				break;
			}
			strType = pChar;
			if (strType == "file")
			{
				CWFileLog* pLog = new CWFileLog();
				bool bRes = pLog->Init(item);
				if (!bRes)
				{
					delete pLog;
					break;
				}
				m_arrLog.push_back(pLog);
			}
			else if (strType == "udp")
			{
				CWUdpLog* pLog = new CWUdpLog();
				bool bRes = pLog->Init(item);
				if (!bRes)
				{
					delete pLog;
					break;
				}
				m_arrLog.push_back(pLog);
			}
		} while (0);
		item = item->NextSiblingElement("item");
	}
	//初始化写线程
	m_bFlag = true;
	boost::function0<void> f = boost::bind(&CWLog::OnWriteLogThread, this); 
	m_thSend = boost::thread(f);
}
CWLog::~CWLog()
{
	if (m_bFlag)
	{
		m_bFlag = false;
		m_logEvent.SetEvent();
		m_thSend.join();
	}
}
CWLog& CWLog::GetInstance()
{
	static CWLog log;
	return log;
}
void CWLog::WriteLog(UINT8 uflvl, const char * pfmt, ...)
{
	boost::posix_time::ptime time = boost::posix_time::microsec_clock::local_time();
	va_list argList;
	va_start(argList, pfmt);
	char pChar[1224] = {0};
	vsprintf(pChar, pfmt, argList);
	va_end(argList);
	WriteStringLog(uflvl, pChar);
}
void CWLog::WriteStringLog(UINT8 uflvl, const string& strLog)
{
	LogItem item;
	item.strLog = strLog;
	item.flag.uflvl = uflvl;
	gettimeofday(&item.time, NULL);
	//item.time = boost::posix_time::microsec_clock::local_time();
	CWnScopedLock Lock(m_itemLock);
	if (m_arrItemLog.size() < 30000)  //限制是一种保护
	{
		m_arrItemLog.push_back(item);
		m_logEvent.SetEvent();
	}
}
void CWLog::SetLevel(UINT8 uLevel)
{
}

int CWLog::OnWriteLogThread()
{
//	WriteLog(0, "thread %d(CWLog::OnWriteLogThread) start ", Public::GetCurrentThreadId());
	vector<LogItem> arrItem;
	while(m_bFlag)
	{
		m_logEvent.wait();
		{
			CWnScopedLock Lock(m_itemLock);
			arrItem = m_arrItemLog;
			m_arrItemLog.clear();
		}
		if (arrItem.size() > 0)
		{
			for (UINT32 i = 0; i < m_arrLog.size(); i++)
			{
				CWLogSink* pLog = m_arrLog[i];
				for (UINT32 j = 0; j < arrItem.size(); j++)
				{
					LogItem& tmp = arrItem[j];
					if (!pLog->CheckValid(tmp.flag)) continue;
					pLog->Push(tmp);
				}
				pLog->WriteLog();
			}
			arrItem.clear();
		}
	}
	return 0;
}