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
		//�@�������Đ�&The World
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
	//��{�I�ȍX�V
	{
		DamageManager::HitEventBase NoDmgHitEvent;   //�m�[�_���[�W
		SoccerHitEvent              UsualHitEvent(s);//�ʏ�

													 //���G�t���[�����ɂ���ăq�b�g�C�x���g�N���X�̕���
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
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Command);
		}

		void AttackEnd()
		{
			//�U���I�����ɒʏ�ړ����[�h�ɖ߂�
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
//�@The World
void SoccerState_ComputerControll_Finisher::FreezeGame(UINT frame, SoccerPlayer* s)
{
	std::list<LpGameObjectBase> UpdateObjList;

	UpdateObjList.push_back(s);

	DefGameObjMgr.FreezeOtherObjectUpdate(UpdateObjList, frame, true);
}