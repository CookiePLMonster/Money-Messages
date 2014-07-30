#ifndef __MONEYMESSAGES
#define __MONEYMESSAGES

#include "General.h"

#define NUM_MAX_MONEY_MESSAGES		16

// III/VC reversed class
class CMoneyMessage
{
public:
	signed int						m_nTimeRegistered;
	CVector							m_vecPosition;
	wchar_t							m_aText[16];
	CRGBA							m_Colour;
	float							m_fSize;
	float							m_fOpacity;
};

class CMoneyMessages
{
private:
	static CMoneyMessage* const		aMoneyMessages;

public:
	// This function wasn't present in GTA III, so it has to be recreated
	static void						RegisterOne(CVector vecPos, const char* pText, unsigned char bRed, unsigned char bGreen, unsigned char bBlue, float fSize, float fOpacity);

};


static_assert(sizeof(CMoneyMessage) == 0x3C, "Wrong class size: CMoneyMessage");

#endif