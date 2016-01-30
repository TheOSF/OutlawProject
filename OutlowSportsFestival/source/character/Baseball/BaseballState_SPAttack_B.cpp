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


//攻撃力
const float BaseballState_SPAttack_B::m_sDamageValue = 45.0f;


BaseballState_SPAttack_B::BaseballState_SPAttack_B(BaseballPlayer* b) :
m_pChr(b),
m_pStateFunc(&BaseballState_SPAttack_B::State_PreAtk),
m_StateTimer(0),
m_BatScale(0.1f, 0.1f, 0.1f)
{
    {
        //ダメージ初期化
        m_Damage.m_Enable = false;
        m_Damage.AddSkillGaugeValue = 0.0f;
        m_Damage.m_Param.width = 1.5f;
        m_Damage.m_Vec = chr_func::GetFront(m_pChr);
        m_Damage.m_VecPower.x = 1.2f;
        m_Damage.m_VecPower.y = 0.5f;
        m_Damage.m_VecType = DamageCapsure::DamageVecType::MemberParam;
        m_Damage.pParent = m_pChr;
        m_Damage.type = DamageBase::Type::_VanishDamage;
        m_Damage.Value = m_sDamageValue;
    }


    //バットメッシュを作成
    m_pBatMesh = new MeshRenderer(
        DefResource.Get(Resource::MeshType::Bat),
        false,
        MeshRenderer::RenderType::UseColor
        );

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
}

// ステート終了
void BaseballState_SPAttack_B::Exit(BaseballPlayer* b)
{
    //スキルゲージをなくす
  //  chr_func::ResetSkillGauge(m_pChr);
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
    Vector3 Vec;
    Matrix T;

    //バットのボーン行列を取得
    m_pChr->getNowModeModel()->GetWorldBoneMatirx(T, 32);

    //バットの伸びるベクトル
    Vec = -Vector3(T._31, T._32, T._33);
    Vec.Normalize();

    //ダメージ位置をセット
    m_Damage.m_Param.pos1 = Vector3(T._41, T._42, T._43);
    m_Damage.m_Param.pos2 = m_Damage.m_Param.pos1 + Vec * 9.0f;

}


void BaseballState_SPAttack_B::OnHit()
{
    //SE
    Sound::Play(Sound::BaseBall_SP);

    //時間をとめる
    {
        std::list<LpGameObjectBase> UpdateObjList;

        DefGameObjMgr.FreezeOtherObjectUpdate(UpdateObjList, 20, true);
    }

    //エフェクト

    
}



void BaseballState_SPAttack_B::State_PreAtk()
{
    const int AllFrame = 50;
    const int BatScaleUpFrame = 35;

    const Vector3 batScaleTarget(0.2f, 0.2f, 0.2f);

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

