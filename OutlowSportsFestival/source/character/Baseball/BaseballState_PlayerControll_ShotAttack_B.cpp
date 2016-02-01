#include "BaseballState_PlayerControll_ShotAttack_B.h"
#include "BaseballPlayerState.h"
#include "BaseballState_PlayerControll_Evasion.h"
#include "Baseball_HitEvent.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../../Ball/UsualBall.h"
#include "../../Damage/Damage.h"
#include "../CharacterMoveClass.h"
#include "../CharacterShotAttackClass.h"
#include "../CharacterManager.h"


#include "../../Effect/HitEffectObject.h"
#include "../../Sound/Sound.h"
#include "../../GameSystem/GameController.h"

BaseballState_PlayerControll_ShotAttack_B::BaseballState_PlayerControll_ShotAttack_B(
	ControllClass*       pControllClass //終了時にdeleteする
	) :
	m_pControllClass(pControllClass)
{

}

BaseballState_PlayerControll_ShotAttack_B::~BaseballState_PlayerControll_ShotAttack_B()
{
	delete m_pControllClass;
}

void  BaseballState_PlayerControll_ShotAttack_B::Enter(BaseballPlayer* b)
{

	//カウント初期化
	m_Timer = 0;
	//モーションセット
	b->SetMotion(baseball_player::_mb_Shot_B);

}

void BaseballState_PlayerControll_ShotAttack_B::Execute(BaseballPlayer* b)
{
	Vector3 v1(0, 0, 0), v2(0, 0, 0);
	const int EndFrame = 48; //終了フレーム
	const int ShotFrame = 35;//打つフレーム
	const int CancelStart = 25;//キャンセル行動可能なフレーム
	const int AfterAction = 45;//ショット後のアクションが可能になるフレーム
	const float AngleRange = PI / 4;
	pTargetEnemy = nullptr;    //ターゲット保持のポインタ

	//カウント更新
	++m_Timer;

	if (m_Timer == 5)
	{
		{
			//移動量制限
			const float MaxMove = 0.06f;

			if (b->m_Params.move.Length() > MaxMove)
			{
				b->m_Params.move.Normalize();
				b->m_Params.move *= MaxMove;
			}
		}

		//打ち上げボール生成
		BallBase::Params param;

		//移動は上向き
		param.move = b->m_Params.move;
		param.move.y = 0.48f;

		
		//カウンターできないタイプ
		param.type = BallBase::Type::_CantCounter;

		
		//キャラの場所に(最終的に腕の位置に？)
        param.pos = b->getNowModeModel()->GetWorldBonePos(20);
		//親を自分に
		param.pParent = b;

		//生成
		//メモ  --ステージ外でボールを生成するとボールが消えてプログラムが落ちる -- 本来はいけないのでスルーでok
		m_pUpBall = new PhysicallyMoveBall(param, DamageBase::Type::_WeekDamage, 1, -0.025f);

		//打ち上げ中はあたり判定なし
		m_pUpBall->m_Damage.m_Enable = false;

        //スキルゲージ加算
        chr_func::AddSkillGauge(b, UsualBall::AddSkillValueRatio);
	}

	if (m_Timer >= 5)
	{
		//打ちキャンセル
		if (m_Timer > CancelStart && m_Timer < ShotFrame - 3)
		{
            if (m_pControllClass->DoOtherAction())
            {
                m_pUpBall->m_Damage.m_Enable = true;
                m_pUpBall->m_Params.type = BallBase::Type::_CantCounter;
            }
		}

		//ショット後のアクション
		if (m_Timer > AfterAction && m_Timer < EndFrame)
		{
			m_pControllClass->DoShotAfterAction();
		}

		//方向補正
        if (m_Timer < ShotFrame)
        {
            CharacterBase* pTargetCharacter = nullptr;
            const float AngleSpeed = D3DXToRadian(10);

            if (chr_func::CalcAtkTarget(b, AngleSpeed, 50.0f, &pTargetCharacter))
            {
                //自動回転
                chr_func::AngleControll(b, pTargetCharacter->m_Params.pos, AngleSpeed*2.0f);
            }
            else
            {
                Vector3 vec = m_pControllClass->GetVec();
                chr_func::AngleControll(b, b->m_Params.pos + vec, AngleSpeed);
            }

        }

		//打ちあげたボールが落下中の場合ダメージ判定を有効に
		if (m_pUpBall != nullptr&&
			m_pUpBall->m_Params.move.y < 0)
		{
			m_pUpBall->m_Damage.m_Enable = true;
			m_pUpBall->m_Params.type = BallBase::Type::_CantCounter;
		}

		//サウンド
		if (m_Timer == ShotFrame - 3)
		{
			Sound::Play(Sound::BaseBall_kaki_n);
		}

		//打つ！
		if (m_Timer == ShotFrame)
		{
			//打ちあげたボールの状態チェック
			if (
				m_pUpBall != nullptr&&
				m_pUpBall->m_Params.pParent == b&&
				m_pUpBall->m_Params.type == BallBase::Type::_CantCounter
				)
			{

				//ボール発射
				BallBase::Params param;

				//移動は前向き
				chr_func::GetFront(b, &param.move);
				//スピードは適当
				param.move *= 0.68f;

				//キャラの場所に(最終的に腕の位置に？)
				param.pos = m_pUpBall->m_Params.pos;
				//高さをキャラ共通ボール発射のYに
				param.pos.y = BallBase::UsualBallShotY;

				//親を自分に
				param.pParent = b;
				//通常タイプ
				param.type = BallBase::Type::_Usual;

				//生成
                new UsualBall(param, DamageBase::Type::_WeekDamage, 8, UsualBall::GetUsualMoveControll());

				//コントローラを振動
				chr_func::SetControllerShock(
					b,
					0.5f,
					0.15f
					);

				//エフェクト
				{
					COLORf EffectColor(CharacterBase::GetPlayerColor(b->m_PlayerInfo.number));

					//エフェクトの設定
					new HitEffectObject(
						param.pos,
						Vector3Normalize(param.move),
						0.1f,
						0.1f,
						Vector3(EffectColor.r, EffectColor.g, EffectColor.b)
						);
				}

				//上に上げたボールを消去
				m_pUpBall->m_DeleteFlag = true;
				m_pUpBall = nullptr;
			}
		}

		//ステート終了
		if (m_Timer > EndFrame)
		{
			b->SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(b));
		}

		//打ち終わっていた場合
		if (m_Timer > ShotFrame)
		{
			chr_func::XZMoveDown(b, 0.1f);
		}

		//基本的な更新
		{
			BaseballHitEvent HitEvent(b);
			chr_func::UpdateAll(b, &HitEvent);

			//モデル関連の更新
			b->ModelUpdate(2);
            chr_func::CreateTransMatrix(b, &b->getNowModeModel()->m_TransMatrix);
		}
	}
}

void  BaseballState_PlayerControll_ShotAttack_B::Exit(BaseballPlayer* b)
{

}
