#include "AmefootPlayerState_Tackle.h"
#include "../AmefootPlayerState.h"
#include "../AmefootUsualHitEvent.h"
#include "../../CharacterFunction.h"
#include "../../../GameSystem/GameController.h"
#include "../../../Camera/Camera.h"

#include "../../CharacterHitEventFunc.h"
#include "../../../Effect/EffectFactory.h"
#include "../../../Sound/Sound.h"
#include "../../../Effect/ImpactLightObject.h"
#include "../../../Effect/GlavityLocus.h"


AmefootPlayerState_Tackle::AmefootPlayerState_Tackle(AmefootPlayer*const pCharacter) :
m_pAmefootPlayer(pCharacter),
m_pStateFunc(&AmefootPlayerState_Tackle::Pose),
m_pDamageTransform(nullptr),
m_pControllDamage(nullptr),
m_SpeedEffect(1)
{
    m_Damage.m_Enable = false;
    m_Damage.m_Param.pos = m_pAmefootPlayer->m_Params.pos;
    m_Damage.m_Param.size = 2.8f;
    m_Damage.pParent = m_pAmefootPlayer;

    //引っ付きクラスを作成
    m_pDamageTransform = new DamageControll_Transform();
}

AmefootPlayerState_Tackle::~AmefootPlayerState_Tackle()
{

}


void AmefootPlayerState_Tackle::Enter(AmefootPlayer* pCharacter)
{
    
}

void AmefootPlayerState_Tackle::Execute(AmefootPlayer* pCharacter)
{
    (this->*m_pStateFunc)();

    pCharacter->m_Renderer.Update(1);
    chr_func::CreateTransMatrix(pCharacter, &pCharacter->m_Renderer.m_TransMatrix);

    UpdateDamageTransform();
}

void AmefootPlayerState_Tackle::Exit(AmefootPlayer* pCharacter)
{
    if (m_pDamageTransform != nullptr)
    {
        m_pDamageTransform->m_Destroy = true;
    }
    
    delete m_pControllDamage;

    pCharacter->m_Params.DoCheckOtherChrSpace = true;
}


// ステート切り替え
void AmefootPlayerState_Tackle::SetState(StateFunc state)
{
    m_pStateFunc = state;
    m_Timer = 0;
    m_Damage.m_Enable = false;
}

//-------------------------------------------------------//
// 構え中
void AmefootPlayerState_Tackle::Pose()
{
    const int kAllFrame = 10; // 構えフレーム
    const int kAdditionalFrame = 60; // 延長できるフレーム
    const RADIAN kAngleControlSpeed = 0.04f; // 角度補正スピード
    bool go_next = false; // ステート移行するかどうか
    const float kMoveDownSpeed = 0.2f;

    // 開始
    if (m_Timer == 0) { PoseStart(); }
    ++m_Timer;

    chr_func::XZMoveDown(m_pAmefootPlayer, kMoveDownSpeed);

    CharacterBase* pTargetCharacter = nullptr;
    chr_func::CalcAtkTarget(m_pAmefootPlayer, D3DXToRadian(20), 25.0f, &pTargetCharacter);


    if (pTargetCharacter != nullptr)
    {
        //自動回転
        chr_func::AngleControll(m_pAmefootPlayer, pTargetCharacter->m_Params.pos, kAngleControlSpeed);
    }
    else
    {
        const Vector2 Stick = GetControllVec();

        //スティックが一定以上倒されているかどうか
        if (Vector2Length(Stick) > 0.25f)
        {
            Vector3 Vec(Stick.x, 0, Stick.y);

            //スティック値をカメラ空間に
            //Vec = DefCamera.GetForward()*Stick.y + DefCamera.GetRight()*Stick.x;

            //キャラクタ回転
            chr_func::AngleControll(m_pAmefootPlayer, m_pAmefootPlayer->m_Params.pos + Vec, kAngleControlSpeed);
        }
    }


    // ボタンを離したら次のステートへ
    if (m_Timer >= kAllFrame)
    {
        if (!isPoseContinue())
        {
            //タックルパワーを溜めた時間から計算
            m_TacklePower = (float)(m_Timer - kAllFrame) / (float)kAdditionalFrame;
            go_next = true;
        }
    }

    // 最大フレームを超えたら次のステートへ
    if (m_Timer >= (kAllFrame + kAdditionalFrame))
    {
        //タックルパワー最大！
        m_TacklePower = 1.0f;
        go_next = true;
    }

    // 次のステートへ
    if (go_next)
    {

        SetState(&AmefootPlayerState_Tackle::Tackle);
    }

    // キャラクターの更新
    CharacterNoDamageUpdate();
}


// タックル中
void AmefootPlayerState_Tackle::Tackle()
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

    CharacterBase* pTargetCharacter = nullptr;
    chr_func::CalcAtkTarget(m_pAmefootPlayer, D3DXToRadian(20), 25.0f, &pTargetCharacter);

    if (pTargetCharacter)
    {
        //自動回転
        chr_func::AngleControll(m_pAmefootPlayer, pTargetCharacter->m_Params.pos, kAngleControlSpeed);
    }

    // ダメージ座標設定
    m_Damage.m_Param.pos = m_pAmefootPlayer->m_Params.pos + chr_func::GetFront(m_pAmefootPlayer) * kDamagePosOffset;


    {
        Vector3 chrpos = m_pAmefootPlayer->m_Params.pos;
        chrpos.y += 2.0f;

        m_SpeedEffect.Update(chrpos, -m_pAmefootPlayer->m_Params.move);
    }


    //ヒットしたらステート切り替え
    if (m_Damage.HitCount > 0)
    {
        SetState(&AmefootPlayerState_Tackle::DriveAway);
    }

    // フレーム経過
    if (m_Timer >= kAllFrame)
    {
        m_Damage.m_Enable = false;

        SetState(&AmefootPlayerState_Tackle::Failed);
    }

    // キャラクターの更新
    CharacterNoDamageUpdate();
}


// タックル打ち上げ中
void AmefootPlayerState_Tackle::DriveAway()
{
    const int kTackleToDriveAwayFrame = 4;
    const int kAllFrame = 10;
    const float kMoveDownSpeed = 0.2f;
    const float kDamagePosOffset = 4.0f;

    // 開始
    if (m_Timer == kTackleToDriveAwayFrame)
    {
        DrivaAwayStart();
    }
   

    {
        chr_func::XZMoveDown(m_pAmefootPlayer, kMoveDownSpeed);
    }

    ++m_Timer;

    // ダメージ座標設定
    m_Damage.m_Param.pos = m_pAmefootPlayer->m_Params.pos + chr_func::GetFront(m_pAmefootPlayer) * kDamagePosOffset;

    // フレーム経過
    if (m_Timer >= kAllFrame)
    {
        SetState(&AmefootPlayerState_Tackle::Jump);
    }

    // キャラクターの更新
    CharacterNoDamageUpdate();
}


// 飛び上がり中
void AmefootPlayerState_Tackle::Jump()
{
    const int kAllFrame = 10;
    const float kDamagePosOffset = 2.0f;
    const float kDamagePosYOffset = 2.0f;

    if (m_Timer == 0) { JumpStart(); }
    m_Timer++;

    // ダメージ座標設定
    m_pControllDamage->m_Param.pos = m_pAmefootPlayer->m_Params.pos + chr_func::GetFront(m_pAmefootPlayer) * kDamagePosOffset;
    m_pControllDamage->m_Param.pos.y += kDamagePosYOffset;

    m_pControllDamage->m_Param.size = 3.0f;


    if (m_Timer >= kAllFrame)
    {
        SetState(&AmefootPlayerState_Tackle::Touchdown);
    }

    // キャラクターの更新
    CharacterNoDamageUpdate();
}


// タッチダウン中
void AmefootPlayerState_Tackle::Touchdown()
{
    const float kMoveDownSpeed = 0.1f;
    const int StopFrame = 30;

    // 開始
    if (m_Timer == 0) { TouchdownStart(); }
    ++m_Timer;

    // 移動
    if (m_Timer < StopFrame)
    {
        chr_func::XZMoveDown(m_pAmefootPlayer, kMoveDownSpeed);
        //m_pAmefootPlayer->m_Params.move.y *= kMoveDownSpeed;
    }

    if (m_Timer == StopFrame)
    {
        m_pAmefootPlayer->m_Params.move.y = -0.8f;
    }

    // m_pAmefootPlayer->m_Params.move.y = kMoveY;

    //地面についていたら移行
    if (chr_func::isTouchGround(m_pAmefootPlayer))
    {
        SetState(&AmefootPlayerState_Tackle::Standup);
    }

    // キャラクターの更新
    CharacterNoDamageUpdate();
}


// 立ち上がり中
void AmefootPlayerState_Tackle::Standup()
{
    const int kStandupFrame = 20;

    // 開始
    if (m_Timer == 0) { StandupStart(); }
    ++m_Timer;

    if (m_Timer >= kStandupFrame)
    {
        SetState(&AmefootPlayerState_Tackle::End);
    }

    // キャラクターの更新
    CharacterNoDamageUpdate();
}


// 失敗
void AmefootPlayerState_Tackle::Failed()
{
    const int kFailedFrame = 40;

    chr_func::XZMoveDown(m_pAmefootPlayer, 0.15f);

    // 開始
    if (m_Timer == 0) { FailedStart(); }
    ++m_Timer;

    if (m_Timer >= kFailedFrame)
    {
        SetState(&AmefootPlayerState_Tackle::End);
    }

    // キャラクターの更新
    CharacterUsualUpdate();
}


// 終了
void AmefootPlayerState_Tackle::End()
{
    m_pAmefootPlayer->SetState(new AmefootPlayerState_UsualMove());
}

// 構え開始の瞬間
void AmefootPlayerState_Tackle::PoseStart()
{
    m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Tackle_Save_Power);
}


// タックル開始の瞬間
void AmefootPlayerState_Tackle::TackleStart(float MoveValue, float DamageValue)
{
    const float kAcceleration = MoveValue;

    // 移動
    chr_func::ResetMove(m_pAmefootPlayer);
    chr_func::AddMoveFront(m_pAmefootPlayer, kAcceleration, kAcceleration);

    //ＳＥ
    Sound::Play(Sound::Sand1);

    // ダメージ設定
    m_Damage.m_Enable = true;
    m_Damage.Value = DamageValue;
    m_Damage.type = DamageBase::Type::_WeekDamage;

    m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Tackle_Charge);
}


// タックル打ち上げ開始の瞬間
void AmefootPlayerState_Tackle::DrivaAwayStart()
{
    m_Damage.m_Enable = false;  //通常ダメージをoffに

    //コントロールダメージ作成
    {
        class GetTransform :public DamageControllVanish::GetDamageControllTransformClass
        {
            DamageControll_Transform* const m_pTransform;
        public:
            GetTransform(DamageControll_Transform* const pTransform) :
                m_pTransform(pTransform)
            {}

            DamageControll_Transform* Get()
            {
                return m_pTransform;
            }
        };

        //生成
        m_pControllDamage = new DamageControllVanish(new GetTransform(m_pDamageTransform));

        //パラメータセット
        m_pControllDamage->m_Enable = true;
        m_pControllDamage->pParent = m_pAmefootPlayer;
        m_pControllDamage->type = DamageBase::Type::_ControllDamage;
    }

    //他のキャラを押し出さないように(つかむので)
    m_pAmefootPlayer->m_Params.DoCheckOtherChrSpace = false;

    m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Tackle_Touchdown);
}


// ジャンプ開始の瞬間
void AmefootPlayerState_Tackle::JumpStart()
{

    const float kMoveSpeed = 0.6f;
    const float kMoveY = 0.55f;

    // 移動
    chr_func::ResetMove(m_pAmefootPlayer);

    chr_func::AddMoveFront(m_pAmefootPlayer, kMoveSpeed, kMoveSpeed);
    m_pAmefootPlayer->m_Params.move.y = kMoveY;

    //m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Tackle_Touchdown);
}


// タッチダウン開始の瞬間
void AmefootPlayerState_Tackle::TouchdownStart()
{
   // m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Tackle_Touchdown);
}


// 立ち上がり開始の瞬間
void AmefootPlayerState_Tackle::StandupStart()
{
    //SE
    Sound::Play(Sound::Explode);

    //付いているキャラクタを離す
    if (m_pDamageTransform != nullptr)
    {
        Vector3 Vec = -chr_func::GetFront(m_pAmefootPlayer);
        Vec *= 0.3f;
        Vec.y = 0.35f;

        m_Damage.m_Enable = false;

        m_pDamageTransform->AllFree(Vec);


        //エフェクト
        SmokeEffect();
    }
    

    //エフェクト
    for (int i = 0; i < 5; ++i)
    {
        EffectFactory::Smoke(
            m_pAmefootPlayer->m_Params.pos + Vector3Rand()*0.5f ,
            Vector3(0, 0.1f, 0),
            1.5f,
            0.1f
            );
    }

    m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Tackle_Touchdown_andStand);
}


// 失敗開始の瞬間
void AmefootPlayerState_Tackle::FailedStart()
{
    m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Touchdown_Failed);
}


void AmefootPlayerState_Tackle::SmokeEffect()
{
    Vector3  color(0.8f, 0.4f, 0.0f);
    Vector3 Nmove = Vector3AxisY*0.5f;
    Vector3 power(0, -0.02f, 0);
    Vector3 move;
    GlavityLocus* g;

    const Vector4
        stCol(color.x, color.y, color.z, 1.0f),
        endCol(color.x, color.y, color.z, 0.5f);

    const Vector4
        stHdCol(color.x, color.y, color.z, 1.0f),
        endHdCol(color.x, color.y, color.z, 0.5f);


    for (int i = 0; i < 15; ++i)
    {
        move = Vector3Rand() + Nmove;
        move *= frand();
        move.y *= 2.0f;
        move *= 0.75f;

        g = new GlavityLocus(
            m_pAmefootPlayer->m_Params.pos+Vector3Rand(), move, power, 8, 40 + rand() % 20
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
        SmokePos = m_pAmefootPlayer->m_Params.pos;

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

    //カメラのゆれ
    DefCamera.SetShock(Vector2(0.3f, 0.3f), 30);
}

//パワーから突進のパラメータを得るゲッタ
void AmefootPlayerState_Tackle::GetTackleParamByPower(
    RATIO p, int *pOutFrame, float* pOutMoveValue, float* pOutDamagePower)const
{
    struct
    {
        RATIO ok_value;
        int   Frame;
        float MoveValue;
        float Damage;
    }
    Params[] =
    {
        { 1.0f, 22, 0.7f, 1.0f },  //最大パワー時
        { 0.75f, 22, 0.6f, 1.0f },
        { 0.5f, 20, 0.55f, 1.0f },
        { 0.25f, 19, 0.5f, 1.0f },
        { 0.0f, 18, 0.4f, 1.0f }, //最弱パワー時
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
void AmefootPlayerState_Tackle::UpdateDamageTransform()
{
    if (m_pDamageTransform == nullptr)
    {
        return;
    }

    Matrix* T = &m_pDamageTransform->m_Transform;

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
            (m_pAmefootPlayer->m_Renderer.GetWorldBonePos(22) +
            m_pAmefootPlayer->m_Renderer.GetWorldBonePos(28))*0.5f;

        T->_41 = Pos.x;
        T->_42 = Pos.y;
        T->_43 = Pos.z;
    }
}


//向きたい方向をゲット！
Vector2 AmefootPlayerState_Tackle::GetControllVec()const
{
    Vector2 ret(0, 0);

    if (m_pAmefootPlayer->m_PlayerInfo.player_type == PlayerType::_Player)
    {
        //プレイヤーの場合スティックの値を返す
        ret = controller::GetStickValue(controller::stick::left, m_pAmefootPlayer->m_PlayerInfo.number);

        //ビュー空間に変換
        {
            Vector3 temp;
            temp = DefCamera.GetForward()*ret.y + DefCamera.GetRight()*ret.x;
            ret.x = temp.x;
            ret.y = temp.z;
        }
    }
    else
    {
        //コンピュータの場合近い敵へのベクトルを返す

        CharacterBase* pTarget = nullptr;

        if (chr_func::CalcAtkTarget(
            m_pAmefootPlayer,
            PI,
            50.0f,
            &pTarget
            ))
        {
            ret.x = pTarget->m_Params.pos.x - m_pAmefootPlayer->m_Params.pos.x;
            ret.y = pTarget->m_Params.pos.z - m_pAmefootPlayer->m_Params.pos.z;
        }
    }


    return ret;
}


//まだ構えるかどうかを得る
bool AmefootPlayerState_Tackle::isPoseContinue()const
{
    bool ret = false;

    if (m_pAmefootPlayer->m_PlayerInfo.player_type == PlayerType::_Player)
    {
        //プレイヤーの場合ボタンの値を返す
        ret = controller::GetPush(controller::button::shikaku, m_pAmefootPlayer->m_PlayerInfo.number);
    }
    else
    {
        //コンピュータの場合近くに敵がいると溜めない　+　乱数

        CharacterBase* pTarget = nullptr;

        ret = chr_func::CalcAtkTarget(
            m_pAmefootPlayer,
            PI,
            20.0f,
            &pTarget
            );

        //乱数
        const RATIO random = 0.02f;

        if (frand() <= random)
        {
            ret = rand() % 2 == 0;
        }
    }


    return ret;
}


//通常更新
void AmefootPlayerState_Tackle::CharacterUsualUpdate()
{
    AmefootUsualHitEvent HitEvent(m_pAmefootPlayer);
    chr_func::UpdateAll(m_pAmefootPlayer, &HitEvent);
}

//アーマー状態の更新
void AmefootPlayerState_Tackle::CharacterNoDamageUpdate()
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

        bool Hit(DamageBase* pDamage)
        {
            Vector3 damageVec = Vector3Zero;
            CharacterHitEventFunc::SetType type = CharacterHitEventFunc::CheckDamage(pDamage, m_pAmefoot, &damageVec);
           
            switch (type)
            {
            case CharacterHitEventFunc::SetType::Weak_Hit:
                //アーマーだ！！

                //エフェクト
                Effect();

                //ＳＥ
                Sound::Play(Sound::AtkHit1);

                isDoArmor = true;

                break;

            case CharacterHitEventFunc::SetType::Vanish_Hit:
                m_pAmefoot->SetState(new AmefootPlayerState_ReceiveVanishDamage(damageVec));
                break;

            case CharacterHitEventFunc::SetType::Die:
                m_pAmefoot->SetState(new AmefootPlayerState_Die(m_pAmefoot, damageVec));
                break;

            case CharacterHitEventFunc::SetType::Controll_Hit:
            {
                //コントロール吹き飛びステートへ
                AmefootState_ControllVanish* p = new AmefootState_ControllVanish(m_pAmefoot);
                m_pAmefoot->SetState(p, 2);
                ((DamageControllVanish*)pDamage)->GetDamageControll_Transform()->AddControllClass(p->GetControllClass());
            }
                break;

            case CharacterHitEventFunc::SetType::_None:
                return false;
            default:
                MyAssert(false, "ひるみ分岐ができていません\n");
                return false;
            }

            return true;
        }
    };

    chr_func::UpdateAll(m_pAmefootPlayer, &ArmorHitEvent(m_pAmefootPlayer));
}