//항구내 활동에 필요한 코드들을 넣을것.
#include<iostream>
#include <random>
#include <cmath>
#include <windows.h>
#include "struct.h"
#include "general.h"
#include "port.h"
#include "dot.h"
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")


extern int sail_port;
extern Port targetport;
extern int sailling_day;
extern DayTime realtime;

bool alreadyinport = 0;

//항구 입장시
void InPort(Player* player)
{


	if (alreadyinport == 0)
	{
		if (player->nowPort.conT == 0 || player->nowPort.conT == 1 || player->nowPort.conT == 2 || player->nowPort.conT == 3 || player->nowPort.conT == 4 || player->nowPort.conT == 5 || player->nowPort.conT == 6 || player->nowPort.conT == 7)
		{
			PlaySound(L"sound\\europa.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
		}
		else if (player->nowPort.conT == 8)
		{
			PlaySound(L"sound\\islamic.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
		}
		else if (player->nowPort.conT == 9)
		{
			PlaySound(L"sound\\india.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
		}

		if (player->nowPort.localT == 3)
		{
			PlaySound(L"sound\\africa.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
		}

		printdayport(cleanportcoler);
		if (player->nowPort.conT == player->palyer_country)
		{

			GotoxyAndPrint(40, 15, "#=========================#");
			GotoxyAndPrint(40, 16, "| 고국에 온것을 환영하네! |");
			GotoxyAndPrint(40, 17, "#=========================#");
			movecurser(0);
		}

		else
		{
			GotoxyAndPrint(20, 15, "#=======================================================#");
			GotoxyAndPrint(20, 16, "| 우리 항구를 이용하려면 세금을 내야 하네. 300두카트야. |");
			GotoxyAndPrint(20, 17, "#=======================================================#");
			movecurser(0);
			player->truemoney -= 300;
			player->money -= 300;
		}
		alreadyinport = true;
		Inport_ValueCalculator(player);
	}
}

//무역거래소
void TradeCenter(Player* player)
{
	int Trade_action = 0;
	bool whilecontrol = 1;
	while (whilecontrol == 1)
	{
		system("cls");
		printUIaction(inport_UI_base);
		printportrait(trader_col);

		gotoxy(23, 1);
		printf("어서옵쇼, 무엇을 하시겠습니까요?\n");
		Trade_action = TradeCenterSelect();
		switch (Trade_action)
		{
			case 1://화물 구매
				printUIaction(inport_UI_base);
				printportrait(trader_col);
				gotoxy(23, 1);
				TradeCenter_BuyCargo(player);
				break;
			case 2://화물 판매
				TradeCenter_SellCargo(player);
				break;
			case 3://시세확인
				TradeCenter_ConfirmValue(player);
				break;
			case 4:
				gotoxy(23, 1);
				printf("아이고 고객님, 다음에 뵙죠!                      ");
				movecurser(0);
				whilecontrol = 0;
				break;
			default:
				break;
		}
	}
}

int TradeCenterSelect()
{
	{
		int seleting = 1;
		int sel_save = 1;

		int x = 23;
		int y = 10;
		GotoxyAndPrint(x, y, "> 화물 구매");
		GotoxyAndPrint(x, y + 1, "  화물 판매");
		GotoxyAndPrint(x, y + 2, "  시세 확인");
		GotoxyAndPrint(x, y + 3, "  돌아 가기");

		while (true)
		{
			seleting = movecurser(seleting);
			if (seleting < 1 && seleting != -10) seleting = 4;
			else if (seleting > 4 && seleting != -10) seleting = 1;

			switch (seleting)
			{
				case 1:
					GotoxyAndPrint(x, y, "> 화물 구매");
					GotoxyAndPrint(x, y + 1, "  화물 판매");
					GotoxyAndPrint(x, y + 2, "  시세 확인");
					GotoxyAndPrint(x, y + 3, "  돌아 가기");
					break;
				case 2:
					GotoxyAndPrint(x, y, "  화물 구매");
					GotoxyAndPrint(x, y + 1, "> 화물 판매");
					GotoxyAndPrint(x, y + 2, "  시세 확인");
					GotoxyAndPrint(x, y + 3, "  돌아 가기");
					break;
				case 3:
					GotoxyAndPrint(x, y, "  화물 구매");
					GotoxyAndPrint(x, y + 1, "  화물 판매");
					GotoxyAndPrint(x, y + 2, "> 시세 확인");
					GotoxyAndPrint(x, y + 3, "  돌아 가기");
					break;
				case 4:
					GotoxyAndPrint(x, y, "  화물 구매");
					GotoxyAndPrint(x, y + 1, "  화물 판매");
					GotoxyAndPrint(x, y + 2, "  시세 확인");
					GotoxyAndPrint(x, y + 3, "> 돌아 가기");
					break;
				case -10:
					return sel_save;
			}
			sel_save = seleting;
		}
	}
}

void TradeCenter_BuyCargo(Player* player)
{
	system("cls");
	printUIaction(inport_UI_base);
	printportrait(trader_col);
	gotoxy(23, 1);
	GotoxyAndPrint(23, 1, "무얼 사실건가요?");
	int buypick = 0;
	int buycount = 0;
	//판매 물품 4개중 구매 물품을 지정
	buypick = buySelect(player);
	if (buypick == 4) return;

	//항구에 잔여량이 있는지 확인
	buybuy(player, buypick, buycount);
}

void FoundSpaceAndPush(Player* player, int buypick, int couunt)
{
	int aleadyfound = 0;
	int index = 0;
	//플레이어 인쉽 카고 배열에 빈 인덱스or 이미 그 카고타입이 있는지 확인
	if (aleadyfound == 0)
	{
		for (int i = 0; i < 12; i++)
			if (player->inship_cargo[i].cargoNumber == player->nowPort.cargo[buypick].cargoNumber)
			{
				index = i;
				aleadyfound = 1;
			}
		//2.있다면 해당 인덱스 번호와 동일한 카고 벨류 인덱스에 화물 벨류 추가
		if (aleadyfound == 1)player->inship_cargo_vlaue[index] += couunt;

	}
	if (aleadyfound == 0)
	{
		for (int i = 0; i < 12; i++)
			if (player->inship_cargo[i].cargoNumber == 0)
			{
				index = i;
				break;
			}
		//1.없다면 for문을 돌려서 나온 빈공간 인덱스에 카고타입 삽입
		player->inship_cargo[index] = player->nowPort.cargo[buypick];
		//1.인쉽카고벨류 배열의 동일 인덱스에 화물 벨류 추가
		player->inship_cargo_vlaue[index] += couunt;
	}
}

void buybuy(Player* player, int buypick, int buycount)
{

	int curser = 0;
	while (player->nowPort.cargo[buypick].cargoNumber == player->nowPort.cargoStock[curser].cargoNumber)
	{
		curser++;
	}

	if (player->nowPort.cargoStock[curser].defaultStoke != 0)
	{
		//얼마나 구매할지 지정
		system("cls");
		printUIaction(inport_UI_base);
		printportrait(trader_col);
		gotoxy(23, 1);
		std::cout << player->nowPort.cargo[buypick].cargoName << "을 선택하셨습니다." << std::endl;
		GotoxyAndPrint(23, 2, "  얼마나 구매하시겠습니까?\n");
		gotoxy(23, 10);
		printf("수량 : ");
		scanf_s("%d", &buycount, 4);
		while (getchar() != '\n');
		if (buycount == -1 || buycount == 0)return;

		if (player->nowPort.cargoStock[curser].defaultStoke > buycount && buycount >= 0)
		{
			//금액이 충분한지 확인
			if (player->money < player->nowPort.cargo[buypick].defaultPrice * buycount)
			{
				system("cls");
				printUIaction(inport_UI_base);
				printportrait(trader_col);
				gotoxy(23, 1);
				printf("돈이 모자라신데요.\n");
				movecurser(0);
			}
			else
			{
				//함선에 빈 공간이 충분한지 확인
				if (player->my_ship.shipNowCargo + buycount <= player->my_ship.shipCargo)
				{
					FoundSpaceAndPush(player, buypick, buycount);
					player->money -= player->nowPort.cargo[buypick].defaultPrice * buycount;
					player->truemoney -= player->nowPort.cargo[buypick].defaultPrice * buycount;
					player->nowPort.cargo[buypick].defaultStoke -= buycount;

					player->my_ship.shipNowCargo += buycount;
				}
				else
				{
					system("cls");
					printUIaction(inport_UI_base);
					printportrait(trader_col);
					gotoxy(23, 1);
					printf("고객님 배에 다 안들어 갑니다.\n");
					movecurser(0);
				}
			}
		}
		else if (buycount <= 0)
		{
			system("cls");
			printUIaction(inport_UI_base);
			printportrait(trader_col);
			gotoxy(23, 1);
			printf("0보다 작은 갯수를 살 수 있다고 생각하시는 건가요?\n");
			movecurser(0);
		}
		else
		{
			system("cls");
			printUIaction(inport_UI_base);
			printportrait(trader_col);
			GotoxyAndPrint(23, 1, "그만큼이나 남아있진 않네요.\n");
		}
	}
	else
	{
		system("cls");
		printUIaction(inport_UI_base);
		printportrait(trader_col);
		GotoxyAndPrint(23, 1, "그건 재고가 없어요.\n");
	}
}

int buySelect(Player* player)
{
	int index[4] = { 0,0,0,0 };

	for (int i = 0; i < 12; i++)
	{
		if (player->nowPort.cargoStock[i].cargoNumber == player->nowPort.cargo[0].cargoNumber)index[0] = i;
		else if (player->nowPort.cargoStock[i].cargoNumber == player->nowPort.cargo[1].cargoNumber)index[1] = i;
		else if (player->nowPort.cargoStock[i].cargoNumber == player->nowPort.cargo[2].cargoNumber)index[2] = i;
		else if (player->nowPort.cargoStock[i].cargoNumber == player->nowPort.cargo[3].cargoNumber)index[3] = i;
	}


	int seleting = 0;
	int sel_save = 0;

	int x = 23;
	int y = 11;
	GotoxyAndPrint(23, 10, "    이름   |  가격   수량");
	GotoxyAndPrint(x, y, "");
	std::cout << "> " << player->nowPort.cargo[0].cargoName << " |  " << player->nowPort.cargo[0].defaultPrice << "     " << player->nowPort.cargoStock[index[0]].defaultStoke;
	GotoxyAndPrint(x, y + 1, "");
	std::cout << "  " << player->nowPort.cargo[1].cargoName << " |  " << player->nowPort.cargo[1].defaultPrice << "     " << player->nowPort.cargoStock[index[1]].defaultStoke;
	GotoxyAndPrint(x, y + 2, "");
	std::cout << "  " << player->nowPort.cargo[2].cargoName << " |  " << player->nowPort.cargo[2].defaultPrice << "     " << player->nowPort.cargoStock[index[2]].defaultStoke;
	GotoxyAndPrint(x, y + 3, "");
	std::cout << "  " << player->nowPort.cargo[3].cargoName << " |  " << player->nowPort.cargo[3].defaultPrice << "     " << player->nowPort.cargoStock[index[3]].defaultStoke;
	GotoxyAndPrint(x, y + 4, "   돌아가기");

	while (true)
	{
		seleting = movecurser(seleting);
		if (seleting < 0 && seleting != -10) seleting = 4;
		else if (seleting > 4 && seleting != -10) seleting = 0;

		switch (seleting)
		{
			case 0:
				GotoxyAndPrint(x, y, "");
				std::cout << "> " << player->nowPort.cargo[0].cargoName << " |  " << player->nowPort.cargo[0].defaultPrice << "     " << player->nowPort.cargoStock[index[0]].defaultStoke;
				GotoxyAndPrint(x, y + 1, "");
				std::cout << "  " << player->nowPort.cargo[1].cargoName << " |  " << player->nowPort.cargo[1].defaultPrice << "     " << player->nowPort.cargoStock[index[1]].defaultStoke;
				GotoxyAndPrint(x, y + 2, "");
				std::cout << "  " << player->nowPort.cargo[2].cargoName << " |  " << player->nowPort.cargo[2].defaultPrice << "     " << player->nowPort.cargoStock[index[2]].defaultStoke;
				GotoxyAndPrint(x, y + 3, "");
				std::cout << "  " << player->nowPort.cargo[3].cargoName << " |  " << player->nowPort.cargo[3].defaultPrice << "     " << player->nowPort.cargoStock[index[3]].defaultStoke;
				GotoxyAndPrint(x, y + 4, "   돌아가기");
				break;
			case 1:
				GotoxyAndPrint(x, y, "");
				std::cout << "  " << player->nowPort.cargo[0].cargoName << " |  " << player->nowPort.cargo[0].defaultPrice << "     " << player->nowPort.cargoStock[index[0]].defaultStoke;
				GotoxyAndPrint(x, y + 1, "");
				std::cout << "> " << player->nowPort.cargo[1].cargoName << " |  " << player->nowPort.cargo[1].defaultPrice << "     " << player->nowPort.cargoStock[index[1]].defaultStoke;
				GotoxyAndPrint(x, y + 2, "");
				std::cout << "  " << player->nowPort.cargo[2].cargoName << " |  " << player->nowPort.cargo[2].defaultPrice << "     " << player->nowPort.cargoStock[index[2]].defaultStoke;
				GotoxyAndPrint(x, y + 3, "");
				std::cout << "  " << player->nowPort.cargo[3].cargoName << " |  " << player->nowPort.cargo[3].defaultPrice << "     " << player->nowPort.cargoStock[index[3]].defaultStoke;
				GotoxyAndPrint(x, y + 4, "   돌아가기");
				break;
			case 2:
				GotoxyAndPrint(x, y, "");
				std::cout << "  " << player->nowPort.cargo[0].cargoName << " |  " << player->nowPort.cargo[0].defaultPrice << "     " << player->nowPort.cargoStock[index[0]].defaultStoke;
				GotoxyAndPrint(x, y + 1, "");
				std::cout << "  " << player->nowPort.cargo[1].cargoName << " |  " << player->nowPort.cargo[1].defaultPrice << "     " << player->nowPort.cargoStock[index[1]].defaultStoke;
				GotoxyAndPrint(x, y + 2, "");
				std::cout << "> " << player->nowPort.cargo[2].cargoName << " |  " << player->nowPort.cargo[2].defaultPrice << "     " << player->nowPort.cargoStock[index[2]].defaultStoke;
				GotoxyAndPrint(x, y + 3, "");
				std::cout << "  " << player->nowPort.cargo[3].cargoName << " |  " << player->nowPort.cargo[3].defaultPrice << "     " << player->nowPort.cargoStock[index[3]].defaultStoke;
				GotoxyAndPrint(x, y + 4, "   돌아가기");
				break;
			case 3:
				GotoxyAndPrint(x, y, "");
				std::cout << "  " << player->nowPort.cargo[0].cargoName << " |  " << player->nowPort.cargo[0].defaultPrice << "     " << player->nowPort.cargoStock[index[0]].defaultStoke;
				GotoxyAndPrint(x, y + 1, "");
				std::cout << "  " << player->nowPort.cargo[1].cargoName << " |  " << player->nowPort.cargo[1].defaultPrice << "     " << player->nowPort.cargoStock[index[1]].defaultStoke;
				GotoxyAndPrint(x, y + 2, "");
				std::cout << "  " << player->nowPort.cargo[2].cargoName << " |  " << player->nowPort.cargo[2].defaultPrice << "     " << player->nowPort.cargoStock[index[2]].defaultStoke;
				GotoxyAndPrint(x, y + 3, "");
				std::cout << "> " << player->nowPort.cargo[3].cargoName << " |  " << player->nowPort.cargo[3].defaultPrice << "     " << player->nowPort.cargoStock[index[3]].defaultStoke;
				GotoxyAndPrint(x, y + 4, "   돌아가기");
				break;
			case 4:
				GotoxyAndPrint(x, y, "");
				std::cout << "  " << player->nowPort.cargo[0].cargoName << " |  " << player->nowPort.cargo[0].defaultPrice << "     " << player->nowPort.cargoStock[index[0]].defaultStoke;
				GotoxyAndPrint(x, y + 1, "");
				std::cout << "  " << player->nowPort.cargo[1].cargoName << " |  " << player->nowPort.cargo[1].defaultPrice << "     " << player->nowPort.cargoStock[index[1]].defaultStoke;
				GotoxyAndPrint(x, y + 2, "");
				std::cout << "  " << player->nowPort.cargo[2].cargoName << " |  " << player->nowPort.cargo[2].defaultPrice << "     " << player->nowPort.cargoStock[index[2]].defaultStoke;
				GotoxyAndPrint(x, y + 3, "");
				std::cout << "  " << player->nowPort.cargo[3].cargoName << " |  " << player->nowPort.cargo[3].defaultPrice << "     " << player->nowPort.cargoStock[index[3]].defaultStoke;
				GotoxyAndPrint(x, y + 4, ">  돌아가기");
				break;
			case -10:
				return sel_save;
		}
		sel_save = seleting;
	}

}

void TradeCenter_SellCargo(Player* player)
{
	int numb = 0;
	int index[12] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };
	int callingnumb = 0;
	int countnumb = 0;
	//일단 인쉽 카고에서 none과 식량,음식을 제외한 모든 목록과 현재 가격을 출력(비출력상태에서 인덱스도 저장)
	for (int i = 0; i < 12; i++)
	{
		if (player->inship_cargo[i].cargoNumber != 0 && player->inship_cargo[i].cargoNumber != 1 && player->inship_cargo[i].cargoNumber != 2)
		{
			index[numb] = i;
			numb++;
		}
		else
		{
			index[i] = -1;
		}
	}
	system("cls");
	printUIaction(inport_UI_base);
	printportrait(trader_col);
	GotoxyAndPrint(23, 1, "파시는 겁니까?\n");

	callingnumb = SellSelect(player);
	if (callingnumb == -1) return;
	gotoxy(23, 1);
	std::cout << player->inship_cargo[index[callingnumb]].cargoName << "을 선택하셨습니다." << std::endl;
	gotoxy(23, 2);
	std::cout << "몇개를 판매하실 건가요?" << std::endl;
	gotoxy(23, 3);
	//판매할 화물의 개수를 입력
	if (callingnumb < 11)
	{
		GotoxyAndPrint(23, 1, "                                                                                               ");
		gotoxy(23, 9);
		printf("수량 : ");
		scanf_s("%d", &countnumb, 4);
		while (getchar() != '\n');
		if (countnumb == -1)return;

		if (player->inship_cargo_vlaue[index[callingnumb]] >= countnumb)
		{
			//인덱스에서 화물을 입력수만큼 제거
			player->inship_cargo_vlaue[index[callingnumb]] -= countnumb;
			if (player->inship_cargo_vlaue[index[callingnumb]] <= 0)
			{
				//인덱스가 0이 되면 화물목록에 none을 입력
				GotoxyAndPrint(23, 1, "지정하신 화물을 모두 팔았습니다.\n");
				GetCargoSetiing(&player->inship_cargo[index[callingnumb]], "cargo\\0none.txt");
				movecurser(0);
			}
			else
			{
				gotoxy(23, 1);
				printf("%d개를 팔았습니다.\n", countnumb);
				movecurser(0);
			}
			player->my_ship.shipNowCargo -= countnumb;
			player->nowPort.cargoStock[index[callingnumb]].defaultStoke += countnumb;
			player->money += countnumb * player->nowPort.cargoStock[index[callingnumb]].defaultPrice;
			player->truemoney += countnumb * player->nowPort.cargoStock[index[callingnumb]].defaultPrice;

		}
		else
		{
			gotoxy(23, 1);
			printf("없는 걸 팔겠다고 하시는 겁니까?\n");
			movecurser(0);
		}
	}
}

void TradeCenter_ConfirmValue(Player* player)
{
	system("cls");
	printUIaction(inport_UI_base);
	printportrait(trader_col);
	GotoxyAndPrint(23, 1, "지금 가격은 이렇습니다.\n");
	printf("\n");
	for (int i = 0; i < 10; i++)
	{
		gotoxy(23, 10 + i);
		std::cout << player->nowPort.cargoStock[i].cargoName << " | 가격: " << player->nowPort.cargoStock[i].defaultPrice << " 두카트 / 수량 : " << player->nowPort.cargoStock[i].defaultStoke << std::endl;
	}
	movecurser(0);
}

void TradeCenter_ValueCalculator(Player* player)
{
	std::random_device random;
	std::mt19937 gen(random());
	std::uniform_int_distribution<int> dis(2, 13);

	for (int i = 0; i < 12; i++)
	{
		if (player->nowPort.cargoStock[i].defaultStoke > 200)player->nowPort.cargoStock[i].defaultPrice = player->nowPort.cargoStock[i].defaultPrice - ((player->nowPort.cargoStock->defaultStoke * 0.5f) / 100);
		else player->nowPort.cargoStock[i].defaultPrice = player->nowPort.cargoStock[i].defaultPrice + (player->nowPort.cargoStock[i].defaultStoke / 5);
		if (player->nowPort.cargoStock[i].defaultPrice <= 0)player->nowPort.cargoStock[i].defaultPrice = dis(gen);


		if (player->nowPort.conT == India && player->nowPort.cargoStock[i].cargoNumber == 7)
			player->nowPort.cargoStock[i].defaultStoke /= 10;
		else if (player->nowPort.conT == India && player->nowPort.cargoStock[i].cargoNumber == 10)
			player->nowPort.cargoStock[i].defaultStoke /= 5;
		else if (player->nowPort.conT == France && player->nowPort.cargoStock[i].cargoNumber == 12)
			player->nowPort.cargoStock[i].defaultStoke /= 2;
		else if (player->nowPort.conT == England && player->nowPort.cargoStock[i].cargoNumber == 10)
			player->nowPort.cargoStock[i].defaultStoke *= 2;
	}
	int index[4] = { 0,0,0,0 };

	for (int i = 0; i < 12; i++)
	{
		if (player->nowPort.cargoStock[i].cargoNumber == player->nowPort.cargo[0].cargoNumber)index[0] = i;
		else if (player->nowPort.cargoStock[i].cargoNumber == player->nowPort.cargo[1].cargoNumber)index[1] = i;
		else if (player->nowPort.cargoStock[i].cargoNumber == player->nowPort.cargo[2].cargoNumber)index[2] = i;
		else if (player->nowPort.cargoStock[i].cargoNumber == player->nowPort.cargo[3].cargoNumber)index[3] = i;
	}

	for (int i = 0; i < 4; i++)
	{
		player->nowPort.cargo[i].defaultPrice = player->nowPort.cargoStock[index[i]].defaultPrice;
	}
}

void Tulipvalue(Player* player)
{
	static float pre_tulipvalue = 5;

	if (realtime.year == 1636)
	{
		if (realtime.month == 11 && realtime.year > 5)
		{

			if (realtime.day % 5 == 0) pre_tulipvalue += 22.56;
		}

		else if (realtime.month == 12)
		{
			if (realtime.day % 5 == 0) pre_tulipvalue += 22.56;
		}
		else return;
	}

	else if (realtime.year == 1637)
	{
		switch (realtime.month)
		{
			case 1:
				if (realtime.day % 5 == 0) pre_tulipvalue += 22.56;
				break;
			case 2:
				if (realtime.day < 3) pre_tulipvalue += 22.56;
				else if (realtime.day % 5 == 0) pre_tulipvalue -= 22.56;
				break;
			default:
				if (realtime.day % 5 == 0) pre_tulipvalue -= 22.56;
				break;
		}
	}
	if (pre_tulipvalue <= 0) player->nowPort.cargoStock[8].defaultPrice = 1;
	else player->nowPort.cargoStock[8].defaultPrice = pre_tulipvalue;
}


int SellSelect(Player* player)
{
	{
		int seleting = 0;
		int sel_save = 0;

		int numb = 0;
		int index[12] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };
		int callingnumb = 0;
		int countnumb = 0;


		for (int i = 0; i < 12; i++)
		{
			if (player->inship_cargo[i].cargoNumber != 0 && player->inship_cargo[i].cargoNumber != 1 && player->inship_cargo[i].cargoNumber != 2)
			{
				index[numb] = i;
				numb++;
			}
			else
			{
				index[i] = -1;
			}
		}

		if (index[0] == -1)
		{
			system("cls");
			printUIaction(inport_UI_base);
			printportrait(trader_col);
			GotoxyAndPrint(23, 1, "제가 사갈게 없는걸요.\n");
			seleting = movecurser(seleting);
			return -1;
		}

		for (int i = 0; i < 12; i++)
		{
			if (index[i] == -1) break;
			gotoxy(23, 10 + i);
			if (i == 0) std::cout << "> ";
			else if (i != 0) std::cout << "  ";
			std::cout << player->inship_cargo[index[i]].cargoName << "| 가격 :" << player->nowPort.cargoStock[index[i]].defaultPrice << "보유량 : " << player->inship_cargo_vlaue[index[i]] << std::endl;
		}


		while (true)
		{
			numb = 0;
			for (int i = 0; i < 12; i++)
			{
				if (player->inship_cargo[i].cargoNumber != 0 && player->inship_cargo[i].cargoNumber != 1 && player->inship_cargo[i].cargoNumber != 2)
				{
					index[numb] = i;
					numb++;
				}
				else
				{
					index[i] = -1;
				}
			}

			seleting = movecurser(seleting);
			if (seleting < 0 && seleting != -10) seleting = numb - 1;
			else if (seleting > numb - 1 && seleting != -10) seleting = 0;

			for (int i = 0; i < 12; i++)
			{
				if (index[i] == -1) break;
				if (player->inship_cargo[index[i]].cargoNumber != 0 && player->inship_cargo[index[i]].cargoNumber != 1 && player->inship_cargo[index[i]].cargoNumber != 2)
				{
					gotoxy(23, 10 + i);
					if (i == seleting) std::cout << "> ";
					else if (i != seleting) std::cout << "  ";
					std::cout << player->inship_cargo[index[i]].cargoName << "| 가격 :" << player->nowPort.cargoStock[index[i]].defaultPrice << "보유량 : " << player->inship_cargo_vlaue[index[i]] << std::endl;
				}
			}

			if (seleting == -10)return sel_save;
			sel_save = seleting;
		}
	}
}

void Inport_ValueCalculator(Player* player)
{
	std::random_device random;
	std::mt19937 gen(random());
	std::uniform_int_distribution<int> dis(100, 400);

	for (int i = 0; i < (sizeof(player->nowPort.cargoStock) / sizeof(S_Cargo_type)); i++)
	{
		player->nowPort.cargoStock[i].defaultStoke = dis(gen);
	}

	for (int i = 0; i < (sizeof(player->nowPort.cargo) / sizeof(S_Cargo_type)); i++)
	{
		player->nowPort.cargo[i].defaultStoke = dis(gen);
	}
	TradeCenter_ValueCalculator(player);
	Tulipvalue(player);
}

//은행
void Bank(Player* player, DayTime time)
{
	int bankAction = 0;
	int whilecontrol = 1;
	bool alreadySayHello = 0;


	while (whilecontrol)
	{
		system("cls");
		printUIaction(inport_UI_base);
		printportrait(bank_col);
		gotoxy(23, 1);
		if (alreadySayHello == 0)
		{
			std::cout << "아, " << player->player_name << "님이시군요.";
			alreadySayHello = 1;
			gotoxy(23, 2);
		}

		printf("어떤 서비스를 바라십니까 ? ");
		bankAction = BankSelect();
		switch (bankAction)
		{
			case 1://자금 대출
				if (player->Debt == 0) //빚이 없어
				{
					int canDebt = (player->truemoney * 80) / 100;
					int yesno = 0;
					system("cls");
					printUIaction(inport_UI_base);
					printportrait(bank_col);
					gotoxy(23, 1);
					printf("고객님께선 %d 만큼 대출 가능하십니다.\n", canDebt);
					gotoxy(23, 2);
					printf("이자율은 10\% 입니다.\n");
					yesno = yesnosel();
					if (yesno == 1)
					{
						system("cls");
						printUIaction(inport_UI_base);
						printportrait(bank_col);
						gotoxy(23, 1);
						printf("여기 있습니다. 상환일은 10개월 뒤 입니다.\n");
						movecurser(0);
						player->Debt_val = 0.1f;
						player->Debt = canDebt;
						player->money += canDebt;
						player->truemoney -= canDebt;
						player->DebtEndTime = time;
						for (int i = 0; i < 300; i++)
						{
							TimeCounter(&player->DebtEndTime);
						}
					}
					else if (yesno == 2)
					{
						system("cls");
						printUIaction(inport_UI_base);
						printportrait(bank_col);
						gotoxy(23, 1);
						printf("대출받지 않기로 결정하셨군요? 알겠습니다.\n");
						movecurser(0);
					}
					else
					{
						system("cls");
						printUIaction(inport_UI_base);
						printportrait(bank_col);
						gotoxy(23, 1);
						printf("무슨말씀이신지 잘 모르겠군요.\n");
						movecurser(0);
					}
				}
				else if (player->Debt > player->money) //빚이 보유금보다 많아
				{
					system("cls");
					printUIaction(inport_UI_base);
					printportrait(bank_col);
					gotoxy(23, 1);
					printf("고객님께선 대출 가능 대상자가 아닙니다.\n빚이 너무 많거나 자본금이 모자랍니다.\n");
					movecurser(0);
				}
				else //빚이 보유금 보단 적어
				{
					int canDebt = ((player->truemoney - player->Debt) * 80) / 100;
					if (canDebt <= 0)
					{
						system("cls");
						printUIaction(inport_UI_base);
						printportrait(bank_col);
						gotoxy(23, 1);
						printf("확인해 보니 안되겠네요.\n저희는 고객님의 실질자산을 기준으로 대출금을 책정합니다.\n");
						movecurser(0);
						return;
					}
					int yesno = 0;
					system("cls");
					printUIaction(inport_UI_base);
					printportrait(bank_col);
					gotoxy(23, 1);
					printf("고객님의 자본금 중 대출금을 제외한 %d 만큼 대출 가능하십니다.\n", canDebt);
					gotoxy(23, 2);
					printf("이자율은 여전히 10\% 입니다.\n");
					yesno = yesnosel();
					if (yesno == 1)
					{
						player->Debt += canDebt;
						player->money += canDebt;
						player->truemoney -= canDebt;
					}
					else if (yesno == 2)
					{
						system("cls");
						printUIaction(inport_UI_base);
						printportrait(bank_col);
						gotoxy(23, 1);
						printf("대출받지 않기로 결정하셨군요? 알겠습니다.\n");
						movecurser(0);
					}
					else
					{

					}
				}
				break;
			case 2://대출 상환
				if (player->Debt == 0) //빚이 없어
				{
					system("cls");
					printUIaction(inport_UI_base);
					printportrait(bank_col);
					gotoxy(23, 1);
					printf("상환하실 금액이 없으십니다.\n");
					movecurser(0);
				}
				else if (player->Debt > player->money)//빚갚을 돈이 없어
				{
					system("cls");
					printUIaction(inport_UI_base);
					printportrait(bank_col);
					gotoxy(23, 1);
					printf("고객님께선 상환하시기에 자본금이 충분하지 못하십니다.\n일부상환은 불가능합니다.\n");
					movecurser(0);
				}
				else
				{
					system("cls");
					printUIaction(inport_UI_base);
					printportrait(bank_col);
					gotoxy(23, 1);
					printf("부채상환을 끝내셨습니다. 축하드립니다.\n");//빚갚을 돈이 있어
					movecurser(0);
					player->money -= player->Debt;
					player->truemoney += player->Debt;
					player->Debt = 0;

					player->DebtEndTime.year = 9999;
					player->DebtEndTime.month = 99;
					player->DebtEndTime.day = 99;
					player->Debt_val = 0;
				}
				break;
			case 3://대출, 상환일 확인
				system("cls");
				printUIaction(inport_UI_base);
				printportrait(bank_col);
				gotoxy(23, 1);
				printf("고객님께선 %d만큼의 금액을 상환하셔야 합니다.\n", player->Debt);
				gotoxy(23, 2);
				printf("기한은 %d년 %d월 %d일 까지 입니다.\n", player->DebtEndTime.year, player->DebtEndTime.month, player->DebtEndTime.day);
				movecurser(0);
				break;
			case 4:
				system("cls");
				printUIaction(inport_UI_base);
				printportrait(bank_col);
				gotoxy(23, 1);
				printf("안녕히 가세요!\n");
				movecurser(0);
				whilecontrol = 0;
				break;
			default:
				break;
		}
	}

}

int BankSelect()
{
	{
		int seleting = 1;
		int sel_save = 1;

		int x = 23;
		int y = 10;
		GotoxyAndPrint(x, y, "> 자금 대출");
		GotoxyAndPrint(x, y + 1, "  대출 상환");
		GotoxyAndPrint(x, y + 2, "  부채 확인");
		GotoxyAndPrint(x, y + 3, "  돌아 가기");

		while (true)
		{
			seleting = movecurser(seleting);
			if (seleting < 1 && seleting != -10) seleting = 4;
			else if (seleting > 4 && seleting != -10) seleting = 1;

			switch (seleting)
			{
				case 1:
					GotoxyAndPrint(x, y, "> 자금 대출");
					GotoxyAndPrint(x, y + 1, "  대출 상환");
					GotoxyAndPrint(x, y + 2, "  부채 확인");
					GotoxyAndPrint(x, y + 3, "  돌아 가기");
					break;
				case 2:
					GotoxyAndPrint(x, y, "  자금 대출");
					GotoxyAndPrint(x, y + 1, "> 대출 상환");
					GotoxyAndPrint(x, y + 2, "  부채 확인");
					GotoxyAndPrint(x, y + 3, "  돌아 가기");
					break;
				case 3:
					GotoxyAndPrint(x, y, "  자금 대출");
					GotoxyAndPrint(x, y + 1, "  대출 상환");
					GotoxyAndPrint(x, y + 2, "> 부채 확인");
					GotoxyAndPrint(x, y + 3, "  돌아 가기");
					break;
				case 4:
					GotoxyAndPrint(x, y, "  자금 대출");
					GotoxyAndPrint(x, y + 1, "  대출 상환");
					GotoxyAndPrint(x, y + 2, "  부채 확인");
					GotoxyAndPrint(x, y + 3, "> 돌아 가기");
					break;
				case -10:
					return sel_save;
			}
			sel_save = seleting;
		}
	}
}

int yesnosel()
{
	{
		int seleting = 1;
		int sel_save = 1;

		int x = 23;
		int y = 10;
		GotoxyAndPrint(x, y, "> 네");
		GotoxyAndPrint(x, y + 1, "  아니요");

		while (true)
		{
			seleting = movecurser(seleting);
			if (seleting < 1 && seleting != -10) seleting = 2;
			else if (seleting > 2 && seleting != -10) seleting = 1;

			switch (seleting)
			{
				case 1:
					GotoxyAndPrint(x, y, "> 네");
					GotoxyAndPrint(x, y + 1, "  아니요");
					break;
				case 2:
					GotoxyAndPrint(x, y, "  네");
					GotoxyAndPrint(x, y + 1, "> 아니요");
					break;
				case -10:
					return sel_save;
			}
			sel_save = seleting;
		}
	}
}

int yesnosel2()
{
	{
		int seleting = 1;
		int sel_save = 1;

		int x = 45;
		int y = 19;
		GotoxyAndPrint(x, y, "> 네    ");
		GotoxyAndPrint(x, y + 1, "  아니요");

		while (true)
		{
			seleting = movecurser(seleting);
			if (seleting < 1 && seleting != -10) seleting = 2;
			else if (seleting > 2 && seleting != -10) seleting = 1;

			switch (seleting)
			{
				case 1:
					GotoxyAndPrint(x, y, "> 네    ");
					GotoxyAndPrint(x, y + 1, "  아니요");
					break;
				case 2:
					GotoxyAndPrint(x, y, "  네    ");
					GotoxyAndPrint(x, y + 1, "> 아니요");
					break;
				case -10:
					return sel_save;
			}
			sel_save = seleting;
		}
	}
}

//여관
void Tavern(Player* player, DayTime* time)
{
	int whilecontrol = 1;
	while (whilecontrol)
	{
		int action = 0;

		system("cls");
		printUIaction(inport_UI_base);
		printportrait(tavern_col);
		gotoxy(23, 1);
		printf("어서오세요, 잠자기에 이만한 데가 없다구요.\n");
		action = Tavernsel(action);

		int callman = 0;
		int mancount = 0;
		switch (action)
		{
			case 1: //숙박
				system("cls");
				printUIaction(inport_UI_base);
				printportrait(tavern_col);
				gotoxy(23, 1);
				printf("하루를 보냈습니다\n.");
				movecurser(0);
				TimeCounter(time);
				if ((realtime.year == player->DebtEndTime.year) && (realtime.month == player->DebtEndTime.month) && (realtime.day == player->DebtEndTime.day))
				{
					boxprint();
					gotoxy(40, 11);
					printf("통지서");
					GotoxyAndPrint(21, 13, "은행입니다. 빚을 갚아야 할 시기가 도래하셨더군요.");
					GotoxyAndPrint(21, 14, "원래 채무란 가차없는 법 입니다.");
					movecurser(0);
					if (player->money >= player->Debt)
					{
						player->money -= player->Debt;
						player->truemoney += player->Debt;
						player->DebtEndTime.year = 9999;
						player->DebtEndTime.month = 99;
						player->DebtEndTime.day = 99;
						player->Debt_val = 0;
						player->Debt = 0;

						GotoxyAndPrint(21, 16, "하지만 다행히도 적당한 금액을 가지고 계셨더군요.");
						GotoxyAndPrint(21, 17, "계좌에서 인출했습니다.");
						GotoxyAndPrint(21, 18, "이용해주셔서 감사합니다.");
						movecurser(0);
					}
					else if (player->money < player->Debt)
					{
						player->Debt -= player->money;
						player->truemoney += player->money;
						player->money = 0;
						player->truemoney -= (player->Debt * 30) / 100;
						player->Debt += (player->Debt * 30) / 100;

						for (int i = 0; i < 150; i++)
						{
							TimeCounter(&player->DebtEndTime);
						}
						GotoxyAndPrint(21, 16, "안타깝게도 보유금 전부로도 자금이 모자라시더군요.");
						GotoxyAndPrint(21, 17, "일단 계좌에 있는 금액 전체를 인출했습니다.");
						GotoxyAndPrint(21, 18, "남은 대출금의 30 % 를 연체금으로 부과하겠습니다.");
						GotoxyAndPrint(21, 19, "연채시 만기일은 5달 뒤 입니다.\n");
						movecurser(0);
					}
				}
				break;
			case 2: //선원 고용
				system("cls");
				printUIaction(inport_UI_base);
				printportrait(tavern_col);
				GotoxyAndPrint(23, 1, "사람을 모아 달라구요? 제가 시간이 없네요.");
				GotoxyAndPrint(23, 2, "하지만 시간은 돈으로 살 수 있다잖아요? 50두카트면 되겠네요.");
				callman = yesnosel();
				if (callman == 1)
				{
					player->money -= 50;
					player->truemoney -= 50;
					system("cls");
					printUIaction(inport_UI_base);
					printportrait(tavern_col);
					gotoxy(23, 1);
					printf("예, 분부대로 뱃놈들 좀 모아왔습니다요. 몇명이나 필요하실까요?\n");

					gotoxy(23, 10);
					printf("수량 : ");
					scanf_s("%d", &mancount, 4);
					while (getchar() != '\n');
					if (mancount + player->nowman > player->my_ship.maxCrew)
					{
						system("cls");
						printUIaction(inport_UI_base);
						printportrait(tavern_col);
						gotoxy(23, 1);
						printf("그만한 인원은 못태울것 같은데요.\n");
						movecurser(0);
					}
					else if (mancount + player->nowman <= player->my_ship.maxCrew)
					{
						system("cls");
						printUIaction(inport_UI_base);
						printportrait(tavern_col);
						gotoxy(23, 1);
						printf("전부 승선했습니다요.\n");
						player->nowman += mancount;
						movecurser(0);
					}
				}
				else if (callman == 2)
				{
					system("cls");
					printUIaction(inport_UI_base);
					printportrait(tavern_col);
					gotoxy(23, 1);
					printf("예. 뭐, 제가 아쉬울건 없죠..\n");
					movecurser(0);

				}
				break;
			case 3:
				system("cls");
				printUIaction(inport_UI_base);
				printportrait(tavern_col);
				gotoxy(23, 1);
				printf("또 오세요!\n");
				movecurser(0);
				whilecontrol = 0;
				break;
			default:
				break;
		}
	}
}

int Tavernsel(int picker)
{
	int seleting = 1;
	int sel_save = 1;

	int x = 23;
	int y = 10;
	GotoxyAndPrint(x, y, ">   숙  박  ");
	GotoxyAndPrint(x, y + 1, "  선원 고용");
	GotoxyAndPrint(x, y + 2, "  돌아 가기");

	while (true)
	{
		seleting = movecurser(seleting);
		if (seleting < 1 && seleting != -10) seleting = 3;
		else if (seleting > 3 && seleting != -10) seleting = 1;

		switch (seleting)
		{
			case 1:
				GotoxyAndPrint(x, y, ">   숙  박  ");
				GotoxyAndPrint(x, y + 1, "  선원 고용");
				GotoxyAndPrint(x, y + 2, "  돌아 가기");
				break;
			case 2:
				GotoxyAndPrint(x, y, "    숙  박  ");
				GotoxyAndPrint(x, y + 1, "> 선원 고용");
				GotoxyAndPrint(x, y + 2, "  돌아 가기");
				break;
			case 3:
				GotoxyAndPrint(x, y, "    숙  박  ");
				GotoxyAndPrint(x, y + 1, "  선원 고용");
				GotoxyAndPrint(x, y + 2, "> 돌아 가기");
				break;
			case -10:
				return sel_save;
		}
		sel_save = seleting;
	}
}


//부두
void Port_port(Player* player, DayTime* time, int* goout)
{
	int switchControl = 0;
	int whileControl = 1;
	while (whileControl)
	{
		system("cls");
		printUIaction(inport_UI_base);
		printportrait(port_col);
		gotoxy(23, 1);
		printf("볼일이 뭐요?\n");
		switchControl = portnsel(switchControl);
		switch (switchControl)
		{
			case 1: // 보급
				system("cls");
				printUIaction(inport_UI_base);
				printportrait(port_col);
				gotoxy(23, 1);
				printf("저쪽으로 가보슈.\n");
				Portdealer(player);
				break;
			case 2: // 출항
				system("cls");
				printUIaction(inport_UI_base);
				printportrait(port_col);
				gotoxy(23, 1);
				TargetPortPicker(player, time, goout, &whileControl);
				break;
			case 3: // 돌아가기
				whileControl = 0;
				break;
			default:
				break;
		}
	}
}

void Portdealer(Player* player)
{
	int picker = 0;
	int i = 0;
	int sp_count = 0;
	system("cls");
	printUIaction(inport_UI_base);
	printportrait(port_col);
	gotoxy(23, 1);
	printf("뭐로 하실 겁니까?\n");
	picker = putout();
	if (i == 3)
	{
		return;
	}
	switch (picker)
	{
		case 1: //보급
			i = 0;
			sp_count = 0;
			system("cls");
			printUIaction(inport_UI_base);
			printportrait(port_col);
			gotoxy(23, 1);
			printf("보급 받으시는군요? 식량, 물 어느겁니까?\n");
			i = waterfood();
			if (i == 3)
			{
				return;
			}

			system("cls");
			printUIaction(inport_UI_base);
			printportrait(port_col);
			gotoxy(23, 1);
			printf("얼마나 받아가십니까?\n");

			gotoxy(23, 10);
			printf("수량 : ");
			scanf_s("%d", &sp_count);
			while (getchar() != '\n');

			FoundSpaceAndSUPP(player, i, sp_count);
			break;
		case 2: //내리기
			i = 0;
			sp_count = 0;
			system("cls");
			printUIaction(inport_UI_base);
			printportrait(port_col);
			gotoxy(23, 1);
			printf("반납 하시는 거군요? 식량, 물 어느겁니까?\n");
			i = waterfood();
			if (i == 3)
			{
				return;
			}

			system("cls");
			printUIaction(inport_UI_base);
			printportrait(port_col);
			gotoxy(23, 1);
			printf("얼마나 내리실겁니까?\n");
			gotoxy(23, 2);
			gotoxy(23, 10);
			printf("수량 : ");
			scanf_s("%d", &sp_count);
			while (getchar() != '\n');
			FoundCargoAndOUT(player, i, sp_count);
			break;
		case 3:
			return;
		default:
			break;
	}
}

void FoundSpaceAndSUPP(Player* player, int subpick, int couunt)
{
	if (player->my_ship.shipNowCargo + couunt <= player->my_ship.shipCargo)
	{
		int aleadyfound = 0;
		int index = 0;
		//플레이어 인쉽 카고 배열에 빈 인덱스or 이미 그 카고타입이 있는지 확인
		if (aleadyfound == 0)
		{
			for (int i = 0; i < 12; i++)
				if (player->inship_cargo[i].cargoNumber == subpick)
				{
					index = i;
					aleadyfound = 1;
				}
			//2.있다면 해당 인덱스 번호와 동일한 카고 벨류 인덱스에 화물 벨류 추가
			if (aleadyfound == 1)player->inship_cargo_vlaue[index] += couunt;

		}
		if (aleadyfound == 0)
		{
			for (int i = 0; i < 12; i++)
				if (player->inship_cargo[i].cargoNumber == 0)
				{
					index = i;
					break;
				}
			//1.없다면 for문을 돌려서 나온 빈공간 인덱스에 카고타입 삽입
			if (subpick == 1) GetCargoSetiing(&player->inship_cargo[index], "cargo\\1food.txt");
			else if (subpick == 2) GetCargoSetiing(&player->inship_cargo[index], "cargo\\2water.txt");
			//1.인쉽카고벨류 배열의 동일 인덱스에 화물 벨류 추가
			player->inship_cargo_vlaue[index] += couunt;
		}
		player->my_ship.shipNowCargo += couunt;
	}
	else
	{
		system("cls");
		printUIaction(inport_UI_base);
		printportrait(port_col);
		gotoxy(23, 1);
		printf("자리가 없으신것 같군요.\n");
		movecurser(0);

	}
}

void FoundCargoAndOUT(Player* player, int subpick, int couunt)
{
	int index = 0;
	int notfound = 0;
	//플레이어 인쉽 카고 배열에 빈 인덱스or 이미 그 카고타입이 있는지 확인
	for (int i = 0; i < 12; i++)
	{
		if (player->inship_cargo[i].cargoNumber == subpick)
		{
			index = i;
			notfound = 1;
		}
	}

	if (!notfound)
	{
		system("cls");
		printUIaction(inport_UI_base);
		printportrait(port_col);
		gotoxy(23, 1);
		printf("이미 없는데요?\n");
		movecurser(0);
		return;
	}

	else if (player->inship_cargo_vlaue[index] < couunt)
	{
		system("cls");
		printUIaction(inport_UI_base);
		printportrait(port_col);
		gotoxy(23, 1);
		printf("말씀하신 수량만큼은 안되네요.\n");
		movecurser(0);
		return;
	}
	//2.있다면 해당 인덱스 번호와 동일한 카고 벨류 인덱스에 화물 벨류 제거
	else
	{
		player->inship_cargo_vlaue[index] -= couunt;

		if (player->inship_cargo_vlaue[index] >= 0)GetCargoSetiing(&player->inship_cargo[index], "cargo\\0none.txt");
		player->my_ship.shipNowCargo -= couunt;
	}
}

void TargetPortPicker(Player* player, DayTime* time, int* goout, int* stoper)
{
	int picker = 0;
	int funcpicker = 1;
	int pickbuuf = 1;
	bool already = false;

	picker = TargetPortPickerPrinter();
	Port target_port;
	std::string file_path;
	float distan = 0;

	switch (picker)
	{
		case 1:
			file_path = "port\\lisbon.txt";
			break;
		case 2:
			file_path = "port\\valencia.txt";
			break;
		case 3:
			file_path = "port\\barcelona.txt";
			break;
		case 4:
			file_path = "port\\marseille.txt";
			break;
		case 5:
			file_path = "port\\genova.txt";
			break;
		case 6:
			file_path = "port\\napoli.txt";
			break;
		case 7:
			file_path = "port\\venezia.txt";
			break;
		case 8:
			file_path = "port\\istanbul.txt";
			break;
		case 9:
			file_path = "port\\aqyar.txt";
			break;
		case 10:
			file_path = "port\\algiers.txt";
			break;
		case 11:
			file_path = "port\\tunis.txt";
			break;
		case 12:
			file_path = "port\\alexandria.txt";
			break;
		case 13:
			file_path = "port\\mumbai.txt";
			break;
		case 14:
			file_path = "port\\capetown.txt";
			break;
		case 15:
			file_path = "port\\amsterdam.txt";
			break;
		case 16:
			file_path = "port\\dublin.txt";
			break;
		case 17:
			file_path = "port\\liverpool.txt";
			break;
		case 18:
			file_path = "port\\brest.txt";
			break;
		default:
			return;
	}

	GetPortSetiing(&target_port, file_path);
	if (player->nowPort.name == target_port.name)
	{

		system("cls");
		printUIaction(inport_UI_base);
		printportrait(port_col);
		gotoxy(23, 1);
		printf("지금 당신이 서있는 곳이라우.\n");
		movecurser(0);
	}

	else
	{
		system("cls");
		printUIaction(inport_UI_base);
		printportrait(port_col);
		gotoxy(23, 1);
		distan = TargetPortCalculator(player, target_port, time);
		printf("거리는 %d 입니다. 선장님 함선의 속도가 %d이므로 %d일이 걸립니다.", (int)distan, player->my_ship.shipRealSpeed, (int)distan / player->my_ship.shipRealSpeed);
		gotoxy(23, 2);
		printf("출항하시겠습니까?\n");
		int selectnum = 0;
		selectnum = yesnosel();
		if (player->my_ship.needCrew > player->nowman)
		{
			system("cls");
			printUIaction(inport_UI_base);
			printportrait(port_col);
			gotoxy(23, 1);
			printf("최소 요구 선원 수 보다 선원이 적습니다.");
			movecurser(0);
			selectnum = 2;
		}

		int yes = 0;
		for (int i = 0; i < 12; i++)
		{
			if (player->inship_cargo[i].cargoNumber == 1) yes++;
			else if (player->inship_cargo[i].cargoNumber == 2) yes++;
		}

		if (yes != 2)
		{
			system("cls");
			printUIaction(inport_UI_base);
			printportrait(port_col);
			gotoxy(23, 1);
			printf("식량과 물 둘 중 하나, 혹은 둘 모두 없습니다.");
			movecurser(0);
			selectnum = 2;
		}

		switch (selectnum)
		{
			case 1:
				goout = 0;
				sail_port = 4;
				targetport = target_port;
				sailling_day = (int)distan / player->my_ship.shipRealSpeed;
				alreadyinport = false;
				*stoper = 0;
				return;
			case 2:
				return;
			default:
				break;
		}
	}
}

int TargetPortPickerPrinter()
{
	int seleting = 1;
	int sel_save = 1;

	system("cls");
	printworldmap();
	gotoxy(0, 0);
	printf("어디로 갈거요?");
	gotoxy(3, 24);
	setcolor(15, 12);
	printf("  ");
	gotoxy(3, 25);
	setcolor(15, 0);
	printf("리스본, 포르투갈");

	while (true)
	{
		seleting = movecurser(seleting);
		if (seleting < 1 && seleting != -10) seleting = 18;
		else if (seleting > 18 && seleting != -10) seleting = 1;

		switch (seleting)
		{
			case 1:
				system("cls");
				printworldmap();
				gotoxy(0, 0);
				printf("어디로 갈거요?");
				gotoxy(3, 24);
				setcolor(15, 12);
				printf("  ");
				gotoxy(3, 25);
				setcolor(15, 0);
				printf("리스본, 포르투갈");
				break;
			case 2:
				system("cls");
				printworldmap();
				gotoxy(0, 0);
				printf("어디로 갈거요?");
				gotoxy(25, 24);
				setcolor(15, 12);
				printf("  ");
				gotoxy(25, 25);
				setcolor(15, 0);
				printf("발렌시아, 스페인");
				break;
			case 3:
				system("cls");
				printworldmap();
				gotoxy(0, 0);
				printf("어디로 갈거요?");
				gotoxy(28, 22);
				setcolor(15, 12);
				printf("  ");
				gotoxy(28, 23);
				setcolor(15, 0);
				printf("바르셀로나, 스페인");
				break;
			case 4:
				system("cls");
				printworldmap();
				gotoxy(0, 0);
				printf("어디로 갈거요?");
				gotoxy(40, 19);
				setcolor(15, 12);
				printf("  ");
				gotoxy(40, 20);
				setcolor(15, 0);
				printf("마르세유, 프랑스");
				break;
			case 5:
				system("cls");
				printworldmap();
				gotoxy(0, 0);
				printf("어디로 갈거요?");
				gotoxy(49, 18);
				setcolor(15, 12);
				printf("  ");
				gotoxy(49, 19);
				setcolor(15, 0);
				printf("제노바");
				break;
			case 6:
				system("cls");
				printworldmap();
				gotoxy(0, 0);
				printf("어디로 갈거요?");
				gotoxy(59, 22);
				setcolor(15, 12);
				printf("  ");
				gotoxy(59, 23);
				setcolor(15, 0);
				printf("나폴리,시칠리아");
				break;
			case 7:
				system("cls");
				printworldmap();
				gotoxy(0, 0);
				printf("어디로 갈거요?");
				gotoxy(59, 16);
				setcolor(15, 12);
				printf("  ");
				gotoxy(59, 17);
				setcolor(15, 0);
				printf("베네치아");
				break;
			case 8:
				system("cls");
				printworldmap();
				gotoxy(0, 0);
				printf("어디로 갈거요?");
				gotoxy(95, 21);
				setcolor(15, 12);
				printf("  ");
				gotoxy(82, 22);
				setcolor(15, 0);
				printf("이스탄불,오스만");
				break;
			case 9:
				system("cls");
				printworldmap();
				gotoxy(0, 0);
				printf("어디로 갈거요?");
				gotoxy(108, 17);
				setcolor(15, 12);
				printf("  ");
				gotoxy(95, 18);
				setcolor(15, 0);
				printf("아크야르,오스만");
				break;
			case 10:
				system("cls");
				printworldmap();
				gotoxy(0, 0);
				printf("어디로 갈거요?");
				gotoxy(28, 26);
				setcolor(15, 12);
				printf("  ");
				gotoxy(28, 27);
				setcolor(15, 0);
				printf("알제, 오스만");
				break;
			case 11:
				system("cls");
				printworldmap();
				gotoxy(0, 0);
				printf("어디로 갈거요?");
				gotoxy(52, 26);
				setcolor(15, 12);
				printf("  ");
				gotoxy(52, 27);
				setcolor(15, 0);
				printf("튀니스, 오스만");
				break;
			case 12:
				system("cls");
				printworldmap();
				gotoxy(0, 0);
				printf("어디로 갈거요?");
				gotoxy(97, 31);
				setcolor(15, 12);
				printf("  ");
				gotoxy(80, 32);
				setcolor(15, 0);
				printf("알렉산드리아,오스만");
				break;
			case 13:
				system("cls");
				printworldmap();
				gotoxy(0, 0);
				printf("어디로 갈거요?");
				gotoxy(96, 3);
				setcolor(15, 12);
				printf("  ");
				gotoxy(87, 4);
				setcolor(15, 0);
				printf("뭄바이,무굴");
				break;
			case 14:
				system("cls");
				printworldmap();
				printafricamap();
				gotoxy(0, 0);
				printf("어디로 갈거요?");
				gotoxy(36, 22);
				setcolor(15, 12);
				printf("  ");
				gotoxy(36, 23);
				setcolor(15, 0);
				printf("케이프 타운,포르투갈");
				break;
			case 15:
				system("cls");
				printworldmap();
				gotoxy(0, 0);
				printf("어디로 갈거요?");
				gotoxy(36, 9);
				setcolor(15, 12);
				printf("  ");
				gotoxy(36, 10);
				setcolor(15, 0);
				printf("암스테르담, 네덜란드");
				break;
			case 16:
				system("cls");
				printworldmap();
				gotoxy(0, 0);
				printf("어디로 갈거요?");
				gotoxy(10, 7);
				setcolor(15, 12);
				printf("  ");
				gotoxy(10, 8);
				setcolor(15, 0);
				printf("더블린, 잉글랜드");
				break;
			case 17:
				system("cls");
				printworldmap();
				gotoxy(0, 0);
				printf("어디로 갈거요?");
				gotoxy(17, 8);
				setcolor(15, 12);
				printf("  ");
				gotoxy(17, 9);
				setcolor(15, 0);
				printf("리버풀, 잉글랜드");
				break;
			case 18:
				system("cls");
				printworldmap();
				gotoxy(0, 0);
				printf("어디로 갈거요?");
				gotoxy(15, 14);
				setcolor(15, 12);
				printf("  ");
				gotoxy(15, 15);
				setcolor(15, 0);
				printf("브레스트, 프랑스");
				break;
			case -10:
				return sel_save;
		}
		sel_save = seleting;
	}
}

float TargetPortCalculator(Player* player, Port target_port, DayTime* time)
{
	PortPosition now_pos = player->nowPort.port_Position;
	PortPosition target_pos = target_port.port_Position;

	PortPosition Gibraltar = { Gibraltar.x_pos = -5, Gibraltar.y_pos = 36 };
	PortPosition Dakar = { Dakar.x_pos = -17, Dakar.x_pos = 14 };
	PortPosition Aden = { Aden.x_pos = 45,Aden.y_pos = 12 };
	PortPosition CapeTown = { CapeTown.x_pos = 17,CapeTown.y_pos = -34 };

	//둘이 지역 태그가 같다면
	if (player->nowPort.localT == target_port.localT)
	{
		double Distace = 0;

		Distace = sqrt(pow(now_pos.x_pos - target_pos.x_pos, 2) + pow(now_pos.y_pos - target_pos.y_pos, 2));
		return Distace;
	}
	else
	{
		//누구 하나라도 아프리카나 인도를 가진다면
		if ((player->nowPort.localT == India_Asia || player->nowPort.localT == Africa) || (target_port.localT == India_Asia || target_port.localT == Africa))
		{
			//목표가 아프리카, 현위치가 인도거나 목표가 인도, 현위치가 아프리카라면
			if (target_port.localT == Africa && player->nowPort.localT == India_Asia || target_port.localT == India_Asia && player->nowPort.localT == Africa)
			{
				double Distace1 = 0;
				double Distace2 = 0;

				Distace1 = sqrt(pow(now_pos.x_pos - Aden.x_pos, 2) + pow(now_pos.y_pos - Aden.y_pos, 2));
				Distace2 = sqrt(pow(Aden.x_pos - target_pos.x_pos, 2) + pow(Aden.y_pos - target_pos.y_pos, 2));

				return Distace1 + Distace2;
			}

			else
			{
				//목표가 인도라면
				if (target_port.localT == India_Asia)
				{
					double Distace1 = 0;
					double Distace2 = 0;
					double Distace3 = 0;
					double Distace4 = 0;
					double Distace5 = 0;

					Distace1 = sqrt(pow(target_pos.x_pos - Aden.x_pos, 2) + pow(target_pos.y_pos - Aden.y_pos, 2));
					Distace2 = sqrt(pow(Aden.x_pos - CapeTown.x_pos, 2) + pow(Aden.y_pos - CapeTown.y_pos, 2));
					Distace3 = sqrt(pow(CapeTown.x_pos - Dakar.x_pos, 2) + pow(CapeTown.y_pos - Dakar.y_pos, 2));
					Distace4 = sqrt(pow(Dakar.x_pos - Gibraltar.x_pos, 2) + pow(Dakar.y_pos - Gibraltar.y_pos, 2));
					Distace5 = sqrt(pow(Gibraltar.x_pos - now_pos.x_pos, 2) + pow(Gibraltar.y_pos - now_pos.y_pos, 2));

					return Distace1 + Distace2 + Distace3 + Distace4 + Distace5;
				}
				//현위치가 인도라면
				else if (player->nowPort.localT == India_Asia)
				{
					double Distace1 = 0;
					double Distace2 = 0;
					double Distace3 = 0;
					double Distace4 = 0;
					double Distace5 = 0;

					Distace1 = sqrt(pow(now_pos.x_pos - Aden.x_pos, 2) + pow(now_pos.y_pos - Aden.y_pos, 2));
					Distace2 = sqrt(pow(Aden.x_pos - CapeTown.x_pos, 2) + pow(Aden.y_pos - CapeTown.y_pos, 2));
					Distace3 = sqrt(pow(CapeTown.x_pos - Dakar.x_pos, 2) + pow(CapeTown.y_pos - Dakar.y_pos, 2));
					Distace4 = sqrt(pow(Dakar.x_pos - Gibraltar.x_pos, 2) + pow(Dakar.y_pos - Gibraltar.y_pos, 2));
					Distace5 = sqrt(pow(Gibraltar.x_pos - target_pos.x_pos, 2) + pow(Gibraltar.y_pos - target_pos.y_pos, 2));

					return Distace1 + Distace2 + Distace3 + Distace4 + Distace5;
				}
				//목표가 아프리카라면
				if (target_port.localT == Africa)
				{
					double Distace1 = 0;
					double Distace2 = 0;
					double Distace3 = 0;

					Distace1 = sqrt(pow(now_pos.x_pos - Gibraltar.x_pos, 2) + pow(now_pos.y_pos - Gibraltar.y_pos, 2));
					Distace2 = sqrt(pow(Gibraltar.x_pos - Dakar.x_pos, 2) + pow(Gibraltar.y_pos - Dakar.y_pos, 2));
					Distace3 = sqrt(pow(Dakar.x_pos - target_pos.x_pos, 2) + pow(Dakar.y_pos - target_pos.y_pos, 2));

					return Distace1 + Distace2 + Distace3;
				}
				//현위치가 아프리카라면
				else if (player->nowPort.localT == Africa)
				{
					double Distace1 = 0;
					double Distace2 = 0;
					double Distace3 = 0;

					Distace1 = sqrt(pow(now_pos.x_pos - Dakar.x_pos, 2) + pow(now_pos.y_pos - Dakar.y_pos, 2));
					Distace2 = sqrt(pow(Dakar.x_pos - Gibraltar.x_pos, 2) + pow(Dakar.y_pos - Gibraltar.y_pos, 2));
					Distace3 = sqrt(pow(Gibraltar.x_pos - target_pos.x_pos, 2) + pow(Gibraltar.y_pos - target_pos.y_pos, 2));

					return Distace1 + Distace2 + Distace3;
				}
			}
		}

		//아프리카나 인도 태그가 어디에도 없다면
		else
		{
			double Distace1 = 0;
			double Distace2 = 0;

			Distace1 = sqrt(pow(now_pos.x_pos - Gibraltar.x_pos, 2) + pow(now_pos.y_pos - Gibraltar.y_pos, 2));
			Distace2 = sqrt(pow(Gibraltar.x_pos - target_pos.x_pos, 2) + pow(Gibraltar.y_pos - target_pos.y_pos, 2));

			return Distace1 + Distace2;
		}
	}
}

int portnsel(int picker)
{
	int seleting = 1;
	int sel_save = 1;

	int x = 23;
	int y = 10;
	GotoxyAndPrint(x, y, "> 보급/반납");
	GotoxyAndPrint(x, y + 1, "    항  해  ");
	GotoxyAndPrint(x, y + 2, "  돌아 가기");

	while (true)
	{
		seleting = movecurser(seleting);
		if (seleting < 1 && seleting != -10) seleting = 3;
		else if (seleting > 3 && seleting != -10) seleting = 1;

		switch (seleting)
		{
			case 1:
				GotoxyAndPrint(x, y, "> 보급/반납");
				GotoxyAndPrint(x, y + 1, "    항  해  ");
				GotoxyAndPrint(x, y + 2, "  돌아 가기");
				break;
			case 2:
				GotoxyAndPrint(x, y, "  보급/반납");
				GotoxyAndPrint(x, y + 1, ">   항  해  ");
				GotoxyAndPrint(x, y + 2, "  돌아 가기");
				break;
			case 3:
				GotoxyAndPrint(x, y, "  보급/반납");
				GotoxyAndPrint(x, y + 1, "    항  해  ");
				GotoxyAndPrint(x, y + 2, "> 돌아 가기");
				break;
			case -10:
				return sel_save;
		}
		sel_save = seleting;
	}
}

int putout()
{
	int seleting = 1;
	int sel_save = 1;

	int x = 23;
	int y = 10;
	GotoxyAndPrint(x, y, ">    보  급  ");
	GotoxyAndPrint(x, y + 1, "     반  납  ");
	GotoxyAndPrint(x, y + 2, "  돌아 가기");

	while (true)
	{
		seleting = movecurser(seleting);
		if (seleting < 1 && seleting != -10) seleting = 3;
		else if (seleting > 3 && seleting != -10) seleting = 1;

		switch (seleting)
		{
			case 1:
				GotoxyAndPrint(x, y, ">    보  급  ");
				GotoxyAndPrint(x, y + 1, "     반  납  ");
				GotoxyAndPrint(x, y + 2, "  돌아 가기");
				break;
			case 2:
				GotoxyAndPrint(x, y, "     보  급  ");
				GotoxyAndPrint(x, y + 1, ">    반  납  ");
				GotoxyAndPrint(x, y + 2, "  돌아 가기");
				break;
			case 3:
				GotoxyAndPrint(x, y, "     보  급  ");
				GotoxyAndPrint(x, y + 1, "     반  납  ");
				GotoxyAndPrint(x, y + 2, "> 돌아 가기");
				break;
			case -10:
				return sel_save;
		}
		sel_save = seleting;
	}
}

int waterfood()
{
	int seleting = 1;
	int sel_save = 1;

	int numb = 0;
	int index[2] = { -1,-1 };
	int callingnumb = 0;
	int countnumb = 0;

	int x = 23;
	int y = 10;
	GotoxyAndPrint(x, y, ">   식  량");
	GotoxyAndPrint(x, y + 1, "    식  수");
	GotoxyAndPrint(x, y + 2, "  돌아 가기");

	while (true)
	{
		seleting = movecurser(seleting);
		if (seleting < 1 && seleting != -10) seleting = 3;
		else if (seleting > 3 && seleting != -10) seleting = 1;

		switch (seleting)
		{
			case 1:
				GotoxyAndPrint(x, y, ">   식  량");
				GotoxyAndPrint(x, y + 1, "    식  수");
				GotoxyAndPrint(x, y + 2, "  돌아 가기");
				break;
			case 2:
				GotoxyAndPrint(x, y, "    식  량");
				GotoxyAndPrint(x, y + 1, ">   식  수");
				GotoxyAndPrint(x, y + 2, "  돌아 가기");
				break;
			case 3:
				GotoxyAndPrint(x, y, "    식  량  ");
				GotoxyAndPrint(x, y + 1, "    식  수");
				GotoxyAndPrint(x, y + 2, "> 돌아 가기");
				break;
			case -10:
				return sel_save;
		}
		sel_save = seleting;
	}
}

//조선소
void Shipyard(Player* player)
{
	int picker = 0;
	int shippicker = 0;
	Ship newship;

	system("cls");
	printUIaction(inport_UI_base);
	printportrait(dock_col);
	gotoxy(23, 1);
	printf("뭔데? 난 바빠. 용건만 간단히 하자고.");
	picker = shipyardsel();
	switch (picker)
	{
		case 1:
			printf("뭘 사려고?"); //목록 출력은 나중에 콘솔에서 구현하자
			shippicker = shipbuysel();
			switch (shippicker)
			{
				case 1:
					GetShipSetiing(&newship, "ship\\galleon.txt");
					break;
				case 2:
					GetShipSetiing(&newship, "ship\\venetiangalleass.txt");
					break;
				case 3:
					GetShipSetiing(&newship, "ship\\xebec.txt");
					break;
				case 4:
					GetShipSetiing(&newship, "ship\\carrack.txt");
					break;
				case 5:
					GetShipSetiing(&newship, "ship\\sloop.txt");
					break;
				case 6:
					GetShipSetiing(&newship, "ship\\brigantine.txt");
					break;
				case 7:
					GetShipSetiing(&newship, "ship\\galley.txt");
					break;
				case 8:
					GetShipSetiing(&newship, "ship\\dau.txt");
					break;
				case 9:
					GetShipSetiing(&newship, "ship\\hanzacock.txt");
					break;
				case 10:
					return;
				default:
					break;
			}
			if (player->money + player->my_ship.shipPrice >= newship.shipPrice)
			{
				if (player->my_ship.shipPrice > newship.shipPrice)
				{
					int nowshipcargo = player->my_ship.shipNowCargo;
					player->money += player->my_ship.shipPrice - newship.shipPrice;
					player->truemoney += player->my_ship.shipPrice - newship.shipPrice;
					player->my_ship = newship;
					player->my_ship.shipNowCargo = nowshipcargo;
				}
				else
				{
					int nowshipcargo = player->my_ship.shipNowCargo;
					player->money -= newship.shipPrice - player->my_ship.shipPrice;
					player->truemoney -= newship.shipPrice - player->my_ship.shipPrice;
					player->my_ship = newship;
					player->my_ship.shipNowCargo = nowshipcargo;
				}
			}

			else
			{
				printUIaction(inport_UI_base);
				printportrait(dock_col);
				gotoxy(23, 1);
				printf("돈도 안들고 와서 뭘 사겠다는거야!");
				movecurser(0);
			}
			break;
		case 2:
			printUIaction(inport_UI_base);
			printportrait(dock_col);
			gotoxy(23, 1);
			printf("일 다 봤으면 빨리 가! 납기일 낮추려면 시간이 모자라다고.");
			movecurser(0);
			break;
		default:
			picker = 2;
			break;
	}
}

int shipyardsel()
{
	int seleting = 1;
	int sel_save = 1;

	int x = 23;
	int y = 10;
	GotoxyAndPrint(x, y, "> 함선 구매");
	GotoxyAndPrint(x, y + 1, "  돌아 가기");

	while (true)
	{
		seleting = movecurser(seleting);
		if (seleting < 1 && seleting != -10) seleting = 2;
		else if (seleting > 2 && seleting != -10) seleting = 1;

		switch (seleting)
		{
			case 1:
				GotoxyAndPrint(x, y, "> 함선 구매");
				GotoxyAndPrint(x, y + 1, "  돌아 가기");
				break;
			case 2:
				GotoxyAndPrint(x, y, "  함선 구매");
				GotoxyAndPrint(x, y + 1, "> 돌아 가기");
				break;
			case -10:
				return sel_save;
		}
		sel_save = seleting;
	}
}

int shipbuysel()
{
	int seleting = 1;
	int sel_save = 1;

	int x = 23;
	int y = 10;
	GotoxyAndPrint(x, y, "  |    이름              속도   적재   최소선원  최대선원    가격 ");
	GotoxyAndPrint(x, y + 1, "> |           갤리온     85     800    45        200         140000");
	GotoxyAndPrint(x, y + 2, "  |베네치안 갤리어스     70     800    45        200         140000");
	GotoxyAndPrint(x, y + 3, "  |             지벡     70     600    25        300         118000");
	GotoxyAndPrint(x, y + 4, "  |             카락     65     600    30        160          94000");;
	GotoxyAndPrint(x, y + 5, "  |           슬루프     85     600    10        60           81000");
	GotoxyAndPrint(x, y + 6, "  |         브린간틴     85     250    15        60           52000");;
	GotoxyAndPrint(x, y + 7, "  |             갤리     85     120    5         20           35000");
	GotoxyAndPrint(x, y + 8, "  |             다우     85     70     5         20           20000");
	GotoxyAndPrint(x, y + 9, "  |           한자콕     85     60     5         20           10000");
	GotoxyAndPrint(x, y + 10, "  뒤로가기");

	while (true)
	{
		seleting = movecurser(seleting);
		if (seleting < 1 && seleting != -10) seleting = 10;
		else if (seleting > 10 && seleting != -10) seleting = 1;

		switch (seleting)
		{
			case 1:
				GotoxyAndPrint(x, y, "  |    이름              속도   적재   최소선원  최대선원    가격 ");
				GotoxyAndPrint(x, y + 1, "> |           갤리온     85     800    45        200         140000");
				GotoxyAndPrint(x, y + 2, "  |베네치안 갤리어스     70     800    45        200         140000");
				GotoxyAndPrint(x, y + 3, "  |             지벡     70     600    25        300         118000");
				GotoxyAndPrint(x, y + 4, "  |             카락     65     600    30        160          94000");;
				GotoxyAndPrint(x, y + 5, "  |           슬루프     85     600    10        60           81000");
				GotoxyAndPrint(x, y + 6, "  |         브린간틴     85     250    15        60           52000");;
				GotoxyAndPrint(x, y + 7, "  |             갤리     85     120    5         20           35000");
				GotoxyAndPrint(x, y + 8, "  |             다우     85     70     5         20           20000");
				GotoxyAndPrint(x, y + 9, "  |           한자콕     85     60     5         20           10000");
				GotoxyAndPrint(x, y + 10, "  뒤로가기");
				break;
			case 2:
				GotoxyAndPrint(x, y, "  |    이름              속도   적재   최소선원  최대선원    가격 ");
				GotoxyAndPrint(x, y + 1, "  |           갤리온     85     800    45        200         140000");
				GotoxyAndPrint(x, y + 2, "> |베네치안 갤리어스     70     800    45        200         140000");
				GotoxyAndPrint(x, y + 3, "  |             지벡     70     600    25        300         118000");
				GotoxyAndPrint(x, y + 4, "  |             카락     65     600    30        160          94000");;
				GotoxyAndPrint(x, y + 5, "  |           슬루프     85     600    10        60           81000");
				GotoxyAndPrint(x, y + 6, "  |         브린간틴     85     250    15        60           52000");;
				GotoxyAndPrint(x, y + 7, "  |             갤리     85     120    5         20           35000");
				GotoxyAndPrint(x, y + 8, "  |             다우     85     70     5         20           20000");
				GotoxyAndPrint(x, y + 9, "  |           한자콕     85     60     5         20           10000");
				GotoxyAndPrint(x, y + 10, "  뒤로가기");
				break;
			case 3:
				GotoxyAndPrint(x, y, "  |    이름              속도   적재   최소선원  최대선원    가격 ");
				GotoxyAndPrint(x, y + 1, "  |           갤리온     85     800    45        200         140000");
				GotoxyAndPrint(x, y + 2, "  |베네치안 갤리어스     70     800    45        200         140000");
				GotoxyAndPrint(x, y + 3, "> |             지벡     70     600    25        300         118000");
				GotoxyAndPrint(x, y + 4, "  |             카락     65     600    30        160          94000");;
				GotoxyAndPrint(x, y + 5, "  |           슬루프     85     600    10        60           81000");
				GotoxyAndPrint(x, y + 6, "  |         브린간틴     85     250    15        60           52000");;
				GotoxyAndPrint(x, y + 7, "  |             갤리     85     120    5         20           35000");
				GotoxyAndPrint(x, y + 8, "  |             다우     85     70     5         20           20000");
				GotoxyAndPrint(x, y + 9, "  |           한자콕     85     60     5         20           10000");
				GotoxyAndPrint(x, y + 10, "  뒤로가기");
				break;
			case 4:
				GotoxyAndPrint(x, y, "  |    이름              속도   적재   최소선원  최대선원    가격 ");
				GotoxyAndPrint(x, y + 1, "  |           갤리온     85     800    45        200         140000");
				GotoxyAndPrint(x, y + 2, "  |베네치안 갤리어스     70     800    45        200         140000");
				GotoxyAndPrint(x, y + 3, "  |             지벡     70     600    25        300         118000");
				GotoxyAndPrint(x, y + 4, "> |             카락     65     600    30        160          94000");;
				GotoxyAndPrint(x, y + 5, "  |           슬루프     85     600    10        60           81000");
				GotoxyAndPrint(x, y + 6, "  |         브린간틴     85     250    15        60           52000");;
				GotoxyAndPrint(x, y + 7, "  |             갤리     85     120    5         20           35000");
				GotoxyAndPrint(x, y + 8, "  |             다우     85     70     5         20           20000");
				GotoxyAndPrint(x, y + 9, "  |           한자콕     85     60     5         20           10000");
				GotoxyAndPrint(x, y + 10, "  뒤로가기");
				break;
			case 5:
				GotoxyAndPrint(x, y, "  |    이름              속도   적재   최소선원  최대선원    가격 ");
				GotoxyAndPrint(x, y + 1, "  |           갤리온     85     800    45        200         140000");
				GotoxyAndPrint(x, y + 2, "  |베네치안 갤리어스     70     800    45        200         140000");
				GotoxyAndPrint(x, y + 3, "  |             지벡     70     600    25        300         118000");
				GotoxyAndPrint(x, y + 4, "  |             카락     65     600    30        160          94000");;
				GotoxyAndPrint(x, y + 5, "> |           슬루프     85     600    10        60           81000");
				GotoxyAndPrint(x, y + 6, "  |         브린간틴     85     250    15        60           52000");;
				GotoxyAndPrint(x, y + 7, "  |             갤리     85     120    5         20           35000");
				GotoxyAndPrint(x, y + 8, "  |             다우     85     70     5         20           20000");
				GotoxyAndPrint(x, y + 9, "  |           한자콕     85     60     5         20           10000");
				GotoxyAndPrint(x, y + 10, "  뒤로가기");
				break;
			case 6:
				GotoxyAndPrint(x, y, "  |    이름              속도   적재   최소선원  최대선원    가격 ");
				GotoxyAndPrint(x, y + 1, "  |           갤리온     85     800    45        200         140000");
				GotoxyAndPrint(x, y + 2, "  |베네치안 갤리어스     70     800    45        200         140000");
				GotoxyAndPrint(x, y + 3, "  |             지벡     70     600    25        300         118000");
				GotoxyAndPrint(x, y + 4, "  |             카락     65     600    30        160          94000");;
				GotoxyAndPrint(x, y + 5, "  |           슬루프     85     600    10        60           81000");
				GotoxyAndPrint(x, y + 6, "> |         브린간틴     85     250    15        60           52000");;
				GotoxyAndPrint(x, y + 7, "  |             갤리     85     120    5         20           35000");
				GotoxyAndPrint(x, y + 8, "  |             다우     85     70     5         20           20000");
				GotoxyAndPrint(x, y + 9, "  |           한자콕     85     60     5         20           10000");
				GotoxyAndPrint(x, y + 10, "  뒤로가기");
				break;
			case 7:
				GotoxyAndPrint(x, y, "  |    이름              속도   적재   최소선원  최대선원    가격 ");
				GotoxyAndPrint(x, y + 1, "  |           갤리온     85     800    45        200         140000");
				GotoxyAndPrint(x, y + 2, "  |베네치안 갤리어스     70     800    45        200         140000");
				GotoxyAndPrint(x, y + 3, "  |             지벡     70     600    25        300         118000");
				GotoxyAndPrint(x, y + 4, "  |             카락     65     600    30        160          94000");;
				GotoxyAndPrint(x, y + 5, "  |           슬루프     85     600    10        60           81000");
				GotoxyAndPrint(x, y + 6, "  |         브린간틴     85     250    15        60           52000");;
				GotoxyAndPrint(x, y + 7, "> |             갤리     85     120    5         20           35000");
				GotoxyAndPrint(x, y + 8, "  |             다우     85     70     5         20           20000");
				GotoxyAndPrint(x, y + 9, "  |           한자콕     85     60     5         20           10000");
				GotoxyAndPrint(x, y + 10, "  뒤로가기");
				break;
			case 8:
				GotoxyAndPrint(x, y, "  |    이름              속도   적재   최소선원  최대선원    가격 ");
				GotoxyAndPrint(x, y + 1, "  |           갤리온     85     800    45        200         140000");
				GotoxyAndPrint(x, y + 2, "  |베네치안 갤리어스     70     800    45        200         140000");
				GotoxyAndPrint(x, y + 3, "  |             지벡     70     600    25        300         118000");
				GotoxyAndPrint(x, y + 4, "  |             카락     65     600    30        160          94000");;
				GotoxyAndPrint(x, y + 5, "  |           슬루프     85     600    10        60           81000");
				GotoxyAndPrint(x, y + 6, "  |         브린간틴     85     250    15        60           52000");;
				GotoxyAndPrint(x, y + 7, "  |             갤리     85     120    5         20           35000");
				GotoxyAndPrint(x, y + 8, "> |             다우     85     70     5         20           20000");
				GotoxyAndPrint(x, y + 9, "  |           한자콕     85     60     5         20           10000");
				GotoxyAndPrint(x, y + 10, "  뒤로가기");
				break;
			case 9:
				GotoxyAndPrint(x, y, "  |    이름              속도   적재   최소선원  최대선원    가격 ");
				GotoxyAndPrint(x, y + 1, "  |           갤리온     85     800    45        200         140000");
				GotoxyAndPrint(x, y + 2, "  |베네치안 갤리어스     70     800    45        200         140000");
				GotoxyAndPrint(x, y + 3, "  |             지벡     70     600    25        300         118000");
				GotoxyAndPrint(x, y + 4, "  |             카락     65     600    30        160          94000");;
				GotoxyAndPrint(x, y + 5, "  |           슬루프     85     600    10        60           81000");
				GotoxyAndPrint(x, y + 6, "  |         브린간틴     85     250    15        60           52000");;
				GotoxyAndPrint(x, y + 7, "  |             갤리     85     120    5         20           35000");
				GotoxyAndPrint(x, y + 8, "  |             다우     85     70     5         20           20000");
				GotoxyAndPrint(x, y + 9, "> |           한자콕     85     60     5         20           10000");
				GotoxyAndPrint(x, y + 10, "  뒤로가기");
				break;
			case 10:
				GotoxyAndPrint(x, y, "  |    이름              속도   적재   최소선원  최대선원    가격 ");
				GotoxyAndPrint(x, y + 1, "  |           갤리온     85     800    45        200         140000");
				GotoxyAndPrint(x, y + 2, "  |베네치안 갤리어스     70     800    45        200         140000");
				GotoxyAndPrint(x, y + 3, "  |             지벡     70     600    25        300         118000");
				GotoxyAndPrint(x, y + 4, "  |             카락     65     600    30        160          94000");;
				GotoxyAndPrint(x, y + 5, "  |           슬루프     85     600    10        60           81000");
				GotoxyAndPrint(x, y + 6, "  |         브린간틴     85     250    15        60           52000");;
				GotoxyAndPrint(x, y + 7, "  |             갤리     85     120    5         20           35000");
				GotoxyAndPrint(x, y + 8, "  |             다우     85     70     5         20           20000");
				GotoxyAndPrint(x, y + 9, "  |           한자콕     85     60     5         20           10000");
				GotoxyAndPrint(x, y + 10, "> 뒤로가기");
				break;
			case -10:
				return sel_save;
		}
		sel_save = seleting;
	}
}

//묶음
void AllinOnePort(Player* player, DayTime* time)
{
	int funcpicker = 1;
	int whilecontrol = 1;
	int picker = 0;
	InPort(player);
	funcpicker = PortPicker(player, funcpicker);
	switch (funcpicker)
	{
		case 1:
			TradeCenter(player);
			break;
		case 2:
			Tavern(player, time);
			break;
		case 3:
			Bank(player, *time);
			break;
		case 4:
			Shipyard(player);
			break;
		case 5:
			Port_port(player, time, &whilecontrol);
			break;
	}
}

int PortPicker(Player* player, int picknum)
{
	int funcpicker = 1;
	int pickbuuf = 1;
	bool already = false;
	int pic = 0;

	int numb = 0;
	int index[12] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };

	printdayport(BGport_tradecenter);
	gotoxy(88, 3);
	printf("무역거래소");
	sprintfincc(player);

	while (true)
	{
		funcpicker = movecurser(funcpicker);
		if (already)
		{
			funcpicker = pickbuuf;
			already = false;
		}
		else if (funcpicker < 1 && funcpicker != -10 && funcpicker < 70) funcpicker = 5;
		else if (funcpicker > 5 && funcpicker != -10 && funcpicker < 70) funcpicker = 1;

		switch (funcpicker)
		{
			case 1:
				printdayport(BGport_tradecenter);
				gotoxy(88, 3);
				printf("무역거래소");
				sprintfincc(player);
				break;
			case 2:
				printdayport(BGport_Tavern);
				gotoxy(27, 2);
				printf("여관");
				sprintfincc(player);
				break;
			case 3:
				printdayport(BGport_bank);
				gotoxy(38, 4);
				printf("은행");
				sprintfincc(player);
				break;
			case 4:
				printdayport(BGport_dockyeard);
				gotoxy(23, 7);
				printf("조선소");
				sprintfincc(player);
				break;
			case 5:
				printdayport(BGport_port);
				gotoxy(57, 14);
				printf("항구");
				gotoxy(0, 24);
				sprintfincc(player);
				break;
			case 71:
				printinvandship(ship_inventory);
				gotoxy(90, 6);
				printf("함선 정보");
				gotoxy(82, 8);
				std::cout << "함선 이름 :" << player->my_ship.shipName;
				gotoxy(82, 9);
				std::cout << "함선 속도 :" << player->my_ship.shipDefalutSpeed;
				gotoxy(82, 10);
				std::cout << "적재 능력 :" << player->my_ship.shipNowCargo << "/" << player->my_ship.shipCargo;
				gotoxy(82, 11);
				std::cout << "최소 선원 :" << player->my_ship.needCrew;
				gotoxy(82, 12);
				std::cout << "현재 선원 :" << player->nowman << "/" << player->my_ship.maxCrew;
				gotoxy(82, 13);
				std::cout << "함선 가격 :" << player->my_ship.shipPrice;
				setcolor(15, 0);
				gotoxy(0, 24);
				already = true;
				break;
			case 70:
				printinvandship(cargo_inventory);
				gotoxy(90, 6);
				printf("보유 화물");

				numb = 0;

				for (int i = 0; i < 12; i++)
				{
					if (player->inship_cargo[i].cargoNumber != 0)
					{
						index[numb] = i;
						numb++;
					}
					else index[i] = -1;
				}
				if (player->inship_cargo[0].cargoNumber == -1)
				{
					printf("아무것도 없습니다.");
					return 0;
				}

				for (int i = 0; i < 12; i++)
				{
					if (index[i] == -1) break;
					gotoxy(85, 8 + i);
					if (player->inship_cargo[index[i]].cargoNumber != 0)
						std::cout << player->inship_cargo[index[i]].cargoName << "| 보유량 : " << player->inship_cargo_vlaue[index[i]] << std::endl;

				}

				setcolor(15, 0);
				gotoxy(0, 24);
				already = true;
				break;
			case 72:
				GotoxyAndPrint(40, 15, "#=========================#");
				GotoxyAndPrint(40, 16, "|    저장하시겠습니까?    |");
				GotoxyAndPrint(40, 17, "#=========================#");
				pic = yesnosel2();
				if (pic == 1)
				{
					writesav();
				}
				break;
			case -10:
				return pickbuuf;
		}
		if (!already)pickbuuf = funcpicker;
		gotoxy(0, 25);
	}
}

void sprintfincc(Player* player)
{
	gotoxy(0, 24);
	std::cout << "현재 위치 :" << player->nowPort.name;
	gotoxy(30, 24);
	std::cout << realtime.year << "." << realtime.month << "." << realtime.day;
	gotoxy(0, 25);
	std::cout << player->player_name;

	gotoxy(5, 27);
	if (player->palyer_country == England) printf("소속 : 잉글랜드 왕국");
	else if (player->palyer_country == Portugal) printf("소속 : 포르투갈 왕국");
	else if (player->palyer_country == France) printf("소속 : 프랑스 왕국");
	else if (player->palyer_country == Ottuman) printf("소속 : 오스만 제국");
	gotoxy(5, 29);
	printf("잔고 : %d", player->money);
	gotoxy(35, 27);
	std::cout << "현재 함선 :" << player->my_ship.shipName;
	gotoxy(33, 29);
	printf("잔여 대출금 : %d", player->Debt);
}