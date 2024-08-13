#include <iostream>

enum class Cargo_type
{
	tulip,		//튤립
	pepper,		//후추
	suger,		//설탕
	wine,		//와인
	Dye,		//염료
	carpet,		//융단	
	ceramic,	//도자기
	glassDish,	//유리그릇		
	tea,		//차
	perfume		//향수
};

enum class LocalTag
{	
	Mediterranean_Sea,
	English_Channel,
	India_Asia,
	Africa
};

enum class countryTag
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

struct portPosition
{
	float x_pos;
	float y_pos;
};

struct port
{
	//이름
	const char* name;

	//지역 정보
	LocalTag localT;
	countryTag conT;
	portPosition port_Position;

	//취급화물
	Cargo_type cargo[];
};

struct ship
{
	//이름
	const char* shipName;

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

struct player
{

};