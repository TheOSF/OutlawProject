#include "PlayerGauge.h"


PlayerGauge::PlayerGauge(CharacterBase*   pOwner)
{

}


PlayerGauge::~PlayerGauge()
{

}


bool PlayerGauge::Update()
{

	return true;
}

bool PlayerGauge::Msg(MsgType mt)
{


	return false;
}


void PlayerGauge::CalcZ()
{
	m_SortZ = 0;
}


void PlayerGauge::Render()
{

}