#pragma once

#include "../Render/Renderer.h"
#include "../GameSystem/GameObject.h"


//-------------------------------------------------------
//                FIGHT!! 表示クラス
//-------------------------------------------------------

class FightUI :public UserInterfaceRenderer, public GameObjectBase
{
public:
    FightUI(float Z);
    ~FightUI();

private:
    const float  m_Z;
    iex2DObj*    m_pTexture;

    float        m_Count;
    bool         m_Delete;

    void CalcZ();
    //描画(自動的に呼ばれる)
    void Render();

    bool Update();	//falseを返すと消去する
    bool Msg(MsgType mt)
    {
        return false;
    }

    void FightRender(
        int    SizeX,
        int    SizeY,
        DWORD  dw_flg,
        RATIO  alpha,
        int    diffPosY
        );
};