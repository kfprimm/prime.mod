
Strict

Module Prime.pthreads2
ModuleInfo "Author: Kevin Primm"
ModuleInfo "CC_OPTS: -D__CLEANUP_C -O3 -DPTW32_BUILD_INLINED -DPTW32_STATIC_LIB -DHAVE_PTW32_CONFIG_H -Wall"

?Win32
Import "pthreads/*.h"
Import "pthreads/pthread.c"

DebugLog "pthreads2"

Extern
	Function pthread_win32_thread_attach_np()
	Function pthread_win32_thread_detach_np()
End Extern

pthread_win32_thread_attach_np()
OnEnd pthread_win32_thread_detach_np


?
