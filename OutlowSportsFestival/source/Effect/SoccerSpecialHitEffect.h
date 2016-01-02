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

    TornadoEffect*              m_pTornadoEffect;

    const Vector3               m_Pos;
    const RATIO                 m_Level;
                                
    const int                   m_LiveTime;
    int                         m_Count;

    DamageControllVanish*       m_pDamage;

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


//-----------------------------------------------------//
//   サッカーの必殺技がヒットしたとき吹き飛び操作クラス
//-----------------------------------------------------//


class TornadoVanishControll :public  GameObjectBase
{
public:
    struct InitParam
    {
        Vector3 from;

        RADIAN  start_angle_seed;
        RADIAN  all_rotation_angle;
        
        float   start_width;
        float   end_width;

        float   height;

        RATIO   speed;

        float   damage;
    };

    TornadoVanishControll(
        const InitParam& param,
        DamageControll_Transform* pControll_Transform);

    ~TornadoVanishControll();

    bool Update();	//falseを返すと消去する
    bool Msg(MsgType mt);

private:
    const InitParam                 m_Param;
    DamageControll_Transform* const m_pControll_Transform;
    RATIO                           m_Count;
    int                             m_DamageCount;

    //移動更新
    bool Move();

    //姿勢行列を更新
    void SetTransform();

    //ダメージを加算
    void AddDamage(float value, bool dontDie);
};