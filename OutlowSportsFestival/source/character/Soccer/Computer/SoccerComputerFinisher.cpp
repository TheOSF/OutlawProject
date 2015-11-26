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
		//�@�������Đ�&The World
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
	

	// �X�V
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
		SoccerPlayer* m_pSoccer;//�@�싅
		Snakeshot* snake;
	public:
		//�@�{�[��
		BallBase::Params param;
		//�@�^�[�Q�b�g
		Vector3 target;
	public:
		//�@�R���X�g���N�^
		ShotAttackEvent(SoccerPlayer* pSoccer) :target(0, 0, 0),
			m_pSoccer(pSoccer) {}
		//�@�X�V
		void Update()override {
			//�@���f���X�V
			m_pSoccer->m_Renderer.Update(1.0f);

			// �]���s��X�V
			chr_func::CreateTransMatrix(
				m_pSoccer,
				0.05f,
				&m_pSoccer->m_Renderer.m_TransMatrix);
		}
	public:
		// �_���[�W����J�n & �{�[������
		void Shot()
		{
			//�@�������U��(param�v�Z)
			Vector3 pos, vec;

			chr_func::GetFront(m_pSoccer, &vec);

			pos = m_pSoccer->m_Params.pos;
			pos.y = UsualBall::UsualBallShotY;

			//����
			Sound::Play(Sound::Beam2);
			new Snakeshot(pos, vec, m_pSoccer, 1);
		}

		//�@�������U���J�n
		void AttackStart()override {
			//�@�����[�V����
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Shot);
		}

		void AttackEnd()
		{
			//�U���I�����ɒʏ�ړ����[�h�ɖ߂�
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
//�@The World
void SoccerState_ComputerControll_Finisher::FreezeGame(UINT frame, SoccerPlayer* s)
{
	std::list<LpGameObjectBase> UpdateObjList;

	UpdateObjList.push_back(s);

	DefGameObjMgr.FreezeOtherObjectUpdate(UpdateObjList, frame, true);
}