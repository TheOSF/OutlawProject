#include "CharacterDamageVanish.h"
#include "CharacterFunction.h"
#include "CharacterBase.h"

#include "../Effect/EffectFactory.h"
#include "../Sound/Sound.h"
#include "../Collision/Collision.h"
#include "../Camera/Camera.h"


CharacterDamageVanish::Param::Param() :
move(Vector3AxisY),
rotate_speed(Vector3Zero),
down_frame(0),
standup_frame(0)
{

}

CharacterDamageVanish::CharacterDamageVanish(
    CharacterBase*    pCharacter,   //吹き飛ぶキャラクタ
    const Param&      param,        //吹きとびパラメーター
    Event*            pEvent,       //イベントクラスへのポインタ(デストラクタでdeleteする)
    DamageManager::HitEventBase* pHitEvent
    ) :
    m_pCharacter(pCharacter),
    m_pEvent(pEvent),
    m_pStateFunc(&CharacterDamageVanish::Initialize),
    m_Count(0),
    m_Rotate(0, 0, 0),
    m_pHitEvent(pHitEvent),
    m_WallHit(false),
    m_FirstSlow(false),
    m_VanishAngle(PI*0.25f),
    m_Locus(15)
{
    m_Param = param;

    {
        m_Locus.m_Visible = false;  //////////

        m_Locus.m_StartParam.Color = Vector4(1, 1, 1, 1);
        m_Locus.m_StartParam.HDRColor = Vector4(0,0,0,0);
        m_Locus.m_StartParam.Width = 1.0f;

        m_Locus.m_EndParam.Color = Vector4(1, 1, 1, 0.5f);
        m_Locus.m_EndParam.HDRColor = Vector4(0, 0, 0, 0);
        m_Locus.m_EndParam.Width = 0.1f;
    }
}


CharacterDamageVanish::~CharacterDamageVanish()
{
    delete m_pEvent;
    delete m_pHitEvent;
}


// 更新
void CharacterDamageVanish::Update()
{
    (this->*m_pStateFunc)();
}


void CharacterDamageVanish::Initialize()
{
    //キャラクタをダメージの方向に向かせる
    chr_func::AngleControll(
        m_pCharacter,
        m_pCharacter->m_Params.pos - m_Param.move
        );

    //移動方向を設定
    m_pCharacter->m_Params.move = m_Param.move;

    //スタートイベント呼び出し
    m_pEvent->FlyStart();

    //吹き飛びステートへ
    m_pStateFunc = &CharacterDamageVanish::Flying;

    //移動更新
    chr_func::UpdateAll(m_pCharacter, &DamageManager::HitEventBase());

    //サウンド
    Sound::Play(Sound::Damage2);

    
}

void CharacterDamageVanish::Flying()
{
    const float WallRefrectXZmoveValue = 0.2f;

    ++m_Count;

    //エフェクト
    {
        EffectFactory::Smoke(
            m_pCharacter->m_Params.pos + Vector3(0, 2.5f, 0) + Vector3Rand()*0.2f,
            Vector3Zero,
            1.5f + frand()*0.5f, 
            0.1f
            );
    }

    {
        //飛行中の回転行列を作成する
        Matrix R;

        //{
        //    m_Rotate += m_Param.rotate_speed;

        //    D3DXMatrixRotationYawPitchRoll(
        //        &R,
        //        m_Rotate.y, m_Rotate.x, m_Rotate.z
        //        );
        //}

        {
            float angle = Vector3Radian(m_pCharacter->m_Params.move, -chr_func::GetFront(m_pCharacter));

            if (m_pCharacter->m_Params.move.y < 0)
            {
                angle = -angle;
            }
            else
            {
                angle = 0.0f;
            }

            m_VanishAngle += (angle - m_VanishAngle)*0.1f;

            D3DXMatrixRotationX(&R, m_VanishAngle);
        }
        //吹き飛び中関数呼び出し
        m_pEvent->Flying(R, 1);
    }


    //地面についていた場合はステート移行
    if (chr_func::isTouchGround(m_pCharacter) &&
        m_pCharacter->m_Params.move.y <= 0)
    {
        m_pEvent->DownStart();
        m_pStateFunc = &CharacterDamageVanish::Dowing;
        m_Count = 0;

        //ズザー音
        Sound::Play(Sound::Sand2);
    }

    //壁についていた場合は反転
    if (m_WallHit == false && Vector3XZLength(m_pCharacter->m_Params.move) > WallRefrectXZmoveValue)
    {
        Vector3 out, pos(m_pCharacter->m_Params.pos), vec(m_pCharacter->m_Params.move);
        float dist = 10.0f;
        int material;

        vec.y = 0;
        vec.Normalize();

        pos.y = 3.0f; //高度は考慮しない

        if (DefCollisionMgr.RayPick(
            &out,
            &pos,
            &vec,
            &dist,
            &material,
            CollisionManager::RayType::_Character
            ) != nullptr)
        {
            //最近傍点を算出
            Vector3 ToChr = (m_pCharacter->m_Params.pos - out);

            ToChr.y = 0.0f;
            vec.Normalize();

            //壁から一定距離以下の場合
            if (Vector3Dot(ToChr, vec) < m_pCharacter->m_Params.move.Length() + m_pCharacter->m_Params.size)
            {
                m_Count = 0;

                m_pCharacter->m_Params.move = Vector3Refrect(m_pCharacter->m_Params.move, vec)*0.25f;
                chr_func::AngleControll(m_pCharacter, m_pCharacter->m_Params.pos + m_pCharacter->m_Params.move);

                m_pStateFunc = &CharacterDamageVanish::HitWallAndDown;
                m_WallHit = true;

                //エフェクト
                DefCamera.SetShock(Vector2(0.3f, 0.3f), 15);

                {
                    Vector3 Pos = out;
                    Pos.y = m_pCharacter->m_Params.pos.y + 3.0f;

                    EffectFactory::CircleAnimation(
                        Pos,
                        vec,
                        Vector3Zero,
                        Vector3Zero,
                        Vector2(8, 8), 
                        0xFFFFFFFF,
                        0x80FFFFFF
                        );
                }
            }
        }
    }

    {
        //移動更新
        chr_func::UpdateAll(m_pCharacter, m_pHitEvent);
    }

    //軌跡更新
    LocusUpdate(false);
}


void CharacterDamageVanish::Dowing()
{
    ++m_Count;

    chr_func::XZMoveDown(m_pCharacter, 0.08f);

    m_pEvent->Downing();


    //エフェクト
    if (m_Count > 4 && m_Count < 10)
    {
        EffectFactory::Smoke(
            m_pCharacter->m_Params.pos + Vector3(frand() - 0.5f, frand(), frand() - 0.5f)*2.0f,
            Vector3(frand() - 0.5f, frand()*0.1f, frand() - 0.5f)*0.05f, 
            2.0f + frand()*2.0f,
            1.0f,
            true
            );
    }

    //ステート移行
    if (m_Count > m_Param.down_frame)
    {
        m_pEvent->StandUpStart();
        m_pStateFunc = &CharacterDamageVanish::StandUping;
        m_Count = 0;
    }

    {
        //ノーダメージ版
        chr_func::UpdateAll(m_pCharacter, &DamageManager::HitEventBase());
    }

    //軌跡更新
    LocusUpdate(true);
}


void CharacterDamageVanish::StandUping()
{
    ++m_Count;

    chr_func::XZMoveDown(m_pCharacter, 0.08f);

    m_pEvent->StandUping();


    //起き上がり途中は他のアクションでキャンセル可能
    m_pEvent->CanActionUpdate();

    //ステート移行
    if (m_Count > m_Param.standup_frame)
    {
        m_pEvent->End();
        m_pStateFunc = &CharacterDamageVanish::End;
        m_Count = 0;
    }

    {
        //ノーダメージ版
        chr_func::UpdateAll(m_pCharacter, &DamageManager::HitEventBase());
    }

    //軌跡更新
    LocusUpdate(true);
}


void CharacterDamageVanish::End()
{

    if (m_Count == 0)
    {
        m_Count = 1;
        m_pEvent->End();
    }

    //移動更新
    {
        chr_func::UpdateAll(m_pCharacter, m_pHitEvent);
    }

}

void CharacterDamageVanish::LocusUpdate(bool FadeOut)
{
    Vector3 Pos, Vec;

    Pos = m_pCharacter->m_Params.pos;
    Pos += Vector3(0, 2, 0);

    Vector3Cross(Vec, m_pCharacter->m_Params.move, DefCamera.GetForward());
    Vec.Normalize();

    m_Locus.AddPoint(Pos, Vec);

    if (FadeOut)
    {
        m_Locus.m_StartParam.Color.w *= 0.9f;
        m_Locus.m_EndParam.Color.w *= 0.9f;
    }
}

void CharacterDamageVanish::HitWallAndDown()
{
    const int DownFrame = 18;

    if (m_Count == 0)
    {
        m_Count = 1;
        m_pEvent->HitWall();
    }

    //エフェクト
    {
        EffectFactory::Smoke(
            m_pCharacter->m_Params.pos + Vector3(0, 2.5f, 0) + Vector3Rand()*0.2f,
            Vector3Zero,
            1.5f + frand()*0.5f,
            0.1f
            );
    }

    //落下フレームなら
    if (m_Count >= DownFrame)
    {
        //重力加算
        chr_func::UpdateMoveY(m_pCharacter);

        //床に当たっていたなら
        if (chr_func::isTouchGround(m_pCharacter))
        {
            m_Count = 0;
            m_pStateFunc = &CharacterDamageVanish::HitFloorAndStandUp;
        }
    }
    else
    {
        ++m_Count;
    }

    //位置を更新
    chr_func::PositionUpdate(m_pCharacter);

    //壁との接触判定
    chr_func::CheckWall(m_pCharacter);

    //床との接触判定
    chr_func::CheckGround(m_pCharacter);

    //あたり判定を取る
    chr_func::DamageCheck(m_pCharacter, &DamageManager::HitEventBase());


    //イベント更新
    m_pEvent->HitWallUpdate();

    //軌跡更新
    LocusUpdate(false);
}



void CharacterDamageVanish::HitFloorAndStandUp()
{
    const int StandUpStart = 10;
    const int StandUpEnd = 50;

    //床に当たったイベント
    if (m_Count == 0)
    {
        m_pEvent->HitFloor();
    }

    //エフェクト
    if (m_Count < StandUpStart)
    {
        EffectFactory::Smoke(
            m_pCharacter->m_Params.pos + Vector3(frand() - 0.5f, frand(), frand() - 0.5f)*2.0f,
            Vector3(frand() - 0.5f, frand()*0.1f, frand() - 0.5f)*0.05f,
            2.0f + frand()*2.0f,
            1.0f,
            true
            );
    }
    

    //立ち上がりイベント
    if (m_Count == StandUpStart)
    {
        m_pEvent->HitFloorAndStandUp();
    }

    //立ち上がり終了イベント
    if (m_Count == StandUpEnd)
    {
        m_pEvent->End();
    }

    //カウンター更新
    if (m_Count <= StandUpEnd)
    {
        ++m_Count;
    }

    //起き上がり途中は他のアクションでキャンセル可能
    m_pEvent->CanActionUpdate();
    
    //移動更新
    {
        chr_func::XZMoveDown(m_pCharacter, 0.15f);
        chr_func::UpdateAll(m_pCharacter, m_pHitEvent);
    }

    //イベント更新
    m_pEvent->HitWallUpdate();

    //軌跡更新
    LocusUpdate(true);
}