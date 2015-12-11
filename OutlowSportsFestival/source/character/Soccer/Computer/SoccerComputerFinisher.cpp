#include "SoccerComputerFinisher.h"
#include "../../CharacterFunction.h"
#include "../../CharacterManager.h"
#include "../SoccerPlayerState.h"
#include "../Snakeshot.h"
#include "../SoccerHitEvent.h"
#include "../../../Sound/Sound.h"
#include "../../../Effect/SpecialAttackEffect.h"

//CharacterShotAttack * SoccerState_ComputerControll_Finisher::SnakeShotClass(SoccerPlayer * s)
//{
//	return nullptr;
//}

SoccerState_ComputerControll_Finisher::SoccerState_ComputerControll_Finisher() :
	m_pSnakeShotClass(nullptr)
{
	
}
void SoccerState_ComputerControll_Finisher::Enter(SoccerPlayer* s)
{
	m_pSnakeShotClass = this->SnakeShotClass(s);

	m_Timer = 0;
	timeflg = false;
	chr_func::ResetSkillGauge(s);
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
			FreezeGame(55,s);
			new SpecialAttackEffect(s, 55);
		}

		if (m_Timer == 60)
		{
			s->m_Renderer.SetMotion(SoccerPlayer::_ms_Shot);
		}
		if (m_Timer >= 80)
		{
			timeflg = true;
			m_Timer = 0;
		}
	}
	else
	{
		m_pSnakeShotClass->SetStickValue(Vector2(0, 0));
	}
	//基本的な更新
	{
		DamageManager::HitEventBase NoDmgHitEvent;   //ノーダメージ
		SoccerHitEvent              UsualHitEvent(s);//通常

													 //無敵フレームかによってヒットイベントクラスの分岐
		/*if (m_Timer < NoDamageFrame)
		{
			chr_func::UpdateAll(s, &NoDmgHitEvent);
		}
		else
		{
			chr_func::UpdateAll(s, &UsualHitEvent);
		}*/
		chr_func::UpdateAll(s, &NoDmgHitEvent);
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
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Command);
		}

		void AttackEnd()
		{
			//攻撃終了時に通常移動モードに戻る
			m_pSoccer->SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove(m_pSoccer));
		}
	};

	CharacterShotAttack::AttackParams atk;

	atk.AllFrame = 80;
	atk.AttackPower = 8;
	atk.MaxTurnRadian = PI / 4;
	atk.MoveDownSpeed = 0.3f;
	atk.ShotFrame = 70;

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