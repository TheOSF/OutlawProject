#ifndef __WIND_EFFECT_H__
#define __WIND_EFFECT_H__

#include "../GameSystem/GameObject.h"
#include "../utillity/Locus.h"
#include "../utillity/StateTemplate.h"

//----------------------------------------------
//　風エフェクトクラス
//----------------------------------------------

class WindEffect :public GameObjectBase
{
public:
	typedef State<WindEffect, void> MyState;

	bool    m_Alive;
	Locus   m_Locus;

	WindEffect(size_t length = 10);
	~WindEffect();

	void SetNewState(MyState* NewState);

private:
	typedef StateMachine<WindEffect, void> MyStateMachine;

	MyStateMachine* m_pMyStateMachine;

	bool Update();
	bool Msg(MsgType mt);
};


//----------------------------------------------
//　回るクラス
//----------------------------------------------

class WindRotateUp :public WindEffect::MyState
{
public:
	Vector3  m_Center;
	float    m_Angle;
	float    m_CenterLen;
	float    m_Height;

	float    m_AngleSpeed;
	float    m_CenterLenSpeed;
	float    m_HeightSpeed;

	WindRotateUp();

private:
	void Enter(_Client_type_ptr p);
	void Execute(_Client_type_ptr p);
	void Exit(_Client_type_ptr p);
};

#endif