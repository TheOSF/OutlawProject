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
	b->SetMotion(baseball_player::_mb_Shot_B);

}

void BaseballState_PlayerControll_ShotAttack_B::Execute(BaseballPlayer* b)
{
	Vector3 v1(0, 0, 0), v2(0, 0, 0);
	const int EndFrame = 48; //�I���t���[��
	const int ShotFrame = 35;//�łt���[��
	const int CancelStart = 25;//�L�����Z���s���\�ȃt���[��
	const int AfterAction = 45;//�V���b�g��̃A�N�V�������\�ɂȂ�t���[��
	const float AngleRange = PI / 4;
	pTargetEnemy = nullptr;    //�^�[�Q�b�g�ێ��̃|�C���^

	//�J�E���g�X�V
	++m_Timer;

	if (m_Timer == 5)
	{
		{
			//�ړ��ʐ���
			const float MaxMove = 0.06f;

			if (b->m_Params.move.Length() > MaxMove)
			{
				b->m_Params.move.Normalize();
				b->m_Params.move *= MaxMove;
			}
		}

		//�ł��グ�{�[������
		BallBase::Params param;

		//�ړ��͏����
		param.move = b->m_Params.move;
		param.move.y = 0.48f;

		
		//�J�E���^�[�ł��Ȃ��^�C�v
		param.type = BallBase::Type::_CantCounter;

		
		//�L�����̏ꏊ��(�ŏI�I�ɘr�̈ʒu�ɁH)
        param.pos = b->getNowModeModel()->GetWorldBonePos(20);
		//�e��������
		param.pParent = b;

		//����
		//����  --�X�e�[�W�O�Ń{�[���𐶐�����ƃ{�[���������ăv���O������������ -- �{���͂����Ȃ��̂ŃX���[��ok
		m_pUpBall = new PhysicallyMoveBall(param, DamageBase::Type::_WeekDamage, 1, -0.025f);

		//�ł��グ���͂����蔻��Ȃ�
		m_pUpBall->m_Damage.m_Enable = false;

        //�X�L���Q�[�W���Z
        chr_func::AddSkillGauge(b, UsualBall::AddSkillValueRatio);
	}

	if (m_Timer >= 5)
	{
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
            CharacterBase* pTargetCharacter = nullptr;
            const float AngleSpeed = D3DXToRadian(10);

            if (chr_func::CalcAtkTarget(b, AngleSpeed, 50.0f, &pTargetCharacter))
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
				param.move *= 0.68f;

				//�L�����̏ꏊ��(�ŏI�I�ɘr�̈ʒu�ɁH)
				param.pos = m_pUpBall->m_Params.pos;
				//�������L�������ʃ{�[�����˂�Y��
				param.pos.y = BallBase::UsualBallShotY;

				//�e��������
				param.pParent = b;
				//�ʏ�^�C�v
				param.type = BallBase::Type::_Usual;

				//����
                new UsualBall(param, DamageBase::Type::_WeekDamage, 8, UsualBall::GetUsualMoveControll());

				//�R���g���[����U��
				chr_func::SetControllerShock(
					b,
					0.5f,
					0.15f
					);

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
			b->ModelUpdate(2);
            chr_func::CreateTransMatrix(b, &b->getNowModeModel()->m_TransMatrix);
		}
	}
}

void  BaseballState_PlayerControll_ShotAttack_B::Exit(BaseballPlayer* b)
{

}
