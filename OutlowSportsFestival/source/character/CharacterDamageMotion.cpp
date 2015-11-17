#include "CharacterDamageMotion.h"
#include "CharacterFunction.h"
#include "CharacterBase.h"
#include "../Camera/Camera.h"
#include "../Sound/Sound.h"


CharacterDamageMotion::CharacterDamageMotion(
	CharacterBase* pCharacter,
	Event* pEvent,
	DamageManager::HitEventBase* pHitEvent,
	const Params& params
	):
	m_pCharacter(pCharacter),
	m_Timer(0),
	m_pEvent(pEvent),
	m_Start(false),
	m_End(false),
	m_pHitEvent(pHitEvent)
{
	m_Params = params;
    m_Pos = m_pCharacter->m_Params.pos;


    //ひるむ方向がない場合今向いている方向のまま
    if (m_Params.damage_vec.x == 0 && m_Params.damage_vec.z)
    {
        chr_func::GetFront(m_pCharacter, &m_Params.damage_vec);
        m_Params.damage_vec = -m_Params.damage_vec * 0.001f;
    }
}

CharacterDamageMotion::~CharacterDamageMotion()
{
    //ライトをoffにする
    m_pEvent->SetLight(0);

	delete m_pEvent;
	delete m_pHitEvent;
}


// 更新
void CharacterDamageMotion::Update()
{
    const int AllFrame = 25;
    const int NoDamageFrame = 5;

	//フレーム更新
	++m_Timer;

	if (m_Start == false)
	{
		m_Start = true;

		//キャラクタをダメージの方向に向かせる
		chr_func::AngleControll(
			m_pCharacter,
			m_pCharacter->m_Params.pos - m_Params.damage_vec
			);

        //ヒットバック処理
        {
            const float val = Vector3XZLength(m_Params.damage_vec);
            chr_func::AddMoveFront(m_pCharacter, -val, val);
        }

        //カメラのゆれ
        DefCamera.SetShock(Vector2(0.05f, 0.05f), 15);

		//スタートイベント呼び出し
		m_pEvent->Start();

        //サウンド
        Sound::Play(Sound::Damage3);
	}

    //若干キャラクタ座標を揺らす(食らっているのを強調するため)
    {
        m_Pos = m_pCharacter->m_Params.pos;

        if (m_Timer < 15)
        {
            m_pCharacter->m_Params.pos += Vector3Rand()*0.25f;
        }
    }

    //キャラクタを若干光らせるパラメータを送る
    {
        m_pEvent->SetLight(pow(1 - min(((float)m_Timer / 15.0f), 1), 2)*0.25f);
    }

    //イベントクラス更新
    m_pEvent->Update(1.0f);

    //終了判定
    if (m_End == false &&
        AllFrame <= m_Timer)
    {
        m_End = true;
        m_pEvent->End();
    }



    //無敵フレーム判定
    if (m_Timer > NoDamageFrame)
    {
        //ダメージ判定
        chr_func::DamageCheck(m_pCharacter, m_pHitEvent);
    }

    

    //前回のフレームで揺れて動いた分を元に戻す
    m_pCharacter->m_Params.pos = m_Pos;


    //キャラクタ更新
    {
        //ヒットバック減衰
        chr_func::XZMoveDown(m_pCharacter, 0.1f);

        //重力加算
        chr_func::UpdateMoveY(m_pCharacter);

        //位置を更新
        chr_func::PositionUpdate(m_pCharacter);

        //壁との接触判定
        chr_func::CheckWall(m_pCharacter);

        //床との接触判定
        chr_func::CheckGround(m_pCharacter);

    }
}