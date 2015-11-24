#include "ScreenEffect.h"

ScreenEffect::ScreenEffect():
m_val(0),
m_speed(0),
m_rec_mode(false)
{
	m_next_color.a = 0;
	m_next_color.r = 0;
	m_next_color.g = 0;
	m_next_color.b = 0;

	m_pre_color.a = 0;
	m_pre_color.r = 0;
	m_pre_color.g = 0;
	m_pre_color.b = 0;
}

ScreenEffect::~ScreenEffect()
{

}

void ScreenEffect::Fade_In(float a, float r, float g, float b, float speed)
{
	m_pre_color = m_next_color;
	m_next_color.a = a;
	m_next_color.r = r;
	m_next_color.g = g;
	m_next_color.b = b;
	m_speed=speed;
	m_val = 0;
}

void ScreenEffect::Fade_Out(float speed)
{
	Fade_In(0,0,0,0,speed);
}

void ScreenEffect::Fade_In_Out(
	float a,
	float r,
	float g,
	float b,
	float ChangeSpeed,
	float RecSpeed)
{
	Fade_In(a, r, g, b, ChangeSpeed);
	m_rec_mode = true;
	m_rec_speed = RecSpeed;
}

void ScreenEffect::Update()
{
	m_val += m_speed;
	if (m_val > 1)
	{
		if (m_rec_mode)
		{
			m_rec_mode = false;
			Fade_Out(m_rec_speed);
		}else{
			m_val = 1;
		}
	}
	if (m_val < 0)m_val = 0;
}

void ScreenEffect::Render()
{
	_usecolor nowcolor=
	{
		m_pre_color.a*(1 - m_val) + m_next_color.a*m_val,
		m_pre_color.r*(1 - m_val) + m_next_color.r*m_val,
		m_pre_color.g*(1 - m_val) + m_next_color.g*m_val,
		m_pre_color.b*(1 - m_val) + m_next_color.b*m_val
	};
	iexPolygon::Rect(
		0,
		0,
		iexSystem::ScreenWidth,
		iexSystem::ScreenHeight,
		RS_COPY,
		D3DCOLOR_COLORVALUE(
		nowcolor.r,
		nowcolor.g,
		nowcolor.b,
		nowcolor.a
		)
		);
}
