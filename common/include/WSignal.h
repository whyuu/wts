#pragma once
extern bool g_bRun;
typedef  void (* Term_Func)(void* param);
void init_signals(Term_Func func = 0, void* param = 0);
