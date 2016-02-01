#pragma once

#include "../../GameSystem/GameObject.h"
#include "BaseballPlayer.h"

//--------------------------------------------------------//
//  �@�@�@�@�@�@�@�s�b�`���O�}�V�[��
//--------------------------------------------------------//

class PitchingMachine : public GameObjectBase
{
public:

    PitchingMachine(
        BaseballPlayer*     pChr,
        CrVector3           pos,
        CrVector3           vec
        );

    ~PitchingMachine();


    bool Update();
    bool Msg(MsgType mt);

private:

    BaseballPlayer* const       m_pChr;

};