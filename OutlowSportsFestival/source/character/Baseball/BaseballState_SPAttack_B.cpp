#include "BaseballState_SPAttack_B.h"
#include "BaseballPlayerState.h"
#include "Computer\/BaseballPlayerState_ComMove.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"

#include "../../Effect/GlavityLocus.h"
#include "../../Effect/BlurImpact.h"
#include "../../Effect/ThunderEffect.h"
#include "../../Camera/Camera.h"
#include "../../Sound/Sound.h"
#include "../../Effect/SpecialAttackEffect.h"
#include "../../GameSystem/ResourceManager.h"
#include "../../Effect/ImpactLightObject.h"
#include "../../Effect/EffectFactory.h"

//攻撃力
const float BaseballState_SPAttack_B::m_sDamageValue = 45.0f;


BaseballState_SPAttack_B::BaseballState_SPAttack_B(BaseballPlayer* b) :
m_pChr(b),
m_pStateFunc(&BaseballState_SPAttack_B::State_PreAtk),
m_StateTimer(0),
m_BatScale(0.1f, 0.1f, 0.1f),
m_ChrLiveCount(0),
m_DoOnHit(false),
m_Locus(8)
{
    {
        //ダメージ初期化
        m_Damage.m_Enable = false;
        m_Damage.AddSkillGaugeValue = 0.0f;
        m_Damage.m_Param.width = 3.5f;
        m_Damage.m_Vec = chr_func::GetFront(m_pChr);
        m_Damage.m_Vec.y = 0.8f;
        m_Damage.m_VecPower.x = 1.0f;
        m_Damage.m_VecPower.y = 1.5f;
        m_Damage.m_VecType = DamageCapsure::DamageVecType::MemberParam;
        m_Damage.pParent = m_pChr;
        m_Damage.type = DamageBase::Type::_VanishDamage;
        m_Damage.Value = m_sDamageValue;
    }

    
    {
        //軌跡初期化
        Vector4 LocusColor(1, 0.5f, 0, 1);

        m_Locus.m_Division = 3;

        m_Locus.m_StartParam.Color = LocusColor;
        m_Locus.m_StartParam.HDRColor = LocusColor;
        m_Locus.m_StartParam.Width = 3.5f;

        LocusColor.w = 0.0f;

        m_Locus.m_EndParam.Color = LocusColor;
        m_Locus.m_EndParam.HDRColor = LocusColor;
        m_Locus.m_EndParam.Width = 3.5f;
    }

    //バットメッシュを作成
    m_pBatMesh = new MeshRenderer(
        DefResource.Get(Resource::MeshType::Bat),
        false,
        MeshRenderer::RenderType::UseColor
        );

    m_pBatMesh->m_HDR = Vector3(1, 0.5f, 0);

    //初期更新
    UpdateBatMesh();
}


BaseballState_SPAttack_B::~BaseballState_SPAttack_B()
{
    delete m_pBatMesh;
}

// ステート開始
void  BaseballState_SPAttack_B::Enter(BaseballPlayer* b)
{
    //時間をとめる
    std::list<LpGameObjectBase> UpdateObjList;

    UpdateObjList.push_back(b);

    DefGameObjMgr.FreezeOtherObjectUpdate(UpdateObjList, 50, true);

    //エフェクト
    new SpecialAttackEffect(b, 50);

    //SE
    Sound::Play(Sound::Skill);

    //モーションをセット
    m_pChr->SetMotion(baseball_player::_mb_SpAtk_B);

    //移動値を消す
    chr_func::ResetMove(b);
}

// ステート実行
void BaseballState_SPAttack_B::Execute(BaseballPlayer* b)
{
    //カウント進行
    m_StateTimer++;

    //モデル更新
    b->ModelUpdate(2);

    //ステート実行
    (this->*m_pStateFunc)();

    //基本更新
    chr_func::UpdateAll_DontCheckDamage(b);
    chr_func::CreateTransMatrix(b, &b->getNowModeModel()->m_TransMatrix);

    //軌跡の更新
    AddLocusPoint();
}

// ステート終了
void BaseballState_SPAttack_B::Exit(BaseballPlayer* b)
{
    //スキルゲージをなくす
    chr_func::ResetSkillGauge(m_pChr);
}

//-------------------------------------------------------------------//
//                            private
//-------------------------------------------------------------------//

void BaseballState_SPAttack_B::SetState(StateFunc state)
{
    m_StateTimer = 0;
    m_pStateFunc = state;
}

void BaseballState_SPAttack_B::UpdateBatMesh()
{
    Matrix T;

    //バットをつけるボーン行列を取得
    m_pChr->getNowModeModel()->GetWorldBoneMatirx(T, 32);

    //Z軸が逆っぽいので反転(ごり押しだけど…
    {
        T._31 = -T._31;
        T._32 = -T._32;
        T._33 = -T._33;
    }

    //スケール成分を正規化
    D3DXMatrixWorldScaleNormalize(T);

    //バットのスケールにセット
    {
        T._11 *= m_BatScale.x;
        T._12 *= m_BatScale.x;
        T._13 *= m_BatScale.x;

        T._21 *= m_BatScale.y;
        T._22 *= m_BatScale.y;
        T._23 *= m_BatScale.y;

        T._31 *= m_BatScale.z;
        T._32 *= m_BatScale.z;
        T._33 *= m_BatScale.z;
    }

    //セット
    m_pBatMesh->SetMatrix(T);
}

void BaseballState_SPAttack_B::UpdateDamagePos()
{
    //Vector3 Vec;
    //Matrix T;

    ////バットのボーン行列を取得
    //m_pChr->getNowModeModel()->GetWorldBoneMatirx(T, 32);

    ////バットの伸びるベクトル
    //Vec = -Vector3(T._31, T._32, T._33);
    //Vec.Normalize();

    ////ダメージ位置をセット
    //m_Damage.m_Param.pos1 = Vector3(T._41, T._42, T._43);
    //m_Damage.m_Param.pos2 = m_Damage.m_Param.pos1 + Vec * 9.0f;

    Vector3 pos = m_pChr->m_Params.pos;

    pos += chr_func::GetFront(m_pChr)*3.0f;
    
    m_Damage.m_Param.pos2 = pos;

    pos += Vector3(0, 2, 0);

    m_Damage.m_Param.pos1 = pos;
}


void BaseballState_SPAttack_B::OnHit()
{
    //　カキーン
    Sound::Play(Sound::BaseBall_SP);

    Vector3 power(0, -0.02f, 0);
    GlavityLocus* g;

    const Vector4
        stCol(1, 1, 1, 0.0f),
        endCol(1, 1, 1, 0);

    const Vector4
        stHdCol(1, 1, 1, 0.5f),
        endHdCol(1, 1, 1, 0);

    const Vector3 move = Vector3Normalize(m_Damage.m_Vec)*0.8f;

    Vector3 pos = m_Damage.m_Param.pos1;


    //　エフェクト
    for (int i = 0; i < 60; ++i)
    {
        g = new GlavityLocus(
            pos, move*1.5f + Vector3Rand() * 0.8f, power, 4, 40 + rand() % 30
            );

        g->m_BoundRatio = 0.2f;
        g->m_CheckWall = false;

        g->m_Locus.m_StartParam.Color = stCol;
        g->m_Locus.m_EndParam.Color = endCol;

        g->m_Locus.m_StartParam.HDRColor = stHdCol;
        g->m_Locus.m_EndParam.HDRColor = endHdCol;

        g->m_Locus.m_StartParam.Width = 0.1f;
        g->m_Locus.m_EndParam.Width = 0.00f;

    }

    //　雷
    for (int i = 0; i < 8; i++)
    {
        new ThunderEffect
            (pos/* - move*0.2f*i,*/,
            pos + move * 10 + Vector3Rand() * 10,
            4.5f,
            0.1f,
            10,
            Vector4(1, 0.5f, 0, 0),
            15);
    }

    new ImpactLightObject(
        pos,
        50.0f,
        Vector3(0, 0.4f, 0.8f),
        0.1f
        );


    //　ブラー
    new BlurImpactSphere(
        pos,
        30,
        100,
        10
        );

    //　画面揺れ
    DefCamera.SetShock(Vector2(0.5f, 0.5f), 50);

    if (m_ChrLiveCount == DefCharacterMgr.GetCharacterLiveCount())
    {
        std::list<LpGameObjectBase> UpdateObjList;

        DefGameObjMgr.FreezeOtherObjectUpdate(UpdateObjList, 20);
    }
}

void BaseballState_SPAttack_B::AddLocusPoint()
{
    Vector3 pos1, pos2;
    Matrix T;

    //バットをつけるボーン行列を取得
    m_pChr->getNowModeModel()->GetWorldBoneMatirx(T, 32);

    //バットの根元
    pos1 = Vector3(T._41, T._42, T._43);

    //バットの先
    pos2 = pos1 + Vector3Normalize(-Vector3(T._31, T._32, T._33)) * 30.0f * m_BatScale.z;

    //点を追加
    m_Locus.AddPoint(
        (pos1 + pos2)*0.5f,
        Vector3Normalize(pos1 - pos2)
        );
}

void BaseballState_SPAttack_B::BallCounterUpdate(CrVector3 pos, float size)
{
    BallBase* pBall = nullptr;
    Vector3 OutMovePos;

    //カウンター可能なボールを取得
    if (DefBallMgr.GetCounterBall(&pBall, pos, &OutMovePos, size, 1, m_pChr) == false)
    {
        return;
    }

    //ボールのパラメタ更新
    pBall->m_Params.move = Vector3Normalize(pBall->m_Params.pos - m_pChr->m_Params.pos) * 1.2f;
    pBall->m_Params.move.y = 0.0f;

    pBall->Counter(m_pChr);

    //エフェクト
    EffectFactory::CircleAnimationBillbord(
        pBall->m_Params.pos,
        Vector3Zero,
        Vector3Zero,
        Vector2(5,5),
        0xFFFFFFFF,
        RS_ADD
        );

    //SE
    Sound::Play(Sound::AtkHit2);
}

void BaseballState_SPAttack_B::State_PreAtk()
{
    const int AllFrame = 50;
    const int BatScaleUpFrame = 35;

    const Vector3 batScaleTarget(0.125f, 0.125f, 0.125f);

    //腕にバットをセット
    UpdateBatMesh();

    //バットを大きくする
    if (m_StateTimer > BatScaleUpFrame)
    {
        m_BatScale += (batScaleTarget - m_BatScale)*0.15f;
    }

    //フレーム経過で攻撃ステートへ
    if (m_StateTimer > AllFrame)
    {
        //生存人数を保存
        m_ChrLiveCount = DefCharacterMgr.GetCharacterLiveCount();

        SetState(&BaseballState_SPAttack_B::State_Atk);
    }
}

void BaseballState_SPAttack_B::State_Atk()
{
    const int AllFrame = 20;
    const int BatSmallFrame = 5;
    const int DamageEndFrame = 5;


    //ダメージ位置をセット
    UpdateDamagePos();

    //腕にバットをセット
    UpdateBatMesh();

    //ダメージの有効・無効をセット
    m_Damage.m_Enable = (m_StateTimer < DamageEndFrame);

    //ボールのカウンターする
    if (m_Damage.m_Enable)
    {
        BallCounterUpdate(m_Damage.m_Param.pos1, 4.0f);
    }

    //ヒットした場合エフェクト関数を呼ぶ
    if (m_Damage.HitCount > 0 && m_DoOnHit == false)
    {
        m_DoOnHit = true;
        OnHit();
    }

    //バットを小さくする
    if (m_StateTimer > BatSmallFrame)
    {
        m_BatScale *= 0.85f;
    }

    //フレーム経過で通常ステートへ
    if (m_StateTimer > AllFrame)
    {
        m_pChr->SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(m_pChr));
    }
}

