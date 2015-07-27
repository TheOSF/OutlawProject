

#include "WindEffect.h"

WindEffect::WindEffect(size_t length) :
m_Locus(length),
m_Alive(true)
{
	m_pMyStateMachine = new MyStateMachine(this);
}

WindEffect::~WindEffect()
{
	delete m_pMyStateMachine;
}

void WindEffect::SetNewState(MyState* NewState)
{
	m_pMyStateMachine->set_state(NewState);
}


bool WindEffect::Update()
{
	m_pMyStateMachine->state_execute();

	return m_Alive;
}

bool WindEffect::Msg(MsgType mt)
{
	return false;
}


//----------------------------------------------
//@‰ñ‚éƒNƒ‰ƒX
//----------------------------------------------


WindRotateUp::WindRotateUp() :
m_Center(0, 0, 0),

m_Angle(0),
m_CenterLen(0),
m_Height(0),

m_AngleSpeed(0.1f),
m_CenterLenSpeed(0.1f),
m_HeightSpeed(0.1f)
{
	
}

void WindRotateUp::Enter(_Client_type_ptr p)
{

}

void WindRotateUp::Execute(_Client_type_ptr p)
{
	Vector3 pos = m_Center;

	pos.x += sinf(m_Angle)*m_CenterLen;
	pos.y += m_Height;
	pos.z += cosf(m_Angle)*m_CenterLen;

	p->m_Locus.AddPoint(
		pos, 
		Vector3AxisY
		);

	m_Angle += m_AngleSpeed;
	m_Height += m_HeightSpeed;
	m_CenterLen += m_CenterLenSpeed;

	p->m_Locus.m_StartParam.Color.w = max(p->m_Locus.m_StartParam.Color.w - 0.015f, 0);

	p->m_Alive = p->m_Locus.m_StartParam.Color.w > 0.01f;

}

void WindRotateUp::Exit(_Client_type_ptr p)
{

}