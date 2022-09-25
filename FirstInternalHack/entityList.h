#include <Windows.h>
#include <cstdio>
#include <cstdint>

class Entity1
{
public:
	char pad_0000[4]; //0x0000
	Vector3 HeadPos; //0x0004
	char pad_0010[36]; //0x0010
	Vector3 BodyPos; //0x0034
	Vector3 Angles; //0x0040
	char pad_004C[172]; //0x004C
	int32_t Health; //0x00F8
	char pad_00FC[296]; //0x00FC
	int8_t bAttack; //0x0224
	char pad_0225[263]; //0x0225
	int32_t team; //0x032C
	char pad_0330[68]; //0x0330
	class Weapon1* currentWeapon; //0x0374
	char pad_0378[1248]; //0x0378
}; //Size: 0x0858
static_assert(sizeof(Entity1) == 0x858);

class Weapon1
{
public:
	char pad_0000[4]; //0x0000
	int32_t weaponID; //0x0004
	class Entity1* ownerPtr; //0x0008
	char pad_000C[4]; //0x000C
	class ammoPtr1* ammoReserve; //0x0010
	class ammoPtr1* N00000275; //0x0014
	char pad_0018[44]; //0x0018
}; //Size: 0x0044
static_assert(sizeof(Weapon1) == 0x44);

class ammoPtr1
{
public:
	int32_t ammo; //0x0000
}; //Size: 0x0004
static_assert(sizeof(ammoPtr1) == 0x4);

class entityLists
{
public:
	class Entity1* ent; //0x0000
}; //Size: 0x0004
static_assert(sizeof(entityLists) == 0x4);

class entList
{
public:
	char pad_0000[4]; //0x0000
	class entityLists entArray[64]; //0x0004
}; //Size: 0x0104
static_assert(sizeof(entList) == 0x104);