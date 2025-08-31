#include <windows.h>

DWORD WINAPI MainThread(LPVOID lpParam) {
  HINSTANCE hinstDLL = (HINSTANCE)lpParam;

  MessageBoxA(NULL, "I have been loaded", "indeed", MB_OK);

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