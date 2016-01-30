#pragma once

#include "CharacterBase.h"

//--------------------------------------------------------------------
//		キャラクター共通 姿勢制御ダメージを受けたときの更新クラス
//--------------------------------------------------------------------

class CharacterDamageControll
{
public:
    //操作クラス
    class EventClass
    {
    public:
        CharacterBase* const pOwner;            //操るキャラクタへのポインタ
        int            const VanishMotion;      //吹き飛びモーションの番号
        CharacterRenderer* const pRenderer;     //キャラクタレンダラー

        EventClass(CharacterBase*  pOwner, int VanishMotion, CharacterRenderer* pRenderer);

        virtual ~EventClass();
        virtual void SetVanishState(CrVector3 v) = 0;   //吹き飛びステートに移行する
    };

    CharacterDamageControll(EventClass* pEventClass);
    ~CharacterDamageControll();

    //姿勢行列をセット
    void SetTransform(const Matrix& m);

    //操作解除(通常の吹き飛びステートに移行する)
    void ToFree();
    void ToFree(CrVector3 Vec);

    //ダメージを与える(引数：１　ダメージ量　：２　体力を１残すかどうか)
    void AddDamage(float value, bool dontDie);

    //毎フレームの更新
    void Update();

public:
    EventClass* const       m_pEventClass;
    Matrix                  m_Transform;

    Vector3                 m_Move;
    bool                    m_Init;

    RATIO                   m_DamageShockCount;

    int                     m_NotControllCount;
};