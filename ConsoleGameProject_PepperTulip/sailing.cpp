//항해에 관련한 코드를 입력할것.
#include <iostream>
#include <windows.h>
#include <random>
#include <fstream>
#include <string>
#include <conio.h>
#include "sailing.h"
#include "struct.h"
#include "general.h"
#include "dot.h"
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

extern int sail_port;
extern Port targetport;
extern int sailling_day;
extern Player player;
extern DayTime realtime;

bool daynight = 1; // 1이 낮 0이 밤
int angraymood = 0;

float fooduse = 0;
float warteruse = 0;

bool alreadyplayed = 0;

void nowsailling(bool* inport)
{
	if (!alreadyplayed)
	{
		PlaySound(L"sound\\sailling.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
		alreadyplayed = 1;
	}

	int reverse_sailling_day = 0;

	int madmanevent = 0;

	std::random_device random;
	std::mt19937 gen(random());
	std::uniform_int_distribution<int> dis(5, 100);

	if (daynight == 1)
	{
		if ((realtime.year == player.DebtEndTime.year) && (realtime.month == player.DebtEndTime.month) && (realtime.day == player.DebtEndTime.day))
		{
			boxprint();
			gotoxy(40, 11);
			printf("통지서");
			GotoxyAndPrint(21, 12, "은행입니다. 빚을 갚아야 할 시기가 도래하셨더군요.");
			GotoxyAndPrint(21, 13, "원래 채무란 가차없는 법 입니다.");
			movecurser(0);
			if (player.money >= player.Debt)
			{
				player.money -= player.Debt;
				player.truemoney += player.Debt;
				player.DebtEndTime.year = 9999;
				player.DebtEndTime.month = 99;
				player.DebtEndTime.day = 99;
				player.Debt_val = 0;
				player.Debt = 0;

				GotoxyAndPrint(21, 15, "하지만 다행히도 적당한 금액을 가지고 계셨더군요.");
				GotoxyAndPrint(21, 16, "계좌에서 인출했습니다.");
				GotoxyAndPrint(21, 17, "이용해주셔서 감사합니다.");
				movecurser(0);
			}
			else if (player.money < player.Debt)
			{
				player.Debt -= player.money;
				player.truemoney += player.money;
				player.money = 0;
				player.truemoney -= (player.Debt * 30) / 100;
				player.Debt += (player.Debt * 30) / 100;

				for (int i = 0; i < 150; i++)
				{
					TimeCounter(&player.DebtEndTime);
				}
				GotoxyAndPrint(21, 15, "안타깝게도 보유금 전부로도 자금이 모자라시더군요.");
				GotoxyAndPrint(21, 16, "일단 계좌에 있는 금액 전체를 인출했습니다.");
				GotoxyAndPrint(21, 17, "남은 대출금의 30 % 를 연체금으로 부과하겠습니다.");
				GotoxyAndPrint(21, 18, "연채시 만기일은 5달 뒤 입니다.\n");
				movecurser(0);
			}
		}

		printdaysailling();
		gotoxy(103, 25);
		std::cout<<"[" << player.nowPort.name << "] 에서";
		gotoxy(103, 26);
		std::cout << "[" << targetport.name << "] 까지";
		gotoxy(103, 27);
		std::cout << "남은 일수 :" << sailling_day << "일";

		gotoxy(1, 26);
		printf("자금");
		gotoxy(1, 27);
		printf("%d", player.money);

		gotoxy(15, 26);
		printf("선원");
		gotoxy(15, 27);
		printf("%d/%d(%d)", player.nowman, player.my_ship.maxCrew, player.my_ship.needCrew);

		gotoxy(30, 26);
		printf("적재량");
		gotoxy(30, 27);
		printf("%d/%d", player.my_ship.shipNowCargo, player.my_ship.shipCargo);

		RandomBlablaOutputgenerator();

		Sleep(3500);

		if (_kbhit())
		{

			int pic = pausesel();
			switch (pic)
			{
			case 1:
				angraymood -= 3;
				player.money -= 300;
				break;
			case 2:
				sailling_day -= 2;
				angraymood += 5;
				break;
			case 3:
				sailling_day = reverse_sailling_day;
				targetport = player.nowPort;
				break;
			default:
				break;
			}
		}

		daynight = 0;
	    madmanevent = dis(gen);
		if (angraymood > madmanevent)MadmanEvent();
		else if (madmanevent == 100);
		if (player.nowman < player.my_ship.needCrew) Nomanship();
	}
	else
	{
		printnightsailling();
		gotoxy(103, 25);
		std::cout << "[" << player.nowPort.name << "] 에서";
		gotoxy(103, 26);
		std::cout << "[" << targetport.name << "] 까지";
		gotoxy(103, 27);
		std::cout << "남은 일수 :" << sailling_day << "일";

		gotoxy(1, 26);
		printf("자금");
		gotoxy(1, 27);
		printf("%d", player.money);

		gotoxy(15, 26);
		printf("선원");
		gotoxy(15, 27);
		printf("%d/%d(%d)", player.nowman, player.my_ship.maxCrew, player.my_ship.needCrew);

		gotoxy(30, 26);
		printf("적재량");
		gotoxy(30, 27);
		printf("%d/%d", player.my_ship.shipNowCargo, player.my_ship.shipCargo);

		sailling_day -= 1;
		reverse_sailling_day++;

		for (int i = 0; i < 12; i++)
		{
			if (player.inship_cargo[i].cargoNumber == 1)
			{
				if (player.inship_cargo_vlaue[i] <= 0)
				{
					player.nowman--;
					break;
				}
				fooduse += player.nowman * 0.1f;
				if (fooduse > 1)
				{
					player.inship_cargo_vlaue[i] -= fooduse;
					player.my_ship.shipNowCargo -= fooduse;
					fooduse = 0;
				}
			}
			if (player.inship_cargo[i].cargoNumber == 2)
			{
				if (player.inship_cargo_vlaue[i] <= 0)
				{
					player.nowman--;
					break;
				}
				warteruse += player.nowman * 0.1f;
				if (warteruse > 1)
				{
					player.inship_cargo_vlaue[i] -= warteruse;
					player.my_ship.shipNowCargo -= warteruse;
					warteruse = 0;
				}
			}
		}

		Sleep(1500);
		if (_kbhit())
		{
			int pic = pausesel();
			switch (pic)
			{
			case 1:
				if (player.money > 500)
				{
					angraymood -= 1;
					player.money -= 500;
				}
				else
				{
					GotoxyAndPrint(81, 8, "돈은 있으십니까?       ");
					GotoxyAndPrint(81, 9, "                   ");
					GotoxyAndPrint(81, 10, "                   ");
					GotoxyAndPrint(81, 11, "                   ");
					GotoxyAndPrint(81, 12, "                   ");
					GotoxyAndPrint(81, 13, "                   ");
					GotoxyAndPrint(81, 14, "                   ");
					GotoxyAndPrint(81, 15, "                   ");
				}
				break;
			case 2:
				sailling_day -= 1;
				angraymood += 7;
				break;
			case 3:
				sailling_day = reverse_sailling_day;
				targetport = player.nowPort;
				break;
			default:
				break;
			}
		}

		angraymood++;

		TimeCounter(&realtime);
		if (sailling_day <= 0)
		{
			reverse_sailling_day = 0;
			alreadyplayed = 0;
			player.nowPort = targetport;
			sail_port = 1;
			*inport = 0;
		}

		daynight = 1;
	}
}

void MadmanEvent()
{
	int result = 0;

	boxprint();
	gotoxy(41, 11);
	printf("선상 반란!");

	gotoxy(21, 13);
	printf("불만가득한 선원들의 선동으로 선상에서 반란이 일어났습니다.");
	gotoxy(21, 14);
	printf("선상반란의 경우 어떠한 예외없이 사형이므로"); 
	gotoxy(21, 15);
	printf("그들은 목숨걸고 싸울것 입니다.");
	gotoxy(21, 16);
	printf("죽거나, 살아남거나. 이제 둘중 하나만이 당신을 기다립니다.");
	movecurser(0);

	std::random_device random;
	std::mt19937 gen(random());
	std::uniform_int_distribution<int> dis(1, 2);

	result = dis(gen);

	if (result == 1)
	{
		boxprint();
		gotoxy(41, 11);
		printf("선상 반란!");
		gotoxy(21, 13);
		printf("처절한 전투 끝에 당신은 살아남았습니다.");
		gotoxy(21, 14);
		printf("남은 배신자들은 방금 모두 바닷 속으로 던져진 참입니다.");
		gotoxy(21, 15);
		printf("너무 큰 피해를 입었지만, 살아남아서 감사할 일입니다.");
		gotoxy(21, 16);
		printf("아마도요.");
		player.nowman /= 2;
		angraymood = 0;
		movecurser(0);
	}
	else if (result == 2)
	{
		boxprint();
		gotoxy(41, 11);
		printf("선상 반란!");
		gotoxy(21, 13);
		printf("처절한 전투 끝에 당신은 패배했습니다.");
		gotoxy(21, 14);
		printf("고국으로 돌아가도 사형밖에 없는 저들은 해적이 되겠지요.");
		gotoxy(21, 15);
		printf("결박되어 바다로 떨어지는 당신에겐 중요하지 않은 일이지만.");
		movecurser(0);
		
		angraymood = 0;
		GameOver(1);
	}
}


void Nomanship()
{
	std::random_device random;
	std::mt19937 gen(random());
	std::uniform_int_distribution<int> dis(1, 7);

	boxprint();
	gotoxy(41, 11);
	printf("선원 부족!");

	gotoxy(21, 13);
	printf("배를 운용하는데 필요한 최소선원보다 남은 인원이 적습니다.");
	gotoxy(21, 14);
	printf("이대로 최후까지 망망대해를 표류하겠죠.");
	gotoxy(21, 15);
	printf("운이 좋다면 지나가던 함선이 당신을 구할 수 있을지도?");
	gotoxy(21, 16);
	printf("정말 운이 좋다면 겸사겸사 배도 인양해줄지도 모를 일이죠.");
	movecurser(0);

	int result = dis(gen);
	
	if(result == 1)
	{
		boxprint();
		gotoxy(41, 13);
		printf("선원 부족!");
		gotoxy(21, 14);
		printf("정직하고 마음씨 좋은 선단을 만났습니다.");
		gotoxy(21, 15);
		printf("그들은 당신의 배를 선뜻 인양해 주었고, 무사히 돌아왔습니다.");
		gotoxy(21, 16);
		printf("배도 멀쩡하고, 화물도 멀쩡하니 정말 다행입니다.");
		movecurser(0);
		sailling_day = 0;
	}
	else if (result == 2)
	{
		boxprint();
		gotoxy(41, 13);
		printf("선원 부족!");
		gotoxy(21, 14);
		printf("운이 좋았는지 머지않아 누군가의 배를 얻어 탈 수 있었습니다.");
		gotoxy(21, 15);
		printf("배도 잃었고, 혼자만이 살아남았지만 다시 시작할 수 있을겁니다.");
		gotoxy(21, 16);
		printf("당신이 희망을 잃지 않는다면 말이죠!");
		movecurser(0);

		GetShipSetiing(&player.my_ship, "ship\\hanzacock.txt");
		for (int i = 0; i < 12 ; i++)
		{
			GetCargoSetiing(&player.inship_cargo[i], "cargo\\none.text");
			player.inship_cargo_vlaue[i] = 0;
		}
		player.nowman = 0;
		sailling_day = 0;
	}
	else if (result >= 3)
	{
		boxprint();
		gotoxy(41, 11);
		printf("선원 부족!");
		gotoxy(21, 13);
		printf("망망대해를 표류하는 배의 운명은 정해져있습니다.");
		gotoxy(21, 14);
		printf("아직 배에 사람은 있지만, 그들의 운명은 알길이 없죠.");
		gotoxy(21, 15);
		printf("하지만 운명이란것이 당신의 편이 아니었던것 같습니다...");
		gotoxy(21, 16);
		printf("그저 배만이 남아, 이들 처럼 유령선으로 유명해지겠죠.");
		movecurser(0);
		GameOver(2);
	}

}

void GameOver(int dieCase)
{
	//반란진압 실패
	//선원부족
	system("cls");
	boxprint();
	gotoxy(41, 11);
	printf("게임 오버");
	switch (dieCase)
	{
		case 1://반란진압 실패
			gotoxy(21, 13);
			printf("선원들의 불만도를 잘 확인하세요.");
			gotoxy(21, 14);
			printf("항해가 길어질 수록 반란이 일어날 확률이 증가합니다.");
			movecurser(0);
			system("cls");
			sail_port = 0;
			return;

		case 2:
			gotoxy(21, 13);
			printf("무슨일이 있어도 최소선원을 유지해야 합니다.");
			gotoxy(21, 14);
			printf("다양한 이벤트가 선원들을 위협하므로 조심해주세요.");
			gotoxy(21, 15);
			printf("항상 넉넉한 선원 수를 유지하는것이 중요합니다.");
			movecurser(0);
			system("cls");
			sail_port = 0;
			return;
		default:
			gotoxy(21, 11);
			printf("정상적인 상황에선 이 문구를 볼일이 없습니다.");
			gotoxy(21, 12);
			printf("지금 이 문구가 출력 되었다면 개발자에게 알려주세요.");
			movecurser(0);
			system("cls");
			sail_port = 0;
			return;
	}
}

void RandomBlablaOutputgenerator()
{
	std::random_device randomblabla;
	std::mt19937 genblabla(randomblabla());
	std::uniform_int_distribution<int> disblabla(1, 71);

	int inputnum = 0;
	std::string line;
	std::ifstream file("path\\blabla.txt");

	int i = disblabla(genblabla);

		for (int j = 1; j < i;j++)
		{
			getline(file, line);
		}

		gotoxy(0, 24);
		std::cout << line;
}

int pausesel()
{
	int seleting = 1;
	int sel_save = 1;

	int numb = 0;
	int index[2] = { -1,-1 };
	int callingnumb = 0;
	int countnumb = 0;

	printinvandship(cargo_inventory);
	gotoxy(90, 6);
	printf("일시 정지");

	int x = 81;
	int y = 8;
	GotoxyAndPrint(x, y,     "> 럼주주기");
	GotoxyAndPrint(x, y + 1, "  재촉하기");
	GotoxyAndPrint(x, y + 2, "  귀항하기");
	GotoxyAndPrint(x, y + 3, "  돌아가기");

	GotoxyAndPrint(x, y + 6, "럼주를 주어 선원들을");
	GotoxyAndPrint(x, y + 7, "다독입니다.");
	GotoxyAndPrint(x, y + 8, "불만도 -3, 보유금 -300");

	while (true)
	{

		seleting = movecurser(seleting);
		if (seleting < 1 && seleting != -10) seleting = 4;
		else if (seleting > 4 && seleting != -10) seleting = 1;

		switch (seleting)
		{
		case 1:
			printinvandship(cargo_inventory);
			gotoxy(90, 6);
			printf("일시 정지");

			GotoxyAndPrint(x, y, "> 럼주주기");
			GotoxyAndPrint(x, y + 1, "  재촉하기");
			GotoxyAndPrint(x, y + 2, "  귀항하기");
			GotoxyAndPrint(x, y + 3, "  돌아가기");

			GotoxyAndPrint(x, y + 6, "럼주를 주어 선원들을");
			GotoxyAndPrint(x, y + 7, "다독입니다.");
			GotoxyAndPrint(x, y + 8, "불만도 -1, 보유금 -500");
			setcolor(15, 0);
			break;
		case 2:
			printinvandship(cargo_inventory);
			gotoxy(90, 6);
			printf("일시 정지");

			GotoxyAndPrint(x, y, "  럼주주기");
			GotoxyAndPrint(x, y + 1, "> 재촉하기");
			GotoxyAndPrint(x, y + 2, "  귀항하기");
			GotoxyAndPrint(x, y + 3, "  돌아가기");

			GotoxyAndPrint(x, y + 6, "선원들을 다그쳐 일정을");
			GotoxyAndPrint(x, y + 8, "당깁니다.");
			GotoxyAndPrint(x, y + 9, "불만도 +7, 남은 항해일수 -1");
			setcolor(15, 0);
			break;
		case 3:
			printinvandship(cargo_inventory);
			gotoxy(90, 6);
			printf("일시 정지");

			GotoxyAndPrint(x, y, "  럼주주기");
			GotoxyAndPrint(x, y + 1, "  재촉하기");
			GotoxyAndPrint(x, y + 2, "> 귀항하기");
			GotoxyAndPrint(x, y + 3, "  돌아가기");

			GotoxyAndPrint(x, y + 6, "출발한 곳으로 되돌아갑니다.");
			setcolor(15, 0);
			break;
		case 4:
			printinvandship(cargo_inventory);
			gotoxy(90, 6);
			printf("일시 정지");

			GotoxyAndPrint(x, y, "  럼주주기");
			GotoxyAndPrint(x, y + 1, "  재촉하기");
			GotoxyAndPrint(x, y + 2, "  귀항하기");
			GotoxyAndPrint(x, y + 3, "> 돌아가기");

			GotoxyAndPrint(x, y + 6, "항해를 계속 이어나갑니다.");
			setcolor(15, 0);
			break;
		case -10:
			return sel_save;
		}
		sel_save = seleting;
	}
}