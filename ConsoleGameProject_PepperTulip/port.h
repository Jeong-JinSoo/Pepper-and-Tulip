#pragma once
#include "struct.h"

//항구 입장시 세금
void InPort(Player* player);
//무역거래소
void TradeCenter(Player* player);
int TradeCenterSelect();
void TradeCenter_BuyCargo(Player* player);
void FoundSpaceAndPush(Player* player, int buypick, int couunt);
void buybuy(Player* player, int buypick, int buycount);
int buySelect(Player* player);
void TradeCenter_SellCargo(Player* player);
int SellSelect(Player* player);
void TradeCenter_ConfirmValue(Player* player);
void TradeCenter_ValueCalculator(Player* player);
void Inport_ValueCalculator(Player* player);
void Tulipvalue(Player* player);
//은행
void Bank(Player* player, struct DayTime time);
int BankSelect();
int yesnosel();
//여관
void Tavern(Player* player, DayTime* time);
int Tavernsel(int picker);
//부두
void Port_port(Player* player, DayTime* time, int* goout);
void Portdealer(Player* player);
void FoundSpaceAndSUPP(Player* player, int subpick, int couunt);
void FoundCargoAndOUT(Player* player, int subpick, int couunt);
void TargetPortPicker(Player* player, DayTime* time, int* goout, int* stoper);
int TargetPortPickerPrinter();
float TargetPortCalculator(Player* player, Port target_port, DayTime* time);
int portnsel(int picker);
int waterfood();
int putout();
//조선소
void Shipyard(Player* player);
int shipyardsel();
int shipbuysel();

//통합
void AllinOnePort(Player* player, DayTime* time);
int PortPicker(Player* player, int picknum);
void sprintfincc(Player* player);
void writesav(const char* filename, Player* main_player);