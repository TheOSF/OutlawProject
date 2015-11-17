#pragma once

#include "../GameSystem/GameObject.h"
#include "../character/CharacterBase.h"
#include "../Render/BlurObject.h"
#include "../Damage/Damage.h"
#include "TornadoEffect.h"

//------------------------------------------------//
//   サッカーの必殺技がヒットしたときのクラス
//------------------------------------------------//

class SoccerSpecialHit :public GameObjectBase
{
public:

    SoccerSpecialHit(
        CharacterBase* pOwner,//親キャラクタへのポインタ
        CrVector3      pos,   //出現座標
        RATIO          level, //エフェクトのクオリティ(０～１)
        UINT           time
        );

    ~SoccerSpecialHit();


private:
    CharacterBase*const  m_pOwner;
    void(SoccerSpecialHit::*m_pStateFunc)();

    TornadoEffect*       m_pTornadoEffect;

    const Vector3        m_Pos;
    const RATIO          m_Level;
                         
    const int            m_LiveTime;
    int                  m_Count;

    DamageCapsure         m_Damage;

    SoccerSpecialHit(const SoccerSpecialHit&);

    bool Update();
    bool Msg(MsgType mt);

    void EffectApper(int n,RATIO scale);
    void Particle(int n);

    void State_Init();
    void State_BlurToSmal();
    void State_Impact();
    void State_Finish();
};
