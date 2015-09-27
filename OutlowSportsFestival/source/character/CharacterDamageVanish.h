#pragma once

#include "iextreme.h"
#include "../GameSystem/ForwardDecl.h"
#include "../Damage/Damage.h"

//***************************************************
//		キャラクター共通 吹き飛びひるみクラス
//***************************************************
class CharacterDamageVanish
{
public:
	struct Param
	{
		Vector3 move;         //移動速度
		Vector3 rotate_speed; //各軸の回転速度

        int     down_frame;
        int     down_muteki_frame;

        int     standup_frame;
        int     standup_muteki_frame;
	};

    class Event
    {
    public:
        virtual ~Event() {}
        virtual void FlyStart() = 0;                     //吹き飛びスタート
        virtual void Flying(const Matrix& Rotate) = 0;   //吹き飛んでいる最中
        virtual void DownStart() = 0;                         //着地時
        virtual void Downing() = 0;                      //根っころがっている最中
        virtual void StandUpStart() = 0;                 //起き上がりスタート
        virtual void StandUping() = 0;                  //起き上がり最中
        virtual void End() = 0;                          //ステート終了
    };

    CharacterDamageVanish(
        CharacterBase*    pCharacter,//吹き飛ぶキャラクタ
        const Param&      param, //吹きとびパラメーター
        Event*            pEvent, //イベントクラスへのポインタ(デストラクタでdeleteする)
        DamageManager::HitEventBase* pHitEvent //ヒットイベントクラスへのポインタ(デストラクタでdeleteする)
        );

    ~CharacterDamageVanish();

    // 更新
    void Update();

private:
    DamageManager::HitEventBase* m_pHitEvent;
    CharacterBase*  m_pCharacter;
    Param           m_Param;
    Vector3         m_Rotate;
    Event*          m_pEvent;      // イベント
    int             m_Count;

    void(CharacterDamageVanish::*m_pStateFunc)();

    void Initialize();
    void Flying();
    void Dowing();
    void StandUping();
    void End();
};

