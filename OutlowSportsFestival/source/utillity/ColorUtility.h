#ifndef __COLOR_UTILITY_H__
#define __COLOR_UTILITY_H__


typedef unsigned long DWORD;
typedef DWORD COLOR;

#ifndef ARGB
#define ARGB(a,r,g,b)	((COLOR)( (a<<24) | (r<<16) | (g<<8) | (b) ) )
#endif

#define ARGB_RATIO(a,r,g,b) ARGB((DWORD)((a)*255.f),(DWORD)((r)*255.f),(DWORD)((g)*255.f),(DWORD)((b)*255.f))

#define COLORtoF(a,r,g,b,color) \
{\
	a = (float)(((color >> 24) & 0xFF) / 255); \
	r = (float)(((color >> 16) & 0xFF) / 255); \
	g = (float)(((color >> 8) & 0xFF) / 255); \
	b = (float)(((color)& 0xFF) / 255); \
}

#define COLOR_A(val) ((val >> 24) & 0xFF)
#define COLOR_R(val) ((val >> 16) & 0xFF)
#define COLOR_G(val) ((val >> 8) & 0xFF)
#define COLOR_B(val) ((val ) & 0xFF)

struct ColorComp
{
	union
	{
		COLOR color_value;
		struct
		{
			unsigned char color_b, color_g, color_r, color_a;
		};
	};
};

class COLORf
{
public:
	typedef float RATIO;
	union{
		struct{
			RATIO a, r, g, b;
		};
		RATIO _array[4];
	};
public:
	COLORf();
	COLORf(RATIO a, RATIO r, RATIO g, RATIO b);
	COLORf(RATIO _array[4]);
	COLORf(DWORD color);
public:
	static COLORf Lerp(COLORf c1, COLORf c2, RATIO t);

	void	SetColor(DWORD col);
	void	SetColor(RATIO a, RATIO r, RATIO g, RATIO b);
	void	ClampValue();
	DWORD	toDWORD();
};

COLOR	ColorLerp(COLOR col1, COLOR col2, float v);

namespace color_smp
{
	static const COLOR

		WHITE = 0xFFFFFFFF,
		GRAY = 0xFF808080,
		BLACK = 0xFF000000,

		RED = 0xFFFF0000,
		GREEN = 0xFF00FF00,
		BLUE = 0xFF0000FF,

		YELLOW = 0xFFFFFF00,
		PURPLE = 0xFF800080,
		CYAN = 0xFF00FFFF;
};

#endif