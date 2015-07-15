

#include <Windows.h>
#include "ColorUtility.h"
#include <d3d9types.h>

DWORD	ColorLerp(DWORD col1, DWORD col2, float v)
{
	ColorComp color;
	float invv = 1 - v;

	int c[4]=
	{
		(int)((float)COLOR_A(col1)*invv + (float)COLOR_A(col2)*v),
		(int)((float)COLOR_R(col1)*invv + (float)COLOR_R(col2)*v),
		(int)((float)COLOR_G(col1)*invv + (float)COLOR_G(col2)*v),
		(int)((float)COLOR_B(col1)*invv + (float)COLOR_B(col2)*v)
	};

	for (int i = 0; i < 4; i++)
		c[i] = min(UCHAR_MAX, c[i]);

	color.color_a = (UCHAR)c[0];
	color.color_r = (UCHAR)c[1];
	color.color_g = (UCHAR)c[2];
	color.color_b = (UCHAR)c[3];

	return color.color_value;
}

COLORf::COLORf()
{
	for (int i = 0; i < 4; ++i)
	{
		_array[i] = 1;
	}
}

COLORf::COLORf(RATIO a, RATIO r, RATIO g, RATIO b)
{
	SetColor(a, r, g, b);
}

COLORf::COLORf(RATIO _array[4])
{
	for (int i = 0; i < 4; ++i)
	{
		this->_array[i] = _array[i];
	}
}

COLORf::COLORf(DWORD color)
{
	SetColor(color);
}

COLORf COLORf::Lerp(COLORf c1, COLORf c2, RATIO t)
{
	const RATIO mt = 1 - t;

	return COLORf(
		c1.a*mt + c2.a*t,
		c1.r*mt + c2.r*t,
		c1.g*mt + c2.g*t,
		c1.b*mt + c2.b*t
		);
}

void COLORf::SetColor(DWORD col)
{
	a = (float)COLOR_A(col) / 255.0f;
	r = (float)COLOR_R(col) / 255.0f;
	g = (float)COLOR_G(col) / 255.0f;
	b = (float)COLOR_B(col) / 255.0f;

	ClampValue();
}

void	COLORf::SetColor(RATIO a, RATIO r, RATIO g, RATIO b)
{
	this->a = a;
	this->r = r;
	this->g = g;
	this->b = b;
}

void	COLORf::ClampValue()
{
	for (int i = 0; i < 4; ++i)
	{
		_array[i] = max(0, _array[i]);
		_array[i] = min(1, _array[i]);
	}
}

DWORD	COLORf::toDWORD()
{
	ClampValue();
	return D3DCOLOR_COLORVALUE(r, g, b, a);
}