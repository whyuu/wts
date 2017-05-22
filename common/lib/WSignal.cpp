#include <signal.h>
#if (defined(WIN32) || defined(_WINDLL))
#include <Windows.h>
#endif
#include "WLog.h"
#include "WSignal.h"
#if (defined(WIN32) || defined(_WINDLL))
#define crash_catch_func(a) 
#else
#include <execinfo.h>
static void crash_catch_func(int nSig)
{
  //25��ʾ�鿴��ǰ�����25����ջ
  void * array[25];
  int nSize = backtrace(array, 25);
  char ** symbols = backtrace_symbols(array, nSize);
  for (int i = 0; i < nSize; i++)
  {
    //symbols[i]��ʾ�ö�ջ����ϸ��Ϣ���ַ���char*��ʾ
    puts(symbols[i]);
  }
  for (int i = 0; i < nSize; i++)
  {
    //symbols[i]��ʾ�ö�ջ����ϸ��Ϣ���ַ���char*��ʾ
    CWLog::GetInstance().WriteLog(LOG_ERR, symbols[i]);
  }
  free(symbols);
}
#endif

bool g_bRun = true;
//�쳣�˳�ʱ�����������
static Term_Func g_Func = NULL;
static void* g_param = NULL;

static void sig(int signo)
{
  switch (signo)
  {
  case SIGSEGV:
    CWLog::GetInstance().WriteLog(LOG_EMERG, "segment fault, aborting");
    crash_catch_func(signo);
    break;
  case SIGTERM:
  case SIGINT:
    CWLog::GetInstance().WriteLog(LOG_EMERG, "user abort %d", signo);
    if (g_Func != NULL)
    {
      g_Func(g_param);
    }
    break;
  case SIGABRT:
#if !(defined(WIN32) || defined(_WINDLL))
  case SIGBUS:
#endif
    CWLog::GetInstance().WriteLog(LOG_EMERG, "system abnormal %d", signo);
    crash_catch_func(signo);;
    break;
  default:
    CWLog::GetInstance().WriteLog(LOG_EMERG, "signal %d", signo);
    crash_catch_func(signo);
  }
  Sleep(50);
  g_bRun = false;
  exit(0);
}

void init_signals(Term_Func func, void* param)
{
  g_Func = func;
  g_param = param;
  signal(SIGTERM, sig);
  signal(SIGINT, sig);
  signal(SIGSEGV, sig);
  signal(SIGABRT, sig);
#if !(defined(WIN32) || defined(_WINDLL))
  //signal(SIGPIPE, sig);
  signal(SIGPIPE,SIG_IGN);
  signal(SIGBUS, sig);
#endif
}