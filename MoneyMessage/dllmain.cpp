#include "StdAfx.h"

#include "MoneyMessages.h"

static CRGBA			MoneyMessagesColours[5];
static uint8_t			CurrentPickedColour;

// III/VC function pointers
auto WorldRemove = AddressByVersion<void(*)(void*)>(0x4AE9D0, 0x4AEAC0, 0x4AEA50, 0, 0, 0);


// Functions replace GTA III sprintf calls, which formatted later unused money messages
void RegisterMoneyMessageIII_Pickup(char* str, const char* format, CPickupIII* arg)
{
	sprintf(str, format, arg->wPickupQuantity);
	CMoneyMessages::RegisterOne(arg->vecPos + CVector(0.0f, 0.0f, 1.0f), str, 0, 255, 0, 0.5f, 0.5f);
}

void RegisterMoneyMessageIII_VehicleExplosion(char* str, CPlaceableIII* pVehicle, uint32_t value)
{
	sprintf(str, "$%d", value);
	CMoneyMessages::RegisterOne(*pVehicle->m_Matrix.GetPos() + CVector(0.0f, 0.0f, 2.0f), str, MoneyMessagesColours[CurrentPickedColour].r, MoneyMessagesColours[CurrentPickedColour].g, MoneyMessagesColours[CurrentPickedColour].b, 2.0f, 0.75f);
	CurrentPickedColour = ++CurrentPickedColour % 5;
}

void RegisterMoneyMessageIII_VehicleDamage(char* str, CPlaceableIII* pVehicle, uint32_t value)
{
	sprintf(str, "$%d", value);
	CMoneyMessages::RegisterOne(*pVehicle->m_Matrix.GetPos() + CVector(0.0f, 0.0f, 1.5f), str, MoneyMessagesColours[CurrentPickedColour].r, MoneyMessagesColours[CurrentPickedColour].g, MoneyMessagesColours[CurrentPickedColour].b, 1.0f, 0.5f);
	CurrentPickedColour = ++CurrentPickedColour % 5;
}

void RegisterMoneyMessageIII_HeliBlowup(CPlaceableIII* pHeli)
{
	// You always get $250 for blowing up a heli in III
	CMoneyMessages::RegisterOne(*pHeli->m_Matrix.GetPos() + CVector(0.0f, 0.0f, 2.5f), "$250", MoneyMessagesColours[CurrentPickedColour].r, MoneyMessagesColours[CurrentPickedColour].g, MoneyMessagesColours[CurrentPickedColour].b, 2.5f, 0.75f);
	CurrentPickedColour = ++CurrentPickedColour % 5;

	// Call CWorld::Remove
	WorldRemove(pHeli);
}

void Patch_III_10()
{
	using namespace MemoryVP;

	// Money pickups
	InjectHook(0x431342, RegisterMoneyMessageIII_Pickup);
	Nop(0x431333, 4);
	Patch<BYTE>(0x431337, 0x56);

	// Vehicle explosions
	InjectHook(0x4A1678, RegisterMoneyMessageIII_VehicleExplosion);
	Nop(0x4A166E, 1);
	Patch<DWORD>(0x4A166F, 0x202474FF);

	// Vehicle damage
	Nop(0x52FDCB, 4);
	Patch<BYTE>(0x52FDCF, 0x55);
	InjectHook(0x52FDD5, RegisterMoneyMessageIII_VehicleDamage);

	// Helicopter explosion
	InjectHook(0x54A063, RegisterMoneyMessageIII_HeliBlowup);
}

void Patch_III_11()
{
	using namespace MemoryVP;

	// Money pickups
	InjectHook(0x431342, RegisterMoneyMessageIII_Pickup);
	Nop(0x431333, 4);
	Patch<BYTE>(0x431337, 0x56);

	// Vehicle explosions
	InjectHook(0x4A1768, RegisterMoneyMessageIII_VehicleExplosion);
	Nop(0x4A175E, 1);
	Patch<DWORD>(0x4A175F, 0x202474FF);

	// Vehicle damage
	Nop(0x53000B, 4);
	Patch<BYTE>(0x53000F, 0x55);
	InjectHook(0x530015, RegisterMoneyMessageIII_VehicleDamage);

	// Helicopter explosion
	InjectHook(0x54A263, RegisterMoneyMessageIII_HeliBlowup);
}

void Patch_III_Steam()
{
	using namespace MemoryVP;

	// Money pickups
	InjectHook(0x431342, RegisterMoneyMessageIII_Pickup);
	Nop(0x431333, 4);
	Patch<BYTE>(0x431337, 0x56);

	// Vehicle explosions
	InjectHook(0x4A16F8, RegisterMoneyMessageIII_VehicleExplosion);
	Nop(0x4A16EE, 1);
	Patch<DWORD>(0x4A16EF, 0x202474FF);

	// Vehicle damage
	Nop(0x52FF9E, 4);
	Patch<BYTE>(0x52FF9F, 0x55);
	InjectHook(0x52FFA5, RegisterMoneyMessageIII_VehicleDamage);

	// Helicopter explosion
	InjectHook(0x54A213, RegisterMoneyMessageIII_HeliBlowup);
}


void Patch_VC_10()
{
	using namespace MemoryVP;

}

void Patch_VC_11()
{
	using namespace MemoryVP;
}

void Patch_VC_Steam()
{
	using namespace MemoryVP;
}

// extern "C" is not really needed there - it's only added for MinGW's sake
// When using Visual Studio, no need to use it
extern "C" BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(hModule);
	UNREFERENCED_PARAMETER(lpReserved);
	if(reason == DLL_PROCESS_ATTACH)
	{
		if (*(uint32_t*)0x5C1E70 == 0x53E58955) Patch_III_10();
		else if (*(uint32_t*)0x5C2130 == 0x53E58955) Patch_III_11();
		else if (*(uint32_t*)0x5C6FD0 == 0x53E58955) Patch_III_Steam();
		else if (*(uint32_t*)0x667BF0 == 0x53E58955) Patch_VC_10();
		else if (*(uint32_t*)0x667C40 == 0x53E58955) Patch_VC_11();
		else if (*(uint32_t*)0x666BA0 == 0x53E58955) Patch_VC_Steam();
		else return FALSE;	// If game version couldn't be detected

		// Initialise MoneyMessagesColours array
		// Colour 1 - red
		MoneyMessagesColours[0].r = 255;
		MoneyMessagesColours[0].g = 0;
		MoneyMessagesColours[0].b = 0;

		// Colour 2 - white
		MoneyMessagesColours[1].r = 255;
		MoneyMessagesColours[1].g = 255;
		MoneyMessagesColours[1].b = 255;

		// Colour 3 - green
		MoneyMessagesColours[2].r = 0;
		MoneyMessagesColours[2].g = 255;
		MoneyMessagesColours[2].b = 0;

		// Colour 4 - yellow
		MoneyMessagesColours[3].r = 255;
		MoneyMessagesColours[3].g = 255;
		MoneyMessagesColours[3].b = 0;
			
		// Colour 5 - light blue
		MoneyMessagesColours[4].r = 0;
		MoneyMessagesColours[4].g = 200;
		MoneyMessagesColours[4].b = 255;

		CurrentPickedColour = 0;
	}
	return TRUE;
}