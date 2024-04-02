#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <vector>


DWORD GetProcId(const char* ProcName);

DWORD GetModuleBaseAdress(DWORD pID, const char* ModuleName);

uintptr_t FindDMAAdd(HANDLE handle, uintptr_t pointer, std::vector<unsigned int> offsets);
