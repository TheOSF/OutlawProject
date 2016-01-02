#pragma once

#include "iextreme.h"
#include "../GameSystem/ForwardDecl.h"
#include "../Damage/Damage.h"
#include "../utillity/LocusHDR.h"

//***************************************************
//		キャラクター共通 吹き飛びひるみクラス
//***************************************************
class CharacterDamageVanish
{
public:
	struct Param
	{
        Param();

		Vector3 move;         //移動速度
		Vector3 rotate_speed; //各軸の回転速度

        int     down_frame;
        int     standup_frame;
	};

    class Event
    {
    public:
        virtual ~Event() {}
        virtual void FlyStart() = 0;                                //吹き飛びスタート
        virtual void Flying(const Matrix& Rotate, RATIO Speed) = 0; //吹き飛んでいる最中

        virtual void DownStart() = 0;                               //着地時
        virtual void Downing() = 0;                                 //根っころがっている最中

        virtual void StandUpStart() = 0;                            //起き上がりスタート
        virtual void StandUping() = 0;                              //起き上がり最中

        virtual void End() = 0;                                     //ステート終了

        virtual void HitWall(){}                                    //壁に当たった時の挙動
        virtual void HitFloor(){}                                   //壁に当たってからの床に当たった時に呼ばれる
        virtual void HitFloorAndStandUp(){}                         //床に当たってからの立ち上がり時に呼ばれる
        virtual void HitWallUpdate(){}                              //壁に当たった後に立ち上がるまで舞いフレーム呼ばれる
    
        virtual void CanActionUpdate(){}                            //行動分岐が可能なときに呼ばれる
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
    bool            m_WallHit;
    bool            m_FirstSlow;
    float           m_VanishAngle;
    LocusHDR        m_Locus;

    void(CharacterDamageVanish::*m_pStateFunc)();

    void Initialize();
    void Flying();
    void Dowing();
    void StandUping();
    void End();

    void HitWallAndDown();
    void HitFloorAndStandUp();

    void LocusUpdate(bool FadeOut);
};

