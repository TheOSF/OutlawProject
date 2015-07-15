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

        UINT    standup_frame;//着地してから操作可能になるまでの時間
	};

    class Event
    {
    public:
        virtual ~Event() {}
        virtual void Flying(const Matrix& Rotate) = 0;//飛んでいる最中に呼ばれる関数
        virtual void StandUpStart() = 0;              //立ち上がりスタート時に呼ばれる関数
        virtual void StandUping() = 0;                //立ち上がっている最中に呼ばれる関数
        virtual void Start() = 0;                     //吹き飛びスタート時に呼ばれる関数
        virtual void End() = 0;                       //立ち上がり終了時に呼ばれる関数
    };

    CharacterDamageVanish(
        CharacterBase*    pCharacter,//吹き飛ぶキャラクタ
        const Param&      param, //吹きとびパラメーター
        Event*            pEvent //イベントクラスへのポインタ(デストラクタでdeleteする)
        );

    ~CharacterDamageVanish();

    // 更新
    void Update();

private:

    CharacterBase*  m_pCharacter;
    Param           m_Param;
    Vector3         m_Rotate;
    bool            m_Start;
    bool            m_End;
    Event*          m_pEvent;      // イベント
    UINT            m_Count;
    void(CharacterDamageVanish::*m_pStateFunc)();


    void Flying();
    void StandUp();
};

