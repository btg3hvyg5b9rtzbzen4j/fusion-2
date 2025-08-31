#include "dll.h"
#include "java.h"

DWORD WINAPI MainThread(LPVOID lpParam) {
  HINSTANCE hinstDLL = (HINSTANCE)lpParam;

  AllocConsole();
  FILE *f = NULL;
  freopen_s(&f, "CONOUT$", "w", stdout);
  ShowWindow(GetConsoleWindow(), SW_SHOW);

  setup_jenv();

  MessageBox(NULL, "Goodbye", "Ok", MB_OK);

  FreeConsole();
  fclose(f);
  FreeLibraryAndExitThread(hinstDLL, 0);
  return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
  if (fdwReason == DLL_PROCESS_ATTACH) {
    HANDLE thread = CreateThread(NULL, 0, MainThread, hinstDLL, 0, NULL);
    if (thread)
      CloseHandle(thread);
  }
  return TRUE;
}