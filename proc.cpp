#include <iostream>
#include <vector>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

DWORD GetProcId(const char* ProcName){
	DWORD pID = 0;
	HANDLE ProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(ProcSnap != INVALID_HANDLE_VALUE){
		PROCESSENTRY32 procEntry;
		procEntry.dwSize == sizeof(PROCESSENTRY32);
		
		if(Process32First(ProcSnap, &procEntry)){
			do{
				if(!_tcscmp((char *)procEntry.szExeFile, ProcName)){
					pID = procEntry.th32ProcessID;
					break;
				}
			}while(Process32Next(ProcSnap, &procEntry));
		}
		
	}
	
	CloseHandle(ProcSnap);
	return pID;
}
uintptr_t GetModuleBaseAdress(DWORD pID, const char* ModuleName){
	uintptr_t pointer;
	HANDLE ModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE , pID);
	if(ModuleSnap != INVALID_HANDLE_VALUE){
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(MODULEENTRY32);
		
		if(Module32First(ModuleSnap, &modEntry)){
			do{
				if(_tcscmp(modEntry.szModule, ModuleName) == 0){
					pointer = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			}while(Module32Next(ModuleSnap, &modEntry));
		}
	}
   CloseHandle(ModuleSnap);	
   return pointer;	
}
uintptr_t FindDMAAdd(HANDLE handle, uintptr_t pointer, std::vector<unsigned int> offsets){
	DWORD addr = pointer;
	
	for(int i = 0; i <= offsets.size() -1 ; ++i){		
		ReadProcessMemory(handle, (LPCVOID *)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	
	return (uintptr_t)addr;
}
