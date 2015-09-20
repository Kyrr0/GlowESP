#include <Windows.h>
#include "ProcMem.h"
#include "Offsets.h"

#pragma region Settings
//Stealth
bool StealthActive = true;

//Glow
bool GlowActive = true;
bool GlowTeamCheck = true; 
float GlowTerroristRed = 1.f;
float GlowTerroristGreen = 0.f;
float GlowTerroristBlue = 0.f;
float GlowTerroristAlpha = 1.f; 

float GlowCounterTerroristRed = 0.f;
float GlowCounterTerroristGreen = 0.f;
float GlowCounterTerroristBlue = 1.f;
float GlowCounterTerroristAlpha = 1.f;
#pragma endregion

ProcMem proM;
DWORD Client;

int main(); 
void GlowEsp();
void Stealth();

int main()
{
	proM.Process("csgo.exe");
	Sleep(200);
	Client = proM.Module("client.dll");
	if (StealthActive)
		Stealth(); 

	while (true)
	{
		if (GlowActive)
			GlowEsp();
	}
}

void Stealth()
{
	HWND Stealth;
	AllocConsole();
	Stealth = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(Stealth, 0);
}


void GlowEsp()
{
	DWORD glow_LocalBase = proM.Read<DWORD>(Client + m_dwLocalPlayerIndex);
	DWORD glow_Pointer = proM.Read<DWORD>(Client + m_dwGlowObject);
	int MyTeamID = proM.Read<int>(glow_LocalBase + m_iTeamNum);

	for (int i = 0; i < 32; i++)
	{
		int glow_currentPlayer = proM.Read<int>(Client + m_dwEntityList + i * 0x10);
		bool glow_currentPlayerDormant = proM.Read<bool>(glow_currentPlayer + m_bDormant);
		int glow_currentPlayerGlowIndex = proM.Read<int>(glow_currentPlayer + m_iGlowIndex);
		int EntityBaseTeamID = proM.Read<int>(glow_currentPlayer + m_iTeamNum);

		if (glow_currentPlayerDormant == 1 || EntityBaseTeamID == 0)
			continue;
		else
			if (MyTeamID != EntityBaseTeamID || GlowTeamCheck == false)
				switch (EntityBaseTeamID)	// 1 GoTV; 2 T; 3 CT
				{
					case 2:
						proM.Write<float>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x34) + 0x4)), GlowTerroristRed);
						proM.Write<float>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x34) + 0x8)), GlowTerroristGreen);
						proM.Write<float>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x34) + 0xC)), GlowTerroristBlue);
						proM.Write<float>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x34) + 0x10)), GlowTerroristAlpha);
						proM.Write<BOOL>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x34) + 0x24)), true);
						proM.Write<BOOL>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x34) + 0x25)), false);
						break;
					case 3:
						proM.Write<float>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x34) + 0x4)), GlowCounterTerroristRed);
						proM.Write<float>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x34) + 0x8)), GlowCounterTerroristGreen);
						proM.Write<float>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x34) + 0xC)), GlowCounterTerroristBlue);
						proM.Write<float>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x34) + 0x10)), GlowCounterTerroristAlpha);
						proM.Write<BOOL>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x34) + 0x24)), true);
						proM.Write<BOOL>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x34) + 0x25)), false);
						break;
				}
	}
}
