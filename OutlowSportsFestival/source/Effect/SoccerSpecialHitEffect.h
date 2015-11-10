#pragma once

#include "../GameSystem/GameObject.h"
#include "../character/CharacterBase.h"
#include "../Render/BlurObject.h"


//------------------------------------------------//
//   サッカーの必殺技がヒットしたときのクラス
//------------------------------------------------//

class SoccerSpecialHit :public GameObjectBase
{
public:

    SoccerSpecialHit(
        CharacterBase* pOwner,//親キャラクタへのポインタ
        CrVector3      pos,   //出現座標
        CrVector3      vec,   //方向
        RATIO          level  //エフェクトのクオリティ(０～１)
        );

    ~SoccerSpecialHit();


private:
    CharacterBase*const  m_pOwner;
    void(SoccerSpecialHit::*m_pStateFunc)();

    BlurObjectSphere     m_BlurSphere;

    const Vector3        m_Pos;
    const Vector3        m_Vec;
    const RATIO          m_Level;
                         
    int                  m_Count;

    SoccerSpecialHit(const SoccerSpecialHit&);

    bool Update();
    bool Msg(MsgType mt);

    void EffectApper(int n);
    void Particle(int n);

    void State_Init();
    void State_BlurToSmal();
    void State_Impact();
    void State_Finish();
};
