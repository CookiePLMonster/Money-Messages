#include "StdAfx.h"
#include "MoneyMessages.h"

CMoneyMessage* const	CMoneyMessages::aMoneyMessages = (CMoneyMessage*)0x7268F0;

// GTA global variables
// We need this one only in this .cpp file, so we can define it there
signed int&				snTimeInMilliseconds = *(signed int*)0x885B48;

// This function wasn't present in GTA III, so it has to be recreated
void CMoneyMessages::RegisterOne(CVector vecPos, const char* pText, unsigned char bRed, unsigned char bGreen, unsigned char bBlue, float fSize, float fOpacity)
{
	int		nIndex = 0;
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