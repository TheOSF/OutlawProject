#include "../SoccerPlayer.h"
#include "SoccerComputerShot.h"
#include "SoccerComputerMove.h"
#include "../../CharacterManager.h"
#include "../../CharacterFunction.h"
#include "../../../Sound/Sound.h"


void SoccerState_ComputerControll_Shot::Enter(SoccerPlayer* s)
{
	class SoccerShotEvent :public CharacterShotAttack::Event
	{
		SoccerPlayer* m_pSoccer;
	public:
		SoccerShotEvent(SoccerPlayer* pSoccer) :
			m_pSoccer(pSoccer) {}

		void Update()
		{
			m_pSoccer->m_Renderer.Update(1);

		}

		void AttackStart()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Shot);
		}

		void AttackEnd()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Stand);
		}
		void Shot()
		{
			BallBase::Params param;

			chr_func::GetFront(m_pSoccer, &param.move);
			param.move *= 0.5f;
			param.pos = m_pSoccer->m_Params.pos;
			param.pos.y = BallBase::UsualBallShotY;
			param.pParent = m_pSoccer;
			param.type = BallBase::Type::_Usual;
			Sound::Play(Sound::Impact2);
			new UsualBall(param, DamageBase::Type::_WeekDamage, 10);
		}
	};

	class SoccerHitEvent :public DamageManager::HitEventBase
	{
	public:
		SoccerHitEvent(SoccerPlayer* ps) :m_pSoccer(ps) {}

		bool Hit(DamageBase* pDmg)
		{
			if (pDmg->pParent == nullptr)
			{
				return false;
			}
			//�����������ɌĂ΂��֐�(�߂�l�F�����������ǂ���)
			//�����̍���Ă���_���[�W�������ꍇ�͉������Ȃ�
			if (pDmg->pParent->m_PlayerInfo.number == m_pSoccer->m_PlayerInfo.number)
			{
				return false;
			}

			//�����������ɂ��̃_���[�W�̎�ނ���A���ꂼ��̃X�e�[�g�ɔh��������
			switch (pDmg->type)
			{
			case DamageBase::Type::_WeekDamage:
				//��U��
				//m_pSoccer->SetState();
				return true;

				/*
				//���쐬
				case DamageBase::Type::_VanishDamage:
				//������у_���[�W
				m_pSoccer->SetState();
				return true;
				case DamageBase::Type::_UpDamage:
				//��ɐ������
				m_pSoccer->SetState();
				return true;
				*/

			default:break;
			}

			return false;

		}
	private:
		SoccerPlayer* m_pSoccer;
	};
	CharacterShotAttack::AttackParams p;

	p.ShotFrame = 10;
	p.AllFrame = 35;
	p.MoveDownSpeed = 0.2f;

	m_pShotClass = new CharacterShotAttack(s, new SoccerShotEvent(s), p, new SoccerHitEvent(s));
}
void SoccerState_ComputerControll_Shot::Execute(SoccerPlayer* s)
{
	//�^�[�Q�b�g�I�聕�����␳
	
	CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();

	const float  AutoDistance = 400.0f;               //�������ł���ő勗��
	const RADIAN AutoMaxAngle = D3DXToRadian(90);   //�������ł���ő�p�x

	const CharacterBase* pTargetEnemy = nullptr;    //�^�[�Q�b�g�ێ��̃|�C���^
	RADIAN MostMinAngle = PI;                       //�����Ƃ������p�x
	RADIAN TempAngle;

	Vector3 MyFront;      //���g�̑O���x�N�g��
	chr_func::GetFront(s, &MyFront);

	auto it = ChrMap.begin();

	while (it != ChrMap.end())
	{
		//���g�����O
		if (s->m_PlayerInfo.number == it->first->m_PlayerInfo.number ||
			chr_func::isDie(it->first)
			)
		{
			++it;
			continue;
		}

		//���������ȏ�̃L�����N�^�����O����
		if (Vector3Distance(it->first->m_Params.pos, s->m_Params.pos) > AutoDistance)
		{
			it = ChrMap.erase(it);
			continue;
		}

		//�O�x�N�g���ƓG�ւ̃x�N�g���̊p�x���v�Z����
		TempAngle = Vector3Radian(MyFront, (it->first->m_Params.pos - s->m_Params.pos));

		//�p�x����ԋ���������X�V
		if (TempAngle < MostMinAngle)
		{
			pTargetEnemy = it->first;
			MostMinAngle = TempAngle;
		}

		++it;
	}
	if (pTargetEnemy != nullptr)
	{
		chr_func::AngleControll(s, pTargetEnemy->m_Params.pos, 0.3f);
	}
	if (!m_pShotClass->Update())
	{
		s->SetState(new SoccerState_ComputerControll_Move);
    }

	//��{�I�ȍX�V
	SoccerHitEvent HitEvent(s);
	chr_func::UpdateAll(s, &HitEvent);
	
	//���f���֘A�̍X�V
	s->m_Renderer.Update(1);
	chr_func::CreateTransMatrix(s, 0.05f, &s->m_Renderer.m_TransMatrix);
}
void SoccerState_ComputerControll_Shot::Exit(SoccerPlayer* s)
{
	delete m_pShotClass;
}