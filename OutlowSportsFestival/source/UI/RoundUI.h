#pragma once

#include "../Render/Renderer.h"
#include "../GameSystem/GameObject.h"


//-------------------------------------------------------
//  ラウンド表示クラス
//-------------------------------------------------------

class RoundUI :public UserInterfaceRenderer, public GameObjectBase
{
public:
    RoundUI(UINT Round, float Z);
    ~RoundUI();

private:
    const float  m_Z;
    const UINT   m_Round;
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
    
    void RoundRender();
    void NumberRender(int num, int posX, int sizeX, float t);
};