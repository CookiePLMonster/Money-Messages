#include "StdAfx.h"

#include "MoneyMessages.h"

static CRGBA			MoneyMessagesColours[5];
static uint8_t			CurrentPickedColour;

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
	((void(*)(void*))0x4AE9D0)(pHeli);
}

// extern "C" is not really needed there - it's only added for MinGW's sake
// When using Visual Studio, no need to use it
extern "C" BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(hModule);
	UNREFERENCED_PARAMETER(lpReserved);
	if(reason == DLL_PROCESS_ATTACH)
	{
		// Initialise MoneyMessagesColours array
		// Colour 1 - red
		MoneyMessagesColours[0].r = 255;
		MoneyMessagesColours[0].g = 0;
		MoneyMessagesColours[0].b = 0;

		// Colour 2 - gray
		MoneyMessagesColours[1].r = 125;
		MoneyMessagesColours[1].g = 125;
		MoneyMessagesColours[1].b = 125;

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

		// Money pickups
		MemoryVP::InjectHook(0x431342, RegisterMoneyMessageIII_Pickup);
		MemoryVP::Nop(0x431333, 4);
		MemoryVP::Patch<BYTE>(0x431337, 0x56);

		// Vehicle explosions
		MemoryVP::InjectHook(0x4A1678, RegisterMoneyMessageIII_VehicleExplosion);
		MemoryVP::Nop(0x4A166E, 1);
		MemoryVP::Patch<DWORD>(0x4A166F, 0x202474FF);

		// Vehicle damage
		MemoryVP::Nop(0x52FDCB, 4);
		MemoryVP::Patch<BYTE>(0x52FDCF, 0x55);
		MemoryVP::InjectHook(0x52FDD5, RegisterMoneyMessageIII_VehicleDamage);

		// Helicopter explosion
		MemoryVP::InjectHook(0x54A063, RegisterMoneyMessageIII_HeliBlowup);
	}
	return TRUE;
}