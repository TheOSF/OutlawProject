#ifndef __SCREEN_EFFECT_H__
#define __SCREEN_EFFECT_H__
#include "iextreme.h"

//***************************************************
//		画面に色を付けるクラス
//***************************************************

class ScreenEffect
{
	struct _usecolor
	{
		float a, r, g, b;
	};
private:
	_usecolor m_pre_color;
	_usecolor m_next_color;
	float m_val;
	float m_speed;
	float m_rec_speed;
	bool m_rec_mode;
public:
	ScreenEffect();
	~ScreenEffect();
public:
	//指定した色に画面を変化させる
	void Fade_In(float a,float r,float g,float b,float speed);
	//画面を元に戻す
	void Fade_Out(float speed);
	//変化してから戻す
	void Fade_In_Out(float a, float r, float g, float b, float ChangeSpeed,float RecSpeed);

	float GetNowValue()const{ return m_val; }

	void Render();
	void Update();
};
#endif