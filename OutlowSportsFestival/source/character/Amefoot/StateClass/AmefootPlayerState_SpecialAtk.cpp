#include "AmefootPlayerState_SpecialAtk.h"

#include "../AmefootUsualHitEvent.h"
#include "../AmefootPlayerState.h"
#include "../../CharacterFunction.h"
#include "../../../Sound/Sound.h"

#include "../../../Effect/SpecialAttackEffect.h"
#include "../../../GameSystem/GameController.h"
#include "../../../Camera/Camera.h"
#include "../../../Collision/Collision.h"

#include "../../../utillity/DamageObject.h"
#include "../../../Effect/EffectFactory.h"
#include "../../../Effect/GlavityLocus.h"

#include "../../../Effect/BlurImpact.h"
#include "../../../Effect/ImpactLightObject.h"


const float AmefootPlayerState_SpecialAtk::m_DamageValue = 35.0f;

//--------------------------------------------------//
//  アメフト必殺技ステート
//--------------------------------------------------//

AmefootPlayerState_SpecialAtk::AmefootPlayerState_SpecialAtk(AmefootPlayer* pCharacter) :
m_pChr(pCharacter)
{
    SetState(&AmefootPlayerState_SpecialAtk::State_Pose);
}

AmefootPlayerState_SpecialAtk::~AmefootPlayerState_SpecialAtk()
{

}


void AmefootPlayerState_SpecialAtk::Enter(AmefootPlayer* pCharacter)
{
    //ダメージトランスフォームクラスの初期化
    m_pDamageTransform = new DamageControll_Transform();
    UpdateDamageTransform();

    class GetDamageTransform :public DamageControllVanish::GetDamageControllTransformClass
    {
        DamageControll_Transform* const pDamageControll_Transform;
    public:
        GetDamageTransform(DamageControll_Transform* const pDamageControll_Transform):
            pDamageControll_Transform(pDamageControll_Transform)
        {
        }

        DamageControll_Transform* Get()
        {
            return pDamageControll_Transform;
        }
    };

    //ダメージ初期化
    m_pDamage = new DamageControllVanish(
        new GetDamageTransform(m_pDamageTransform)
        );

    m_pDamage->pParent = m_pChr;
    m_pDamage->m_Param.size = 2.2f;
    m_pDamage->Value = 0.0f;
    m_pDamage->type = DamageBase::Type::_ControllDamage;

    UpdateDamage();

    chr_func::ResetSkillGauge(m_pChr);
}

void AmefootPlayerState_SpecialAtk::Execute(AmefootPlayer* pCharacter)
{
    (this->*m_pStateFunc)();
    
    //ダメージ位置更新
    UpdateDamage();
    UpdateDamageTransform();

    m_pChr->m_Renderer.Update(1);
    chr_func::CreateTransMatrix(m_pChr, &m_pChr->m_Renderer.m_TransMatrix);
}

void AmefootPlayerState_SpecialAtk::Exit(AmefootPlayer* pCharacter)
{
    if (m_pDamageTransform != nullptr)
    {
        m_pDamageTransform->m_Destroy = true;
    }

    delete m_pDamage;
}

//----------------------------PrivateFunc--------------------------------//

void AmefootPlayerState_SpecialAtk::SetState(StateFunc state)
{
    m_pStateFunc = state;
    m_Timer = 0;
}

Vector3 AmefootPlayerState_SpecialAtk::GetViewVec()
{
    Vector3 ret(0, 0, 0);

    if (m_pChr->m_PlayerInfo.player_type == PlayerType::_Player)
    {
        //コンピュータ操作ならスティック値を返す
        Vector2 stick = controller::GetStickValue(controller::stick::left, m_pChr->m_PlayerInfo.number);

        ret = DefCamera.GetForward()*stick.y + DefCamera.GetRight()*stick.x;
    }
    else
    {
        //コンピュータの場合は角度の浅い敵にロック
        CharacterBase* pTarget = nullptr;

        if (chr_func::CalcAtkTarget(
            m_pChr,
            PI,
            1000.0f,
            &pTarget
            ))
        {
            ret = pTarget->m_Params.pos - m_pChr->m_Params.pos;
        }
    }

    return ret;
}

bool AmefootPlayerState_SpecialAtk::isHitWall(float Dist)
{
    Vector3 out, pos(m_pChr->m_Params.pos + Vector3(0, 2, 0)) , vec;
    float dist = Dist;
    int Mat;

    chr_func::GetFront(m_pChr, &vec);

    return DefCollisionMgr.RayPick(
        &out,
        &pos,
        &vec,
        &dist,
        &Mat,
        CollisionManager::RayType::_Character
        ) != nullptr;

}

void AmefootPlayerState_SpecialAtk::UpdateDamage()
{
    if (m_pDamage == nullptr)
    {
        return;
    }

    //前にずらす
    chr_func::GetFront(m_pChr, &m_pDamage->m_Param.pos);
    m_pDamage->m_Param.pos *= 2.0f;

    //少し上に
    m_pDamage->m_Param.pos += Vector3(0, 3, 0);

    //キャラクタの座標をセット
    m_pDamage->m_Param.pos += m_pChr->m_Params.pos;
     
}

void AmefootPlayerState_SpecialAtk::UpdateDamageTransform()
{
    if (m_pDamageTransform == nullptr)
    {
        return;
    }

    Matrix* T = &m_pDamageTransform->m_Transform;
    Matrix W;

    //初期化
    D3DXMatrixIdentity(T);

    //縦に向ける(X 軸に 90 度)
    {
        D3DXMatrixRotationX(&W, D3DXToRadian(90));
        *T *= W;
    }

    //キャラクタの向いている方向に向ける(Ｙ軸)
    {
        D3DXMatrixRotationY(&W, m_pChr->m_Params.angle + PI);
        *T *= W;
    }

    //アメフトの少し前に座標をセット
    {
        Vector3 pos;

        chr_func::GetFront(m_pChr, &pos);
        pos *= 5.0f;

        pos += Vector3(0, 1.5f, 0);
        
        pos += m_pChr->m_Params.pos;
        
        T->_41 = pos.x;
        T->_42 = pos.y;
        T->_43 = pos.z;
    }

}

void AmefootPlayerState_SpecialAtk::Explode()
{
    //付いているキャラクタを離す
    m_pDamageTransform->AddDamage(m_DamageValue);
    m_pDamageTransform->m_Destroy = true;
    m_pDamageTransform = nullptr;

    //球状のダメージを生成
    DamageShpere* pDamage = new DamageShpere();
    
    pDamage->m_Param.pos = m_pChr->m_Params.pos;
    pDamage->m_Param.size = 8.0f;

    pDamage->m_Vec = -m_pChr->m_Params.pos;
    pDamage->m_Vec.y = 0.0f;
    pDamage->m_Vec.Normalize();
    pDamage->m_Vec.y = 0.9f;

    pDamage->m_VecPower = Vector2(0.8f, 0.75f);
    pDamage->m_VecType = DamageShpere::DamageVecType::MemberParam;

    pDamage->pParent = m_pChr;
    pDamage->Value = 0;

    new DamageObject(pDamage, 5);

    //SE
    Sound::Play(Sound::Explode);

    //エフェクト
    {
        Vector3  color(1.0f, 0.5f, 0);
        Vector3 power(0, -0.02f, 0);
        Vector3 pos = m_pChr->m_Params.pos + Vector3(0, 2, 0);
        Vector3 move;
        GlavityLocus* g;

        const Vector4
            stCol(color.x, color.y, color.z, 1.0f),
            endCol(color.x, color.y, color.z, 0.5f);

        const Vector4
            stHdCol(color.x, color.y, color.z, 1.0f),
            endHdCol(color.x, color.y, color.z, 0.5f);


        for (int i = 0; i < 50; ++i)
        {
            move = Vector3Rand() *2.0f;
            move *= frand();
            move.y *= 2.0f;
            move *= 0.75f;

            g = new GlavityLocus(
                pos, move, power, 8, 40 + rand() % 20
                );

            g->m_BoundRatio = 0.5f;
            g->m_CheckWall = false;

            g->m_Locus.m_StartParam.Color = stCol;
            g->m_Locus.m_EndParam.Color = endCol;

            g->m_Locus.m_StartParam.HDRColor = stHdCol;
            g->m_Locus.m_EndParam.HDRColor = endHdCol;

            g->m_Locus.m_StartParam.Width = 0.09f;
            g->m_Locus.m_EndParam.Width = 0.00f;

            // g->m_Locus.m_pTexture = DefResource.Get(Resource::TextureType::Locus1);
        }

        const float SmokeSize = 3.0f;
        Vector3 SmokePos;

        for (int i = 0; i < 20; ++i)
        {
            SmokePos = pos;

            SmokePos.x += (frand() - 0.5f)*5.0f;
            SmokePos.z += (frand() - 0.5f)*5.0f;

            SmokePos.y += frand()*3.5f + SmokeSize*0.5f;

            EffectFactory::Smoke(
                SmokePos,
                Vector3(frand() - 0.5f, 0.5f, frand() - 0.5f)*0.15f,
                SmokeSize,
                0.2f
                );
        }

        EffectFactory::CircleAnimation(
            pos + Vector3(0, 2, 0),
            Vector3AxisY,
            Vector3Zero,
            Vector3Zero,
            Vector2(20, 20),
            0x0,
            0xA0FFFFFF,
            1.25f
            );

        //ブラーエフェクト
        new BlurImpactSphere(
            pos,
            35,
            80,
            30
            );

        new ImpactLightObject(
            pos + Vector3(0, 3, 0),
            35,
            Vector3(0, 0.5f, 1)*1.0f,
            0.1f
            );


        //カメラのゆれ
        DefCamera.SetShock(Vector2(0.5f, 0.5f), 30);
    }
    
}


RADIAN AmefootPlayerState_SpecialAtk::GetControllAngleSpeed()const
{
    if (m_pChr->m_PlayerInfo.player_type == PlayerType::_Player)
    {
        return D3DXToRadian(3);
    }
    else
    {
        switch (m_pChr->m_PlayerInfo.strong_type)
        {
        case StrongType::_Weak:
            return D3DXToRadian(1);

        case StrongType::_Usual:
            return D3DXToRadian(2);

        case StrongType::_Strong:
            return D3DXToRadian(3);

        default:
            break;
        }
    }

    MyAssert(false, "対応していない強さのタイプ");

    return 0.0f;
}

void AmefootPlayerState_SpecialAtk::NoDamageUpdate()
{
    class ArmorHitEvent :public DamageManager::HitEventBase
    {
        AmefootPlayer* const  m_pAmefoot;
    public:
        bool isDoArmor;

        ArmorHitEvent(AmefootPlayer* pChr) :
            m_pAmefoot(pChr),
            isDoArmor(false)
        {

        }

        void Effect()
        {
            const Vector3 EffectPos = m_pAmefoot->m_Params.pos + Vector3(0, 2, 0);
            const Vector3 Glavity(0, -0.02f, 0);
            const COLORf  EffectColor(1.0f, 0, 0.8f, 1);

            //エフェクト
            new ImpactLightObject(
                EffectPos,
                45,
                Vector3(0, 0.4f, 1),
                0.1f
                );

            //ＳＥ
            Sound::Play(Sound::AtkHit1);

            for (int i = 0; i < 10; ++i)
            {
                EffectFactory::LocusParticle(
                    EffectPos,
                    (Vector3Rand() - chr_func::GetFront(m_pAmefoot))*1.5f,
                    Glavity,
                    0.1f,
                    8,
                    EffectColor,
                    EffectColor,
                    45,
                    0.1f,
                    0.2f
                    );
            }
        }

        bool Hit(DamageBase* pDmg)
        {
            //自分の作っているダメージだった場合は何もしない
            if (pDmg->pParent != nullptr &&
                pDmg->pParent->m_PlayerInfo.number == m_pAmefoot->m_PlayerInfo.number ||
                pDmg->isCanHitCharacter(m_pAmefoot) == false
                )
            {
                return false;
            }

            //pDmg->OnHitCharacter(m_pAmefoot);

            //エフェクト
            Effect();

            return true;
        }
    };

    chr_func::UpdateAll(m_pChr, &ArmorHitEvent(m_pChr));
}


// ダメージ判定がある状態の更新
void AmefootPlayerState_SpecialAtk::UsualDamageUpdate() {

    AmefootUsualHitEvent hitevent(m_pChr);
    chr_func::UpdateAll(m_pChr, &hitevent);

}

//----------------------------StateFunc----------------------------------//


void AmefootPlayerState_SpecialAtk::State_Pose()
{
    const int AllFrame = 55;

    //カウント進行
    ++m_Timer;

    //初期化
    if (m_Timer == 1)
    {
        //以前の移動値をリセット
        chr_func::ResetMove(m_pChr);

        //SE
        Sound::Play(Sound::Skill);

        //自分以外の時間をとめる
        std::list<GameObjectBase*> MoveList;
        MoveList.push_back(m_pChr);
        DefGameObjMgr.FreezeOtherObjectUpdate(MoveList, 55, true);

        //エフェクトクラス生成
        new SpecialAttackEffect(m_pChr, 55);

        //モーションセット
        m_pChr->m_Renderer.SetMotion(AmefootPlayer::Motion_Special_Attack_Start);
    }

    //時間経過で突進ステートに移行
    if (m_Timer > AllFrame)
    {
        SetState(&AmefootPlayerState_SpecialAtk::State_Dush);
    }

    //更新
    NoDamageUpdate();
}

void AmefootPlayerState_SpecialAtk::State_Dush()
{

    const int       AllFrame = 180;
    const RADIAN    AngleSpeed = GetControllAngleSpeed();
    const float     MoveSpeed = 0.5f;

    //カウント進行
    ++m_Timer;

    //初期化
    if (m_Timer == 1)
    {
        //モーションセット
        m_pChr->m_Renderer.SetMotion(AmefootPlayer::Motion_Special_Attack_Running);
    }


    //方向制御
    {
        Vector3 ViewVec = GetViewVec();

        chr_func::AngleControll(m_pChr, m_pChr->m_Params.pos + ViewVec, AngleSpeed);
    }

    //現在の向いている方向に移動値をセット
    {
        chr_func::GetFront(m_pChr, &m_pChr->m_Params.move);

        m_pChr->m_Params.move *= MoveSpeed;
    }

    //敵をつかんでいたらつかみダッシュステートへ
    if (m_pDamage->HitCount > 0)
    {
        SetState(&AmefootPlayerState_SpecialAtk::State_HangDush);

        //更新
        NoDamageUpdate();

        return;
    }

    //壁に当たっていたら or 時間経過で失敗ステートに移行
    if (isHitWall(5.0f) || (m_Timer > AllFrame))
    {
        m_pDamage->m_Enable = false;
        m_pDamageTransform->m_Destroy = true;
        m_pDamageTransform =  nullptr;

        SetState(&AmefootPlayerState_SpecialAtk::State_DushEnd);
    }

    //更新
    NoDamageUpdate();
}


void AmefootPlayerState_SpecialAtk::State_DushEnd()
{
    const int       AllFrame = 48;
    const float     MoveDownSpeed = 0.15f;

    //カウント進行
    ++m_Timer;

    //初期化
    if (m_Timer == 1)
    {
        //モーションセット
        m_pChr->m_Renderer.SetMotion(AmefootPlayer::Motion_Touchdown_Failed);
    }

    //移動量を落とす
    {
        chr_func::XZMoveDown(m_pChr, MoveDownSpeed);
    }

    //時間経過で終了ステートへ
    if (m_Timer > AllFrame)
    {
        SetState(&AmefootPlayerState_SpecialAtk::State_Finish);
    }

    //更新
    UsualDamageUpdate();
}

void AmefootPlayerState_SpecialAtk::State_HangDush()
{
    const float MoveSpeed = 0.6f;

    //カウント進行
    ++m_Timer;

    //初期化
    if (m_Timer == 1)
    {
        //モーションセット
        m_pChr->m_Renderer.SetMotion(AmefootPlayer::Motion_Special_Attack_Running);
    }

    //現在の向いている方向に移動値をセット
    {
        chr_func::GetFront(m_pChr, &m_pChr->m_Params.move);

        m_pChr->m_Params.move *= MoveSpeed;
    }

    //壁に当たっていたら壁あたりステートに移行
    if (isHitWall(10.0f))
    {
        SetState(&AmefootPlayerState_SpecialAtk::State_WallHit);
    }

    //更新
    NoDamageUpdate();
}

void AmefootPlayerState_SpecialAtk::State_WallHit()
{
    const int       AllFrame = 48;
    const int       ExplodeFrame = 18;
    const float     MoveDownSpeed = 0.2f;
    const int       AcceFrame = 15;

    //カウント進行
    ++m_Timer;

    //初期化
    if (m_Timer == 1)
    {
        //モーションセット
        m_pChr->m_Renderer.SetMotion(AmefootPlayer::Motion_Special_Attack_Finish);
    }

    //加速
    if (m_Timer == AcceFrame)
    {
        chr_func::AddMoveFront(m_pChr, 0.6f, 1.0f);
    }


    //爆発！
    if (m_Timer == ExplodeFrame)
    {
        Explode();
    }

    //移動量を落とす
    {
        chr_func::XZMoveDown(m_pChr, MoveDownSpeed);
    }

    //時間経過で終了ステートへ
    if (m_Timer > AllFrame)
    {
        SetState(&AmefootPlayerState_SpecialAtk::State_Finish);
    }

    //更新
    UsualDamageUpdate();
}

void AmefootPlayerState_SpecialAtk::State_Finish()
{
    m_pChr->SetState(AmefootPlayerState_UsualMove::GetPlayerControllMove(m_pChr));
}