#include "CharacterBase.h"
#include "CharacterEvasionClass.h"
#include "CharacterFunction.h"


//***************************************************
//		ラクロス用回避クラス
//***************************************************


// コンストラクタ
CharacterEvasion::CharacterEvasion(
	CharacterBase* pCharacterBase,
	Event* pEvent,
	const EvasionParams& evasion_params
	) :
	m_EvasionParams(evasion_params),
	m_StickValue(0, 0),
	m_pCharacterBase(pCharacterBase),
	m_Timer(0),
	m_pEvent(pEvent)
{

}


// デストラクタ
CharacterEvasion::~CharacterEvasion()
{
	delete m_pEvent;
}


// 更新
bool CharacterEvasion::Update()
{

	if (m_Timer == 0)
	{// 回避開始
        m_pEvent->EvasionStart();

        {
            Vector3 ViewPos;

            ViewPos = m_pCharacterBase->m_Params.pos;
            ViewPos += Vector3(m_StickValue.x, 0, m_StickValue.y);

            // 向き補正
            chr_func::AngleControll(
                m_pCharacterBase,
                ViewPos,
                cosf(m_EvasionParams.MaxTurnRadian)
                );
        }

        // 移動
        chr_func::AddMoveFront(
            m_pCharacterBase,
            m_EvasionParams.MoveSpeed,
            m_EvasionParams.MoveSpeed);
	}

	// 座標更新
	chr_func::PositionUpdate(m_pCharacterBase);

	// 減速
	chr_func::XZMoveDown(
		m_pCharacterBase,
		m_EvasionParams.MoveDownSpeed);

	// 更新
	m_pEvent->Update();

	if (m_Timer >= m_EvasionParams.AllFrame)
	{
          if(!m_pEvent->IsEvasionContinue())
          {
               // 回避終了
               m_pEvent->EvasionEnd();
               return false;
          }
	}

	// タイマー更新
	m_Timer++;
	
	return true;
}


// スティックの値セット
void CharacterEvasion::SetStickValue(CrVector2 stickValue)
{
	m_StickValue = stickValue;
}


// ダメージを受けることができるかどうか
bool CharacterEvasion::CanGetDamage()const
{
	return (
		m_Timer < m_EvasionParams.NoDamageStartFrame || 
		m_Timer >= m_EvasionParams.NoDamageEndFrame
		);
}
