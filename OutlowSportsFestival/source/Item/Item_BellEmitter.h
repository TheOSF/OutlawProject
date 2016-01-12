#pragma once

#include "../GameSystem/GameObject.h"
#include "../Render/Renderer.h"
#include "../Camera/Camera.h"

//---------------------------------------------------------//
//  ベルイベントを出現させるクラス
//---------------------------------------------------------//

/*
    出現条件

    はじめのラウンドでない               &
    前に出てから一定時間以上経過した。   &
    生きているプレイヤーが３人以上
*/

class Item_BellEmitter :public GameObjectBase
{
public:

    Item_BellEmitter(UINT ApperFrame);
    ~Item_BellEmitter();

private:
    const int  m_ApperFrame;
    int        m_ApperCount;
    bool       m_FirstRound;
    

    bool isCanApper();

    bool Update();
    bool Msg(MsgType mt);
};