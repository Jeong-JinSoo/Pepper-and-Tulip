#include <iostream>

enum class Cargo_type
{
	tulip,		//ƫ��
	pepper,		//����
	suger,		//����
	wine,		//����
	Dye,		//����
	carpet,		//����	
	ceramic,	//���ڱ�
	glassDish,	//�����׸�		
	tea,		//��
	perfume		//���
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
	//�̸�
	const char* name;

	//���� ����
	LocalTag localT;
	countryTag conT;
	portPosition port_Position;

	//���ȭ��
	Cargo_type cargo[];
};

struct ship
{
	//�̸�
	const char* shipName;

	//�ӵ�
	int shipDefalutSpeed;
	int shipRealSpeed;

	//���緮
	int shipNowCargo;
	int shipCargo;

	//�¹���
	int needCrew;
	int maxCrew;

	//����
	int shipPrice;
};

struct player
{

};