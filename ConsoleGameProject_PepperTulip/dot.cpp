#include<iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include "struct.h"
#include "general.h"
#include "dot.h"
#include "new_conio.h"

extern DayTime realtime;
extern int sailling_day;
extern Player player;
extern int angraymood;

void setcolor(unsigned short text, unsigned short back)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text | (back << 4));
}

void LoadTitle()
{
	printf("\n\n\n\n");
	printf("        ******   ******   ******   ******   ******   ******             *******  **   **  **     *****   *****           \n");
	printf("        **   **  ******   **    *  **    *  ******   **   **    ***     *******  **   **  **     *****   **   **         \n");
	printf("        **   **  **       **    *  **    *  **       **   **   *   *      ***    **   **  **      ***    **   **         \n");
	printf("        ******   *****    ******   ******   *****    *****      ***       ***    **   **  **      ***    *****           \n");
	printf("        **       **       **       **       **       **   **   *    * *   ***    **   **  **      ***    **              \n");
	printf("        **       ******   **       **       ******   **   **   *    **    ***    *******  *****  *****   **              \n");
	printf("        **       ******   **       **       ******   **   **    ****  *   ***     *****   *****  *****   **              \n");
	printf("                                                                                                                         \n");
	printf("=========================================================================================================================\n");
	printf("\n\n\n");
}

 void gotoxy(int x, int y)
 {
 	COORD CursorPosition = { x, y };
 	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);
 }

void GotoxyAndPrint(int x, int y, const char* stri)
{
	gotoxy(x, y);
	std::cout << stri;
}

void printdaysailling()
{
	setcolor(15, 0);
	system("cls");
	printf("    '           '                        '             '           '                0       00      ||\n");
	printf("                                                                                     0 000 0        ||\n");
	printf("                     '                        '                         '         0 ' 00000      '  ||\n");
	printf("                                                                                   00000000000      ||\n");
	printf("                                                                                      00000   0     ||\n");
	printf("                                                                                   00  000  0       ||\n");
	printf("                                                                                            0       ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                 '  ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("        '                                                                           '            '  ||\n");
	printf("                                                                                                    ||\n");
	printf("====================================================================================================ㅁ===================\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	
	gotoxy(103, 1);
	printf("%d. %d. %d |", realtime.day, realtime.month, realtime.year);
	printf("낮|");

	int index_food = 0;
	int index_water = 0;
	for (int i = 0; i < 12; i++)
	{
		if (player.inship_cargo[i].cargoNumber == 1)index_food = i;
		else if (player.inship_cargo[i].cargoNumber == 2) index_water = i;
	}

	gotoxy(103, 3);
	printf("불만도 : %d", angraymood);
	gotoxy(103, 4);
	printf("  식량 : %d", player.inship_cargo_vlaue[index_food]);
	gotoxy(103, 5);
	printf("    물 : %d", player.inship_cargo_vlaue[index_water]);


	for (int i = 0; i < 23; i++)
	{

		for (int j = 0; j < 100; j++)
		{
			gotoxy(0 + j, 0 + i);
			setcolor(0, day_back_ground[i][j]);
			printf(" ");
		}
		printf("\n");
	}

	for (int i = 0; i < 19; i++)
	{
		gotoxy(25, 2 + i);
		for (int j = 0; j < 51; j++)
		{
			if (i < 14)setcolor(0, 11);
			else setcolor(0, 9);
			if (dot_dau[i][j] != '0')
			{
				setcolor(7, dot_dau_backgound[i][j]);
				printf("%c", dot_dau[i][j]);
			}
			else printf(" ");
		}
	}

	for (int i = 0; i < 7; i++)
	{
		gotoxy(80, 1 + i);
		for (int j = 0; j < 14; j++)
		{
			setcolor(0, 11);
			if (sun[i][j] == '0')
			{
				setcolor(15, 14);
				printf("%c", sun[i][j]);
			}
			else printf(" ");
		}
	}
	setcolor(15, 0);
}

void printnightsailling()
{

	setcolor(15, 0);
	system("cls");
	printf("     #      #                           0              0           0           0            0       ||\n");
	printf("    ##                                                                                              ||\n");
	printf("    ###         @          @                  @                         @           @            @  ||\n");
	printf("     ###                                                                                    @       ||\n");
	printf("       ###         @                                                                                ||\n");
	printf("                                                                                  @                 ||\n");
	printf("                                                 @                                                  ||\n");
	printf("         @                     @                                @                                   ||\n");
	printf("                @                                                                                   ||\n");
	printf("                                                                               @                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                 '  ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("        '            '                                                              '            '  ||\n");
	printf("                                                                                                    ||\n");
	printf("====================================================================================================ㅁ===================\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");
	printf("                                                                                                    ||\n");

	gotoxy(103, 1);
	printf("%d. %d. %d |", realtime.day, realtime.month, realtime.year);
	printf("밤|");

	int index_food = 0;
	int index_water = 0;
	for (int i = 0; i < 12; i++)
	{
		if (player.inship_cargo[i].cargoNumber == 1)index_food = i;
		else if (player.inship_cargo[i].cargoNumber == 2) index_water = i;
	}

	gotoxy(103, 3);
	printf("불만도 : %d", angraymood);
	gotoxy(103, 4);
	printf("  식량 : %d", player.inship_cargo_vlaue[index_food]);
	gotoxy(103, 5);
	printf("    물 : %d", player.inship_cargo_vlaue[index_water]);

	for (int i = 0; i < 23; i++)
	{

		for (int j = 0; j < 100; j++)
		{
			gotoxy(0 + j, 0 + i);
			setcolor(0, night_back_ground[i][j]);
			printf(" ");
		}
	}

	for (int i = 0; i < 19; i++)
	{
		gotoxy(25, 2 + i);
		for (int j = 0; j < 51; j++)
		{
			if (i < 14)setcolor(0, 1);
			else setcolor(0, 9);
			if (dot_dau[i][j] != '0')
			{
				setcolor(7, dot_dau_backgound[i][j]);
				printf("%c", dot_dau[i][j]);
			}
			else printf(" ");
		}
	}

	for (int i = 0; i < 7; i++)
	{
		gotoxy(0, 0 + i);
		for (int j = 0; j < 14; j++)
		{
			setcolor(0, 1);
			if (moon[i][j] == '0')
			{
				setcolor(15, 14);
				printf("%c", moon[i][j]);
			}
			else printf(" ");
		}
	}
	setcolor(15, 0);
}

void printdayport(const int BGround[][121])
{
	system("cls");

	for (int i = 0; i < 24; i++)
	{
		gotoxy(0, 0 + i);
		for (int j = 0; j < 121; j++)
		{
			{
				setcolor(15, BGround[i][j]);
				printf("%c", dot_port[i][j]);
			}
		}
	}
	setcolor(15, 0);
	gotoxy(0,24);
}

void printworldmap()
{
	system("cls");

	for (int i = 0; i < 35; i++)
	{
		gotoxy(0, 0 + i);
		for (int j = 0; j < 121; j++)
		{
			setcolor(0, 9);
			if (worldmapdot[i][j] != '0')
			{
				setcolor(15, worldmapcolor[i][j]);
				printf("%c", worldmapdot[i][j]);
			}
			else printf(" ");
		}
	}
	setcolor(15, 0);
	gotoxy(0, 24);
}

void printafricamap()
{
	for (int i = 0; i < 16; i++)
	{
		gotoxy(30, 10 + i);
		for (int j = 0; j < 53; j++)
		{
			setcolor(0, 9);
			if (africa_dot[i][j] != '0')
			{
				setcolor(15, africa_col[i][j]);
				printf("%c", africa_dot[i][j]);
			}
			else printf(" ");
		}
	}
	setcolor(15, 0);
}

void printUIaction(const char BGround[][121])
{
	system("cls");

	for (int i = 0; i < 35; i++)
	{
		gotoxy(0, 0 + i);
		for (int j = 0; j < 121; j++)
		{
			{
				setcolor(15, 0);
				printf("%c", inport_UI_base[i][j]);
			}
		}
	}
	setcolor(15, 0);
	portsidevalueprinter(&player);
}

void printinvandship(const char BGround[][31])
{
	setcolor(0, 6);

	for (int i = 0; i < 17; i++)
	{
		gotoxy(80, 5 + i);
		for (int j = 0; j < 31; j++)
		{
				printf("%c", BGround[i][j]);
		}
	}
}

void portsidevalueprinter(Player* player)
{
	gotoxy(1, 10);
	printf("%d.%d.%d", realtime.year, realtime.month, realtime.day);
	gotoxy(1, 12);
	printf("자금");
	gotoxy(1, 13);
	printf("%d", player->money);

	gotoxy(1, 15);
	printf("선원");
	gotoxy(1, 16);
	printf("%d/%d(%d)", player->nowman, player->my_ship.maxCrew, player->my_ship.needCrew);

	gotoxy(1, 18);
	printf("적재량");
	gotoxy(1, 19);
	printf("%d/%d", player->my_ship.shipNowCargo, player->my_ship.shipCargo);
}

void printportrait(const int input[][7])
{
	for (int i = 0; i < 7; i++)
	{
		gotoxy(7, 1 + i);
		for (int j = 0; j < 7; j++)
		{
			setcolor(15, 0);
			if (input[i][j] != 0)
			{
				setcolor(15, input[i][j]);
				printf(" ");
			}
			else printf(" ");
		}
	}
	setcolor(15, 0);
}