#include "StdAfx.h"
#include "MoneyMessages.h"

CMoneyMessage* const	CMoneyMessages::aMoneyMessages = *AddressByVersion<CMoneyMessage**>(0x51AF79, 0x51B1A9, 0x51B139, 0x574F14, 0x574F34, 0x574E04);

// GTA global variables
// We need this one only in this .cpp file, so we can define it there
int32_t&				snTimeInMilliseconds = **AddressByVersion<int32_t**>(0x40B3B8, 0x40B3B8, 0x40B3B8, 0x418CFC, 0x418CFC, 0x418CFC);

// This function wasn't present in GTA III, so it has to be recreated
void CMoneyMessages::RegisterOne(CVector vecPos, const char* pText, uint8_t bRed, uint8_t bGreen, uint8_t bBlue, float fSize, float fOpacity)
{
	uint32_t		nIndex = 0;
	while ( aMoneyMessages[nIndex].m_nTimeRegistered )
	{
		++nIndex;

		// Out of space?
		if ( nIndex >= NUM_MAX_MONEY_MESSAGES )
			return;
	}

	// Add data of this money message to the array
	AsciiToUnicode(pText, aMoneyMessages[nIndex].m_aText, 16);

	aMoneyMessages[nIndex].m_nTimeRegistered = snTimeInMilliseconds;
	aMoneyMessages[nIndex].m_vecPosition = vecPos;
	aMoneyMessages[nIndex].m_Colour.r = bRed;
	aMoneyMessages[nIndex].m_Colour.g = bGreen;
	aMoneyMessages[nIndex].m_Colour.b = bBlue;
	aMoneyMessages[nIndex].m_fSize = fSize;
	aMoneyMessages[nIndex].m_fOpacity = fOpacity;
}