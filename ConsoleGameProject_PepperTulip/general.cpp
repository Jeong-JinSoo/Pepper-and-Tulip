//일반적인 시스템관련 코드를 넣을것.
#include<iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <conio.h>
#include "struct.h"
#include "dot.h"
#include "general.h"
#include "port.h"

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define Enter '\n'

extern Player player;
extern DayTime realtime;

int MoveMaker(int picker)
{
	int seleting = 1;
	int sel_save = 1;

	int x = 50;
	int y = 15;
	GotoxyAndPrint(x-2, y, "> 새로 하기");
	GotoxyAndPrint(x, y + 2, "로드 하기");
	GotoxyAndPrint(x, y + 4, "종료 하기");

	while (true)
	{
		seleting = movecurser(seleting);
		if (seleting < 1 && seleting != -10 ) seleting = 3;
		else if (seleting > 3 && seleting != -10) seleting = 1;

		switch (seleting)
		{
		case 1:
			GotoxyAndPrint(x - 2, y, "> 새로 하기");
			GotoxyAndPrint(x - 2, y + 2, "  로드 하기");
			GotoxyAndPrint(x - 2, y + 4, "  종료 하기");
			break;
		case 2:
			GotoxyAndPrint(x - 2, y, "  새로 하기");
			GotoxyAndPrint(x - 2, y + 2, "> 로드 하기");
			GotoxyAndPrint(x - 2, y + 4, "  종료 하기");
			break;
		case 3:
			GotoxyAndPrint(x - 2, y, "  새로 하기");
			GotoxyAndPrint(x - 2, y + 2, "  로드 하기");
			GotoxyAndPrint(x - 2, y + 4, "> 종료 하기");
			break;
		case -10:
			return sel_save;
		}
		sel_save = seleting;
	}
}

int pickcountry()
{
	int seleting = 1;
	int sel_save = 1;

	int x = 21;
	int y = 13;
	GotoxyAndPrint(x, y,     "> 포르투갈 왕국");
	GotoxyAndPrint(x, y + 1, "  잉글랜드 왕국");
	GotoxyAndPrint(x, y + 2, "  프랑스 왕국");
	GotoxyAndPrint(x, y + 3, "  오스만 제국");

	while (true)
	{
		seleting = movecurser(seleting);
		if (seleting < 1 && seleting != -10) seleting = 4;
		else if (seleting > 4 && seleting != -10) seleting = 1;

		switch (seleting)
		{
		case 1:
			GotoxyAndPrint(x, y,     "> 포르투갈 왕국");
			GotoxyAndPrint(x, y + 1, "  잉글랜드 왕국");
			GotoxyAndPrint(x, y + 2, "  프랑스 왕국");
			GotoxyAndPrint(x, y + 3, "  오스만 제국");
			break;
		case 2:
			GotoxyAndPrint(x, y,     "  포르투갈 왕국");
			GotoxyAndPrint(x, y + 1, "> 잉글랜드 왕국");
			GotoxyAndPrint(x, y + 2, "  프랑스 왕국");
			GotoxyAndPrint(x, y + 3, "  오스만 제국");
			break;
		case 3:
			GotoxyAndPrint(x, y,     "  포르투갈 왕국");
			GotoxyAndPrint(x, y + 1, "  잉글랜드 왕국");
			GotoxyAndPrint(x, y + 2, "> 프랑스 왕국");
			GotoxyAndPrint(x, y + 3, "  오스만 제국");
			break;
		case 4:
			GotoxyAndPrint(x, y,     "  포르투갈 왕국");
			GotoxyAndPrint(x, y + 1, "  잉글랜드 왕국");
			GotoxyAndPrint(x, y + 2, "  프랑스 왕국");
			GotoxyAndPrint(x, y + 3, "> 오스만 제국");
			break;
		case -10:
			return sel_save;
		}
		sel_save = seleting;
	}
}

void GetPortSetiing(Port* port, std::string filePath)
{
	int inputnum =0 ;
	std::string line;
	std::string line2;
	std::ifstream file(filePath);
	std::ifstream file2("path\\cargopath.txt");
	int i = 0;

	if (file.is_open())
	{
		getline(file, line); // 이름
		port->name = line;

		getline(file, line); // 지역
		inputnum = stoi(line);
		port->localT = (LocalTag)inputnum;

		getline(file, line); // 국가
		inputnum = stoi(line);
		port->conT = (CountryTag)inputnum;

		getline(file, line); //위치 X
		inputnum = stoi(line);
		port->port_Position.x_pos = inputnum;

		getline(file, line); //위치 Y
		inputnum = stoi(line);
		port->port_Position.y_pos = inputnum;

		while (getline(file, line))
		{
			GetCargoSetiing(&port->cargo[i], line);
			i++;
		}
		i = 0;
		while (getline(file2, line2))
		{ 
			GetCargoSetiing(&port->cargoStock[i], line2);
			i++;
		}

		file.close();
	}

	else
	{
		std::cout << "Unable to open port file" << std::endl;
	}
}

void GetShipSetiing(Ship* ship, std::string filePath)
{
	int inputnum = 0;
	std::string line;
	std::ifstream file(filePath);
	if (file.is_open())
	{
		getline(file, line); // 이름
		ship->shipName = line;

		getline(file, line); // 타입
		inputnum = stoi(line);
		ship->Stype = (ShipType)inputnum;

		getline(file, line); // 최고속도
		inputnum = stoi(line);
		ship->shipDefalutSpeed = inputnum;

		getline(file, line); // 실제속도
		inputnum = stoi(line);
		ship->shipRealSpeed = inputnum;

		getline(file, line); // 현재 적재량
		inputnum = stoi(line);
		ship->shipNowCargo = inputnum;

		getline(file, line); // 최대 적재량
		inputnum = stoi(line);
		ship->shipCargo = inputnum;

		getline(file, line); // 최소 승무원
		inputnum = stoi(line);
		ship->needCrew = inputnum;

		getline(file, line); // 최대 승무원
		inputnum = stoi(line);
		ship->maxCrew = inputnum;

		getline(file, line); // 가격
		inputnum = stoi(line);
		ship->shipPrice = inputnum;

		file.close();
	}

	else
	{
		std::cout << "Unable to open ship file" << std::endl;
	}
}

void GetCargoSetiing(S_Cargo_type* cargo, std::string filePath)
{
	int inputnum = 0;
	std::string line;
	std::ifstream file(filePath);
	if (file.is_open())
	{
		getline(file, line); // 이름
		cargo->cargoName = line;

		getline(file, line); // 관리번호
		inputnum = stoi(line);
		cargo->cargoNumber = inputnum;

		getline(file, line); // 기본가
		inputnum = stoi(line);
		cargo->defaultPrice = inputnum;

		getline(file, line); // 기본보유량
		inputnum = stoi(line);
		cargo->defaultStoke = inputnum;

		file.close();
	}
	else
	{
		std::cout << "Unable to open cargo file" << std::endl;
	}
}

void TimeCounter(DayTime* daytime)
{

	daytime->day = daytime->day + 1;
	if (daytime->day > 31 && (daytime->month == 1 || daytime->month == 3 || daytime->month == 5 || daytime->month == 7 || daytime->month == 8 || daytime->month == 10 || daytime->month == 12))
	{
		if (daytime != &player.DebtEndTime) PrintmonthSettlement();
		daytime->day = 1;
		daytime->month = daytime->month + 1;
		if (daytime->month > 12)
		{
			daytime->year = daytime->year + 1;
			daytime->month = 1;
		}
	}
	else if (daytime->day > 30 && (daytime->month == 4 || daytime->month == 6 || daytime->month == 9 || daytime->month == 11))
	{
		if (daytime != &player.DebtEndTime) PrintmonthSettlement();
		daytime->day = 1;
		daytime->month = daytime->month + 1;
	}
	else if (daytime->day > 28 && daytime->month == 2)
	{
		if (daytime != &player.DebtEndTime) PrintmonthSettlement();
		daytime->day = 1;
		daytime->month = daytime->month + 1;
	}
	if(daytime != &player.DebtEndTime) Tulipvalue(&player);
}

void MakePlayer(Player* player, bool* maked)
{	
	boxprint();
	gotoxy(21, 11);
	printf("플레이어의 이름을 공백없이 영문으로 입력하세요.");
	gotoxy(21, 12);
	printf("이름 :");
	std::cin >> player->player_name;

	boxprint();
	gotoxy(21, 11);
	printf("플레이어의 국가를 선택 하세요.\n");

	int conum = 1;
	int stop = 0;
	conum = pickcountry();
	if (conum == 1)
	{
		system("cls");
		boxprint();
		player->palyer_country = CountryTag::Portugal;
		GetPortSetiing(&player->nowPort, "port\\lisbon.txt");
		GotoxyAndPrint(21, 11, "포르투갈 왕국을 선택 하셨습니다.");
		GotoxyAndPrint(21, 12, "당신은 리스본에서 시작합니다.");
	}
	else if (conum == 2)
	{
		system("cls");
		boxprint();
		player->palyer_country = CountryTag::England;
		GetPortSetiing(&player->nowPort, "port\\liverpool.txt");
		GotoxyAndPrint(21, 11, "잉글랜드 왕국을 선택 하셨습니다."); 
		GotoxyAndPrint(21, 12, "당신은 리버풀에서 시작합니다.");
	}
	else if (conum == 3)
	{
		system("cls");
		boxprint();
		player->palyer_country = CountryTag::France;
		GetPortSetiing(&player->nowPort, "port\\marseille.txt");
		GotoxyAndPrint(21, 11, "프랑스 왕국을 선택 하셨습니다.");
		GotoxyAndPrint(21, 12, "당신은 마르세유에서 시작합니다.");
	}
	else if (conum == 4)
	{
		system("cls");
		boxprint();
		player->palyer_country = CountryTag::Ottuman;
		GetPortSetiing(&player->nowPort, "port\\istanbul.txt");
		GotoxyAndPrint(21, 11, "오스만 제국을 선택 하셨습니다.");
		GotoxyAndPrint(21, 12, "당신은 이스탄불에서 시작합니다.");
	}

	player->money = 10000;
	player->truemoney = 10000;
	GetShipSetiing(&player->my_ship, "ship\\dau.txt");
	for (int i = 0; i < 12; i++)
		GetCargoSetiing(&player->inship_cargo[i], "cargo\\0none.txt");

	player->Debt = 0;
	player->Debt_val = 0;
	player->DebtEndTime.year = 9999;
	player->DebtEndTime.month = 99;
	player->DebtEndTime.day = 99;

	gotoxy(21, 13);
	printf("당신은 %d 두카트를 보유하고 있습니다.", player->money);
	GotoxyAndPrint(21, 14, "현재 당신의 함선은 다우 입니다.");
	GotoxyAndPrint(21, 15, "즐거운 여행되시길 바랍니다.");
	*maked = 1;
	movecurser(0);
}

int movecurser(int value)
{
	char c;
	while (1)
	{
		if (_kbhit())
		{
			c = _getch();
			if (c == -32)
			{
				c = _getch();
				switch (c)
				{
				case LEFT:
					return value - 1;
				case RIGHT:
					return value + 1;
				case UP:
					return value - 1;
				case DOWN:
					return value + 1;
				}
			}
			else if (c == 13)
			{
				return -10;
			}
			else if (c == 91)
			{
				return 70;
			}
			else if (c == 93)
			{
				return 71;
			}
			else if (c == 92)
			{
				return 72;
			}
		}
	}
	return 0;
}

void boxprint()
{
	GotoxyAndPrint(20, 10, "#==========================================================#");
	GotoxyAndPrint(20, 11, "|                                                          |");
	GotoxyAndPrint(20, 12, "|                                                          |");
	GotoxyAndPrint(20, 13, "|                                                          |");
	GotoxyAndPrint(20, 14, "|                                                          |");
	GotoxyAndPrint(20, 15, "|                                                          |");
	GotoxyAndPrint(20, 16, "|                                                          |");
	GotoxyAndPrint(20, 17, "|                                                          |");
	GotoxyAndPrint(20, 18, "|                                                          |");
	GotoxyAndPrint(20, 19, "|                                                          |");
	GotoxyAndPrint(20, 20, "|                                                          |");
	GotoxyAndPrint(20, 21, "|                                                          |");
	GotoxyAndPrint(20, 22, "|                                                          |");
	GotoxyAndPrint(20, 23, "|                                                          |");
	GotoxyAndPrint(20, 24, "|                                                          |");
	GotoxyAndPrint(20, 25, "|                                                          |");
	GotoxyAndPrint(20, 26, "|                                                          |");
	GotoxyAndPrint(20, 27, "|                                                          |");
	GotoxyAndPrint(20, 28, "#==========================================================#");
}

void PrintmonthSettlement ()
{
	static Player premonthPlayer = player;

	setcolor(0, 6);
	boxprint();
	GotoxyAndPrint(31, 11, "월간 결산");
	gotoxy(21, 13);
	printf("선원임금 -%d", (int)player.nowman * 25);
	gotoxy(21, 14);
	printf("대출이자 -%d", (int)player.Debt / 10);
	GotoxyAndPrint(21, 15, "    세금 -200");
	GotoxyAndPrint(21, 16, "--------------------");
	player.money = player.money - ((player.nowman * 2.5f) + (player.Debt * player.Debt_val) + 200);
	gotoxy(21, 17);
	printf("최종자산 %d", player.money);
	GotoxyAndPrint(21, 18, "--------------------");
	GotoxyAndPrint(21, 19, "분류     지난달       현제       차");

	gotoxy(21, 20);
	printf("자산     %d", premonthPlayer.money);
	gotoxy(43, 20);
	printf("%d", player.money);
	gotoxy(54, 20);
	printf("%d", premonthPlayer.money-player.money);

	gotoxy(21, 21);
	printf("부채     %d", premonthPlayer.Debt);
	gotoxy(43, 21);
	printf("%d", player.Debt);
	gotoxy(54, 21);
	printf("%d", premonthPlayer.Debt - player.Debt);
	movecurser(0);

	premonthPlayer = player;

	setcolor(15, 0);
}

void writesav()
{
	std::string line;
	std::fstream file("savdata.sav");
	if (file.is_open())
	{
		//이름, 국적
		file << player.player_name << "\n";
		file << player.palyer_country << "\n";

		//항구---------
		//위치
		file << player.nowPort.name << "\n";
		file << player.nowPort.localT << "\n";
		file << player.nowPort.conT << "\n";
		file << player.nowPort.port_Position.x_pos << "\n";
		file << player.nowPort.port_Position.y_pos << "\n";
		//항구-구매0
		file << player.nowPort.cargo[0].cargoName << "\n";
		file << player.nowPort.cargo[0].cargoNumber << "\n";
		file << player.nowPort.cargo[0].defaultPrice << "\n";
		file << player.nowPort.cargo[0].defaultStoke << "\n";
		//항구-구매1
		file << player.nowPort.cargo[1].cargoName << "\n";
		file << player.nowPort.cargo[1].cargoNumber << "\n";
		file << player.nowPort.cargo[1].defaultPrice << "\n";
		file << player.nowPort.cargo[1].defaultStoke << "\n";
		//항구-구매2
		file << player.nowPort.cargo[2].cargoName << "\n";
		file << player.nowPort.cargo[2].cargoNumber << "\n";
		file << player.nowPort.cargo[2].defaultPrice << "\n";
		file << player.nowPort.cargo[2].defaultStoke << "\n";
		//항구-구매3
		file << player.nowPort.cargo[3].cargoName << "\n";
		file << player.nowPort.cargo[3].cargoNumber << "\n";
		file << player.nowPort.cargo[3].defaultPrice << "\n";
		file << player.nowPort.cargo[3].defaultStoke << "\n";

		//항구-판매0
		file << player.nowPort.cargo[0].cargoName << "\n";
		file << player.nowPort.cargo[0].cargoNumber << "\n";
		file << player.nowPort.cargo[0].defaultPrice << "\n";
		file << player.nowPort.cargo[0].defaultStoke << "\n";

		//항구-판매1
		file << player.nowPort.cargo[1].cargoName << "\n";
		file << player.nowPort.cargo[1].cargoNumber << "\n";
		file << player.nowPort.cargo[1].defaultPrice << "\n";
		file << player.nowPort.cargo[1].defaultStoke << "\n";

		//항구-판매2
		file << player.nowPort.cargo[2].cargoName << "\n";
		file << player.nowPort.cargo[2].cargoNumber << "\n";
		file << player.nowPort.cargo[2].defaultPrice << "\n";
		file << player.nowPort.cargo[2].defaultStoke << "\n";

		//항구-판매3
		file << player.nowPort.cargo[3].cargoName << "\n";
		file << player.nowPort.cargo[3].cargoNumber << "\n";
		file << player.nowPort.cargo[3].defaultPrice << "\n";
		file << player.nowPort.cargo[3].defaultStoke << "\n";

		//항구-판매4
		file << player.nowPort.cargoStock[4].cargoName << "\n";
		file << player.nowPort.cargoStock[4].cargoNumber << "\n";
		file << player.nowPort.cargoStock[4].defaultPrice << "\n";
		file << player.nowPort.cargoStock[4].defaultStoke << "\n";

		//항구-판매5
		file << player.nowPort.cargoStock[5].cargoName << "\n";
		file << player.nowPort.cargoStock[5].cargoNumber << "\n";
		file << player.nowPort.cargoStock[5].defaultPrice << "\n";
		file << player.nowPort.cargoStock[5].defaultStoke << "\n";

		//항구-판매6
		file << player.nowPort.cargoStock[6].cargoName << "\n";
		file << player.nowPort.cargoStock[6].cargoNumber << "\n";
		file << player.nowPort.cargoStock[6].defaultPrice << "\n";
		file << player.nowPort.cargoStock[6].defaultStoke << "\n";

		//항구-판매7
		file << player.nowPort.cargoStock[7].cargoName << "\n";
		file << player.nowPort.cargoStock[7].cargoNumber << "\n";
		file << player.nowPort.cargoStock[7].defaultPrice << "\n";
		file << player.nowPort.cargoStock[7].defaultStoke << "\n";

		//항구-판매8
		file << player.nowPort.cargoStock[8].cargoName << "\n";
		file << player.nowPort.cargoStock[8].cargoNumber << "\n";
		file << player.nowPort.cargoStock[8].defaultPrice << "\n";
		file << player.nowPort.cargoStock[8].defaultStoke << "\n";

		//항구-판매9
		file << player.nowPort.cargoStock[9].cargoName << "\n";
		file << player.nowPort.cargoStock[9].cargoNumber << "\n";
		file << player.nowPort.cargoStock[9].defaultPrice << "\n";
		file << player.nowPort.cargoStock[9].defaultStoke << "\n";
		//항구-----------------------

		//재산
		file << player.money << "\n";
		file << player.truemoney << "\n";
		file << player.Debt << "\n";
		file << player.Debt_val << "\n";
		//대출 만기일
		file << player.DebtEndTime.year<< "\n";
		file << player.DebtEndTime.month<< "\n";
		file << player.DebtEndTime.day<< "\n";

		//함선------------
		file << player.my_ship.shipName << "\n";
		file << player.my_ship.shipDefalutSpeed << "\n";
		file << player.my_ship.shipRealSpeed << "\n";
		file << player.my_ship.shipCargo << "\n";
		file << player.my_ship.shipNowCargo << "\n";
		file << player.my_ship.needCrew << "\n";
		file << player.my_ship.maxCrew << "\n";
		file << player.my_ship.shipPrice << "\n";
		//함선------------

		//인벤토리
		file << player.nowman << "\n";

		//카고-----
		//화물0
		file << player.inship_cargo[0].cargoName << "\n";
		file << player.inship_cargo[0].cargoNumber << "\n";
		file << player.inship_cargo[0].defaultPrice << "\n";
		file << player.inship_cargo[0].defaultStoke << "\n";
		//화물1
		file << player.inship_cargo[1].cargoName << "\n";
		file << player.inship_cargo[1].cargoNumber << "\n";
		file << player.inship_cargo[1].defaultPrice << "\n";
		file << player.inship_cargo[1].defaultStoke << "\n";
		//화물2
		file << player.inship_cargo[2].cargoName << "\n";
		file << player.inship_cargo[2].cargoNumber << "\n";
		file << player.inship_cargo[2].defaultPrice << "\n";
		file << player.inship_cargo[2].defaultStoke << "\n";
		//화물3
		file << player.inship_cargo[3].cargoName << "\n";
		file << player.inship_cargo[3].cargoNumber << "\n";
		file << player.inship_cargo[3].defaultPrice << "\n";
		file << player.inship_cargo[3].defaultStoke << "\n";
		//화물4
		file << player.inship_cargo[4].cargoName << "\n";
		file << player.inship_cargo[4].cargoNumber << "\n";
		file << player.inship_cargo[4].defaultPrice << "\n";
		file << player.inship_cargo[4].defaultStoke << "\n";
		//화물5
		file << player.inship_cargo[5].cargoName << "\n";
		file << player.inship_cargo[5].cargoNumber << "\n";
		file << player.inship_cargo[5].defaultPrice << "\n";
		file << player.inship_cargo[5].defaultStoke << "\n";
		//화물6
		file << player.inship_cargo[6].cargoName << "\n";
		file << player.inship_cargo[6].cargoNumber << "\n";
		file << player.inship_cargo[6].defaultPrice << "\n";
		file << player.inship_cargo[6].defaultStoke << "\n";
		//화물7
		file << player.inship_cargo[7].cargoName << "\n";
		file << player.inship_cargo[7].cargoNumber << "\n";
		file << player.inship_cargo[7].defaultPrice << "\n";
		file << player.inship_cargo[7].defaultStoke << "\n";
		//화물8
		file << player.inship_cargo[8].cargoName << "\n";
		file << player.inship_cargo[8].cargoNumber << "\n";
		file << player.inship_cargo[8].defaultPrice << "\n";
		file << player.inship_cargo[8].defaultStoke << "\n";
		//화물9
		file << player.inship_cargo[9].cargoName << "\n";
		file << player.inship_cargo[9].cargoNumber << "\n";
		file << player.inship_cargo[9].defaultPrice << "\n";
		file << player.inship_cargo[9].defaultStoke << "\n";
		//화물10
		file << player.inship_cargo[10].cargoName << "\n";
		file << player.inship_cargo[10].cargoNumber << "\n";
		file << player.inship_cargo[10].defaultPrice << "\n";
		file << player.inship_cargo[10].defaultStoke << "\n";
		//화물11
		file << player.inship_cargo[11].cargoName << "\n";
		file << player.inship_cargo[11].cargoNumber << "\n";
		file << player.inship_cargo[11].defaultPrice << "\n";
		file << player.inship_cargo[11].defaultStoke << "\n";
		//카고-----
		file << player.inship_cargo_vlaue[0] << "\n";
		file << player.inship_cargo_vlaue[1] << "\n";
		file << player.inship_cargo_vlaue[2] << "\n";
		file << player.inship_cargo_vlaue[3] << "\n";
		file << player.inship_cargo_vlaue[4] << "\n";
		file << player.inship_cargo_vlaue[5] << "\n";
		file << player.inship_cargo_vlaue[6] << "\n";
		file << player.inship_cargo_vlaue[7] << "\n";
		file << player.inship_cargo_vlaue[8] << "\n";
		file << player.inship_cargo_vlaue[9] << "\n";
		file << player.inship_cargo_vlaue[10] << "\n";
		file << player.inship_cargo_vlaue[11] << "\n";

		//현재시간
		file << realtime.year << "\n";
		file << realtime.month << "\n";
		file << realtime.day << "\n";

		file.close();
	}
	else
	{
		std::cout << "Unable to open cargo file" << std::endl;
		
	}
}
void roadsav()
{
	int num;
	std::string line;
	std::fstream file("savdata.sav");
	if (file.is_open())
	{
		//이름, 국적
		getline(file, line);
		player.player_name =line;

		getline(file, line);
		num = stoi(line);
		player.palyer_country = (CountryTag)num;
		

		//항구---------
		//위치
		getline(file, line);
		player.nowPort.name = line;
		getline(file, line);
		num = stoi(line);
		player.nowPort.localT= (LocalTag)num;
		getline(file, line);
		num = stoi(line);
		player.nowPort.conT = (CountryTag)num;
		getline(file, line);
		num = stoi(line);
		player.nowPort.port_Position.x_pos =num;
		getline(file, line);
		num = stoi(line);
		player.nowPort.port_Position.y_pos = num;

		//항구-구매
		for (int i = 0; i < 4; i++)
		{
			getline(file, line);
			player.nowPort.cargo[i].cargoName = line;
			getline(file, line);
			num = stoi(line);
			player.nowPort.cargo[i].cargoNumber = num;
			getline(file, line);
			num = stoi(line);
			player.nowPort.cargo[i].defaultPrice = num;
			getline(file, line);
			num = stoi(line);
			player.nowPort.cargo[i].defaultStoke = num;
		}

		//항구-판매
		for (int i = 0; i < 10; i++)
		{
			getline(file, line);
			player.nowPort.cargoStock[i].cargoName = line;
			getline(file, line);
			num = stoi(line);
			player.nowPort.cargoStock[i].cargoNumber = num;
			getline(file, line);
			num = stoi(line);
			player.nowPort.cargoStock[i].defaultPrice = num;
			getline(file, line);
			num = stoi(line);
			player.nowPort.cargoStock[i].defaultStoke = num;
		}
		//항구-----------------------

		//재산
		getline(file, line);
		num = stoi(line);
		player.money = num;
		getline(file, line);
		num = stoi(line);
		player.truemoney = num;
		getline(file, line);
		num = stoi(line);
		player.Debt = num;
		getline(file, line);
		num = stoi(line);
		player.Debt_val = num;
		
		//대출 만기일
		getline(file, line);
		num = stoi(line);
		player.DebtEndTime.year = num;
		getline(file, line);
		num = stoi(line);
		player.DebtEndTime.month = num;
		getline(file, line);
		num = stoi(line);
		player.DebtEndTime.day = num;

		//함선------------
		getline(file, line);
		player.my_ship.shipName = line;
		getline(file, line);
		num = stoi(line);
		player.my_ship.shipDefalutSpeed = num;
		getline(file, line);
		num = stoi(line);
		player.my_ship.shipRealSpeed = num;
		getline(file, line);
		num = stoi(line);
		player.my_ship.shipCargo = num;
		getline(file, line);
		num = stoi(line);
		player.my_ship.shipNowCargo = num;
		getline(file, line);
		num = stoi(line);
		player.my_ship.needCrew = num;
		getline(file, line);
		num = stoi(line);
		player.my_ship.maxCrew = num;
		getline(file, line);
		num = stoi(line);
		player.my_ship.shipPrice = num;
		//함선------------

		//인벤토리
		getline(file, line);
		num = stoi(line);
		player.nowman = num;

		//카고-----
		//화물0
		for (int i = 0; i < 12; i++)
		{
			getline(file, line);
			player.inship_cargo[i].cargoName = line;
			getline(file, line);
			num = stoi(line);
			player.inship_cargo[i].cargoNumber = num;
			getline(file, line);
			num = stoi(line);
			player.inship_cargo[i].defaultPrice = num;
			getline(file, line);
			num = stoi(line);
			player.inship_cargo[i].defaultStoke = num;
		}

		for (int i = 0; i < 12; i++)
		{
			getline(file, line);
			num = stoi(line);
			player.inship_cargo_vlaue[i] = num;
		}

		//현재시간
		getline(file, line);
		num = stoi(line);
		realtime.year = num;
		getline(file, line);
		num = stoi(line);
		realtime.month = num;
		getline(file, line);
		num = stoi(line);
		realtime.day = num;

		file.close();
	}
	else
	{
		std::cout << "Unable to open cargo file" << std::endl;

	}
}