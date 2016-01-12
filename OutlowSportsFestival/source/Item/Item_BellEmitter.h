#pragma once

#include "../GameSystem/GameObject.h"
#include "../Render/Renderer.h"
#include "../Camera/Camera.h"

//---------------------------------------------------------//
//  �x���C�x���g���o��������N���X
//---------------------------------------------------------//

/*
    �o������

    �͂��߂̃��E���h�łȂ�               &
    �O�ɏo�Ă����莞�Ԉȏ�o�߂����B   &
    �����Ă���v���C���[���R�l�ȏ�
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