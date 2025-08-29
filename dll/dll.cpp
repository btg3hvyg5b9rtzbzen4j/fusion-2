#include <iostream>
#include <windows.h>

DWORD WINAPI MainThread(LPVOID lpParam) {
  HINSTANCE hinstDLL = (HINSTANCE)lpParam;

  if (AllocConsole()) {
    FILE *f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    std::cout << "Hello from injected DLL!" << std::endl;
  }

  MessageBoxA(NULL, "hi", "yur", MB_OK);

  FreeConsole();
  FreeLibraryAndExitThread(hinstDLL, 0);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
  switch (fdwReason) {
  case DLL_PROCESS_ATTACH: {
    HANDLE thread = CreateThread(nullptr, 0, MainThread, hinstDLL, 0, nullptr);
    if (thread)
      CloseHandle(thread);
    break;
  }
  case DLL_PROCESS_DETACH: {
    if (lpvReserved != nullptr) {
      break;
    }
    break;
  }
  }
  return TRUE;
}