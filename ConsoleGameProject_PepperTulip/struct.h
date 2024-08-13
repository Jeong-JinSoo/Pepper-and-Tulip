#pragma once

enum LocalTag
{
	Mediterranean_Sea,
	English_Channel,
	India_Asia,
	Africa
};

enum CountryTag
{
	Portugal,
	Spain,
	Sicilia,
	Venezia,
	Genova,
	England,
	France,
	Netherland,
	Ottuman,
	India
};

struct DayTime
{
	int year;
	int month;
	int day;
};

struct PortPosition
{
	float x_pos;
	float y_pos;
};

struct S_Cargo_type
{
	std::string cargoName;
	int cargoNumber;

	int defaultPrice;
	int defaultStoke;
}; 

struct Port
{
	//이름
	std::string name;

	//지역 정보
	LocalTag localT;
	CountryTag conT;
	PortPosition port_Position;

	//취급화물
	S_Cargo_type cargo[4];

	//보유화물
	S_Cargo_type cargoStock[10];
};

enum class ShipType
{
	Sailer,
	Galley
};

struct Ship
{
	//이름
	std::string shipName;
	ShipType Stype;

	//속도
	int shipDefalutSpeed;
	int shipRealSpeed;

	//적재량
	int shipNowCargo;
	int shipCargo;

	//승무원
	int needCrew;
	int maxCrew;

	//가격
	int shipPrice;
};

struct Player
{
	//이름, 국적
	std::string player_name;
	CountryTag palyer_country;

	//위치
	Port nowPort;

	//재산
	int money = 10000;
	int truemoney = 10000;
	int Debt = 0;
	float Debt_val = 0;//이자율 
	DayTime DebtEndTime;
	Ship my_ship;

	//카고
	//카고 쓸때는 인덱스 값으로 할것. ex: 인쉽카고 인덱스 0 의 수량은 인쉽카고벨류 인덱스 0에 위치
	int nowman;
	S_Cargo_type inship_cargo[12];
	int inship_cargo_vlaue[12];
};