#include "resource.h"
#include <iostream>
#include<thread>
#include <conio.h>
#include<stdlib.h>
#include<windows.h>
#include <Mmsystem.h> 
#pragma comment(lib, "Winmm.lib") 
#define KEY_DOWN(key_name) ((GetAsyncKeyState(key_name)& 0x8001)?1:0)
#define KEY_UP(key_name) ((GetAsyncKeyState(key_name)& 0x8001)?0:1)
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) 
using namespace std;

bool ModifyRegedit(bool bAutoRun)
{
	char pFileName[MAX_PATH] = { 0 };
	DWORD dwRet = GetModuleFileNameA(NULL, (LPSTR)pFileName, MAX_PATH);
	std::cout << pFileName;
	HKEY hKey;
	LPCSTR lpRun = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
	long lRet = RegOpenKeyExA(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey);
	if (lRet != ERROR_SUCCESS)
	{
		std::cout << "failed";
		return false;
	}


	if (bAutoRun)
		RegSetValueA(hKey, "Merit", (DWORD)REG_SZ, (LPCSTR)pFileName, MAX_PATH);
	else
		RegDeleteValueA(hKey, "Merit");
	RegCloseKey(hKey);
	return true;
}

static void PlaySounds(int i)
{
	if (i == 0)
	{
		PlaySound((LPCTSTR)IDR_WAVE1, NULL, SND_RESOURCE | SND_SYNC);
	}
	else if (i == 1)
	{
		PlaySound((LPCTSTR)IDR_WAVE2, NULL, SND_RESOURCE | SND_SYNC);
	}
	else if (i == 2)
	{
		PlaySound((LPCTSTR)IDR_WAVE3, NULL, SND_RESOURCE | SND_SYNC);
	}
	else if (i == 3)
	{
		PlaySound((LPCTSTR)IDR_WAVE1, NULL, SND_RESOURCE | SND_SYNC);
	}
	else
	{
		// PlaySound((LPCTSTR)IDR_WAVE2, NULL, SND_RESOURCE | SND_SYNC);
	}

}

int main()
{
	
	ModifyRegedit(true);
	bool hasPress[4];
	for (int i = 0; i < 4; i++) {
		hasPress[i] = false;
	}
	int niganma = 0;
	
	for (;;)
	{
		// Esc
		if (KEY_DOWN(27)) {
			if (hasPress[0] == false) {
				hasPress[0] = true;
				thread t1(PlaySounds, 0);
				t1.detach();
				niganma = 0;
			}
		}
		if (KEY_UP(27)) {
			hasPress[0] = false;
		}

		// F
		if (KEY_DOWN('F'))
		{
			if (hasPress[1] == false) {
				hasPress[1] = true;
				thread t1(PlaySounds, 1);
				t1.detach();
				niganma = 0;
			}
			if (KEY_UP('F'))
			{
				hasPress[1] = false;
			}
		}

		// Z
		if (KEY_DOWN('Z'))
		{
			if (hasPress[2] == false) {
				hasPress[2] = true;
				thread t1(PlaySounds, 2);
				t1.detach();
				niganma = 0;
			}
			if (KEY_UP('Z'))
			{
				hasPress[2] = false;
			}
		}

	}
	
}