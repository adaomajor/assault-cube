#include <iostream>
#include <vector>
#include <windows.h>
#include "proc.h"


#define PROCESSNAME "ac_client.exe"
#define MODULEOFFSET 0x17E0A8


void banner(){
	std::cout << " --------------------------------" << std::endl;
	std::cout << "|    ASSAULT CUBE 1.3.0.2 HACK   |" << std::endl;
	std::cout << "|         -++[ gh0xt ]++-        |" << std::endl;
	std::cout << "|          version 1.0.0         |" << std::endl;
	std::cout << " --------------------------------\n\r" << std::endl;
}

int main(){
	int NEWAMMOVALUE = 7000;
	
	//Get the process id
	DWORD PID = GetProcId(PROCESSNAME);
	if(PID == 0){
		banner();
		std::cout << "[+] ac_client is not running " << std::endl;
		getchar();
		exit(-1);	
	}
	
	banner();
	std::cout << "[+] ac_client.exe --> pid --> " << PID << std::endl;
	//get the modulesBaseAdrr
	uintptr_t ac_ModuleBaseAddress = GetModuleBaseAdress(PID, PROCESSNAME);
	std::cout << "[+] ac_client module base adress is:  0x" << std::hex << ac_ModuleBaseAddress << std::endl;
	
	//handle of the game 
	HANDLE gameHandler = NULL;
	gameHandler = OpenProcess(PROCESS_ALL_ACCESS,0,PID);
	if(gameHandler == INVALID_HANDLE_VALUE || gameHandler == NULL){
		std::cout << "[+] ac_client is not open" << std::endl;
		getchar();
		exit(-1);
	}
	
	//dynamic address
	uintptr_t dynamicaddr = ac_ModuleBaseAddress + 0x17E0A8;
	std::cout << "[+] Module dynamicAddr: 0x" << std::hex <<  dynamicaddr << std::endl;
	
	//resolve the ammo pointer chain
	std::vector<unsigned int> ammoOffsets = { 0x364, 0x14, 0x0 };	
	uintptr_t ammoAddr = FindDMAAdd(gameHandler,dynamicaddr, ammoOffsets);
	std::cout << "[+] Ammo Address: 0x" << std::hex << ammoAddr << std::endl;
	
	//reading the ammo valuE
	int CurrentAmmoValue;
	std::cout << "[+] Reading currentAmmo value... " << std::endl;
	ReadProcessMemory(gameHandler, (LPCVOID *)ammoAddr, &CurrentAmmoValue, sizeof(CurrentAmmoValue),0);
	std::cout << "[+] Ammo value: " << std::dec << CurrentAmmoValue << std::endl;
	
	///write the ammo value
	std::cout << "[+] Setting Ammo value to: " << NEWAMMOVALUE << std::endl;
	WriteProcessMemory(gameHandler, (LPCVOID *)ammoAddr,&NEWAMMOVALUE, sizeof(CurrentAmmoValue),0);
	
	//rereading the ammo value
	ReadProcessMemory(gameHandler, (LPCVOID*)ammoAddr, &CurrentAmmoValue, sizeof(CurrentAmmoValue),0);
	std::cout << "[+] New ammo value: " << std::dec << CurrentAmmoValue << std::endl;
	
	CloseHandle(gameHandler);
	
	std::cout << "[+] The hack run successfully...press [ENTER] to close " << std::endl;
	getchar();
    return 0;       
}
