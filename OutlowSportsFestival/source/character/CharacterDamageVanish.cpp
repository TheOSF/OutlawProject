#include "CharacterDamageVanish.h"
#include "CharacterFunction.h"
#include "CharacterBase.h"

CharacterDamageVanish::CharacterDamageVanish(
    CharacterBase*    pCharacter,//吹き飛ぶキャラクタ
    const Param&      param, //吹きとびパラメーター
    Event*            pEvent //イベントクラスへのポインタ(デストラクタでdeleteする)
    ) :
    m_pCharacter(pCharacter),
    m_Start(false),
    m_End(false),
    m_pEvent(pEvent),
    m_pStateFunc(&CharacterDamageVanish::Flying),
    m_Count(0),
    m_Rotate(0,0,0)
{
    m_Param = param;
}


CharacterDamageVanish::~CharacterDamageVanish()
{
    delete m_pEvent;
}


// 更新
void CharacterDamageVanish::Update()
{
    (this->*m_pStateFunc)();
}

void CharacterDamageVanish::Flying()
{
    if (m_Start == false)
    {
        m_Start = true;

        //キャラクタをダメージの方向に向かせる
        chr_func::AngleControll(
            m_pCharacter,
            m_pCharacter->m_Params.pos - m_Param.move
            );

        //移動方向を設定
        m_pCharacter->m_Params.move = m_Param.move;

        //スタートイベント呼び出し
        m_pEvent->Start();
    }

    //飛行中の回転行列を作成する
    Matrix R;

    m_Rotate += m_Param.rotate_speed;

    D3DXMatrixRotationYawPitchRoll(
        &R,
        m_Rotate.y, m_Rotate.x, m_Rotate.z
        );

    m_pEvent->Flying(R);

    {
        //移動更新
        chr_func::UpdateMoveY(m_pCharacter);
        chr_func::PositionUpdate(m_pCharacter);
        chr_func::CheckGround(m_pCharacter);
    }


    //地面についていた場合はステート移行
    if (chr_func::isTouchGround(m_pCharacter) &&
        m_pCharacter->m_Params.move.y <= 0)
    {
        m_pEvent->StandUpStart();
        m_pStateFunc = &CharacterDamageVanish::StandUp;
    }
}



void CharacterDamageVanish::StandUp()
{
    //フレームカウント
    ++m_Count;

    chr_func::XZMoveDown(m_pCharacter, 0.2f);

    //終了判定
    if (m_End == false &&
        m_Param.standup_frame <= m_Count)
    {
        m_End = true;
        m_pEvent->End();
    }
}