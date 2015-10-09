#include "BaseballState_PlayerControll_ShotAttack_B.h"
#include "BaseballPlayerState.h"
#include "Baseball_HitEvent.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../../Ball/UsualBall.h"
#include "../../Damage/Damage.h"
#include "../CharacterCounterClass.h"
#include "../CharacterMoveClass.h"
#include "../CharacterShotAttackClass.h"
#include "../CharacterManager.h"


#include "../../Effect/HitEffectObject.h"
#include "../../Sound/Sound.h"


BaseballState_PlayerControll_ShotAttack_B::BaseballState_PlayerControll_ShotAttack_B(
	ControllClass*       pControllClass //�I������delete����
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
	//�J�E���g������
	m_Timer = 0;

	//���[�V�����Z�b�g
	b->m_Renderer.SetMotion(baseball_player::_mb_Shot);

	{
		//�ړ��ʐ���
		const float MaxMove = 0.06f;

		if (b->m_Params.move.Length() > MaxMove)
		{
			b->m_Params.move.Normalize();
			b->m_Params.move *= MaxMove;
		}
	}

	{
		//�ł��グ�{�[������
		BallBase::Params param;

		//�ړ��͏����
		param.move = b->m_Params.move;
		param.move.y = 0.4f;

		//�L�����̏ꏊ��(�ŏI�I�ɘr�̈ʒu�ɁH)
		param.pos = b->m_Params.pos + Vector3(0, 2.0f, 0);
		//�e��������
		param.pParent = b;
		//�J�E���^�[�ł��Ȃ��^�C�v
		param.type = BallBase::Type::_CantCounter;

		//����
		m_pUpBall = new PhysicallyMoveBall(param, DamageBase::Type::_WeekDamage, 1, -0.025f);

		//�ł��グ���͂����蔻��Ȃ�
		m_pUpBall->m_Damage.m_Enable = false;
	}
}

void BaseballState_PlayerControll_ShotAttack_B::Execute(BaseballPlayer* b)
{
	const int EndFrame = 48; //�I���t���[��
	const int ShotFrame = 30;//�łt���[��
	const int CancelStart = 10;//�L�����Z���s���\�ȃt���[��
	const int AfterAction = 33;//�V���b�g��̃A�N�V�������\�ɂȂ�t���[��

	//�J�E���g�X�V
	++m_Timer;

	//�ł��L�����Z��
	if (m_Timer > CancelStart && m_Timer < ShotFrame - 3)
	{
		if (m_pControllClass->DoOtherAction())
		{
			m_pUpBall->m_Damage.m_Enable = true;
			m_pUpBall->m_Params.type = BallBase::Type::_CantCounter;
		}
	}

	//�V���b�g��̃A�N�V����
	if (m_Timer > AfterAction && m_Timer < EndFrame)
	{
		m_pControllClass->DoShotAfterAction();
	}

	//�����␳
	if (m_Timer < ShotFrame)
	{
		const CharacterBase* const pTargetCharacter = GetFrontTarget(b);
		const float AngleSpeed = D3DXToRadian(3);

		if (pTargetCharacter != nullptr)
		{
			//������]
			chr_func::AngleControll(b, pTargetCharacter->m_Params.pos, AngleSpeed*2.0f);
		}
		else
		{
			Vector3 vec = m_pControllClass->GetVec();
			chr_func::AngleControll(b, b->m_Params.pos + vec, AngleSpeed);
		}
	}

	//�ł��������{�[�����������̏ꍇ�_���[�W�����L����
	if (m_pUpBall != nullptr&&
		m_pUpBall->m_Params.move.y < 0)
	{
		m_pUpBall->m_Damage.m_Enable = true;
		m_pUpBall->m_Params.type = BallBase::Type::_CantCounter;
	}

	//�T�E���h
	if (m_Timer == ShotFrame - 3)
	{
		Sound::Play(Sound::BaseBall_kaki_n);
	}


	//�łI
	if (m_Timer == ShotFrame)
	{
		//�ł��������{�[���̏�ԃ`�F�b�N
		if (
			m_pUpBall != nullptr&&
			m_pUpBall->m_Params.pParent == b&&
			m_pUpBall->m_Params.type == BallBase::Type::_CantCounter
			)
		{

			//�{�[������
			BallBase::Params param;

			//�ړ��͑O����
			chr_func::GetFront(b, &param.move);
			//�X�s�[�h�͓K��
			param.move *= 0.6f;

			//�L�����̏ꏊ��(�ŏI�I�ɘr�̈ʒu�ɁH)
			param.pos = m_pUpBall->m_Params.pos;
			//�������L�������ʃ{�[�����˂�Y��
			param.pos.y = BallBase::UsualBallShotY;

			//�e��������
			param.pParent = b;
			//�ʏ�^�C�v
			param.type = BallBase::Type::_Usual;

			//����
			new UsualBall(param, DamageBase::Type::_WeekDamage, 5);


			//�G�t�F�N�g
			{
				COLORf EffectColor(CharacterBase::GetPlayerColor(b->m_PlayerInfo.number));

				//�G�t�F�N�g�̐ݒ�
				new HitEffectObject(
					param.pos,
					Vector3Normalize(param.move),
					0.1f,
					0.1f,
					Vector3(EffectColor.r, EffectColor.g, EffectColor.b)
					);
			}

			//��ɏグ���{�[��������
			m_pUpBall->m_DeleteFlag = true;
			m_pUpBall = nullptr;
		}
	}

	//�X�e�[�g�I��
	if (m_Timer > EndFrame)
	{
		b->SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(b));
	}

	//�ł��I����Ă����ꍇ
	if (m_Timer > ShotFrame)
	{
		chr_func::XZMoveDown(b, 0.1f);
	}

	//��{�I�ȍX�V
	{
		BaseballHitEvent HitEvent(b);
		chr_func::UpdateAll(b, &HitEvent);

		//���f���֘A�̍X�V
		b->m_Renderer.Update(1);
		chr_func::CreateTransMatrix(b, b->m_ModelSize, &b->m_Renderer.m_TransMatrix);
	}
}

void  BaseballState_PlayerControll_ShotAttack_B::Exit(BaseballPlayer* b)
{

}


const CharacterBase*  BaseballState_PlayerControll_ShotAttack_B::GetFrontTarget(BaseballPlayer* b)const
{
	CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();

	const CharacterBase* pTargetEnemy = nullptr;    //�^�[�Q�b�g�ێ��̃|�C���^
	RADIAN MostMinAngle = D3DXToRadian(33);         //�����Ƃ������p�x
	RADIAN TempAngle;

	Vector3 MyFront;      //���g�̑O���x�N�g��
	chr_func::GetFront(b, &MyFront);

	auto it = ChrMap.begin();

	while (it != ChrMap.end())
	{
		//���g�����O
		if (b->m_PlayerInfo.number == it->first->m_PlayerInfo.number ||
			chr_func::isDie(it->first))
		{
			++it;
			continue;
		}

		//�O�x�N�g���ƓG�ւ̃x�N�g���̊p�x���v�Z����
		TempAngle = Vector3Radian(MyFront, (it->first->m_Params.pos - b->m_Params.pos));

		//�p�x����ԋ���������X�V
		if (TempAngle < MostMinAngle)
		{
			pTargetEnemy = it->first;
			MostMinAngle = TempAngle;
		}

		++it;
	}
	return pTargetEnemy;
}