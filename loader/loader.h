#pragma once

#include <windows.h>
#include <TlHelp32.h>
#include <stdio.h>

void log(const char *fmt, ...);
int try_inject();
DWORD get_process_id(const char *processName);