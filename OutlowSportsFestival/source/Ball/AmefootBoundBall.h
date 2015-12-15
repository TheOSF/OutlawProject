#pragma once

#include "../GameSystem/GameObject.h"
#include "Ball.h"

// アメフトのバウンドするボール
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

