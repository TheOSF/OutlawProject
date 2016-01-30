#include "TennisPlayerState_Vanish.h"
#include "TennisPlayerState_UsualMove.h"
#include "Tennis_HitEvent.h"
#include "../CharacterFunction.h"

#include "../../Effect/HitEffectObject.h"
#include "../../Effect/SoccerSpecialHitEffect.h"
#include "../../Effect/BlurImpact.h"

#include "../../GameSystem/GameController.h"
#include "../../Effect/EffectFactory.h"
#include "Tennis_DoCancelAction.h"


TennisState_DamageVanish::TennisState_DamageVanish(
    TennisPlayer* pTennis,
    const Vector3& Damage_vec  //�_���[�W���󂯂�����
    ) :
    m_pTennis(pTennis),
    m_Damage_vec(Damage_vec)
{

}

void TennisState_DamageVanish::Enter(TennisPlayer* t)
{
    //�L�����N�^���ʂЂ�݃N���X�̃e�j�X�ŗL�C�x���g�N���X
    class TennisEvent :public CharacterDamageVanish::Event
    {
        Tennis_DoCancelAction* m_pDoCancelAction;
    public:
        TennisEvent(TennisPlayer* pTennis) :m_pTennis(pTennis)
        {
            if (pTennis->m_PlayerInfo.player_type == PlayerType::_Player)
            {
                m_pDoCancelAction = new Tennis_DoCancelAction_Player(pTennis);

                m_pDoCancelAction->SetDontActions(Tennis_DoCancelAction_Player::Action::BoundBallShot);
                m_pDoCancelAction->SetDontActions(Tennis_DoCancelAction_Player::Action::Counter);
                m_pDoCancelAction->SetDontActions(Tennis_DoCancelAction_Player::Action::FarAtk);
                m_pDoCancelAction->SetDontActions(Tennis_DoCancelAction_Player::Action::NearAtk);
            }
            else
            {
                m_pDoCancelAction = new Tennis_DoCancelAction_Computer(pTennis);
            }
        }

        ~TennisEvent()
        {
            delete m_pDoCancelAction;
        }

        void FlyStart()
        {
            //������у��[�V�������Z�b�g
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Damage_Vanish_Fly);
        }

        void Flying(const Matrix& Rotate, RATIO t)
        {
            //���f���̃A�j���[�V�����X�V
            m_pTennis->m_Renderer.Update(t);

            //�ʒu�ɂ��Ƃ����A���[���h�ϊ��s����v�Z
            chr_func::CreateTransMatrix(m_pTennis, &m_pTennis->m_Renderer.m_TransMatrix);

            //������т̉�]������
            m_pTennis->m_Renderer.m_TransMatrix = Rotate*m_pTennis->m_Renderer.m_TransMatrix;
        }

        void DownStart()
        {
            //�_�E�����[�V�������Z�b�g
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Damage_Vanish_Down);
        }

        void Downing()
        {
            m_pTennis->m_Renderer.Update(1);

            //���[���h�ϊ��s����v�Z
            chr_func::CreateTransMatrix(m_pTennis, &m_pTennis->m_Renderer.m_TransMatrix);
        }

        void StandUpStart()
        {
            //�N���オ�胂�[�V�������Z�b�g
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Damage_Vanish_StandUp);
        }

        void StandUping()
        {
            //���f���̃A�j���[�V�����X�V
            m_pTennis->m_Renderer.Update(1);

            //���[���h�ϊ��s����v�Z
            chr_func::CreateTransMatrix(m_pTennis, &m_pTennis->m_Renderer.m_TransMatrix);
        }

        void End()
        {
            //�ʏ�X�e�[�g���Z�b�g
            m_pTennis->SetState(
                TennisState_PlayerControll_Move::GetPlayerControllMove(m_pTennis)
                );
        }

        void HitWall()
        {
            //�ǂɓ����������[�V�������Z�b�g
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Damage_Vanish_HitWallAndDown);
        }

        void HitFloor()
        {
            //���ɓ����������[�V�������Z�b�g
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Damage_Vanish_HitFloor);
        }

        void HitFloorAndStandUp()
        {
            //�����オ�胂�[�V�������Z�b�g
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Damage_Vanish_HitFloorAndStandUp);
        }

        void HitWallUpdate()
        {
            //���f���̃A�j���[�V�����X�V
            m_pTennis->m_Renderer.Update(1);

            //���[���h�ϊ��s����v�Z
            chr_func::CreateTransMatrix(m_pTennis, &m_pTennis->m_Renderer.m_TransMatrix);
        }

        void CanActionUpdate()
        {
            //�s�����򂪉\�ȂƂ��ɌĂ΂��
            m_pDoCancelAction->DoAction();
        }

    private:
        TennisPlayer*  m_pTennis;
    };


    //�_���[�W���[�V�����p�����[�^�[���쐬����
    CharacterDamageVanish::Param Param;

    Param.rotate_speed = Vector3(0.0f, 0.0f, 0.0);
    Param.move = m_Damage_vec;

    Param.down_frame = 15;
    Param.standup_frame = 45;


    //�Ђ�݃N���X���쐬
    m_pDamageVanishClass = new CharacterDamageVanish(
        m_pTennis,
        Param,
        new TennisEvent(t),
        new TennisHitEvent(t)
        );

    //�G�t�F�N�g
    EffectFactory::VanishEffect(
        m_pTennis,
        m_Damage_vec
        );

}

void TennisState_DamageVanish::Execute(TennisPlayer* t)
{
    m_pDamageVanishClass->Update();
}

void TennisState_DamageVanish::Exit(TennisPlayer* t)
{
    delete m_pDamageVanishClass;
}

void TennisState_DamageVanish::StateMachineExit(TennisPlayer* t)
{
    Exit(t);
}