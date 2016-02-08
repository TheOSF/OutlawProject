#include "SoccerCommonState.h"
#include "SoccerPlayerState.h"
#include "SoccerHitEvent.h"
#include "../../Effect/EffectFactory.h"
#include "../../Sound/Sound.h"

#include "../CharacterFunction.h"
#include "../../Effect/EffectFactory.h"
#include "../../Camera/Camera.h"
#include "../../GameSystem/GameController.h"
#include "SoccerRolling.h"
#include "Computer\SoccerComputerUtilityClass.h"

SoccerState_SmallDamage::SoccerState_SmallDamage(
	SoccerPlayer* pSoccer,
	const Vector3& Damage_vec,  //�_���[�W���󂯂�����
    bool           Counter,
    int            Frame
	) :
	m_pSoccer(pSoccer),
	m_Damage_vec(Damage_vec),
    m_Counter(Counter),
    m_Frame(Frame)
{

}

void SoccerState_SmallDamage::Enter(SoccerPlayer* s)
{
	//�L�����N�^���ʂЂ�݃N���X�̃T�b�J�[�ŗL�C�x���g�N���X
	class SoccerEvent :public CharacterDamageMotion::Event
	{
	public:
		SoccerEvent(SoccerPlayer* pSoccer) :m_pSoccer(pSoccer){}

		void Update(float speed)
		{
			//���f���̍X�V�̂�
			m_pSoccer->m_Renderer.Update(speed);
			chr_func::CreateTransMatrix(
				m_pSoccer, 
				&m_pSoccer->m_Renderer.m_TransMatrix);
		}
		void Start()
		{
			//�Ђ�݃��[�V�������Z�b�g
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Damage);
		}
		void End()
		{
			//�ʏ�X�e�[�g���Z�b�g
			m_pSoccer->SetState(
				SoccerState_PlayerControll_Move::GetPlayerControllMove(m_pSoccer)
				);
		}
	private:
		SoccerPlayer*  m_pSoccer;
	};

	//�_���[�W���[�V�����p�����[�^�[���쐬����
	CharacterDamageMotion::Params Param;

	Param.damage_vec = m_Damage_vec;
    Param.counter_hit = m_Counter;
    Param.frame = m_Frame;

	//�Ђ�݃N���X���쐬
    m_pDamageMotionClass = new CharacterDamageMotion(
        m_pSoccer,
        new SoccerEvent(m_pSoccer),
        new SoccerHitEvent(m_pSoccer, false), 
		Param
		);

    //�G�t�F�N�g
    EffectFactory::HitEffect(
        m_pSoccer,
        m_Damage_vec
        );

}
void SoccerState_SmallDamage::Execute(SoccerPlayer* t)
{
	m_pDamageMotionClass->Update();
}

void SoccerState_SmallDamage::Exit(SoccerPlayer* t)
{
	delete m_pDamageMotionClass;

}

SoccerState_DamageVanish::SoccerState_DamageVanish(
	SoccerPlayer* pSoccer,
	const Vector3& Damage_vec  //�_���[�W���󂯂�����
	) :
	m_pSoccer(pSoccer),
	m_Damage_vec(Damage_vec)
{

}
void SoccerState_DamageVanish::Enter(SoccerPlayer* s)
{
	//�L�����N�^���ʂЂ�݃N���X�̃T�b�J�[�ŗL�C�x���g�N���X
	class SoccerEvent :public CharacterDamageVanish::Event
	{
	public:
		SoccerEvent(SoccerPlayer* pSoccer) :m_pSoccer(pSoccer), m_Timer(0){}

		void FlyStart()
		{
			//������у��[�V�������Z�b�g
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Vanish_Fly);
		}
        void Flying(const Matrix& Rotate, RATIO t)
		{
			//���f���̃A�j���[�V�����X�V
			m_pSoccer->m_Renderer.Update(1);

			//�ʒu�ɂ��ƂÂ��A���[���h�ϊ��s����v�Z
			chr_func::CreateTransMatrix(m_pSoccer, &m_pSoccer->m_Renderer.m_TransMatrix);

			//������т̉�]������
			m_pSoccer->m_Renderer.m_TransMatrix = Rotate*m_pSoccer->m_Renderer.m_TransMatrix;
		}
		void DownStart()
		{
			//�_�E�����[�V�������Z�b�g
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Vanish_Down);
		}

		void Downing()
		{
			m_pSoccer->m_Renderer.Update(1);

			//���[���h�ϊ��s����v�Z
			chr_func::CreateTransMatrix(m_pSoccer, &m_pSoccer->m_Renderer.m_TransMatrix);
		}

		void StandUpStart()
		{
			//������у��[�V�������Z�b�g
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_StandUp);
		}

		void StandUping()
		{
			//���f���̃A�j���[�V�����X�V
			m_pSoccer->m_Renderer.Update(1);

			//���[���h�ϊ��s����v�Z
			chr_func::CreateTransMatrix(m_pSoccer, &m_pSoccer->m_Renderer.m_TransMatrix);
		}

		void End()
		{
			//�ʏ�X�e�[�g���Z�b�g
			m_pSoccer->SetState(
				SoccerState_PlayerControll_Move::GetPlayerControllMove(m_pSoccer)
				);
		}

        void HitWall()
        {
            //�ǂɓ����������[�V�������Z�b�g
            m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_HitWallAndFall);
        }

        void HitFloor()
        {
            //���ɓ����������[�V�������Z�b�g
            m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_FallAndDown);
        }

        void HitFloorAndStandUp()
        {
            //�����オ�胂�[�V�������Z�b�g
            m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_StandUp);
        }

        void HitWallUpdate()
        {
            //���f���̃A�j���[�V�����X�V
            m_pSoccer->m_Renderer.Update(1);

            //���[���h�ϊ��s����v�Z
            chr_func::CreateTransMatrix(m_pSoccer, &m_pSoccer->m_Renderer.m_TransMatrix);
        }

        void CanActionUpdate()
        {
            //�s�����򂪉\�ȂƂ��ɌĂ΂��
            if (m_pSoccer->m_PlayerInfo.player_type == PlayerType::_Player)
            {
                if (controller::GetPush(controller::button::batu, m_pSoccer->m_PlayerInfo.number))
                {
                    m_pSoccer->SetState(new SoccerState_Rolling(new SoccerUtillityClass::PlayerRollingControll(m_pSoccer), false));
                }
            }
            else
            {
                if (frand() < 0.02f)
                {
                    m_pSoccer->SetState(new SoccerState_Rolling(new SocceComputerrUtillityClass::ComputerRollingControll(m_pSoccer, -Vector3Normalize(m_pSoccer->m_Params.pos)), false));
                }
            }
        }


	private:
		SoccerPlayer*  m_pSoccer;
		int            m_Timer;
	};

	//�_���[�W���[�V�����p�����[�^�[���쐬����
	CharacterDamageVanish::Param Param;

	Param.rotate_speed = Vector3(0.0f, 0.0f, 0.0);
	Param.move = m_Damage_vec;
	Param.standup_frame = 50;
    Param.down_frame = 10;

	//�Ђ�݃N���X���쐬
    m_pDamageVanishClass = new CharacterDamageVanish(
        m_pSoccer,
        Param,
        new SoccerEvent(s),
        new SoccerHitEvent(s, false)
		);

    //�G�t�F�N�g
    EffectFactory::VanishEffect(
        m_pSoccer,
        m_Damage_vec
        );

}
void SoccerState_DamageVanish::Execute(SoccerPlayer* s)
{
	m_pDamageVanishClass->Update();
}

void SoccerState_DamageVanish::Exit(SoccerPlayer* s)
{
	delete m_pDamageVanishClass;
}

SoccerState_DamageMotion_Die::SoccerState_DamageMotion_Die(
	SoccerPlayer* pSoccer,
	const Vector3& Damage_vec  //�_���[�W���󂯂�����
	) :
	m_pSoccer(pSoccer),
	m_Damage_vec(Damage_vec)
{

}

void SoccerState_DamageMotion_Die::Enter(SoccerPlayer* t)
{
	//�L�����N�^���ʂЂ�݃N���X�̃T�b�J�[�ŗL�C�x���g�N���X
	class TennisEvent :public CharacterDamageVanish::Event
	{
	public:
		TennisEvent(SoccerPlayer* pSoccer) :m_pSoccer(pSoccer){}

		void FlyStart()
		{
			//������у��[�V�������Z�b�g
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Vanish_Fly);
            m_pSoccer->m_DrawObject.m_isDraw = false;
		}

        void Flying(const Matrix& Rotate, RATIO t)
		{
			//���f���̃A�j���[�V�����X�V
			m_pSoccer->m_Renderer.Update(t);

			//�ʒu�ɂ��Ƃ����A���[���h�ϊ��s����v�Z
			chr_func::CreateTransMatrix(m_pSoccer, &m_pSoccer->m_Renderer.m_TransMatrix);

			//������т̉�]������
			m_pSoccer->m_Renderer.m_TransMatrix = Rotate*m_pSoccer->m_Renderer.m_TransMatrix;
		}

		void DownStart()
		{
			//�_�E�����[�V�������Z�b�g
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Vanish_Down);
		}

		void Downing()
		{
			m_pSoccer->m_Renderer.Update(1);

			//���[���h�ϊ��s����v�Z
			chr_func::CreateTransMatrix(m_pSoccer, &m_pSoccer->m_Renderer.m_TransMatrix);
		}

		void StandUpStart()
		{
			//�������Ȃ�
		}

		void StandUping()
		{
			//���f���̃A�j���[�V�����X�V
			m_pSoccer->m_Renderer.Update(1);

			//���[���h�ϊ��s����v�Z
			chr_func::CreateTransMatrix(m_pSoccer, &m_pSoccer->m_Renderer.m_TransMatrix);
		}

		void End()
		{
			//�J�����ʂ��t���O��false��
            m_pSoccer->m_DrawObject.m_isDraw = false;
		}

        void HitWall()
        {
            //�ǂɓ����������[�V�������Z�b�g
            m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_HitWallAndFall);
        }

        void HitFloor()
        {
            //���ɓ����������[�V�������Z�b�g
            m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_FallAndDown);
        }

        void HitFloorAndStandUp()
        {
            //�����オ�胂�[�V�������Z�b�g�c���Ȃ��I
            //m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_StandUp);
        }

        void HitWallUpdate()
        {
            //���f���̃A�j���[�V�����X�V
            m_pSoccer->m_Renderer.Update(1);

            //���[���h�ϊ��s����v�Z
            chr_func::CreateTransMatrix(m_pSoccer, &m_pSoccer->m_Renderer.m_TransMatrix);
        }

        void CanActionUpdate()
        {
            //�s�����򂪉\�ȂƂ��ɌĂ΂��
            // m_pDoCancelAction->DoAction();
        }

	private:
		SoccerPlayer*  m_pSoccer;
	};

	//�_���[�W���[�V�����p�����[�^�[���쐬����
	CharacterDamageVanish::Param  Param;

	Param.rotate_speed = Vector3(0.0f, 0.0f, 0.0f);
    Param.move = m_Damage_vec;
    Param.down_frame = 10;
	Param.standup_frame = 50;


	//�Ђ�݃N���X���쐬
	m_pDamageVanishClass = new CharacterDamageVanish(
		m_pSoccer,
		Param,
		new TennisEvent(t),
		new DamageManager::HitEventBase()
		);

    //�G�t�F�N�g
    EffectFactory::DieEffect(
        m_pSoccer,
        m_Damage_vec
        );

}

void SoccerState_DamageMotion_Die::Execute(SoccerPlayer* s)
{
	m_pDamageVanishClass->Update();
}

void SoccerState_DamageMotion_Die::Exit(SoccerPlayer* s)
{
	delete m_pDamageVanishClass;
}

SoccerState_brake::SoccerState_brake(
	SoccerPlayer* pSoccer
	) :
	m_pSoccer(pSoccer)
{

}
void SoccerState_brake::Enter(SoccerPlayer* s)
{
	//�����̂������[�V�����Z�b�g
	s->m_Renderer.SetMotion(SoccerPlayer::_ms_DushStop);

	Sound::Play(Sound::Sand2);
	Sound::Play(Sound::Soccer_Brake);

	for (int i = 0; i < 3; ++i)
	{
		EffectFactory::Smoke(
			s->m_Params.pos + Vector3(frand() - 0.5f, frand(), frand() - 0.5f)*2.0f,
			Vector3Zero,
			1.8f,
			0.2f,
			false
			);
	}

	m_Timer = 0;

}
void SoccerState_brake::Execute(SoccerPlayer* s)
{
	++m_Timer;

    if (++m_Timer > 30)
	{
		s->SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove(s));
	}
		
    chr_func::XZMoveDown(s, 0.08f);

    s->m_Renderer.Update(1);

    chr_func::UpdateAll(s, &SoccerHitEvent(s));
    chr_func::CreateTransMatrix(s, &s->m_Renderer.m_TransMatrix);
}
void SoccerState_brake::Exit(SoccerPlayer* s)
{

}

SoccerState_clash::SoccerState_clash(
	SoccerPlayer* pSoccer
	) :
	m_pSoccer(pSoccer)
{

}
void SoccerState_clash::Enter(SoccerPlayer* s)
{
	class SoccerMoveEvent :public CharacterUsualMove::MoveEvent
	{
		SoccerPlayer* m_pSoccer;
	public:
		SoccerMoveEvent(SoccerPlayer* pSoccer) :
			m_pSoccer(pSoccer) {}

		//�A�j���[�V�����̍X�V
		void Update(bool isRun, RATIO speed_ratio)
		{
			m_pSoccer->m_Renderer.Update(1);
		}
		//����n�߂Ƀ��[�V�������Z�b�g
		void RunStart()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Damage);
		}
		//�����͂��߂Ƀ��[�V�������Z�b�g
		void StandStart()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Damage);
		}
        //����I���
        void RunEnd()
        {
            m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_RunStop);
        }
	};
	CharacterUsualMove::Params p;

	p.Acceleration = 0.0f;
	p.MaxSpeed = 0.2f;
	p.TurnSpeed = 0.0f;
	p.DownSpeed = 0.2f;

	m_pMoveClass = new CharacterUsualMove(
		s,
		p,
		new SoccerMoveEvent(s),
		new SoccerHitEvent(s)
		);
	//�����̂������[�V�����Z�b�g
	s->m_Renderer.SetMotion(SoccerPlayer::_ms_DushStop);
	Sound::Play(Sound::AtkHit1);
	m_count = 0;

	for (int i = 0; i < 3; ++i)
	{
		EffectFactory::Smoke(
			s->m_Params.pos + Vector3(frand() - 0.5f, frand(), frand() - 0.5f)*2.0f,
			Vector3Zero,
			1.8f,
			0.2f,
			false
			);
	}

}
void SoccerState_clash::Execute(SoccerPlayer* s)
{
	++m_count;
	if (m_count>40)
	{
		s->SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove(s));
	}


	m_pMoveClass->Update();

	chr_func::CreateTransMatrix(s, &s->m_Renderer.m_TransMatrix);
}
void SoccerState_clash::Exit(SoccerPlayer* s)
{
	delete m_pMoveClass;
}