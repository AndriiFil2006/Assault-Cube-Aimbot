#include <Windows.h>
#include <cstdio>
#include <cstdint>

struct vec3 { float x, y, z; };

using Vector3 = vec3;

class Entity
{
public:
	DWORD vTable; //0x0
	Vector3 HeadPos; //0x0004
	char pad_0010[36]; //0x0010
	Vector3 BodyPos; //0x0034
	Vector3 Angles; //0x0040
	char pad_004C[52]; //0x004C
	uint16_t uselessSht; //0x0080
	bool isDead; //0x0082
	char pad_0083[117]; //0x0083
	int32_t Health; //0x00F8
	char pad_00FC[296]; //0x00FC
	int8_t bAttack; //0x0224
	char pad_0225[263]; //0x0225
	int32_t team; //0x032C
	char pad_0330[68]; //0x0330
	class Weapon* currentWeapon; //0x0374
	char pad_0378[1248]; //0x0378
}; //Size: 0x0858
//static_assert(sizeof(Entity) == 0x864);

class Weapon
{
public:
	char pad_0000[4]; //0x0000
	int32_t weaponID; //0x0004
	class Entity* ownerPtr; //0x0008
	char pad_000C[4]; //0x000C
	class ammoPtr* ammoReserve; //0x0010
	class ammoPtr* ammoClip; //0x0014
	char pad_0018[44]; //0x0018
}; //Size: 0x0044
static_assert(sizeof(Weapon) == 0x44);

class ammoPtr
{
public:
	int32_t ammo; //0x0000
}; //Size: 0x0004
static_assert(sizeof(ammoPtr) == 0x4);