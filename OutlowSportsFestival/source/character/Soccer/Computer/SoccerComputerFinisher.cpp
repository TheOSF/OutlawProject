#include "SoccerComputerFinisher.h"
#include "../../CharacterFunction.h"
#include "../../CharacterManager.h"
#include "../SoccerPlayerState.h"
#include "../Snakeshot.h"
#include "../SoccerHitEvent.h"
#include "../../../Sound/Sound.h"

//CharacterShotAttack * SoccerState_ComputerControll_Finisher::SnakeShotClass(SoccerPlayer * s)
//{
//	return nullptr;
//}

SoccerState_ComputerControll_Finisher::SoccerState_ComputerControll_Finisher() :
	m_pSnakeShotClass(nullptr)
{
	m_Timer = 0;
	timeflg = false;
}
void SoccerState_ComputerControll_Finisher::Enter(SoccerPlayer* s)
{
	m_pSnakeShotClass = this->SnakeShotClass(s);
}
void SoccerState_ComputerControll_Finisher::Execute(SoccerPlayer* s)
{
	m_Timer++;

	if (!timeflg)
	{
		//　発動音再生&The World
		if (m_Timer == 1)
		{
			Sound::Play(Sound::Skill);
			FreezeGame(30,s);
		}

		if (m_Timer >= 31)
		{
			timeflg = true;
			m_Timer = 0;
		}
	}
	else
	{
		m_pSnakeShotClass->SetStickValue(Vector2(0, 0));
	}
	

	// 更新
	if (m_pSnakeShotClass->Update() == false)
	{
		return;
	}

}
void SoccerState_ComputerControll_Finisher::Exit(SoccerPlayer* s)
{
	delete m_pSnakeShotClass;
}
CharacterShotAttack* SoccerState_ComputerControll_Finisher::SnakeShotClass(SoccerPlayer* s) {
	class ShotAttackEvent :public CharacterShotAttack::Event {
		SoccerPlayer* m_pSoccer;//　野球
		Snakeshot* snake;
	public:
		//　ボール
		BallBase::Params param;
		//　ターゲット
		Vector3 target;
	public:
		//　コンストラクタ
		ShotAttackEvent(SoccerPlayer* pSoccer) :target(0, 0, 0),
			m_pSoccer(pSoccer) {}
		//　更新
		void Update()override {
			//　モデル更新
			m_pSoccer->m_Renderer.Update(1.0f);

			// 転送行列更新
			chr_func::CreateTransMatrix(
				m_pSoccer,
				0.05f,
				&m_pSoccer->m_Renderer.m_TransMatrix);
		}
	public:
		// ダメージ判定開始 & ボール発射
		void Shot()
		{
			//　遠距離攻撃(param計算)
			Vector3 pos, vec;

			chr_func::GetFront(m_pSoccer, &vec);

			pos = m_pSoccer->m_Params.pos;
			pos.y = UsualBall::UsualBallShotY;

			//生成
			Sound::Play(Sound::Beam2);
			new Snakeshot(pos, vec, m_pSoccer, 1);
		}

		//　遠距離攻撃開始
		void AttackStart()override {
			//　☆モーション
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Shot);
		}

		void AttackEnd()
		{
			//攻撃終了時に通常移動モードに戻る
			m_pSoccer->SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove(m_pSoccer));
		}
	};

	CharacterShotAttack::AttackParams atk;

	atk.AllFrame = 40;
	atk.AttackPower = 8;
	atk.MaxTurnRadian = PI / 4;
	atk.MoveDownSpeed = 0.8f;
	atk.ShotFrame = 15;

	return m_pSnakeShotClass = new CharacterShotAttack(
		s,
		new ShotAttackEvent(s),
		atk,
		new  SoccerHitEvent(s)
		);
}
//　The World
void SoccerState_ComputerControll_Finisher::FreezeGame(UINT frame, SoccerPlayer* s)
{
	std::list<LpGameObjectBase> UpdateObjList;

	UpdateObjList.push_back(s);

	DefGameObjMgr.FreezeOtherObjectUpdate(UpdateObjList, frame, true);
}