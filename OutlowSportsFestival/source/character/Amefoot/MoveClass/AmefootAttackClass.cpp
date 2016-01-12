#include "AmefootAttackClass.h"
#include "../../CharacterFunction.h"
#include "../AmefootUsualHitEvent.h"
#include "../../../Effect/LocusBarObject.h"
#include "../AmefootPlayer.h"
#include "../AmefootPlayerState.h"
#include "../../CharacterManager.h"
#include "../../../GameSystem/GameController.h"
#include "../../../Camera/Camera.h"



AmefootAttackClass::DamageHitCallback::DamageHitCallback(AmefootAttackClass* pAmefootAttackClass) :
    m_pOwner(pAmefootAttackClass)
{}

//ダメージが当たったときに自動で呼ばれる
void AmefootAttackClass::DamageHitCallback::Hit(const SphereParam* sp)
{
    if ( m_pOwner->m_NowStateType == AmefootAttackClass::_Tackle )
    {
        m_pOwner->m_NextStateType = AmefootAttackClass::_DriveAway;
        m_pOwner->m_HitStopFrame = 6;
        m_pOwner->SetState(AmefootAttackClass::_HitStop);
        m_pOwner->m_Damage.m_Enable = false;
        return;
    }

    if ( m_pOwner->m_NowStateType == AmefootAttackClass::_DriveAway )
    {
        m_pOwner->m_NextStateType = AmefootAttackClass::_Jump;
        m_pOwner->m_HitStopFrame = 10;
        m_pOwner->SetState(AmefootAttackClass::_HitStop);
        m_pOwner->m_Damage.m_Enable = false;
        return;
    }
}

//キャラクタに当たったときに呼ばれる
void AmefootAttackClass::DamageHitCallback::HitCharacter(CharacterBase* pHitCharacter)
{
    // 来てない！
}



const CharacterBase* GetFrontTargetEnemy(AmefootPlayer* pAmefoot)
{
    CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();

    const float  AutoDistance = 10.0f;               //自動ができる最大距離
    const RADIAN AutoMaxAngle = D3DXToRadian(90);   //自動ができる最大角度

    const CharacterBase* pTargetEnemy = nullptr;    //ターゲット保持のポインタ
    RADIAN MostMinAngle = PI;                       //もっとも狭い角度
    RADIAN TempAngle;

    Vector3 MyFront;      //自身の前方ベクトル
    chr_func::GetFront(pAmefoot, &MyFront);

    auto it = ChrMap.begin();

    while ( it != ChrMap.end() )
    {
        //自身を除外
        if ( pAmefoot->m_PlayerInfo.number == it->first->m_PlayerInfo.number ||
            chr_func::isDie(it->first)
            )
        {
            ++it;
            continue;
        }

        //距離が一定以上のキャラクタを除外する
        if ( Vector3Distance(it->first->m_Params.pos, pAmefoot->m_Params.pos) > AutoDistance )
        {
            it = ChrMap.erase(it);
            continue;
        }

        //前ベクトルと敵へのベクトルの角度を計算する
        TempAngle = Vector3Radian(MyFront, (it->first->m_Params.pos - pAmefoot->m_Params.pos));

        //角度が一番狭かったら更新
        if ( TempAngle < MostMinAngle )
        {
            pTargetEnemy = it->first;
            MostMinAngle = TempAngle;
        }

        ++it;
    }

    return pTargetEnemy;

}


AmefootAttackClass::AmefootAttackClass(
    AmefootPlayer* const pAmefootPlayer,
    Control* pControl
    ) :
    m_pAmefootPlayer(pAmefootPlayer),
    m_pControl(pControl),
    m_NowStateType(StateType::_Pose),
    m_pNowState(&AmefootAttackClass::Pose),
    m_Damage(),
    m_pControllDamage(nullptr)
{
    m_Damage.m_Enable = false;
    m_Damage.m_Param.pos = m_pAmefootPlayer->m_Params.pos;
    m_Damage.m_Param.size = 2.8f;
    m_Damage.pParent = m_pAmefootPlayer;
    m_Damage.pCallBackClass = new DamageHitCallback(this);

}


AmefootAttackClass::~AmefootAttackClass()
{
    delete m_pControl;
}


// 更新
void AmefootAttackClass::Update()
{
    StateUpdate();

    chr_func::CreateTransMatrix(
        m_pAmefootPlayer,
        &m_pAmefootPlayer->m_Renderer.m_TransMatrix
        );

    UpdateDamageTransform();

    m_pAmefootPlayer->m_Renderer.Update(1);
}


// ステート切り替え
void AmefootAttackClass::SetState(StateType type)
{
    switch ( type )
    {
    case _Pose:m_pNowState = &AmefootAttackClass::Pose; break;
    case _Tackle:m_pNowState = &AmefootAttackClass::Tackle; break;
    case _DriveAway:m_pNowState = &AmefootAttackClass::DriveAway; break;
    case _Jump:m_pNowState = &AmefootAttackClass::Jump; break;
    case _Touchdown:m_pNowState = &AmefootAttackClass::Touchdown; break;
    case _Standup:m_pNowState = &AmefootAttackClass::Standup; break;
    case _TackleFailed:m_pNowState = &AmefootAttackClass::Failed; break;
    case _End:m_pNowState = &AmefootAttackClass::End; break;
    case _HitStop:m_pNowState = &AmefootAttackClass::HitStop; break;
    default:m_pNowState = nullptr; return;
    }
    m_NowStateType = type;
    m_Timer = 0;
    m_Damage.m_Enable = false;
}


// ステート更新
void AmefootAttackClass::StateUpdate()
{
    if ( m_pNowState != nullptr )
    {
        (this->*m_pNowState)();
    }
}


// 構え中
void AmefootAttackClass::Pose()
{
    const int kAllFrame = 15; // 構えフレーム
    const int kAdditionalFrame = 60; // 延長できるフレーム
    const RADIAN kAngleControlSpeed = 0.04f; // 角度補正スピード
    bool go_next = false; // ステート移行するかどうか
    const float kMoveDownSpeed = 0.2f;

    // 開始
    if ( m_Timer == 0 ) { PoseStart(); }
    ++m_Timer;

    chr_func::XZMoveDown(m_pAmefootPlayer, kMoveDownSpeed);

    const CharacterBase* const pTargetCharacter = GetFrontTargetEnemy(m_pAmefootPlayer);

    if ( pTargetCharacter != nullptr )
    {
        //自動回転
        chr_func::AngleControll(m_pAmefootPlayer, pTargetCharacter->m_Params.pos, kAngleControlSpeed);
    }
    else
    {
        const Vector2 Stick = m_pControl->GetStickValue();

        //スティックが一定以上倒されているかどうか
        if ( Vector2Length(Stick) > 0.25f )
        {
            Vector3 Vec;

            //スティック値をカメラ空間に
            Vec = DefCamera.GetForward()*Stick.y + DefCamera.GetRight()*Stick.x;

            //キャラクタ回転
            chr_func::AngleControll(m_pAmefootPlayer, m_pAmefootPlayer->m_Params.pos + Vec, kAngleControlSpeed);
        }
    }


    // ボタンを離したら次のステートへ
    if ( m_Timer >= kAllFrame )
    {
        if ( !m_pControl->isPoseContinue() )
        {
            //タックルパワーを溜めた時間から計算
            m_TacklePower = (float)(m_Timer - kAllFrame) / (float)kAdditionalFrame;
            go_next = true;
        }
    }

    // 最大フレームを超えたら次のステートへ
    if ( m_Timer >= (kAllFrame + kAdditionalFrame) )
    {
        //タックルパワー最大！
        m_TacklePower = 1.0f;
        go_next = true;
    }

    // 次のステートへ
    if ( go_next )
    {

        SetState(_Tackle);
    }

    // キャラクターの更新
    CharacterUsualUpdate();
}


// タックル中
void AmefootAttackClass::Tackle()
{
    const float kDamagePosOffset = 4.0f;
    const RADIAN kAngleControlSpeed = 0.04f; // 角度補正スピード

    int kAllFrame = 0;
    float DamageValue = 0.0f;
    float MoveValue = 0.0f;

    //パラメータ取得
    GetTackleParamByPower(m_TacklePower, &kAllFrame, &MoveValue, &DamageValue);


    // 開始
    if (m_Timer == 0) { TackleStart(MoveValue, DamageValue); }
    ++m_Timer;

    const CharacterBase * const pTargetCharacter = GetFrontTargetEnemy(m_pAmefootPlayer);
    if ( pTargetCharacter )
    {
        //自動回転
        chr_func::AngleControll(m_pAmefootPlayer, pTargetCharacter->m_Params.pos, kAngleControlSpeed);
    }

    // ダメージ座標設定
    m_Damage.m_Param.pos = m_pAmefootPlayer->m_Params.pos + chr_func::GetFront(m_pAmefootPlayer) * kDamagePosOffset;

    //chr_func::XZMoveDown(m_pAmefootPlayer, kMoveDownSpeed);

    // フレーム経過
    if ( m_Timer >= kAllFrame )
    {
        SetState(_TackleFailed);
    }

    // キャラクターの更新
    CharacterNoDamageUpdate();
}


// タックル打ち上げ中
void AmefootAttackClass::DriveAway()
{
    const int kTackleToDriveAwayFrame = 6;
    const int kAllFrame = 15;
    const float kMoveDownSpeed = 0.2f;
    const float kDamagePosOffset = 4.0f;

    // 開始
    if ( m_Timer == kTackleToDriveAwayFrame )
    {
        DrivaAwayStart();
    }
    else
    {
        chr_func::XZMoveDown(m_pAmefootPlayer, kMoveDownSpeed);
    }

    ++m_Timer;

    // ダメージ座標設定
    m_Damage.m_Param.pos = m_pAmefootPlayer->m_Params.pos + chr_func::GetFront(m_pAmefootPlayer) * kDamagePosOffset;

    // フレーム経過
    if ( m_Timer >= kAllFrame )
    {
        SetState(_Jump);
    }

    // キャラクターの更新
    CharacterNoDamageUpdate();
}


// 飛び上がり中
void AmefootAttackClass::Jump()
{
    const int kAllFrame = 10;
    const float kMoveSpeed = 0.5f;
    const float kMoveY = 0.8f;
    const float kDamagePosOffset = 4.0f;
    const float kDamagePosYOffset = 2.0f;

    if ( m_Timer == 0 ) { JumpStart(); }
    m_Timer++;

    // 移動
    chr_func::AddMoveFront(m_pAmefootPlayer, kMoveSpeed, kMoveSpeed);
    m_pAmefootPlayer->m_Params.move.y = kMoveY;

    // ダメージ座標設定
    m_pControllDamage->m_Param.pos = m_pAmefootPlayer->m_Params.pos + chr_func::GetFront(m_pAmefootPlayer) * kDamagePosOffset;
    m_pControllDamage->m_Param.pos.y += kDamagePosYOffset;

    m_pControllDamage->m_Param.size = 2.0f;


    if ( m_Timer >= kAllFrame )
    {
        SetState(_Touchdown);
    }

    // キャラクターの更新
    CharacterNoDamageUpdate();
}


// タッチダウン中
void AmefootAttackClass::Touchdown()
{
    const int kTouchdownFrame = 35;
    const float kMoveY = -0.8f;
    const float kMoveDownSpeed = 0.5f;

    // 開始
    if ( m_Timer == 0 ) { TouchdownStart(); }
    ++m_Timer;

    // 移動
    chr_func::XZMoveDown(m_pAmefootPlayer, kMoveDownSpeed);
   // m_pAmefootPlayer->m_Params.move.y = kMoveY;

    if ( m_Timer >= kTouchdownFrame )
    {
        SetState(_Standup);
    }

    // キャラクターの更新
    CharacterNoDamageUpdate();
}


// 立ち上がり中
void AmefootAttackClass::Standup()
{
    const int kStandupFrame = 10;

    // 開始
    if ( m_Timer == 0 ) { StandupStart(); }
    ++m_Timer;

    if ( m_Timer >= kStandupFrame )
    {
        SetState(_End);
    }

    // キャラクターの更新
    CharacterNoDamageUpdate();
}


// 失敗
void AmefootAttackClass::Failed()
{
    const int kFailedFrame = 5;

    // 開始
    if ( m_Timer == 0 ) { FailedStart(); }
    ++m_Timer;

    if ( m_Timer >= kFailedFrame )
    {
        SetState(_End);
    }

    // キャラクターの更新
    CharacterUsualUpdate();
}


// 終了
void AmefootAttackClass::End()
{
    m_pAmefootPlayer->SetState(new AmefootPlayerState_UsualMove());
}


// ヒットストップ
void AmefootAttackClass::HitStop()
{
    assert(m_NextStateType != _Error && "m_NextStateType が _Error の状態でHitStopステートが実行されました。");

    m_Timer++;
    if ( m_Timer >= m_HitStopFrame )
    {
        SetState(m_NextStateType);
        m_NextStateType = _Error;
    }

    // キャラクターの更新
    CharacterHitStopUpdate();
}


// 構え開始の瞬間
void AmefootAttackClass::PoseStart()
{
    m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Tackle_Save_Power);
}


// タックル開始の瞬間
void AmefootAttackClass::TackleStart(float MoveValue, float DamageValue)
{
    const float kAcceleration = MoveValue;

    // 移動
    chr_func::ResetMove(m_pAmefootPlayer);
    chr_func::AddMoveFront(m_pAmefootPlayer, kAcceleration, kAcceleration);

    // ダメージ設定
    m_Damage.m_Enable = true;
    m_Damage.Value = DamageValue;
    m_Damage.type = DamageBase::Type::_WeekDamage;

    m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Tackle_Charge);
}


// タックル打ち上げ開始の瞬間
void AmefootAttackClass::DrivaAwayStart()
{
    m_Damage.ResetCounts();
    m_Damage.SetOption(DamageBase::Option::_DontDie, true);
    m_Damage.m_Enable = true;
    m_Damage.m_VecPower = Vector2(0.5f, 0.8f);
    m_Damage.m_Vec = chr_func::GetFront(m_pAmefootPlayer);
    m_Damage.m_Vec.y = 1.0f;
    m_Damage.m_VecType = DamageShpere::DamageVecType::MemberParam;
    m_Damage.Value = 5;
    m_Damage.type = DamageBase::Type::_VanishDamage;

    m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Tackle_Drive_Away);
}


// ジャンプ開始の瞬間
void AmefootAttackClass::JumpStart()
{
    m_Damage.m_Enable = false;  //通常ダメージをoffに

    //コントロールダメージ作成
    {
        class GetTransform :public DamageControllVanish::GetDamageControllTransformClass
        {
            DamageControll_Transform* const m_pTransform;
        public:
            GetTransform(DamageControll_Transform* const pTransform):
                m_pTransform(pTransform)
            {}

            DamageControll_Transform* Get()
            {
                return m_pTransform;
            }
        };

        //生成
        m_pControllDamage = new DamageControllVanish(new GetTransform(&m_DamageTransform));

        //パラメータセット
        m_pControllDamage->m_Enable = true;
        m_pControllDamage->pParent = m_pAmefootPlayer;
        m_pControllDamage->type = DamageBase::Type::_ControllDamage;
    }


    m_Damage.type = DamageBase::Type::_WeekDamage;
    chr_func::ResetMove(m_pAmefootPlayer);
    m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Tackle_Touchdown);
}


// タッチダウン開始の瞬間
void AmefootAttackClass::TouchdownStart()
{
    m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Tackle_Touchdown);
}


// 立ち上がり開始の瞬間
void AmefootAttackClass::StandupStart()
{
    m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Damage_Vanish_Landing);
}


// 失敗開始の瞬間
void AmefootAttackClass::FailedStart()
{
    m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Stand);
}



// キャラクターの基本的な更新
void AmefootAttackClass::CharacterUsualUpdate()
{
    AmefootUsualHitEvent HitEvent(m_pAmefootPlayer);
    chr_func::UpdateAll(m_pAmefootPlayer, &HitEvent);
}


// 無敵中の更新
void AmefootAttackClass::CharacterNoDamageUpdate()
{
    DamageManager::HitEventBase HitEvent;
    chr_func::UpdateAll(m_pAmefootPlayer, &HitEvent);
}


// ヒットストップ時の更新
void AmefootAttackClass::CharacterHitStopUpdate()
{
    DamageManager::HitEventBase HitEvent;

    //壁との接触判定
    chr_func::CheckWall(m_pAmefootPlayer);

    //床との接触判定
    chr_func::CheckGround(m_pAmefootPlayer);

    //あたり判定を取る
    chr_func::DamageCheck(m_pAmefootPlayer, &HitEvent);

}

//パワーから突進のパラメータを得るゲッタ
void AmefootAttackClass::GetTackleParamByPower(
    RATIO p, int *pOutFrame, float* pOutMoveValue, float* pOutDamagePower)const
{
    struct
    {
        RATIO ok_value;
        int   Frame;
        float MoveValue;
        float Damage;
    }
    Params[]=
    {
        { 1.0f, 32, 1.0f, 6.0f },  //最大パワー時
        { 0.9f, 29, 0.9f, 5.0f },
        { 0.7f, 26, 0.8f, 4.0f },
        { 0.5f, 23, 0.7f, 3.0f },
        { 0.3f, 20, 0.6f, 2.0f },
        { 0.1f, 17, 0.5f, 1.0f },  
        { 0.0f, 14, 0.4f, 1.0f }, //最弱パワー時
    };

    for (auto& it : Params)
    {
        if (it.ok_value <= p)
        {
            *pOutFrame = it.Frame;
            *pOutMoveValue = it.MoveValue;
            *pOutDamagePower = it.Damage;

            return;
        }
    }

    MyAssert(false, "タックルためすぎいいい");

}


//引っ付きクラスを腕の位置に更新する
void AmefootAttackClass::UpdateDamageTransform()
{
    Matrix* T = &m_DamageTransform.m_Transform;

    //腕の行列を取得
    m_pAmefootPlayer->m_Renderer.GetWorldBoneMatirx(*T, 22);

    //ボーン行列のスケール成分を正規化
    for (int i = 0; i < 3; ++i)
    {
        const float Length = Vector3((*T)(0, i), (*T)(1, i), (*T)(2, i)).Length();

        for (int j = 0; j < 3; ++j)
        {
            (*T)(j, i) /= Length;
        }
    }


    //座標は右腕と左腕の中間に
    {
        const Vector3 Pos = 
            (m_pAmefootPlayer->m_Renderer.GetLocalBonePos(22)+
             m_pAmefootPlayer->m_Renderer.GetLocalBonePos(28))*0.5f;

        T->_41 = Pos.x;
        T->_42 = Pos.y;
        T->_43 = Pos.z;
    }
}
