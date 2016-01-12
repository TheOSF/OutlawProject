#pragma once

#include "../GameSystem/GameObject.h"

#include "../brendanimation/BlendAnimationMesh.h"
#include "SceneResult.h"

//-----------------------------------------------------------------------
//  画面の割れるオブジェクトクラス
//-----------------------------------------------------------------------

class BreakScreenObject :public GameObjectBase, public ForwardRenderer
{
public:
    BreakScreenObject(iex2DObj* pTexture);
    ~BreakScreenObject();


private:

    BlendAnimationMesh  m_BreakScreenModel;
    iex2DObj*   m_pTexture;
    int         m_Timer;
    float       m_Scale;
    float       m_Alpha;
    float       m_DiffPosY;
    float       m_MoveY;

    void CalcBreakScreenWorldMatrix(Matrix* T);


    bool Update();
    bool Msg(MsgType mt);


    void CalcZ();

    //描画(自動的に呼ばれる)
    void Render();
};