// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <iostream>
#include "mem.h"
#include "ent.h"
//#include "entityList.h"
#include <Windows.h>
#include <cmath>


//typedef Entity* (__cdecl* tGetCrosshairEnt)();
//typedef Entity* (__cdecl* tIsOnTheGround)(int Entity, float* groundLevel, float* jumpHeight);

//tGetCrosshairEnt GetCrosshairEnt = nullptr;

struct EntList
{
	Entity* ents[31];
};

bool IsValidEnt(Entity* ent)
{
	if (ent)
	{
		if (ent->vTable == 0x4E4A98 || ent->vTable == 0x4E4AC0)
		{
			return true;
		}
	}
	return false;
}



float toDeg(float rad)
{
	float pi = atan(1) * 4;
	return (rad * 180) / pi;
}

void writeCheatstoConsole(bool health, bool ammo, bool recoil, bool trigger, bool aimBot)
{
	system("CLS");

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	std::cout << "[NUM1] Infinite Health: ";

	if (health)
	{
		SetConsoleTextAttribute(hConsole, 10);

		std::cout << "ON" << std::endl;
	}
	else
	{
		SetConsoleTextAttribute(hConsole, 12);

		std::cout << "OFF" << std::endl;
	}

	SetConsoleTextAttribute(hConsole, 7);

	std::cout << "[NUM2] Infinite Ammo: ";

	if (ammo)
	{
		SetConsoleTextAttribute(hConsole, 10);

		std::cout << "ON" << std::endl;
	}
	else
	{
		SetConsoleTextAttribute(hConsole, 12);

		std::cout << "OFF" << std::endl;
	}

	SetConsoleTextAttribute(hConsole, 7);

	std::cout << "[NUM3] No Recoil: ";

	if (recoil)
	{
		SetConsoleTextAttribute(hConsole, 10);

		std::cout << "ON" << std::endl;
	}
	else
	{
		SetConsoleTextAttribute(hConsole, 12);

		std::cout << "OFF" << std::endl;
	}

	SetConsoleTextAttribute(hConsole, 7);

	std::cout << "[NUM4] Trigger Bot: ";

	if (trigger)
	{
		SetConsoleTextAttribute(hConsole, 10);

		std::cout << "ON" << std::endl;
	}
	else
	{
		SetConsoleTextAttribute(hConsole, 12);

		std::cout << "OFF" << std::endl;
	}

	SetConsoleTextAttribute(hConsole, 7);

	std::cout << "[NUM5] AimBot: ";

	if (aimBot)
	{
		SetConsoleTextAttribute(hConsole, 10);

		std::cout << "ON" << std::endl;
	}
	else
	{
		SetConsoleTextAttribute(hConsole, 12);

		std::cout << "OFF" << std::endl;
	}

	SetConsoleTextAttribute(hConsole, 7);
}

typedef Entity* (__cdecl* tGetCrosshairEnt)();

tGetCrosshairEnt GetCrosshairEnt = nullptr;

DWORD WINAPI HackThread(HMODULE hModule)
{
	//Create Console
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"ac_client.exe");

	GetCrosshairEnt = (tGetCrosshairEnt)(moduleBase + 0x607c0);



	bool bHealth = false, bAmmo = false, bRecoil = false, bTriggerBot = false, bAimBot = false;

	writeCheatstoConsole(bHealth, bAmmo, bRecoil, bTriggerBot, bAimBot);

	Entity* localPlayerPtr = { nullptr };
	//entList* ent_List;

	while (true)
	{
		if (GetAsyncKeyState(VK_END) & 1)
		{
			break;
		}

		if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		{
			bHealth = !bHealth;
			writeCheatstoConsole(bHealth, bAmmo, bRecoil, bTriggerBot, bAimBot);
		}

		if (GetAsyncKeyState(VK_NUMPAD2) & 1)
		{
			bAmmo = !bAmmo;
			writeCheatstoConsole(bHealth, bAmmo, bRecoil, bTriggerBot, bAimBot);
		}

		//no recoil NOP
		if (GetAsyncKeyState(VK_NUMPAD3) & 1)
		{
			bRecoil = !bRecoil;

			writeCheatstoConsole(bHealth, bAmmo, bRecoil, bTriggerBot, bAimBot);

			if (bRecoil)
			{
				mem::Nop((BYTE*)(moduleBase + 0x63786), 10);
			}

			else
			{
				//50 8D 4C 24 1C 51 8B CE FF D2 the original stack setup and call
				mem::Patch((BYTE*)(moduleBase + 0x63786), (BYTE*)"\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2", 10);
			}
		}

		if (GetAsyncKeyState(VK_NUMPAD4) & 1)
		{
			bTriggerBot = !bTriggerBot;

			writeCheatstoConsole(bHealth, bAmmo, bRecoil, bTriggerBot, bAimBot);
		}

		if (GetAsyncKeyState(VK_NUMPAD5) & 1)
		{
			bAimBot = !bAimBot;

			writeCheatstoConsole(bHealth, bAmmo, bRecoil, bTriggerBot, bAimBot);
		}

		//need to use uintptr_t for pointer arithmetic later
		localPlayerPtr = *(Entity**)(moduleBase + 0x10F4F4);

		int* gameMode = (int*)(moduleBase + 0x10F49C);

		int* numOfPlayers = (int*)(0x50f500);

		EntList* entList = *(EntList**)(moduleBase + 0x10F4F8);

		/*
		while (true)
		{
			for (int i = 0; i < (*numOfPlayers); i++)
			{
				if (entList && IsValidEnt(entList->ents[i]))
				{
					std::cout << "Ent " << i << " Health: " << entList->ents[i]->Health << std::endl;
					Sleep(1000);
				}
			}
		}*/

		if (localPlayerPtr)
		{
			if (bHealth)
			{
				localPlayerPtr->Health = 1337;
			}

			if (bAmmo)
			{
				uintptr_t ammoAddr = mem::FindDMAAddy(moduleBase + 0x10F4F4, { 0x374, 0x14, 0x0 });
				int* ammo = (int*)ammoAddr;
				*ammo = 420;

				//or just
				*(int*)mem::FindDMAAddy(moduleBase + 0x10F4F4, { 0x374, 0x14, 0x0 }) = 1337;

				localPlayerPtr->currentWeapon->ammoClip->ammo = 420;
			}

			bool isTeamMode = (*gameMode == 0x5) || (*gameMode == 0x6) || (*gameMode == 0x7) || (*gameMode == 0x14) || (*gameMode == 0x15);

			if (bAimBot)
			{
				if (entList && IsValidEnt(entList->ents[1]))
				{	
					float angleXPlayer = 0;
					float angleYPlayer = 0;
					float min_dist = INT_MAX;
					bool isAccess = false;
					float fAngleXPlayer = 0;
					float fAngleYPlayer = 0;

					for (int i = 1; i < *numOfPlayers; i++)
					{
						bool bAlive = !(entList->ents[i]->isDead);

						/*
						if(i == (*numOfPlayers - 2))
						{
							std::cout << "Pisha" << std::endl;
						}
						*/
						
						bool bAlive1 = !(entList->ents[1]->isDead);
						bool bAlive2 = !(entList->ents[2]->isDead);
						bool bAlive3 = !(entList->ents[3]->isDead);

						int plTeam = (int)localPlayerPtr->team;

						int team1 = (int)entList->ents[1]->team;
						int team2 = (int)entList->ents[2]->team;
						int team3 = (int)entList->ents[3]->team;

						if (((entList->ents[i]->team != localPlayerPtr->team || !(isTeamMode)) && bAlive))
						{
							//calculating angles for x-axis
							float opposite = entList->ents[i]->HeadPos.y - localPlayerPtr->HeadPos.y;
							float adj = entList->ents[i]->HeadPos.x - localPlayerPtr->HeadPos.x;

							float hyp = sqrt(pow((localPlayerPtr->HeadPos.x - entList->ents[i]->HeadPos.x), 2) + pow((localPlayerPtr->HeadPos.y - entList->ents[i]->HeadPos.y), 2));
							float pi = atan(1) * 4;
							angleXPlayer = abs(atan(opposite / adj) * 180 / pi);

							if (opposite > 0 && adj > 0)
							{
								angleXPlayer = angleXPlayer + 90;
							}
							else if (opposite > 0 && adj < 0)
							{
								angleXPlayer = 270 - angleXPlayer;
							}
							else if (opposite < 0 && adj < 0)
							{
								angleXPlayer = angleXPlayer - 90;
							}
							else
							{
								angleXPlayer = 90 - angleXPlayer;
							}

							//calculating angles for y-axis
							//float adjY = abs(localPlayerPtr->HeadPos.y - entList->ents[1]->HeadPos.y);

							float adjY = entList->ents[i]->HeadPos.z - localPlayerPtr->HeadPos.z;
							if (adjY != 0)
							{
								if (adjY > 0)
								{
									angleYPlayer = abs(atan(adjY / hyp) * 180 / pi);
								}
								else
								{
									angleYPlayer = -abs((atan(adjY / hyp) * 180 / pi));
								}
							}

							if (hyp <= min_dist)
							{
								localPlayerPtr->Angles.x = angleXPlayer;
								localPlayerPtr->Angles.y = angleYPlayer;

								min_dist = hyp;

								Entity* crosshairEnt = GetCrosshairEnt();

								if (crosshairEnt)
								{
									isAccess = true;
									fAngleXPlayer = angleXPlayer;
									fAngleYPlayer = angleYPlayer;
								}
							}
						}
					}

					if (isAccess)
					{
						localPlayerPtr->Angles.x = angleXPlayer;
						localPlayerPtr->Angles.y = angleYPlayer;
						
						Entity* crosshairEnt = GetCrosshairEnt();

						if (crosshairEnt)
						{
							if (localPlayerPtr->team != crosshairEnt->team || !(isTeamMode))
							{
								localPlayerPtr->bAttack = 1;
							}
						}
						else
						{
							localPlayerPtr->bAttack = 0;
						}
					}
				}
			}

			if (bTriggerBot)
			{
				Entity* crosshairEnt = GetCrosshairEnt();

				if (crosshairEnt)
				{
					if (localPlayerPtr->team != crosshairEnt->team || !(isTeamMode))
					{
						localPlayerPtr->bAttack = 1;
					}
				}

				else 
				{
					localPlayerPtr->bAttack = 0;
				}
			}

		}
		Sleep(5);
		localPlayerPtr->bAttack = 0;
	}

	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}