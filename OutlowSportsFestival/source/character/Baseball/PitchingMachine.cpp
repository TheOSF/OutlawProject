#include "PitchingMachine.h"

PitchingMachine::PitchingMachine(
    BaseballPlayer*     pChr,
    CrVector3           pos,
    CrVector3           vec
    ):
    m_pChr(pChr)
{

}

PitchingMachine::~PitchingMachine()
{

}

bool PitchingMachine::Update()
{
    return true;
}

bool PitchingMachine::Msg(MsgType mt)
{
    return false;
}