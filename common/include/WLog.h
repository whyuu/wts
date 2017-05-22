//////////////////////////////////////////////////////////////
//Copyright (C), 2015-2015, 
//文件名 : WLog.h
//描  述 : 定义日志头文件，该日志遵循sysLog日志模式
//备  注 : 
//历  史 : V1.0. 2015-12-7 创建
//////////////////////////////////////////////////////////////
#pragma once
#include <string>
#include <vector>
#include "WnLock.h"
#include "WnEvent.h"
#ifndef UINT8
#include "commType.h"
#endif
/* level */
#define	LOG_EMERG	0	/* system is unusable */
#define	LOG_ALERT	1	/* action must be taken immediately */
#define	LOG_CRIT	2	/* critical conditions */
#define	LOG_ERR		3	/* error conditions */
#define	LOG_WARNING	4	/* warning conditions */
#define	LOG_NOTICE	5	/* normal but significant condition */
#define	LOG_INFO	6	/* informational */
#define	LOG_DEBUG	7	/* debug-level messages */

/* facility codes */
#define	LOG_KERN	(0<<3)	/* kernel messages */
#define	LOG_USER	(1<<3)	/* random user-level messages */
#define	LOG_MAIL	(2<<3)	/* mail system */
#define	LOG_DAEMON	(3<<3)	/* system daemons */
#define	LOG_AUTH	(4<<3)	/* security/authorization messages */
#define	LOG_SYSLOG	(5<<3)	/* messages generated internally by syslogd */
#define	LOG_LPR		(6<<3)	/* line printer subsystem */
#define	LOG_NEWS	(7<<3)	/* network news subsystem */
#define	LOG_UUCP	(8<<3)	/* UUCP subsystem */
#define	LOG_CRON	(9<<3)	/* clock daemon */
#define	LOG_AUTHPRIV	(10<<3)	/* security/authorization messages (private) */
/* Facility #10 clashes in DEC UNIX, where */
/* it's defined as LOG_MEGASAFE for AdvFS  */
/* event logging.                          */
#define	LOG_FTP		(11<<3)	/* ftp daemon */
#define	LOG_NTP		(12<<3)	/* NTP subsystem */
/* other codes through 15 reserved for system use */
#define	LOG_LOCAL0	(16<<3)	/* reserved for local use */
#define	LOG_LOCAL1	(17<<3)	/* reserved for local use */
#define	LOG_LOCAL2	(18<<3)	/* reserved for local use */
#define	LOG_LOCAL3	(19<<3)	/* reserved for local use */
#define	LOG_LOCAL4	(20<<3)	/* reserved for local use */
#define	LOG_LOCAL5	(21<<3)	/* reserved for local use */
#define	LOG_LOCAL6	(22<<3)	/* reserved for local use */
#define	LOG_LOCAL7	(23<<3)	/* reserved for local use */

class CWLogSink;
struct LogItem;
class CWLog
{
public:
	CWLog();
	~CWLog();
	//描  述 : 获取日志记录实例
	//参  数 : 
	//返  回 : 日志操作单件
	static CWLog& GetInstance();
	void WriteLog(UINT8 uflvl, const char *, ...);
	void WriteStringLog(UINT8 uflvl, const std::string& strLog);
	void SetLevel(UINT8 uLevel);
private:
	std::vector<CWLogSink*> m_arrLog;

	CWnLock m_itemLock;
	std::vector<LogItem> m_arrItemLog;

	boost::thread m_thSend;
	bool m_bFlag;
	CWnEvent m_logEvent;
	int OnWriteLogThread();
};

// #if 0
// #include <boost/log/core.hpp>
// #include <boost/log/trivial.hpp>
// #include <boost/log/expressions.hpp>
// #include <boost/log/sinks/text_file_backend.hpp>
// #include <boost/log/utility/setup/file.hpp>
// #include <boost/log/utility/setup/common_attributes.hpp>
// #include <boost/log/sources/severity_logger.hpp>
// #include <boost/log/sources/record_ostream.hpp>
// #include "boost/log/sinks/syslog_backend.hpp"
// #include <boost/log/sources/logger.hpp>
// #include <boost/log/common.hpp>
// #include <string>
// #include "commType.h"
// using namespace std;
// namespace logging = boost::log;
// namespace src = boost::log::sources;
// namespace sinks = boost::log::sinks;
// namespace keywords = boost::log::keywords;
// namespace trivial = boost::log::trivial;
// namespace attrs = boost::log::attributes;
// namespace expr = boost::log::expressions;
// typedef sinks::synchronous_sink< sinks::text_file_backend > file_sink;
// struct LogConfig
// {
// 	string strPath;
// 	string strFileName;
// 	string strFormat;
// 	string strHostIp;
// 	UINT32 uRotatSize;
// 	bool bSysLog;
// 	string strDstIp;
// 	UINT16 uPort;
// };
// class CWLog
// {
// public:
// 	static void Init(LogConfig& conf)
// 	{
// 		// And also add some attributes
//  		logging::core::get()->add_global_attribute("HostIp", attrs::constant<std::string>(conf.strHostIp));
// // 		logging::core::get()->add_global_attribute("RecordID", attrs::counter< unsigned int >());
// 
// 		if (conf.bSysLog)
// 		{
// 			init_builtin_syslog(conf.strDstIp, conf.uPort);
// 		}
// 		//init_filelog(conf.strFileName, conf.uRotatSize, conf.strFormat);
// 	}
// 	static void SetLevel(trivial::severity_level sl)
// 	{
// 		logging::core::get()->set_filter
// 			(
// 			logging::trivial::severity >= logging::trivial::info
// 			);
// 	}
// private:
// 	static void init_builtin_syslog(string strIp, unsigned short port = 514)
// 	{
// 		typedef sinks::synchronous_sink< sinks::syslog_backend > sink_t;
// 		boost::shared_ptr< logging::core > core = logging::core::get();
// 
// 		// Create a new backend
// 		boost::shared_ptr< sinks::syslog_backend > backend(new sinks::syslog_backend(
// 			keywords::facility = sinks::syslog::local5,
// 			keywords::use_impl = sinks::syslog::udp_socket_based
// 			));
// 
// 		// Setup the target address and port to send syslog messages to
// 		backend->set_target_address(strIp);
// 		// Create and fill in another level translator for "MyLevel" attribute of type string
// // 		sinks::syslog::custom_severity_mapping< std::string > mapping("MyLevel");
// // 		mapping["debug"] = sinks::syslog::debug;
// // 		mapping["normal"] = sinks::syslog::info;
// // 		mapping["warning"] = sinks::syslog::warning;
// // 		mapping["failure"] = sinks::syslog::critical;
// // 		backend->set_severity_mapper(mapping);
// //		backend->set_severity_mapper(sinks::syslog::direct_severity_mapping< int >("Severity"));
// 
// 		// Wrap it into the frontend and register in the core.
// 		boost::shared_ptr< sink_t > fSink = boost::make_shared< sink_t >(backend);
// 		fSink->set_formatter(
// 			expr::format("%1% %2%")
// 			% expr::attr< string >("HostIp")
// 			//% expr::attr< boost::posix_time::ptime >("TimeStamp")
// 			% expr::xml_decor[ expr::stream << expr::smessage ]
// 		);
// 		core->add_sink(fSink);
// 	}
// 	static void init_file_collecting(boost::shared_ptr< file_sink > sink)
// 	{
// 		sink->locked_backend()->set_file_collector(sinks::file::make_collector(
// 			keywords::target = "logs",                      /*< the target directory >*/
// 			keywords::max_size = 16 * 1024 * 1024,          /*< maximum total size of the stored files, in bytes >*/
// 			keywords::min_free_space = 100 * 1024 * 1024    /*< minimum free space on the drive, in bytes >*/
// 			));
// 	}
// 	//[ example_tutorial_file_advanced
// 	static void init_filelog(string strfName, UINT32 uRotateSize, string strFmt)
// 	{
// 		boost::shared_ptr< file_sink > sink = logging::add_file_log
// 			(
// 			keywords::open_mode = std::ios::app,
// 			keywords::file_name = strfName, //"sample_%N.log",                                        /*< file name pattern >*/
// 			keywords::rotation_size = uRotateSize,//10 * 1024 * 1024,                                   /*< rotate files every 10 MiB... >*/
// 			keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0), /*< ...or at midnight >*/
// 			keywords::format = strFmt, //"[%TimeStamp%]: %Message%"  
// 			keywords::auto_flush = true/*< log record format >*/
// 			);
// 		init_file_collecting(sink);
// 		//logging::add_common_attributes();
// 	}
// };
// #endif