#pragma once

#include "../GameSystem/GameObject.h"
#include "Ball.h"

// �A���t�g�̃o�E���h����{�[��
class AmefootBoundBall : public GameObjectBase, public BallBase
{
public:
    AmefootBoundBall(
        CrVector3 position,
        CrVector3 first_move,

        );

    bool Update()override;
    bool Msg(MsgType mt)override;

};

