#pragma once

#include "../Render/Renderer.h"
#include "../GameSystem/GameObject.h"


//-------------------------------------------------------
//  ゲームセット表示クラス
//-------------------------------------------------------

class GameSetUI :public UserInterfaceRenderer, public GameObjectBase
{
public:
    GameSetUI();
    ~GameSetUI();

private:
    const float  m_Z;
    iex2DObj*    m_pTexture;

    float        m_Count;
    float        m_T;

    void CalcZ();
    //描画(自動的に呼ばれる)
    void Render();

    bool Update();	//falseを返すと消去する
    bool Msg(MsgType mt)
    {
        return false;
    }

};