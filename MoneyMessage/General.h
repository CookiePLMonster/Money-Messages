#ifndef __GENERAL
#define __GENERAL

// RenderWare structures
struct RwV3d
{
    float x;   /**< X value */
    float y;   /**< Y value */
    float z;   /**< Z value */
};

struct RwMatrix
{
    /* These are padded to be 16 byte quantities per line */
    RwV3d               right;
    uint32_t            flags;
    RwV3d               up;
    uint32_t            pad1;
    RwV3d               at;
    uint32_t            pad2;
    RwV3d               pos;
    uint32_t            pad3;
};

// Generic GTA classes
class CRGBA
{
public:
	uint8_t r, g, b, a;

	inline CRGBA() {}

	inline CRGBA(const CRGBA& in)
		: r(in.r), g(in.g), b(in.b), a(in.a)
	{}

	inline CRGBA(const CRGBA& in, uint8_t alpha)
		: r(in.r), g(in.g), b(in.b), a(alpha)
	{}

	inline CRGBA(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255)
		: r(red), g(green), b(blue), a(alpha)
	{}
};

class CVector
{
public:
	float	x, y, z;

	CVector()
	{}

	CVector(float fX, float fY, float fZ=0.0f)
		: x(fX), y(fY), z(fZ)
	{}

	friend inline CVector operator*(const CVector& in, float fMul)
		{ return CVector(in.x * fMul, in.y * fMul, in.z * fMul); }
	friend inline CVector operator+(const CVector& vec1, const CVector& vec2)
		{ return CVector(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z); }
	friend inline CVector operator-(const CVector& vec1, const CVector& vec2)
		{ return CVector(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z); }
	friend inline CVector operator-(const CVector& vec)
		{ return CVector(-vec.x, -vec.y, -vec.z); }
};

class CMatrix
{
public:
	RwMatrix	matrix;
	RwMatrix*	pMatrix;
	BOOL		haveRwMatrix;

public:
	inline CVector*	GetRight()
		{ return reinterpret_cast<CVector*>(&matrix.right); }
	inline CVector*	GetUp()
		{ return reinterpret_cast<CVector*>(&matrix.up); }
	inline CVector*	GetAt()
		{ return reinterpret_cast<CVector*>(&matrix.at); }
	inline CVector* GetPos()
		{ return reinterpret_cast<CVector*>(&matrix.pos); }
};

class CPickupIII
{
public:
	uint8_t			bPickupType;
	bool			bRemoved;
	uint16_t		wPickupQuantity;
	void*			pObject;
	int32_t			nTimer;
	int16_t			nModelId;
	uint16_t		wUniqueId;
	CVector			vecPos;
};

// maxorator's definition
class CPickupVC
{
public:
	CVector			vecPos;
	float			fStandProximity;
	void*			pObject;
	void*			pExtraObject;
	uint32_t		dwPickupQuantity;
	int32_t			nTimer;
	uint16_t		wMoneySpeed;
	int16_t			nModelId;
	uint16_t		wUniqueId;
	char			szPickupTextKey[8];
	uint8_t			bPickupType;
	bool			bRemoved;
	bool			bEffects;
};

class CPlaceable
{
public:
	CMatrix			m_Matrix;

public:
	// Just to line up the offsets
	virtual			~CPlaceable()
	{}
};

static_assert(sizeof(CPickupIII) == 0x1C, "Wrong class size: CPickupIII");
static_assert(sizeof(CPickupVC) == 0x34, "Wrong class size: CPickupVC");

// Generic GTA functions - defining there as inline functions to save hassle

inline void AsciiToUnicode(const char* pIn, wchar_t* pOut, size_t num)
{
	// num parameter is custom - added this to prevent potential buffer overflows
	size_t		count = 0;
	while ( pIn[count] && count < num )
	{
		pOut[count] = pIn[count];
		++count;
	}

	pOut[count] = '\0';
}

#endif