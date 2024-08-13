//기본
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

//자작
#include "struct.h"
#include "port.h"
#include "dot.h"
#include "sailing.h"
#include "general.h"

void cursercontrol(bool flag);

Player player;
DayTime realtime = { 1636,1,1 };
Port targetport;

char buff = 0;
int sail_port = 0; //0 = Title, 1 = 항구, 2 = 항해
int sailling_day = 0;

int main()
{
	PlaySound(L"sound\\title.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
	system("mode con cols=121 lines=35 | title Pepper and Tulip");
	cursercontrol(0);
	setcolor(15, 0);
	system("cls");

	bool alreadyinport = 0; //매 항구 정박시 1회만 활성화 이므로 항구 정박시1, 항해시작시 0으로 설정
	int turngame = 1; // 1 = On, 2 = Off
	bool playermaked = 0;
	int pickbuffer = 0;

	while (1)
	{
		switch (sail_port)
		{
			case 0:
				LoadTitle();
				sail_port = MoveMaker(sail_port);
				gotoxy(0, 30);

				system("cls");
				break;
			case 1:
				if (playermaked == 0)MakePlayer(&player, &playermaked);
				system("cls");
				AllinOnePort(&player, &realtime);
				break;
			case 2:
				roadsav();
				playermaked = 1;
				sail_port = 1;
				system("cls");
				AllinOnePort(&player, &realtime);
				break;
			case 3:
				exit(0);
				break;
			case 4:
				nowsailling(&alreadyinport);
				break;
			default:
				break;
		}
	}
}

void cursercontrol(bool flag)
{
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 10;
	cursorInfo.bVisible = flag;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}